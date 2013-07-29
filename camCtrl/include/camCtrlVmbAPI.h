/******************************************************************************************
 * file: camCtrlVmbAPI.h
 *
 * Implement the camCtrlInterface for AVT cameras (GigE and USB) using Vimba SDK 
 *          
 * 2013 Sami Varjo
 ********************************************************************************************/

#ifndef VISME_CAMCTRL_VMBAPI_H
#define VISME_CAMCTRL_VMBAPI_H

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

   void captureImage( void* buffer = NULL ) ;
   void captureStream( void ) ;
   
   void setParameter( camParam_t parameter, void *value, int valueByteSize );
   void getImageSize( int *width, int *height, int *channels, int *bitsPerPixel );

   int InitByIds( std::vector<std::string> IDlist );
   void freeCameras( void );

   //Own interfaces
   int InitAll(void);
   CameraPtr getSelectedCamera(void);
   

  private:

   void initVimba(void);
   void populateMyCameraVector(CameraPtrVector allCameras);
   int  openGrayModeCameras(void);


   VimbaSystem &Vimba; //Note reference! 

   CameraPtrVector m_cameras;  // A vector of std::shared_ptr<AVT::VmbAPI::Camera> objects
   int m_selectedCameraId;
   CameraPtr pSelectedCamera;

   VmbInt64_t *m_payloadSize;   
   FramePtrVector *m_frames;

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
