#include "iniReader.h"
#include "settings.h"
#include "minIni.h"

namespace VisMe{
  namespace Settings{

    void getSaveSettings(saveSettings_t *pSet, const std::string& p_filename)
    {
      minIni ini(p_filename);
    }


    void getExperimentSettings(experimentSettings_t *pSet,const std::string& p_filename)
    {
      minIni ini(p_filename);
    }

  }//end namespace Settings
}//end namespace VisMe
