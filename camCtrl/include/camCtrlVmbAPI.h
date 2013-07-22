/*
 * file: camCtrl.h
 *
 * purpose: define the camera control class for hanlding cameras using 
 *          Vimba SDK from Advancded Vision Technologies. Hide the initialization
 *          of the camera handler, enumeration of the cameras, etc. 
 *
 *          Make public only the required access to the cameras (set parameter, capture image etc..)
 *          
 *
 */

#ifndef VISME_CMACTRL_H
#define VISME_CAMCTRL_H

#include <VimbaCPP/Include/VimbaCPP.h>

#include "camCtrlInterface.h"

using namespace AVT::VmbAPI;

namespace VisMe{

  
  class UserCameraFactory;
  class GigECamera;
  class USBCamera;

  typedef SP_DECL( UserCameraFactory )  UserCameraFactory_t;  
  typedef SP_DECL( GigECamera )         GigECamera_t;
  typedef SP_DECL( USBCamera )          USBCamera_t;
  typedef SP_DECL( Camera )             Camera_t;

  class CamCtrlVmbAPI : public CamCtrlInterface {

  public:
    CamCtrlVmbAPI(void);
   ~CamCtrlVmbAPI(void);

   //The functionality for CamCtrlInterface:
   void selectCamera( int id );
   void selectCamera( const char *pStrId );

   void captureImage( void ) ;
   void captureStream( void ) ;
   
   void setParameter( camParam_t parameter, void *value, int valueByteSize );
   void getImageSize( int *width, int *height, int *channels );


   //
   int Init();

  private:

   int findCameras();
   void freeCameras();

   VimbaSystem &Vimba; //Note reference! 

   CameraPtrVector m_cameras;  // A vector of std::shared_ptr<AVT::VmbAPI::Camera> objects
   int m_selectedCameraId;
   CameraPtr pSelectedCamera;

   VmbInt64_t *pPayloadSize;

   VmbVersionInfo_t m_VimbaVersion;
   VmbInterfaceType m_ifType;
   VmbErrorType err;

  };


/*=============================================================================*/
class GigECamera: 
 public Camera
{
public:
    GigECamera(     const char         *pCameraID,
                    const char         *pCameraName,
                    const char         *pCameraModel,
                    const char         *pCameraSerialNumber,
                    const char         *pInterfaceID,
                    VmbInterfaceType    interfaceType,
                    const char         *pInterfaceName,
                    const char         *pInterfaceSerialNumber,
                    VmbAccessModeType   interfacePermittedAccess );

    void addonGigE( std::string &info );    // custom camera function

};  

class USBCamera: 
    public Camera
{
public:
    USBCamera(      const char         *pCameraID,
                    const char         *pCameraName,
                    const char         *pCameraModel,
                    const char         *pCameraSerialNumber,
                    const char         *pInterfaceID,
                    VmbInterfaceType    interfaceType,
                    const char         *pInterfaceName,
                    const char         *pInterfaceSerialNumber,
                    VmbAccessModeType   interfacePermittedAccess );
    
    void addonUSB( std::string &info );     // custom camera function
};  
 
/*======*/

class UserCameraFactory :
  public ICameraFactory
{
  public:

    CameraPtr CreateCamera( const char *pCameraID,
                            const char *pCameraName,
                            const char *pCameraModel,
                            const char *pCameraSerialNumber,
                            const char *pInterfaceID,
                            VmbInterfaceType interfaceType,
                            const char *pInterfaceName,
                            const char *pInterfaceSerialNumber,
                            VmbAccessModeType interfacePermittedAccess);


};


}; //namespace VisMe

#endif //VISME_CAMCTRL_H
