/**
 * @file camCtrlVmbAPI.h
 *
 * @section DESCRIPTION
 *
 * Implement the camCtrlInterface for AVT cameras (GigE and USB) using Vimba SDK 
 *
 *          
 * @author Sami Varjo 2013
 */

#ifndef VISME_CAMCTRL_VMBAPI_H
#define VISME_CAMCTRL_VMBAPI_H

#include <VimbaCPP/Include/VimbaCPP.h>

#include "settings.h"
#include "camCtrlInterface.h"
#include "GT1290Camera.h"

using namespace AVT::VmbAPI;

namespace VisMe{

  class UserCameraFactory;
  class GigECamera;
  class USBCamera;

  typedef SP_DECL( UserCameraFactory )  UserCameraFactory_t;
  typedef SP_DECL( GT1290Camera ) 		GT1290Camera_t;
  //typedef SP_DECL( GigECamera )         GigECamera_t;
  typedef SP_DECL( USBCamera )          USBCamera_t;

  typedef SP_DECL( Camera )             Camera_t;


  /**
   * @class CamCtrlVmbAPI
   * 
   * A camera controler class for AVT Vimba controlled cameras. Here
   * the control is limited to cameras using GigE and USB interfaces. 
   */
  class CamCtrlVmbAPI : public CamCtrlInterface {

  public:

    CamCtrlVmbAPI(void);
   ~CamCtrlVmbAPI(void);

   //The functionality for CamCtrlInterface:
   void selectCamera( int id );
   void selectCamera( const char *pStrId );

   int captureImage( void* buffer = NULL ) ;
   void captureStream(  void ) ;
   
   void setParameter( camParam_t parameter, void *value, int valueByteSize );
   void getImageSize( int *width, int *height, int *channels, int *bitsPerPixel );

   int InitByIds( std::vector<std::string> IDlist );
   void freeCameras( void );

   //Own interfaces
   void stopCapture( void );


   /**
    * Initialize class and try to find all possible cameras using GigE or USB interfaces
    * @return negative value on error and zero with success. 
    */
   int InitAll(void);

   /**
    * Accessor function
    * @return an auto pointer to the currently selected camera
    */
   CameraPtr getSelectedCamera(void);
   
   /**
    * Accessor function
    * @return return the number of cameras found by the controller
    */
   int getNumberOfCameras(void);

   /**
    * Set the currently selected camera to the given settings
    * @param p_CamSet pointer to a Settings::cameraSettings_t
    */ 
   void setCameraToSettings ( Settings::cameraSettings_t *p_CamSet );


  private:
   
   void initVimba(void);
   void populateMyCameraVector(CameraPtrVector allCameras);
   int  setupGrayModeCameras(void);

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
