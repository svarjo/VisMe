/******************************************************************************************
 * @file settings.h
 *
 * @section DESCRIPTION
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
 * @author Sami Varjo 2013
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

    bool autogain;            ///Use hardware gain control
    bool autoexposure;        ///Use hardware exposure time control
    bool autowhitebalance;    ///Use whitebalance (only RGB cameras - with gray scale this is neglected)
    bool autoiris;            ///Automatic adjustment of DC/P-Iris
 
    double gamma;             ///Gamma correction value (neglected if auto) 
    double gain;              ///Sensor gain value (neglected if auto) 
    double iris;              ///Iris setting (neglected if auto) 
    double exposureTime;      ///Exposure time in ms (neglected if auto) 

  }cameraSettings_t;

  enum experimentMode_t{ 
    IMAGE_STACK_EXPTIME, 
    SINGLE, 
    STREAMING_VIEW, 
    ADAPTIVE, 
    EXTERNAL_SIGNAL, 
    DEBUG_TESTING
  };

  typedef struct s_experimentSettings{
    
    int id;                   ///Experiment unique id (can be set)
    experimentMode_t mode;    ///Experiment mode as in experimentMode_t (IMAGE_STACK_EXPTIME, SINGLE, TEST,...)
    double captureInterval;   ///Interval to capture images in seconds. The actual start time will be used
    bool preview;             ///Show last image captured in a window 
    std::vector<cameraSettings_t> imageStack; //Settings for multiple captures 

  }experimentSettings_t;

  enum fileCompressionType_t{ NO, LZW, ZIP, JPEG, PACKBITS };

  enum imageDirectoryPrefixType_t{ DATETIME, RUNNING, NONE };

  typedef struct s_saveSettings{
    std::string outPath;                                   ///Main folder for data to be saved.
    std::string cameraDirectoryPrefix;                     ///Each camera controlled will have an own folder.
    imageDirectoryPrefixType_t imageDirectoryPrefixType;   ///Generation of folder name under the camera folder (DATETIME, RUNNING, NONE). DATETIME names image folder by the current time, RUNNING uses running number. If NONE is set the images are saved directly in the camera folder. 
    std::string imageDirectoryPrefix;    ///Prefix for the image directories
    std::string filenamePrefix;          ///Imagefilename prefix with sprintf numbering format. Eg img%05d is accepted and filename will be imgXXXXX.
    std::string filenameSuffix;          ///Ending of the filename ( eg .tiff)
    fileCompressionType_t compression;   ///Type of compression to be used. Tiff type compressions available NO, LZW, ZIP, JPEG, PACKBITS.
  }saveSettings_t;

  }//end namespace Settings

}//end namespace VisMe

#endif
