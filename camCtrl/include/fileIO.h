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
  int makeDirectory( const char *path, bool terminateOnError = true, bool verbose = true);
  bool dirExist( char *path );
  bool fileExist( char *path );
  bool parameterFileExists(const std::string &filename); //A bit more verbose than fileExists


}; //end namespace FileIO

#endif 
