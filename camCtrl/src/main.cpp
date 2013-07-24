/*****************************************************************************************
 * main.cpp
 *
 * Main entrypoint for the camera control software
 * Currently only GigE cameras from AVT are supported (see camCtrlVmbAPI.h)
 *
 *
 *
 *  Sami Varjo 2013
 *
 *****************************************************************************************/
#include <iostream>

#include "camCtrlVmbAPI.h"
#include "iniReader.h"
#include "settings.h"

#include "experiments.h"   //Contain also the global variables for experiments

using namespace VisMe;
using namespace VisMe::Settings; 


/********************************************************
 * Signal handler for ctrl+c to quit cleanly 
 ********************************************************/
void signal_SIGINT_callback_handler(int signum)
{
   // Cleanup and close up stuff here
   cleanExit("Caught CTRL+C - exiting");

   // Terminate program
   exit(signum);
}


int main(int argc, char** argv)
{ 

  if (argc < 2) 
    setupFileName = DEFAULT_SETUP_FILE_NAME;
  else 
    setupFileName = std::string(argv[1]);
  
  ////////////////////////////////////
  //Get the settings from the ini file
  getSaveSettings( &saveSettings, setupFileName );
  std::cout <<"Saving settings:\n\t"<< saveSettings.outPath << "\n\t" << saveSettings.cameraDirectoryPrefix <<
    "\n\t" << saveSettings.filenamePrefix << "\n\t" << saveSettings.filenameSuffix << std::endl;
  
  getCameraIds(cameraIds, setupFileName);
  std::cout << "Setting up " << cameraIds.size() << " camera(s):" << std::endl;
  for (int i=0;i<cameraIds.size();i++){
    std::cout << "\t" << cameraIds[i] << std::endl;
  }

  getExperimentSettings( &experimentSettings, setupFileName);

  ///////////////////////////////////////////////////
  //Initialize the Vimba camera controller and 
  // open cameras by ID  (see also possible InitAll)
  camCtrl = new CamCtrlVmbAPI();
  int rval = camCtrl->InitByIds( cameraIds );

  if (rval < 0){
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
      
    }
  }
 
  cleanExit("Done");
}
