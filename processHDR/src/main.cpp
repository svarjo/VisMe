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
#include <cstdio>
#include <limits>

#include "imageProcessing.h"
#include "fileIO.h"

//using namespace VisMe;

//#define DEBUG 1

void printUsage(char* cmdStr)
{
	std::cout << "Usage: " << cmdStr << " <folder> -p <filePrefix> -s <fileSuffix> -o <outName> -e <file> -v"  << std::endl << std::endl;		  
	std::cout << "<folder>          location of the image files to be stacked" << std::endl;
	std::cout << "-p <filePrefix>   For example img (def) for files with name like imag0001.tif"<< std::endl;
	std::cout << "-s <filePuffix>   For example .tif (def) for images img00003.tif" << std::endl;
	std::cout << "-o <outName>      Output file name (will be tiff regardless of the suffix) (def) result.tif"<< std::endl;
	std::cout << "-e <file>         If given load exposure times from given file (one per line as ascii)"<< std::endl;
	std::cout << "-r                Compute retinex filter response (mean response out to std::out) (by default raw mean)" << std::endl;
	std::cout << "-v                be verbose if given"<< std::endl;
	std::cout << std::endl;
	std::cout << "example:> " <<cmdStr<<  " data/2014-08-15/ -o data/2014-08-15_result.tif -e data/expTimes.txt -r -v" << std::endl << std::endl;;

	exit(0);
}

/*********************************************************
 * The program main entry point
 */
int main(int argc, char** argv)
{   
  std::string folderName = "."; 
  std::string filePrefix = "img"; 	//"*";  //default filename start with img eg img00004.tif
  std::string fileSuffix = ".tif";  //"*";  //default file typye .tif
  
  std::string outName = "result.tif";  

  bool loadExptimes = false;
  bool verbose = false;
  bool saveResultImage = false;
  bool doRetinexFiltering = false;

  float expTimesDef[] = { 25,50,100,200,400,800,1600,3200,6400,12800,25600,
						   51200,102400,204800,409600,819200,1638400,3276800,		
						   6553600,13107200,26214400,52428800};

  float *expTimes = expTimesDef;
  unsigned int NexpTimes = 22;
  
  std::string expTimeFileName = "inbuild exposure times (no file given)";
  
  ///////////////////////////////////////////////////
  // Handle the command line parameters 
  ///////////////////////////////////////////////////
  if (argc == 1)
  {
	printUsage(argv[0]);
  }
  
  if (argc >1){
	
    for (int i=1; i<argc;i++){
	
      std::string argStr = std::string(argv[i]);
	  
      //Help requested
      if ( argStr == "-h" ){
		printUsage(argv[0]);
      }	  
      
	  else if ( argStr == "-p" && i<argc-1){	  
		filePrefix = argv[++i];	  	  
	  }
	  else if ( argStr == "-s" && i<argc-1){	  
		fileSuffix = argv[++i];	  
	  }
	  else if (argStr == "-o" && i <argc-1){
		outName = argv[++i];
		saveResultImage = true;
	  }
	  else if (argStr == "-v"){
		verbose = true;
	  }
	  else if (argStr == "-r"){
		doRetinexFiltering = true;
	  }
	  
	  else if (argStr == "-e"){
	  
		if (i < argc-1){ 		
			loadExptimes = true;
			expTimeFileName = argv[++i];
		
			if ( !FileIO::fileExist( (char*)expTimeFileName.c_str() ) ){
				std::cout << "Exposure time file '" << expTimeFileName << "' was not found" << std::endl;
				exit(0);
			}		
		}else{
			std::cout << "Exposure time file not given" << std::endl;
			exit(0);
		}
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

  ////////////////////////////////////////////////////////////////
  //If separate text file containing exposure times read them in
  //(each on single line as ascii)
  if (loadExptimes){
	FILE *pF;
	pF = fopen(expTimeFileName.c_str(),"r");
	int count = 0;
	double val;
	while( !feof( pF ) ){		
		fscanf(pF, "%lf", &val);
		count++;
	}
	NexpTimes = count;
	rewind (pF);
	expTimes = (float*) malloc(NexpTimes*sizeof(float));
	count = 0;
	if (verbose){
		std::cout << NexpTimes << " exposure times in '" << expTimeFileName << "':" << std::endl;
	}
	while( !feof( pF ) ){
		fscanf(pF, "%f", &expTimes[count++]);	
	}	
	fclose(pF);
  }
  else{
	if (verbose){
		std::cout << NexpTimes << " default exposure times in use '" << expTimeFileName << "':" << std::endl;
	}
  }
  if (verbose){
	for (unsigned int id=0; id < NexpTimes; id++)
		std::cout << expTimes[id] << " "; 
	std::cout << std::endl;		
  }

  ///////////////////////////////////////////////////////////
  // Find the files containing the image stack and read them
  // (assumed increasing exposure time in sorted file names 
  // (sorting is done here)
  std::vector<std::string>  fileNames;
  FileIO::getFileNames(fileNames, folderName, filePrefix, fileSuffix);
  
  if (NexpTimes < fileNames.size()){
	std::cout << "More image files found than exposure times given. Terminating..." << std::endl;
	exit(0);
  }
  
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
  
  ////////////////////////////////////////////////////////////////////////////////
  //
  // The actual image stack processing  (there is room to optimize so that 
  //                                     single image is read for stack at a time...
  ////////////////////////////////////////////////////////////////////////////////
    
  int idx = findLastOkExposureImage(imageStack, 0.5); //do not use first over exp  
  if (verbose) {std::cout << "Found max usable image idx:" << idx << std::endl;}
 
  
  commonImage_t hdrImage;
  
  //sumGrayStack( imageStack, &resultImage); //very dummy version   
  sumGrayStackWExpTimes(imageStack, expTimes, &hdrImage, idx);   
  releaseStackData( imageStack );
  imageStack.clear();

  commonImage_t workCopy;	
  commonImage_t imageOut;    
  
  double resSum;
  
  if (doRetinexFiltering) {
	//  normaliseGrayToFloat( &hdrImage, &workCopy );   //TODO check normalisation to double effect
	//  multiscaleRetinexFilter( &workCopy, &hdrImage); 
	multiscaleRetinexFilter( &hdrImage, &workCopy ); 	
	normaliseGrayToFloat(&workCopy, &workCopy);
	
	if (verbose) {
	std::cout << "Retinex filtered mean response: " ;
	}
  }
  else{
	normaliseGrayToFloat(&hdrImage, &workCopy); 
	if (verbose) {
	std::cout << "Mean raw response: " ;
	}
  }

  resSum = totalSum( &workCopy ) / (double)(workCopy.width*workCopy.height);	
  std::cout << resSum << std::endl;
	
  if (saveResultImage) {  
	normaliseGrayTo32bit(&workCopy, &imageOut);  //TIFF 32bit int (retain most information) 
	saveTIFF( outName.c_str(), &imageOut, COMPRESSION_ZIP);  
  }
  //Clean UP  
  fileNames.clear();
  free(imageOut.data);
  free(workCopy.data);
  free(hdrImage.data);
  
  if (expTimes != expTimesDef)
	free(expTimes);
  
  if (verbose){
	std::cout << "Done" << std::endl;
  }

  return 0;
}
