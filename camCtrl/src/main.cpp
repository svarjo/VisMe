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

using namespace std;
using namespace VisMe;

const char *defaultSetupFile = "setup.ini";

int main(int argc, char** argv)
{  
  CamCtrlVmbAPI *camCtrl = new CamCtrlVmbAPI();

  iniReader *iniIn = new iniReader(defaultSetupFile);
  
  delete camCtrl;
}
