#include <iostream>
#include <time.h>

#include "settings.h"
#include "experiments.h"
#include "fileIO.h"

namespace VisMe{

  ///////////////////////////////////////////////////////////////////////
  //Global variables for experiment settings 
  ///////////////////////////////////////////////////////////////////////

  const std::string DEFAULT_SETUP_FILE_NAME = "setup.ini";
  Settings::saveSettings_t saveSettings;
  Settings::experimentSettings_t experimentSettings;
  std::vector<std::string> cameraIds;
  std::string setupFileName; 

  CamCtrlVmbAPI *camCtrl = NULL;   //The camera controller

  /////////////////////////////////////////////////
  //Set of local variables
  /////////////////////////////////////////////////
  const int FILENAME_BUFFER_LENGTH = 256;
  const int PATHNAME_BUFFER_LENGTH = 1024;
  char pathNameBuffer[FILENAME_BUFFER_LENGTH];
  char fileNameBuffer[PATHNAME_BUFFER_LENGTH];

  int currentOutImageIndex=0;
  int currentOutImageFolderIndex=0;

  bool saveAutoExposureFrame = false;

  ///////////////////////////////////////////////////////////////////////
  //Experiment API
  ///////////////////////////////////////////////////////////////////////

  void run_image_stack_capture()
  {
    std::cout << "run_image_stack_capture() stub" << std::endl;
  }

  void run_single_capture()
  {
    std::cout << "run_single_capture() stub" << std::endl;
  }

  void run_streaming_view()
  {
    std::cout << "run_streaming_view() stub" << std::endl;
  }

  /****************************
   * Do cleaning up prior exit
   ****************************/
  void cleanExit(std::string msg){
    
    std::cout << msg << std::endl;    
    camCtrl->freeCameras();
    delete camCtrl;

  }

  ///////////////////////////////////////////////////////////////////////////////////////////
  //OBS be sure that the savesettings struct is populated before calling generateX functions
  //It is also expected that the saving path is created when the save settings is populated
  bool generateCamDir(int idx){
    

    if (snprintf( pathNameBuffer, PATHNAME_BUFFER_LENGTH, "%s%s%d\0", 
		  saveSettings.outPath.c_str(), 
		  saveSettings.cameraDirectoryPrefix.c_str(), 
		  idx) > 0) {
      
      FileIO::makeDirectory( pathNameBuffer, true, true );
      
      return true;
    }    
    return false;
  }

  bool generateImageDir( int camId )
  {

    switch (saveSettings.imageDirectoryPrefixType) {

    case Settings::DATETIME :
      {
	char timeBuf[64];
	time_t now = time(NULL);
	struct tm *p_tm = localtime( &now );
	strftime( timeBuf, sizeof(timeBuf), "%Y-%m-%d_%Hh%Mm%Ss", p_tm );

	if (snprintf( pathNameBuffer, PATHNAME_BUFFER_LENGTH, "%s%s%d/%s\0", 
		      saveSettings.outPath.c_str(), 
		      saveSettings.cameraDirectoryPrefix.c_str(), 
		      camId,
		      timeBuf
		      ) > 0  )
	  {
	    int rval = FileIO::makeDirectory( pathNameBuffer, false, true );
	    if (rval < 0)
	      return false;
	    else
	      return true;
	  }
	break;
      }
    case Settings::RUNNING :
      {
	bool uniqueName=false;
      
	//Iterate through suggested filenames until a free one is found
	// -OBS be wary that the currentOutImageFilderIndex is common for _EVERY_ camera
	//  that is - it is expected that _ALL_ cameras follow the same saving schema
	while (!uniqueName){

	  if (snprintf( pathNameBuffer, FILENAME_BUFFER_LENGTH, "%s%s%d/%s%d", 
			saveSettings.outPath.c_str(), 
			saveSettings.cameraDirectoryPrefix.c_str(), 
			camId,
			saveSettings.imageDirectoryPrefix.c_str(),
			currentOutImageFolderIndex
			) > 0) {

	    int rval = FileIO::makeDirectory( pathNameBuffer, false, false );

	    if (rval == 0){
	      uniqueName = true;
	      return true;
	    }	  
	    else if (rval == FileIO::DIR_EXISTS)
	      currentOutImageFolderIndex++;

	    else if (rval == FileIO::DIR_ERROR){
	      std::cout << "Error creating image directory: " << pathNameBuffer << std::endl;
	      return false;
	    }

	  }
	}
	break;
      }
    default ://NONE
      {
	if (snprintf( pathNameBuffer, FILENAME_BUFFER_LENGTH, "%s%s%d/", 
		      saveSettings.outPath.c_str(), 
		      saveSettings.cameraDirectoryPrefix.c_str(), 
		      camId
		      ) > 0) {
	  return true;
	}else
	  return false;
	break;
      }
    }//end switch

  }

  char* generateImageName()
  {

  }

}//end namespace VisMe
