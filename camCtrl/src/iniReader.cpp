#include <cstdlib>
#include <iostream>
#include <cstdio>

#include <sys/stat.h>
#include <errno.h>

#include "iniReader.h"
#include "settings.h"
#include "minIni.h"

namespace VisMe{
  namespace Settings{

    void getSaveSettings(saveSettings_t *pSet, const std::string& p_filename)
    {
      minIni ini(p_filename);

      //Get the path, test if it exist or not and if not create a one
      pSet->outPath = ini.gets( "Saving", "Path", "./data");

      struct stat st;
      int statOK = stat( pSet->outPath.c_str(), &st) ;
      if (statOK == 0 && S_ISDIR(st.st_mode) ) {
	
	std::cout << "Output path [" << pSet->outPath <<
	  "] already exist! Do you really want to overwrite? y/N" << std::endl;

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
    }


    void getExperimentSettings(experimentSettings_t *pSet,const std::string& p_filename)
    {
      minIni ini(p_filename);
    }

  }//end namespace Settings
}//end namespace VisMe
