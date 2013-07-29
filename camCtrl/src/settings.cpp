#include "settings.h"

#include <camCtrlInterface.h>


namespace VisMe {

  void setCameraToSettings ( CamCtrlInterface *camCtrl,  Settings::cameraSettings_t *p_CamSet )
{
  camCtrl->setParameter( CamCtrlInterface::PARAM_GAIN_AUTO,     (void*)&p_CamSet->autogain,     sizeof(bool) );
  camCtrl->setParameter( CamCtrlInterface::PARAM_EXPTIME_AUTO,  (void*)&p_CamSet->autoexposure, sizeof(bool) );
  camCtrl->setParameter( CamCtrlInterface::PARAM_WHITEBALANCE_AUTO, (void*)&p_CamSet->autowhitebalance, sizeof(bool) );
  camCtrl->setParameter( CamCtrlInterface::PARAM_IRIS_AUTO,     (void*)&p_CamSet->autoiris, sizeof(bool) );

  camCtrl->setParameter( CamCtrlInterface::PARAM_GAMMA_VALUE,   (void*)&p_CamSet->gamma, sizeof(double) );
  camCtrl->setParameter( CamCtrlInterface::PARAM_GAIN_VALUE,    (void*)&p_CamSet->gain, sizeof(double) );
  camCtrl->setParameter( CamCtrlInterface::PARAM_IRIS_VALUE,    (void*)&p_CamSet->iris, sizeof(double) );
  camCtrl->setParameter( CamCtrlInterface::PARAM_EXPTIME_VALUE, (void*)&p_CamSet->exposureTime, sizeof(double) );

}


}
