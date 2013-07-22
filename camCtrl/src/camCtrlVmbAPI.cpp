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
    //    Init();
  }

  /********************************************************************
   * Default destructor for the camera controller
   */
  CamCtrlVmbAPI::~CamCtrlVmbAPI()
  {
    if( !m_cameras.empty() ){
      freeCameras();
    }    
    Vimba.UnregisterCameraFactory();
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
    pSelectedCamera = CameraPtr();
    pSelectedCamera = m_cameras[0];   

    pPayloadSize = new VmbInt64_t[m_cameras.size()];
    
    // Initialize (all) the found cameras
    for ( CameraPtrVector::iterator iter = m_cameras.begin();
	  m_cameras.end() != iter;
	  iter++)
      {
	std::string strID;
	(*iter)->GetID(strID);

	err = (*iter)->Open( VmbAccessModeFull );
	
	if (err != VmbErrorSuccess ){
	  std::cerr << "camCtrlVmbAPI::Init - could not open camera " << strID <<  " err: " << err << std::endl;
	  return(-3);
	}

	FeaturePtr feature;
	VmbInt64_t framePayloadSize;
	err == (*iter)->GetFeatureByName( "PayloadSize", feature);

	if (err != VmbErrorSuccess ){
	  std::cerr << "camCtrlVmbAPI::Init - payloadSize not received from " << strID <<  " err: " << err << std::endl;
	  return(-3);
	}
      }

  }


  /********************************************************************
   * Use the Vimba API system to find all the cameras and populate the
   * m_cameras vector with GigE and USB cameras
   */
  int CamCtrlVmbAPI::findCameras()
  {

    CameraPtrVector allCameras;
    err = Vimba.GetCameras(allCameras);

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
		  m_cameras.push_back(gcam);
		}
		break;
	      }
	    case VmbInterfaceUsb: 
	      {
		USBCamera_t ucam = SP_DYN_CAST( *iter, USBCamera );
		if(ucam != NULL){
		  ucam->addonUSB(strInfo);
		  m_cameras.push_back(ucam);
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

    std::cout << "Found " << m_cameras.size() << " AVT Vimba cameras " << std::endl;
  }


  /********************************************************************
   *
   */
  void CamCtrlVmbAPI::freeCameras()
  {

    if (!m_cameras.empty()){
      m_cameras.clear();
    }

    delete pPayloadSize;

  }


  /*
   * Select the active camera by string descriptor (unique for each camera)
   */
  void CamCtrlVmbAPI::selectCamera( const char *pStrId )
  {
    if (  m_cameras.size() == 0 || pStrId == NULL)
      {
	return;
      }
    
    std::string pString(pStrId);

    for(int id = 0; id < m_cameras.size() ; id ++){

      std::string camIdstr;
      m_cameras[id]->GetID(camIdstr);

      if (camIdstr == pString){
	m_selectedCameraId = id;
	pSelectedCamera = m_cameras[id];
	break;
      }     
    }
  }


  /*****************************************************************************/
  /* Override the virtual interface functions                                  */
  /*****************************************************************************/

  /*
   * Select the active camera by enumeration id (int)
   */
  void CamCtrlVmbAPI::selectCamera( int id )
  {
    if ( id > m_cameras.size()-1 || id < 0 )
    {
       return;
    }

    m_selectedCameraId = id;
    pSelectedCamera = m_cameras[id];

  }


void CamCtrlVmbAPI::captureImage( void )
{
  std::cout << "captureImage Stub" << std::endl;
}

void CamCtrlVmbAPI::captureStream( void )
{
  std::cout << "captureStream Stub" << std::endl;
}

void CamCtrlVmbAPI::setParameter( camParam_t parameter, void *value, int valueByteSize)
{
  
}


void CamCtrlVmbAPI::getImageSize(int *width, int *height, int *channels)
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
    
// unique camera function(s)
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
