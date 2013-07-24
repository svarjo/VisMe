#ifndef INI_READER_H
#define INI_READER_H

#include <vector>
#include "settings.h"

namespace VisMe{

  namespace Settings{

    void getCameraIds(std::vector<std::string> &idStrings, const std::string & p_filename);
    void getSaveSettings(saveSettings_t *pSet, const std::string& p_filename);
    void getExperimentSettings(experimentSettings_t *pSet, const std::string& p_filename);

  }
}

#endif //INI_READER_H
