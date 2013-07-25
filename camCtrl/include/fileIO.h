#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>

namespace FileIO{

  const int DIR_EXISTS = -2;
  const int DIR_ERROR  = -1;

  int makeDirectory( const char *path, bool terminateOnError = true, bool verbose = true);
  bool parameterFileExists(const std::string &filename);


}; //end namespace FileIO

#endif 
