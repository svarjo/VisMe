#include <iostream>
#include <time.h>
#include <unistd.h>


#include "settings.h"
#include "experiments.h"
#include "fileIO.h"
#include "commonImage.h"

namespace VisMe{

  ///////////////////////////////////////////////////////////////////////
  //Global variables for experiment settings 
  ///////////////////////////////////////////////////////////////////////

  const std::string DEFAULT_SETUP_FILE_NAME = "setup.ini";
  Settings::saveSettings_t saveSettings;
  Settings::experimentSettings_t experimentSettings;
  std::vector<std::string> cameraIds;
  std::string setupFileName="undefined";

  CamCtrlVmbAPI *camCtrl = NULL;   //The camera controller

  /////////////////////////////////////////////////
  //Set of local variables
  /////////////////////////////////////////////////
  const int FILENAME_BUFFER_LENGTH = 1024;
  const int PATHNAME_BUFFER_LENGTH = 1024;
  char pathNameBuffer[FILENAME_BUFFER_LENGTH];
  char fileNameBuffer[PATHNAME_BUFFER_LENGTH];

  int currentOutImageIndex=0;
  int currentOutImageFolderIndex=0;

  bool saveAutoExposureFrame = false;

  bool running = false;

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

  //////////////////////////////////////////////////////////////////////////////////////////
  // Generate a cam directory under the main path
  // If directory already exist do nothing
  bool generateCamDir(int idx){    

    if (snprintf( pathNameBuffer, PATHNAME_BUFFER_LENGTH, "%s%s%d\0", 
		  saveSettings.outPath.c_str(), 
		  saveSettings.cameraDirectoryPrefix.c_str(), 
		  idx) > 0) {
      
      int rval = FileIO::makeDirectory( pathNameBuffer, false, true );
      if (rval == FileIO::DIR_ERROR)
	exit(-1);
      
      return true;
    }    
    return false;
  }

  char* generateImageDir( int camId )
  {

    switch (saveSettings.imageDirectoryPrefixType) {

    case Settings::DATETIME :
      {
	char timeBuf[64];
	time_t now = time(NULL);
	struct tm *p_tm = localtime( &now );
	strftime( timeBuf, sizeof(timeBuf), "%Y-%m-%d_%Hh%Mm%Ss", p_tm );

	if (snprintf( pathNameBuffer, PATHNAME_BUFFER_LENGTH, "%s%s%d/%s/\0", 
		      saveSettings.outPath.c_str(), 
		      saveSettings.cameraDirectoryPrefix.c_str(), 
		      camId,
		      timeBuf
		      ) > 0  )
	  {
	    int rval = FileIO::makeDirectory( pathNameBuffer, false, true );
	    if (rval < 0)
	      return NULL;
	    else
	      return pathNameBuffer;
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

	  
	  if (snprintf( pathNameBuffer, PATHNAME_BUFFER_LENGTH, "%s%s%d/%s%06d/", 
			saveSettings.outPath.c_str(), 
			saveSettings.cameraDirectoryPrefix.c_str(),
			camId,			
			saveSettings.imageDirectoryPrefix.c_str(),
			currentOutImageFolderIndex
			) > 0) {

	    int rval = FileIO::makeDirectory( pathNameBuffer, false, false );

	    if (rval == 0){
	      uniqueName = true;
	      return pathNameBuffer;
	    }	  
	    else if (rval == FileIO::DIR_EXISTS)
	      currentOutImageFolderIndex++;

	    else if (rval == FileIO::DIR_ERROR){
	      std::cout << "Error creating image directory: " << pathNameBuffer << std::endl;
	      return NULL;
	    }

	  }
	}
	break;
      }
    default ://NONE
      {
	if (snprintf( pathNameBuffer, PATHNAME_BUFFER_LENGTH, "%s%s%d/", 
		      saveSettings.outPath.c_str(), 
		      saveSettings.cameraDirectoryPrefix.c_str(), 
		      camId
		      ) > 0) {
	  return pathNameBuffer;
	}else
	  return NULL;
	break;
      }
    }//end switch

  }

  char* generateImageName(char *path, int *p_lastIndex)
  {
    if (! FileIO::dirExist(path) ){ return NULL; }

    int idx=0;
    if (p_lastIndex != NULL) {
      idx = *p_lastIndex + 1;
    }

    bool uniqueName=false;      
    char bufNumberedPrefix[256];
    
    //Iterate through suggested filenames until a free one is found   
    while (!uniqueName){

      snprintf( bufNumberedPrefix, 256, saveSettings.filenamePrefix.c_str(), idx );

      if (snprintf( fileNameBuffer, FILENAME_BUFFER_LENGTH, "%s%s%s/", 
		    path, // a slash terminated path is expected
		    bufNumberedPrefix,
		    saveSettings.filenameSuffix.c_str() ) > 0)
	{

	  if ( !FileIO::fileExist(fileNameBuffer) ) {
	    uniqueName = true;
	    if (p_lastIndex != NULL)
	      *p_lastIndex = idx;

	    return fileNameBuffer;
	  }	  
	  else 
	    idx++;	    	    
	}
    }  
    
  }//end generateImageName



  //########################################
  //FEEL FREE TO CLEAN UP
  void run_debug_filenames()
  {
    std::cout << "***** DEBUG filenames *****" << std::endl;
        
    char *p_CurrPath;
    char *p_CurrName;
    while(1){
      
      for (int camId = 1; camId < cameraIds.size()+1 ; camId++){
	p_CurrPath = generateImageDir(camId);
	if (p_CurrPath == NULL){
	  std::cerr << "A NULL path encountered [generateImageDir]";
	  exit(-1);
	}

	currentOutImageIndex = 0;

	for (int imId =0; imId < experimentSettings.imageStack.size(); imId++){	     

	  p_CurrName = generateImageName(p_CurrPath, &currentOutImageIndex );

	  if (p_CurrName == NULL){
	    std::cerr << "A NULL file name encountered [generateImageName]";
	    exit(-1);
	  }

	  std::ofstream outFile( p_CurrName );	  
	  outFile << "DEBUG testFile" << std::endl;
	  outFile.close();
	  std::cout << "Created file: " << p_CurrName << std::endl;

	}
      }
      
      sleep( experimentSettings.captureInterval );

    }//end while 1
  }
 
  void run_test_tiff_saving()
  {
    short data[100][100];
    for (int i=0;i<100;i++)
      for (int j=0;j<100;j++)
	data[i][j]=i*i;

    commonImage::commonImage_t image;
    image.width = 100;
    image.height = 100;
    image.mode = commonImage::Gray14bpp;
    image.data = (void*)(data);

    std::string path = "./debug100x100.tiff";
    commonImage::saveTIFF(path.c_str(), &image, commonImage::COMPRESSION_NONE, true);
    
    
  }

  void run_test_tiff_load()
  {
    commonImage::commonImage_t image;

    std::string fileIn = "./debug100x100.tiff";
    commonImage::readTIFF( fileIn.c_str(), &image );

    if (! image.data )
      std::cout << "Error loading image: " << fileIn << std::endl;
    else{

      short *pData = (short*)(image.data);
      int count = image.height * image.width;
      while (count-- > 0)
	  *pData = 65535-*pData++;
      
      commonImage::saveTIFF( "./debug100x100_load_invert_test.tiff", &image );

    }

    

  }

  //END FEEL FREE TO CLEAN UP
  //########################################

}//end namespace VisMe
