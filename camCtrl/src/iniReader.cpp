#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstring>
 
#include <sys/stat.h>
#include <errno.h>

#include "iniReader.h"
#include "settings.h"
#include "minIni.h"

const char* INI_DELIMITERS = " ,.|";


namespace VisMe{
  namespace Settings{

    bool parameterFileExists(const std::string &filename)
    {      
      struct stat st;
      int statOK = stat( filename.c_str(),&st);
      if (statOK != 0 ){
	std::cout << "Parameter file [" << filename << "] was not found." << std::endl;
	return false;
      }
      if (S_ISDIR(st.st_mode)){
	std::cout << "Given paremeter file is directory!" << std::endl;
	return false;
      }
      return true;
    }

    void getCameraIds(std::vector<std::string> &idStrings, const std::string & p_filename)
    {
      if (!parameterFileExists(p_filename)){
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


    void getSaveSettings(saveSettings_t *pSet, const std::string& p_filename)
    {

      if (!parameterFileExists(p_filename)){
	exit(-1);
      }

      minIni ini(p_filename);     

      //Get the path, test if it exist or not and if not create a one
      pSet->outPath = ini.gets( "Saving", "Path", "./data");

      struct stat st;
      int statOK;
      statOK = stat( pSet->outPath.c_str(), &st) ;
      if (statOK == 0 && S_ISDIR(st.st_mode) ) {
	
	std::cout << "Output path [" << pSet->outPath <<
	  "] already exist!\n\tDo you really want to overwrite? y/N" << std::endl;

	char key;
	key = getchar();
	if (key=='y' || key == 'Y' ){
	  std::cout << "Overwriting files with similar names" << std::endl;	  
	}
	else{
	  std::cout << "Terminating..." << std::endl;
	  exit(0);
	}

      }else{

	if (mkdir(pSet->outPath.c_str(), S_IRWXU|S_IRWXG)!=0) {

	  std::cout << "Error while creating [" << pSet->outPath <<"]" << std::endl;
	  switch (errno){
	  case EACCES:
	    std::cout << "\tNo write permission" << std::endl;
	    break;
	  case EEXIST:
	    std::cout << "\tFile already exist" << std::endl;
	    break;
	  case EMLINK:
	    std::cout << "\tParent has too many entries" << std::endl;
	    break;
	  case ENOSPC:
	    std::cout << "\tFile system space limit reached" << std::endl;
	    break;
	  case EROFS:
	    std::cout << "\tParent directory is read only" << std::endl;
	    break;
	  }	
	  exit(0);
	}	
      }//end path creation
      
      pSet->cameraDirectoryPrefix = ini.gets( "Saving", "CameraDirectoryPrefix", "camera");
      std::string value;
      value = ini.gets( "Saving", "ImageDirectoryPrefix", "running");
      if ( value == "datetime")
	pSet->imageDirectoryPrefixType = DATETIME;
      else if ( value == "running" )
	pSet->imageDirectoryPrefixType = RUNNING;
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


    void getExperimentSettings(experimentSettings_t *pSet,const std::string& p_filename)
    {
      if (!parameterFileExists(p_filename)){
	exit(-1);
      }
      minIni ini(p_filename);

      std::string value;
      value = ini.gets("Interface","Mode", "unknown");

      if (value == "ImageStackExpTime")
	pSet->mode = IMAGE_STACK_EXPTIME;
      else if (value == "single")
	pSet->mode = SINGLE;
      else if (value == "streaming_view")
	pSet->mode = STREAMING_VIEW;
      else if (value == "adaptive")
	pSet->mode = ADAPTIVE;
      else if (value == "externalSignal")
	pSet->mode = EXTERNAL_SIGNAL;
      else{
	std::cout << "Unsupported mode encountered : " << value << "\nExiting..." <<  std::endl ;
	exit(-1);
      }

      pSet->preview = ini.getbool( "Interface", "ShowPreview", false);
      
      cameraSettings_t camBaseSettings;
      camBaseSettings.autogain         = ini.getbool( "Sensor", "AutoGain", false);
      camBaseSettings.autoexposure     = ini.getbool( "Sensor", "AutoExposure", false);
      camBaseSettings.autowhitebalance = ini.getbool( "Sensor", "Whitebalance", false);
      camBaseSettings.autoiris         = ini.getbool( "Iris"  , "Auto", false);

      camBaseSettings.gain         = ini.getf( "Sensor", "Gain", 1.0);
      camBaseSettings.gamma        = ini.getf( "Sensor", "Gamma", 1.0);
      camBaseSettings.exposureTime = ini.getf( "Sensor", "ExposureTime", 1.0);
      camBaseSettings.iris         = ini.getf( "Iris", "Value", 1.0);
 
      int Nimages;
      switch(pSet->mode){
      case IMAGE_STACK_EXPTIME:
	{
	  Nimages = ini.geti( "ImageStackExpTime", "NumberOfImages", 1 );
	  value = ini.gets( "ImageStackExpTime", "ExposureTimes", "250");		
	  
	  char *charValues = (char*)( value.c_str() );

	  char *p;
	  p = strtok(charValues, INI_DELIMITERS);
	  double exptimes[Nimages];
	  for (int imageId = 0; imageId < Nimages; imageId++){
	    if (p==NULL){
	      std::cout << "Unexpcted end of ExporureTimes string in [ImageStackExpTime]" << std::endl;
	      exit(-1);
	    }
	    exptimes[imageId] = atof(p);
	    p=strtok(NULL, INI_DELIMITERS);
	  }
	  delete charValues;
	}
	break;

      default:
	Nimages = 1;
	break;
      }

	

    }


  }//end namespace Settings
}//end namespace VisMe
