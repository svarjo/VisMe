/******************************************************************************
 * main.cpp
 *
 * Main entrypoint for the camera control software
 * Currently only GigE cameras from AVT are supported (see camCtrlVmbAPI.h)
 *
 *  Sami Varjo 2013
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
#include <signal.h>

#include "camCtrlVmbAPI.h"
#include "iniReader.h"
#include "settings.h"
#include "fileIO.h"
#include "experiments.h"   //Contain also the global variables for experiments

using namespace VisMe;
using namespace VisMe::Settings; 

#define DEBUG 1

/********************************************************
 * Signal handler for ctrl+c to quit cleanly 
 ********************************************************/
void signal_SIGINT_callback_handler(int signum)
{
   // Cleanup and close up stuff here
   cleanExit("\nCaught CTRL+C - exiting...\n");
   exit(signum);
}

/*********************************************************
 * The program main entry point
 */
int main(int argc, char** argv)
{ 

  bool forceAllCameras = false;

  ///////////////////////////////////////////////////
  // Register signal handler for ctrl+c (clean exit)
  ///////////////////////////////////////////////////
  signal(SIGINT, signal_SIGINT_callback_handler); 

  ///////////////////////////////////////////////////
  // Handle the command line parameters 
  ///////////////////////////////////////////////////
  if (argc >1){
    for (int i=1; i<argc;i++){
      std::string argStr = std::string(argv[i]);

      if ( argStr == "-h" ){
	std::cout << "Usage: " << argv[0] << " setupFile.ini"  << " [-findAllCameras] [-h]"
		  << std::endl << std::endl
		  << " setupFile.ini   is a required parameter file (default: " 
		    << DEFAULT_SETUP_FILE_NAME << ")." << std::endl
		  << " -findAllCameras forces to enumerate all possible cameras overriding the" 
		    <<".ini file settings.\n" <<std::endl
		  << " -h              show the command line help."
		  << std::endl;
	exit(0);
      }
      else if ( ( argStr.length() >= SETUP_FILE_SUFFIX.length() ) && 
		( argStr.compare( argStr.length()-SETUP_FILE_SUFFIX.length(), 				
				  SETUP_FILE_SUFFIX.length(), SETUP_FILE_SUFFIX) == 0) 
		){
	setupFileName = argStr;
	if( ! FileIO::fileExist( argv[i] ) ){
	  std::cout << "Setup file '" << setupFileName << "' do not exist!" << std::endl;
	  exit(0);
	}
      }
      else if (argStr == "-findAllCameras"){
	forceAllCameras = true;
      }
      else{
	std::cout << "Unknown commandline parameter : " << argStr << std::endl;
      }
    }//end for : command line parameters
  } 

  if (setupFileName == "undefined" ){
    setupFileName = DEFAULT_SETUP_FILE_NAME;
  }

  //////////////////////////////////////
  //Get the settings from the ini file
  getSaveSettings( &saveSettings, setupFileName );
  std::cout <<"Saving settings:\n\t"<< saveSettings.outPath << "\n\t" << saveSettings.cameraDirectoryPrefix <<
    "\n\t" << saveSettings.filenamePrefix << "\n\t" << saveSettings.filenameSuffix << std::endl;
  
  getCameraIds(cameraIds, setupFileName);
  std::cout << "\nSetting up " << cameraIds.size() << " camera(s):" << std::endl;
  for (int i=0;i<cameraIds.size();i++){
    std::cout << "\t" << cameraIds[i] << std::endl;
  }

  getExperimentSettings( &experimentSettings, setupFileName);

  ///////////////////////////////////////////////////
  //Initialize the Vimba camera controller and 
  // open cameras by ID  (see also possible InitAll)
  camCtrl = new CamCtrlVmbAPI();
  int init_rval;

  if (forceAllCameras)
    init_rval = camCtrl->InitAll();
  else
    init_rval = camCtrl->InitByIds( cameraIds );

  //////////////////////////////////////////////////
  //make a data directory for each camera
  for (int i = 0; i<cameraIds.size(); i++)
    generateCamDir(i+1);  


  //////////////////////////////////////////////////
  // Run experiments if everything in init was OK
#ifdef DEBUG    
  init_rval = 0;
#endif

  if (init_rval < 0){
    std::cout << "Error while initializing cameras.\nExiting..." << std::endl;
  }
  else{
    
    switch (experimentSettings.mode) {
   
    case IMAGE_STACK_EXPTIME:
      run_image_stack_capture();
      break;
   
    case SINGLE:
      run_single_capture();
      break;

    case STREAMING_VIEW:
      run_streaming_view();
      break;
      
    case DEBUG_TESTING:

      switch (experimentSettings.id){
      case 0:
	run_debug_filenames(); //OK     
	break;
      case 1:
	run_test_tiff_load( argv[2] );
	break;
      case 2:
	run_test_tiff_saving();
	break;
	
	
	
      default:
	std::cout << "Running [test] but id=" <<  experimentSettings.id << " not implemented" << std::endl;
	break;
      }
    }
  }
 
  cleanExit("Done");
}
