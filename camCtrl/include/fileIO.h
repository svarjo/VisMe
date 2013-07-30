/**
 * @file fileIO.h
 *
 * @DESCRIPTION
 * Common filesystem routines (*nix)
 *
 * namespace:   FileIO::
 *
 * @author Sami Varjo 
 *****************************************************************/

#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>

#include <commonImage.h>

using namespace commonImage;
namespace FileIO{

  //ERROR CODES
  const int DIR_EXISTS = -2;
  const int DIR_ERROR  = -1;

  //API
  
  /**
   * Create a directory with possibility for terminating program with error and verbose output
   * @param path string presenting the path for directory
   * @param terminateOnError whether to end execution on error or not
   * @param verbose be verbose or not
   */
  int makeDirectory( const char *path, bool terminateOnError = true, bool verbose = true);

  /**
   * Test if a directory exist
   * @param path the location of the directory
   */
  bool dirExist( char *path );

  /**
   * Test if a file exist
   * @param path the location of the file
   */
  bool fileExist( char *path );

  /**
   * Test if a parameter file exist. The output is verbose compared to fileExist method and specific for a parameter file. 
   * @param filename a string of filename with full path
   */
  bool parameterFileExists(const std::string &filename); 



}; //end namespace FileIO

#endif 
