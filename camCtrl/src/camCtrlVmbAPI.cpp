//#include <sstream>
#include <iostream>
#include <ostream>
#include <vector>

#include "camCtrlVmbAPI.h"

namespace VisMe{
  
  /********************************************************************
   * Default constructor for the camera controller 
   */ 
  CamCtrlVmbAPI::CamCtrlVmbAPI() : Vimba( AVT::VmbAPI::VimbaSystem::GetInstance() ) 
				   //OBS m_Vimba is reference to a Vimba system engine 
				   //and must be initialized at the beginning of the constructor.
  {
    Init();
  }

  /********************************************************************
   * Default destructor for the camera controller
   */
  CamCtrlVmbAPI::~CamCtrlVmbAPI()
  {
    if( !m_cameras.empty() ){
      freeCameras();
    }    
    Vimba.Shutdown();
  }

  /********************************************************************
   * Do the initialization of the Vimba api engine and populate cameras
   */
  int CamCtrlVmbAPI::Init()
  { 
    err = Vimba.Startup();
    if (err != VmbErrorSuccess ){
      std::cerr << "CamCtrlVmbAPI::Init : VIMBA sytem startup failed : "<< err << std::endl;
      return -1;
    }

    err = Vimba.QueryVersion( m_VimbaVersion );
    if (err != VmbErrorSuccess ){
      std::cerr << "CamCtrlVmbAPI::Init : version Query failed: " << err << std::endl;
      return -2;
    }
    std::cout << "Vimba " << m_VimbaVersion.major << "." << m_VimbaVersion.minor << " initialized" << std::endl;

    //Set the camera object creator
    ICameraFactoryPtr factPtr = UserCameraFactory_t(new UserCameraFactory());
    err = Vimba.RegisterCameraFactory(factPtr);
      
    if (err != VmbErrorSuccess){
      std::cerr << "CamCtrlVmbAPI::Init : CameraFactory not registered" << err << std::endl;
      return -2;
    }
    
    findCameras();
  }


  /********************************************************************
   *
   */
  int CamCtrlVmbAPI::findCameras()
  {

    err = Vimba.GetCameras(m_cameras);
    if (err != VmbErrorSuccess){
      std::cerr << "CamCtrlVmbAPI::findCameras : failed : " << err << std::endl;
      return -3;
    }


    VmbInterfaceType interfaceType;

    std::string strID;
    std::string strName;
    std::string strModelname;
    std::string strSerialNumber;
    std::string strInterfaceID;
    std::string strInfo;
   
    for ( CameraPtrVector::const_iterator iter = m_cameras.begin(); m_cameras.end() != iter; ++iter){

      err = (*iter)->GetID( strID );
      if ( VmbErrorSuccess != err )
	{
	  std::cerr << "CamCtrlVmbAPI::findCameras : Could not get camera ID. Error code: " << err << std::endl;
	}
                
      err = (*iter)->GetName( strName );
      if ( VmbErrorSuccess != err )
	{
	  std::cerr << "CamCtrlVmbAPI::findCameras : Could not get camera name. Error code: " << err << std::endl;
	}

      err = (*iter)->GetModel( strModelname );
      if ( VmbErrorSuccess != err )
	{
	  std::cerr << "CamCtrlVmbAPI::findCameras : Could not get camera mode name. Error code: " << err << std::endl;
	}

      err = (*iter)->GetSerialNumber( strSerialNumber );
      if ( VmbErrorSuccess != err )
	{
	  std::cerr << "CamCtrlVmbAPI::findCameras : Could not get camera serial number. Error code: " << err << std::endl;
	}

      err = (*iter)->GetInterfaceID( strInterfaceID );
      if ( VmbErrorSuccess != err )
	{
	  std::cerr << "CamCtrlVmbAPI::findCameras : Could not get interface ID. Error code: " << err << std::endl;
	}

       err = (*iter)->GetInterfaceType( interfaceType );
       if ( VmbErrorSuccess != err )	 
	 {
	   std::cerr << "CamCtrlVmbAPI::findCameras : Could not get camera interface. Error code: " << err << std::endl;
	   strInfo = "";
	 }
       else
	 {
	   strInfo = "none";
	   switch( interfaceType )
	     {
	     case VmbInterfaceEthernet: 
	       {
		 GigECamera_t gcam = SP_DYN_CAST( *iter, GigECamera );
		 if(gcam != NULL){
		   gcam->addonGigE(strInfo);
		   m_GigEcameras.push_back(gcam);
		 }
		 break;
	       }
	     case VmbInterfaceUsb: 
	       {
		 USBCamera_t ucam = SP_DYN_CAST( *iter, USBCamera );
		 if(ucam != NULL){
		   ucam->addonUSB(strInfo);
		   m_USBcameras.push_back(ucam);
		 }
		 break;
	       }
	     default:
	       {
		 std::cerr << "Unsupported camera type encountered" << std::endl;
		 break;
	       }
	     }
	 }
                    
       std::cout << "/// Camera Name: " << strName <<	\
	 std::endl << "/// Custom Info: " << strInfo << \
	 std::endl << std::endl;
	


      std::cout << "/// Camera Name: " << strName << \
	std::endl << "/// Model Name: " << strModelname << \
	std::endl << "/// Camera ID: " << strID << \
	std::endl << "/// Serial Number: " << strSerialNumber << \
	std::endl << "/// @ Interface ID: " << strInterfaceID << \
	std::endl << std::endl;
	
      //TODO populate struct for each camera...

    }

    std::cout << "FindCameras Stub" << std::endl;
  }


  /********************************************************************
   *
   */
  void CamCtrlVmbAPI::freeCameras()
  {

    if (!m_GigEcameras.empty()){
      m_GigEcameras.clear();
    }

    if (!m_USBcameras.empty()){
      m_USBcameras.clear();
    }

  }



  /*****************************************************************************/
  /* Override the virtual interface function                                   */
  /*****************************************************************************/

  void CamCtrlVmbAPI::openCamera( int camId )
  {
    //TODO
  }

  void CamCtrlVmbAPI::closeCamera( int camId )
  {

  }

  void CamCtrlVmbAPI::captureImage( BUFFER_TYPE *buffer )
  {

  }

  void CamCtrlVmbAPI::captureStream( BUFFER_TYPE *buffer )
  {

  }

  void CamCtrlVmbAPI::setParameter( camParam_t parameter, void *value, int valueByteSize)
  {
    
  }

  /*============================================================================*/

  /*=============================================================================
    GigECamera class
    =============================================================================*/

  GigECamera::GigECamera(     const char         *pCameraID,
			      const char         *pCameraName,
			      const char         *pCameraModel,
			      const char         *pCameraSerialNumber,
			      const char         *pInterfaceID,
			      VmbInterfaceType    interfaceType,
			      const char         *pInterfaceName,
			      const char         *pInterfaceSerialNumber,
			      VmbAccessModeType   interfacePermittedAccess )
    : Camera( pCameraID, pCameraName, pCameraModel, 
	      pCameraSerialNumber, pInterfaceID, interfaceType)
  {
  }

  // advanced camera function
  void GigECamera::addonGigE( std::string &info )
  {
    info = "ethernet interface connection detected";
  }

/*=============================================================================
    USBCamera class
=============================================================================*/

    USBCamera::USBCamera(      const char         *pCameraID,
                               const char         *pCameraName,
                               const char         *pCameraModel,
                               const char         *pCameraSerialNumber,
                               const char         *pInterfaceID,
                               VmbInterfaceType    interfaceType,
                               const char         *pInterfaceName,
                               const char         *pInterfaceSerialNumber,
                               VmbAccessModeType   interfacePermittedAccess )
        : Camera( pCameraID, pCameraName, pCameraModel, 
                  pCameraSerialNumber, pInterfaceID, interfaceType)
    {
    }
    
    // advanced camera function
    void USBCamera::addonUSB( std::string &info )
    {
        info = "usb interface connection detected";
    }

/*=============================================================================
    UserCameraFactory class
=============================================================================*/

  CameraPtr UserCameraFactory::CreateCamera(  const char *pCameraID,
					      const char *pCameraName,
					      const char *pCameraModel,
					      const char *pCameraSerialNumber,
					      const char *pInterfaceID,
					      VmbInterfaceType interfaceType,
					      const char *pInterfaceName,
					      const char *pInterfaceSerialNumber,
					      VmbAccessModeType interfacePermittedAccess)
  {
    // create camera class, depending on camera interface type
    if(VmbInterfaceEthernet == interfaceType)
      {
	return GigECamera_t( new GigECamera(pCameraID, 
					    pCameraName, 
					    pCameraModel, 
					    pCameraSerialNumber, 
					    pInterfaceID, 
					    interfaceType, 
					    pInterfaceName, 
					    pInterfaceSerialNumber, 
					    interfacePermittedAccess));
      }
    else if(VmbInterfaceUsb == interfaceType)
      {
	return USBCamera_t( new USBCamera(pCameraID, 
					  pCameraName, 
					  pCameraModel, 
					  pCameraSerialNumber, 
					  pInterfaceID, 
					  interfaceType, 
					  pInterfaceName, 
					  pInterfaceSerialNumber, 
					  interfacePermittedAccess));
      }
    else // unknown camera interface
      {
	// use default camera class
	return Camera_t( new Camera(pCameraID, 
				    pCameraName, 
				    pCameraModel, 
				    pCameraSerialNumber, 
				    pInterfaceID, 
				    interfaceType));
      }
  }
/*============================================================================*/

}//namespace VisMe
