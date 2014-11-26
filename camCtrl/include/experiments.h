/*********************************************************************
 * experiments.h
 *
 * Define the prototypes for the experiments available for main
 * with the required global variables containing various 
 * settings from ini file. (VisMe project)
 *
 * namespace:  VisMe::
 *
 * S.Varjo 2013
 *********************************************************************/

#ifndef _EXPERIMENTS_H_
#define _EXPERIMENTS_H_

#include <string>
#include "settings.h"
#include "camCtrlVmbAPI.h"
#include "commonImage.h"

namespace VisMe
{  
 //Common GLOBAL variables in VisMe for the experiments
 extern const std::string DEFAULT_SETUP_FILE_NAME;
 extern Settings::saveSettings_t saveSettings;
 extern Settings::experimentSettings_t experimentSettings;
 extern std::vector<std::string> cameraIds;
 extern CamCtrlVmbAPI *camCtrl;
 extern std::string setupFileName; 
 extern Settings::adaptiveSettings_t adaptiveSettings;

 // extern bool running;

 //Misc shared variables required by the experiment settings (ok one could/should make a class of parameters, extend it for each experiment - but...)
  //The experiments
  void run_image_stack_capture();
  void run_single_capture(int camId=0, unsigned int expTime_us=0);
  void run_streaming_view();
  void run_adaptive_hdr_capture();
  void run_external_signal_capture();

  bool generateCamDir(int idx, char *pathNameBuffer);
  char* generateImageDir(int camId, char *pathOut);
  char* generateImageName(char *path, char *nameOut, int *p_lastIndex = NULL);

  unsigned int subAverage(void *buf, int dataLength, int numPoints, int offset);
  unsigned int adaptiveHDRautoexp( int maxExpTime, int targetVal, void *imgBuffer);

  void saveImage( char *nameBuff, commonImage::commonImage_t *imBuff,
		  	  	  Settings::fileCompressionType_t compression, bool verbose );

  void cleanExit(std::string msg);


  //############################
  //TESTING (TODO CLEAN UP)
  void run_debug_filenames();
  void run_test_tiff_saving();
  void run_test_tiff_load(char * fileIn);

}


#endif //EXPERIMENTS_H
