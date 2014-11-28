/******************************************************************************
 * main.cpp
 *
 * Main entrypoint for testing processHDR (from file stack)
 *
 *  Sami Varjo 2014
 *-----------------------------------------------------------------------------
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF TITLE,
 * NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/
#include <iostream>
#include <cstdlib>

#include "imageProcessing.h"
#include "fileIO.h"

//using namespace VisMe;

//#define DEBUG 1


/*********************************************************
 * The program main entry point
 */
int main(int argc, char** argv)
{ 
  
  std::string folderName = "."; 
  std::string filePrefix = "img"; 	//"*";  //default filename start with img eg img00004.tif
  std::string fileSuffix = ".tif";  //"*";  //default file typye .tif
  
  std::string outName = "result.tif";
  
  bool verbose = false;
  
   //Large values lead to double over flow
/*   
  double expTimes[] = { 25,50,100,200,400,800,1600,3200,6400,12800,25600,
						51200,102400,204800,409600,819200,1638400,3276800,		
						6553600,13107200,26214400,52428800};
*/  
  
  //div by factor - This is required to avoid double over flow...
  
  //div by two can be optimized by bit shift
/*  
  double expTimes[] = { 0.0000007450580596923828125, 0.000001490116119384765625, 0.00000298023223876953125, 0.0000059604644775390625, 0.000011920928955078125, 0.00002384185791015625, 0.0000476837158203125, 0.000095367431640625,0.00019073486328125, 0.0003814697265625, 0.000762939453125, 0.00152587890625, 0.0030517578125, 0.006103515625,0.01220703125,0.0244140625, 0.048828125,	0.097656250000,0.1953125,0.390625,0.78125,1.5625,3.125,6.25,12.5,25,50,100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600};
*/  
  

  double expTimes[] = { 0.0244140625, 0.048828125,	0.097656250000,0.1953125,0.390625,0.78125,1.5625,3.125,6.25,12.5,25,50,100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600};

  // double expTimes[] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  
  //double expTimes[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,255};
  
  ///////////////////////////////////////////////////
  // Handle the command line parameters 
  ///////////////////////////////////////////////////
  if (argc == 1)
  {
	std::cout << "Usage: " << argv[0] << " <folder>"  << std::endl << std::endl;		  
	exit(0);
  }
  
  if (argc >1){
  
    
	
    for (int i=1; i<argc;i++){
	
      std::string argStr = std::string(argv[i]);
	  
      //Help requested
      if ( argStr == "-h" ){
		std::cout << "Usage: " << argv[0] << " -f <folder> -p <filePrefix> -s <fileSuffix> -o <outName> -v"  << std::endl << std::endl;		  
		exit(0);
      }	  
      
	  else if ( argStr == "-p" && i<argc-1){	  
		filePrefix = argv[++i];	  	  
	  }
	  else if ( argStr == "-s" && i<argc-1){	  
		fileSuffix = argv[++i];	  
	  }
	  else if (argStr == "-o" && i <argc-1){
		outName = argv[++i];
	  }
	  else if (argStr == "-v"){
		verbose = true;
	  }
	  else {
		
		if( ! FileIO::dirExist( argv[i] ) ){
			std::cout << "folder file '" << argStr << "' do not exist!" << std::endl;
			exit(0);
		}
		else {
			folderName = argStr;			
		}
	  }  
    }
  } //end for : command line parameters

  std::vector<std::string>  fileNames;
  FileIO::getFileNames(fileNames, folderName, filePrefix, fileSuffix);
  
  std::vector<commonImage_t> imageStack;  
    
  if (verbose){ std::cout << "Reading in images..." << std::endl;}
  std::vector<std::string>::iterator fName = fileNames.begin();
  while ( fName != fileNames.end() ){  	
	commonImage_t imIn;
	std::string fullPath = folderName+(*fName++);
	
	if(verbose)  {std::cout << fullPath << " ";}
	readTIFF( fullPath.c_str(), &imIn, verbose);
	imageStack.push_back(imIn);	
  }
  
  commonImage_t resultImage;
  
  int idx = findFirstOverExposedImage(imageStack, 0.5); //do not use first over exp
  
  if (verbose) {std::cout << "found max image idx:" << idx << std::endl;}
  
  //sumGrayStack( imageStack, &resultImage);  
  sumGrayStackWExpTimes(imageStack, expTimes, &resultImage, idx-1);
  
  commonImage_t imageOut;
  
  normaliseGrayTo32bit(&resultImage, &imageOut); 
  //normaliseGrayTo8bit(&resultImage, &imageOut);
  
  saveTIFF( outName.c_str(), &imageOut, COMPRESSION_ZIP);
  
  //Clean UP
  releaseStackData( imageStack );
  fileNames.clear();
  free(imageOut.data);
  free(resultImage.data);
  
  if (verbose){
	std::cout << "Done" << std::endl;
  }
  return 0;
}
