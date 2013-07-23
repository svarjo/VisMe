/*****************************************************************************************
 * main.cpp
 *
 * Main entrypoint for the camera control software
 * Currently only GigE cameras from AVT are supported (see camCtrlVmbAPI.h)
 *
 *
 *
 *  Sami Varjo 2013
 *
 *****************************************************************************************/

#include "camCtrlVmbAPI.h"
#include "iniReader.h"

using namespace VisMe;

std::string defaultSetupFileName = "setup.ini";

int main(int argc, char** argv)
{ 
  std::string setupFileName; 
  if (argc < 2) {
    setupFileName = defaultSetupFileName;
  } else {
    setupFileName = std::string(argv[1]);
  }

  Settings::saveSettings_t saveSettings;
  Settings::getSaveSettings( &saveSettings, setupFileName );

  CamCtrlVmbAPI *camCtrl = new CamCtrlVmbAPI();
  
  delete camCtrl;
}
