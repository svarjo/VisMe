#ifndef SETTINGS_HEADER
#define SETTINGS_HEADER

#include <vector>
#include <string>

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

    int numberOfCameras;
    experimentMode_t mode;   
    double captureInterval;
    bool showPreview;
    std::vector<cameraSettings_t> imageStack;

  }experimentSettings_t;


  enum fileCompressionType_t{ NONE, LZW, GZIP };

  typedef struct s_saveSettings{
    std::string outPath;
    std::string cameraDirectoryPrefix;
    std::string imageDirectoryPrefix;
    std::string FilenamePrefix;
    std::string FilenameSuffx;
    fileCompressionType_t compression;
  }saveSettings_t;


  }//end namespace Settings
}//end namespace VisMe

#endif
