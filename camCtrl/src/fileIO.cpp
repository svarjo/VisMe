#include "fileIO.h"

#include <cstdlib>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>

namespace FileIO
{

  int makeDirectory( const char *path, bool terminateOnError, bool verbose)
  {
    
    struct stat st;
    int rval=0;
    
    if ( stat( path, &st ) != 0){ //Do not exist already
	
      if ( mkdir( path, S_IRWXU|S_IRWXG) != 0){  //error while creating

	if (verbose){

	  std::cout << "Error while creating [" << path <<"]" << std::endl;
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
	}
	rval = DIR_ERROR;
      }
    }
    else{
      if (verbose){
	std::cout << "File or directory [" << path << "] already exists." << std::endl;
      }
      rval = DIR_EXISTS;
    }
    if (rval < 0 && terminateOnError)
      exit(rval);
    else
      return rval;

  }//end makeDirectory
  

  /******************************************************************************
   * check if the parameter file exist
   */
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
  }//end parameterFileExists
}
