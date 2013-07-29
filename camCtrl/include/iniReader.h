/***************************************************************************
 * file: iniReader.h
 * 
 * Methods for accessing ini setup file and reading the data to settings.h
 * datastructures. 
 *
 * namespace:  VisMe::Settings::
 *
 * Sami Varjo 2013
 ***************************************************************************/
#ifndef INI_READER_H
#define INI_READER_H

#include <vector>
#include <string>
#include "settings.h"

namespace VisMe{

  namespace Settings{
    extern const std::string SETUP_FILE_SUFFIX;

    void getCameraIds(std::vector<std::string> &idStrings, const std::string & p_filename);
    void getSaveSettings(saveSettings_t *pSet, const std::string& p_filename);
    void getExperimentSettings(experimentSettings_t *pSet, const std::string& p_filename);

  }
}

#endif //INI_READER_H
