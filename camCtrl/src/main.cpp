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

using namespace std;
using namespace VisMe;


int main(int argc, char** argv)
{
  
  CamCtrlVmbAPI *camCtrl = new CamCtrlVmbAPI();
  camCtrl->Init();

  delete camCtrl;

}
