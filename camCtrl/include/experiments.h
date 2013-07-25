/*
 * Define the prototypes for the experiments available for main
 * with the required global variables containing various 
 * settings from ini file. 
 *
 * S.Varjo 2013
 */

#ifndef _EXPERIMENTS_H_
#define _EXPERIMENTS_H_

#include <string>
#include "settings.h"
#include "camCtrlVmbAPI.h"

namespace VisMe
{
  
 //Common GLOBAL variables in VisMe for the experiments
 extern const std::string DEFAULT_SETUP_FILE_NAME;
 extern Settings::saveSettings_t saveSettings;
 extern Settings::experimentSettings_t experimentSettings;
 extern std::vector<std::string> cameraIds;
 extern CamCtrlVmbAPI *camCtrl;
 extern std::string setupFileName; 

 extern bool running;
 // extern char outputFileName[1024];

 //Misc shared variables required by the experiment settings (ok one could/should make a class of parameters, extend it for each experiment - but...)
 // extern bool saveAutoExposureFrame;

  //The experiments
  void run_image_stack_capture();
  void run_single_capture();
  void run_streaming_view();

  bool generateCamDir(int idx);
  char* generateImageDir(int camId);
  char* generateImageName(char *path, int *p_lastIndex = NULL);

  void cleanExit(std::string msg);

  //############################
  void run_debug_filenames();
}


#endif //EXPERIMENTS_H
