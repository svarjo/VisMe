/*****************************************************************
 * fileIO.cpp
 *
 * implement fileIO.h
 *
 * Sami Varjo 2014
 *****************************************************************/

#include "fileIO.h"

#include <cstdlib>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <algorithm>    // std::sort

#ifdef _WIN32
	#include "dirent.h" //use a local version
	#include <direct.h>
#else
	#include <dirent.h>
#endif

namespace FileIO
{

  int makeDirectory( const char *path, bool terminateOnError, bool verbose)
  {
    
    struct stat st;
    int rval=0;
    
    if ( stat( path, &st ) != 0){ //Do not exist already

#ifdef _WIN32
    if ( _mkdir( path ) != 0){  //error while creating
#else
	if ( mkdir( path, S_IRWXU|S_IRWXG) != 0){  //error while creating
#endif

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


  /******************************************************************************
   * check if the path exist
   */
  bool dirExist( char *path )
  {
    struct stat st;
    int statOK = stat( path ,&st);
    if (statOK != 0 || !S_ISDIR(st.st_mode) )
      return false;
    else 
      return true;
  }
  /******************************************************************************
   * Check if reqular file exist
   */
  bool fileExist( char *path )
  {
    struct stat st;
    int statOK = stat( path,&st);
    if (statOK != 0 || !S_ISREG(st.st_mode) )
      return false;
    else 
      return true;
  }


  /******************************************************************************
   * Get files in given folder pointed by the path   
   * the file name is required to have the given prefix or suffix.
   * No wildcard matching is used (exact matches) as default "*" is used to match any
   * The dir entities . and .. are skipped from the list
   */
  void getFileNames(std::vector<std::string>  &files, std::string path, std::string prefix, std::string suffix)
 {	
	DIR *dp;
	struct dirent *ep;
	
	dp = opendir(path.c_str());
	if (dp != NULL){
					
		while( (ep = readdir( dp )) != NULL ){
		
			bool ok = true;
			std::string fname = ep->d_name;
			
			if (fname == "." || fname == ".."){ continue; }
			
			if ( prefix != "*" && prefix.length() > 0) {
				if ( fname.compare( 0, prefix.length(), prefix) != 0 ){
					ok = false;
				}	
			}
			
			if ( suffix != "*" && suffix.length() > 0 ) {				
				if (fname.length() < suffix.length()){
					ok = false;
				}else
				if ( fname.compare( fname.length()-suffix.length(), suffix.length(), suffix) != 0 ){
					ok = false;
				}
			}
			
			if (ok){						
				files.push_back(fname);
			}						
		}
		closedir(dp);
		
	}
	//Make the names ascending
	std::sort( files.begin(), files.end());
	
 }


}
