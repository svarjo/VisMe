#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstring>
 
#include <sys/stat.h>
#include <errno.h>

#include "fileIO.h"

#include "iniReader.h"
#include "settings.h"
#include "minIni.h"

const char* INI_DELIMITERS = " ,.|";


namespace VisMe{
  namespace Settings{

    const std::string SETUP_FILE_SUFFIX = ".ini";

    /******************************************************************************
     * populate camera idStrings from ini file
     */
    void getCameraIds(std::vector<std::string> &idStrings, const std::string & p_filename)
    {
      if (!FileIO::parameterFileExists(p_filename)){
	exit(-1);
      }      
      minIni ini(p_filename);

      int NCams;

      NCams = ini.geti( "Interface", "NumberOfCameras", 0);

      char camNameBuffer[16];
      
      for(int i = 1;i<=NCams;i++){
	sprintf(camNameBuffer, "CamID%d\0",i);
	std::string IDstr;
	
	IDstr = ini.gets( "Interface", camNameBuffer, "0.0.0.0");

	idStrings.push_back(IDstr);
      }

    }

    /*********************************************************************************
     * populate save settings structure from ini-file
     */
    void getSaveSettings(saveSettings_t *pSet, const std::string& p_filename)
    {

      if (!FileIO::parameterFileExists(p_filename)){
	exit(-1);
      }

      minIni ini(p_filename);     

      //Get the path, test if it exist or not and if not create a one
      pSet->outPath = ini.gets( "Saving", "Path", "./data");

      //Try to make the outPath
      int rval = FileIO::makeDirectory( pSet->outPath.c_str(), false, true);
      if (rval == -2){
	std::cout << "Output path [" << pSet->outPath <<
	  "] already exist!\n\tAre you sure to use old path? y/N" << std::endl;
	char key = getchar();
	if (key=='y' || key == 'Y' ){
	  std::cout << "Using existing output directory" << std::endl;	  
	}
	else{
	  std::cout << "Terminating..." << std::endl;
	  exit(0);
	}
      }
      else if (rval < 0 ){
	exit(rval);
      }

      pSet->cameraDirectoryPrefix = ini.gets( "Saving", "CameraDirectoryPrefix", "camera");

      std::string value;
      value = ini.gets( "Saving", "ImageDirectoryPrefixType", "running");
      if ( value == "datetime")
	pSet->imageDirectoryPrefixType = DATETIME;
      else if ( value == "running" ){
	pSet->imageDirectoryPrefixType = RUNNING;
	pSet->imageDirectoryPrefix = ini.gets( "Saving", "ImageDirectoryPrefix", "ImgSet");
      }
      else if ( value == "none")
	pSet->imageDirectoryPrefixType = NONE;
      
      pSet->filenamePrefix = ini.gets( "Saving", "FilenamePrefix", "image%05d");
      pSet->filenameSuffix = ini.gets( "Saving", "FilenameSuffix", ".tiff");

      value = ini.gets( "Saving", "Compress", "NONE" );

      if ( value == "none" )
	pSet->compression = NO;
      else if ( value == "LZW" )
	pSet->compression = LZW;
      else if ( value == "jpeg" )
	pSet->compression = JPEG;
    
    }//end void getSaveSettings(saveSettings_t *pSet, const std::string& p_filename)


    /*********************************************************************************
     * populate experiment settings structure from ini-file
     */
    void getExperimentSettings(experimentSettings_t *pSet,const std::string& p_filename)
    {
      if (!FileIO::parameterFileExists(p_filename)){
	exit(-1);
      }
      minIni ini(p_filename);

      pSet->id = 0;

      std::string value;
      value = ini.gets("Interface","Mode", "unknown");

      if (value == "ImageStackExpTime"){
	pSet->mode = IMAGE_STACK_EXPTIME;
      }
      else if (value == "single"){
	pSet->mode = SINGLE;
      }
      else if (value == "streaming_view"){
	pSet->mode = STREAMING_VIEW;
      }
      else if (value == "adaptive"){
	pSet->mode = ADAPTIVE;
      }
      else if (value == "externalSignal"){
	pSet->mode = EXTERNAL_SIGNAL;
      }
      else if (value == "debug" || value == "test"){
	pSet->mode = DEBUG_TESTING;
	pSet->id = ini.geti( "test", "id", 0 );
	}
      else{
	std::cout << "Unsupported mode encountered : " << value << "\nExiting..." <<  std::endl ;
	exit(-1);
      }
      std::cout << "\nUsing mode: " << value << std::endl;

      ////////////////////////////////////////////////////
      // Get first the common settings for all modes and
      // make a "initial" struct for all experiments
      pSet->preview = ini.getbool( "Interface", "ShowPreview", false);
      pSet->captureInterval = ini.geti( "Interface", "CaptureInterval", -1 );
      if (pSet->captureInterval < 0){
	std::cout << "[Interface] CaptureInterval is not defined.\nExitting..." << std::endl;
	exit(-1);
      }
      std::cout << "Capturing images every: " << pSet->captureInterval   << " second" << std::endl;

      cameraSettings_t camBaseSettings;
      camBaseSettings.autogain         = ini.getbool( "Sensor", "AutoGain", false);
      camBaseSettings.autoexposure     = ini.getbool( "Sensor", "AutoExposure", false);
      camBaseSettings.autowhitebalance = ini.getbool( "Sensor", "Whitebalance", false);
      camBaseSettings.autoiris         = ini.getbool( "Iris"  , "Auto", false);

      camBaseSettings.gain         = ini.getf( "Sensor", "Gain", 1.0);
      camBaseSettings.gamma        = ini.getf( "Sensor", "Gamma", 1.0);
      camBaseSettings.exposureTime = ini.getf( "Sensor", "ExposureTime", 1.0);
      camBaseSettings.iris         = ini.getf( "Iris", "Value", 1.0);
 
      /////////////////////////////////////////////
      // Handle each experiment mode and generate 
      // suitable camera parameter sets 
      switch(pSet->mode){
      case IMAGE_STACK_EXPTIME:
	{
	  int Nimages = ini.geti( "ImageStackExpTime", "NumberOfImages", 1 );
	  value = ini.gets( "ImageStackExpTime", "ExposureTimes", "250");
	  std::cout << "\tStack of " << Nimages << " images" << std::endl;
	  
	  char buffer[1024];	  
	  char *charValues = strncpy(buffer,  value.c_str(), 1023 );
	  buffer[1023]=0;
	  char *p;
	  p = strtok(charValues, INI_DELIMITERS);
	  std::cout << "\tExposure times: " ;
	  double exptime;
	  for (int imageId = 0; imageId < Nimages; imageId++){
	    if (p==NULL){
	      std::cout << "Unexpcted end of ExposureTimes string in [ImageStackExpTime]" << std::endl;
	      exit(-1);
	    }
	    exptime = atof(p);
	    p=strtok(NULL, INI_DELIMITERS);
	    std::cout << exptime << "ms ";

	    cameraSettings_t frameSetting;
	    frameSetting = camBaseSettings;

	    frameSetting.exposureTime = exptime;
	    pSet->imageStack.push_back(frameSetting);

	  }
	  std::cout << std::endl << std::endl;
	}
	break;

      default:
	pSet->imageStack.push_back(camBaseSettings);
	std::cout << "\tSingle image capture" << std::endl;

	break;
      }
      
      std::cout <<"\tautogain = " << camBaseSettings.autogain << " | gain=" << camBaseSettings.gain << std::endl;
      std::cout <<"\tautoexposure = " << camBaseSettings.autoexposure << " | expTime=" << camBaseSettings.exposureTime << std::endl;
      std::cout <<"\tautowhitebalance = " << camBaseSettings.autowhitebalance << std::endl;
      std::cout <<"\tautoiris = " << camBaseSettings.autoiris << " | value=" << camBaseSettings.iris << std::endl;
      std::cout <<"\tgamma val = " << camBaseSettings.gamma << std::endl << std::endl; 

      	

    }


  }//end namespace Settings
}//end namespace VisMe
