/******************************************************************************************
 * file: settings.h
 *
 * typedefinitions for data structures in VisMe project:
 *
 *  - struct cameraSettings_t           : general settings for capturing data
 *  - struct experimentSettings_t       : settings for a given experiment mode
 *  - struct saveSettings_t             : saving information
 *  - enum imageDirectoryPrefixType_t
 *  - enum fileCompressionType_t
 *  - enum experimentMode_t
 *
 * namespace:  VisMe::Settings::
 *          
 * Sami Varjo 2013
 ********************************************************************************************/

#ifndef SETTINGS_HEADER
#define SETTINGS_HEADER

#include <vector>
#include <string>
#include <cstdlib>

#include <camCtrlInterface.h>

namespace VisMe{

  namespace Settings{

  typedef struct s_cameraSettings{

    bool autogain;
    bool autoexposure;
    bool autowhitebalance;
    bool autoiris;

    double gamma;
    double gain;
    double iris;
    double exposureTime;

  }cameraSettings_t;

  enum experimentMode_t{ IMAGE_STACK_EXPTIME, SINGLE, STREAMING_VIEW, ADAPTIVE, EXTERNAL_SIGNAL, DEBUG_TESTING};

  typedef struct s_experimentSettings{
    
    int id;
    experimentMode_t mode;   
    double captureInterval;
    bool preview;
    std::vector<cameraSettings_t> imageStack;

  }experimentSettings_t;

  enum fileCompressionType_t{ NO, LZW, ZIP, JPEG, PACKBITS };

  enum imageDirectoryPrefixType_t{ DATETIME, RUNNING, NONE };

  typedef struct s_saveSettings{
    std::string outPath;
    std::string cameraDirectoryPrefix;
    imageDirectoryPrefixType_t imageDirectoryPrefixType;
    std::string imageDirectoryPrefix;
    std::string filenamePrefix;
    std::string filenameSuffix;
    fileCompressionType_t compression;
  }saveSettings_t;

  }//end namespace Settings

  //API
  void setCameraToSettings ( CamCtrlInterface *camCtrl, Settings::cameraSettings_t *p_CamSet );


}//end namespace VisMe

#endif
