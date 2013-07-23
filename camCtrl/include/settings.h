#ifndef SETTINGS_HEADER
#define SETTINGS_HEADER

#include <vector>
#include <string>
#include <cstdlib>

namespace VisMe{

  namespace Settings{

  typedef struct s_cameraSettings{

    bool autogain;
    bool autoexpsure;
    bool autowhitebalance;
    bool autoiris;

    double gamma;
    double gain;
    double iris;
    double exposureTime;

  }cameraSettings_t;

  enum experimentMode_t{ IMAGE_STACK, SINGLE, STREAMING_VIEW };

  typedef struct s_experimentSettings{

    experimentMode_t mode;   
    double captureInterval;
    std::vector<cameraSettings_t> imageStack;

  }experimentSettings_t;


  enum fileCompressionType_t{ NO, LZW, JPEG };

  enum imageDirectoryPrefixType_t{ DATETIME, RUNNING, NONE };

  typedef struct s_saveSettings{
    std::string outPath;
    std::string cameraDirectoryPrefix;
    imageDirectoryPrefixType_t imageDirectoryPrefixType;
    std::string filenamePrefix;
    std::string filenameSuffix;
    fileCompressionType_t compression;
  }saveSettings_t;


  }//end namespace Settings
}//end namespace VisMe

#endif
