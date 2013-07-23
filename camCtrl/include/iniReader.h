#ifndef INI_READER_H
#define INI_READER_H

#include "minIni.h"

namespace VisMe{

  class iniReader {

  public:
    iniReader(const std::string& p_filename);
    ~iniReader();

  private:
    std::string filename;    
  };

}

#endif //INI_READER_H
