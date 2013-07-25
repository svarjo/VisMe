#include <cstdlib>
#include <iostream>
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
    m_payloadSize = NULL;
    m_frames = NULL;
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


  void CamCtrlVmbAPI::initVimba(void)
  {
    err = Vimba.Startup();
    if (err != VmbErrorSuccess ){
      std::cerr << "CamCtrlVmbAPI::Init : VIMBA sytem startup failed : "<< err << std::endl;
      exit(-1);
    }

    err = Vimba.QueryVersion( m_VimbaVersion );
    if (err != VmbErrorSuccess ){
      std::cerr << "CamCtrlVmbAPI::Init : version Query failed: " << err << std::endl;
      Vimba.Shutdown();
      exit(-2);
    }
    std::cout << "Vimba " << m_VimbaVersion.major << "." << m_VimbaVersion.minor 
	      << " initialized" << std::endl  << std::endl;

    //Set the camera object creator
    ICameraFactoryPtr factPtr = UserCameraFactory_t(new UserCameraFactory());
    err = Vimba.RegisterCameraFactory(factPtr);
      
    if (err != VmbErrorSuccess){
      std::cerr << "CamCtrlVmbAPI::Init : CameraFactory not registered" << err << std::endl;
      Vimba.Shutdown();
      exit(-3);
    }
    
  }
  

  /********************************************************************
   * Do the initialization of the Vimba api engine and populate cameras
   */
  int CamCtrlVmbAPI::InitAll(void)
  { 
    initVimba();    
    
    CameraPtrVector allCameras;
    err = Vimba.GetCameras(allCameras);

    if (err != VmbErrorSuccess){
      std::cerr << "CamCtrlVmbAPI::findCameras : failed : " << err << std::endl;      
      return -3;
    }
   
    populateMyCameraVector(allCameras);

    if (!m_cameras.empty()){
      selectCamera(0);
    }
    else{
      std::cout << "Not a single suitable camera was found!" << std::endl;
      return -1;
    }
    
    int ok = openGrayModeCameras();

    return ok;
  }

  /*********************************************************************************
   * Open the cameras and set the pixel format to gray 14 bbp (or fall back to 12,10
   * or 8 bits per pixel if not successful at 14 bbp)
   * Also populate m_payloadSize[id] array containging the size of captured frame for
   * each camera in m_cameras vector. In addition FramePtrVector array is created for 
   * cameras at m_frames[id] 
   */
  int CamCtrlVmbAPI::openGrayModeCameras(void)
  {
    m_payloadSize = new VmbInt64_t[m_cameras.size()];
    
    // Initialize (all) the found cameras
    //    for ( CameraPtrVector::iterator iter = m_cameras.begin(); m_cameras.end() != iter;  iter++)
    for ( int id = 0; id < m_cameras.size(); id++)
      {
	std::string strID;
	m_cameras[id]->GetID(strID);

	err = m_cameras[id]->Open( VmbAccessModeFull ); //Now camera should be open and usable.
	
	if (err != VmbErrorSuccess ){
	  std::cerr << "camCtrlVmbAPI::Init - could not open camera " << strID <<  " err: " << err << std::endl;
	  return(-3);
	}
	
	FeaturePtr feature;
	VmbInt64_t value;

	err = m_cameras[id]->GetFeatureByName( "PixelFormat", feature);
	if (err != VmbErrorSuccess){
	  std::cerr << "camCtrlVmbAPI::Init - could not get pixelFormat feature" << std::endl;
	}

	//Try to set as high precision Mono as possible
	err = feature->SetValue( VmbPixelFormatMono14 );
	if (err != VmbErrorSuccess ){ 
	  err = feature->SetValue( VmbPixelFormatMono12 );
	  if (err != VmbErrorSuccess ){ 
	    err = feature->SetValue( VmbPixelFormatMono10 );
	    if (err != VmbErrorSuccess ){ 
	      err = feature->SetValue( VmbPixelFormatMono8 );
	      if (err != VmbErrorSuccess ){ 
		std::cerr << "Could not set camera pixel format to gray (mono8-mono16)" << std::endl;
	      } else{ std::cout << "\tgray 8 bpp camera set" << std::endl; }
	    } else{ std::cout << "\tgray 10 bpp camera set" << std::endl; }
	  } else{ std::cout << "\tgray 12 bpp camera set" << std::endl; }
	} else{ std::cout << "\tgray 14 bpp camera set" << std::endl; }

	err = m_cameras[id]->GetFeatureByName( "PayloadSize", feature); //Required when aquiring frames
	if (err != VmbErrorSuccess ){
	  std::cerr << "camCtrlVmbAPI::Init - payloadSize not received from " << strID <<  " err: " << err << std::endl;
	  return(-3);
	}	
	feature->GetValue( value );
	m_payloadSize[id]=value;	
      }

    m_frames = new FramePtrVector[m_cameras.size()];
    return 0;
  }

  /********************************************************************
   * Go through the camera vector containing N cameras and populate
   * m_cameras vector with GigE and USB cameras. Open them and set the
   * initial mode to 
   */
  void CamCtrlVmbAPI:: populateMyCameraVector(CameraPtrVector allCameras){

    VmbInterfaceType interfaceType;

    std::string strID;
    std::string strName;
    std::string strModelname;
    std::string strSerialNumber;
    std::string strInterfaceID;
    std::string strInfo;

    for ( CameraPtrVector::const_iterator iter = allCameras.begin(); allCameras.end() != iter; ++iter){

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
                    

      std::cout << "/// Camera Name: " << strName <<	   \
	std::endl << "/// Model Name: " << strModelname <<	 \
	std::endl << "/// Camera ID: " << strID <<		 \
	std::endl << "/// Serial Number: " << strSerialNumber << \
	std::endl << "/// @ Interface ID: " << strInterfaceID << \
	std::endl << std::endl;
	
      //TODO populate struct for each camera...

    }

    std::cout << "\nFound " << m_cameras.size() << " AVT Vimba camera(s) " << std::endl;
    }


  /*********************************************************************************
   * Use a list (std::vector<std::string>) of camera ids to initialize cameras
   */
  int CamCtrlVmbAPI::InitByIds( std::vector<std::string> IDlist )
  {

    initVimba();

    CameraPtrVector idCameras;

    for (int i=0; i<IDlist.size();i++){

      CameraPtr pCamera;
      err=Vimba.OpenCameraByID(IDlist[i].c_str(), VmbAccessModeFull, pCamera);

      if (err != VmbErrorSuccess){
	std::cout << "Error while opening camera: " << IDlist[i] ;
	if (err == VmbErrorNotFound){
	  std::cout << " -camera not found (check the ID in setup file) " << std::endl;	
	}
	else{
	  std::cout << " -VmbStartup not done or unknown error encountered! code:" << err << std::endl;
	}
	continue;
      }
      idCameras.push_back(pCamera);
    }

    populateMyCameraVector(idCameras);

    if (!m_cameras.empty()){
      selectCamera(0);
    }
    else{
      std::cout << "Not a single suitable camera was found!" << std::endl;
      return -1;
    }
        
    int ok = openGrayModeCameras();

    return ok;
  }



  /********************************************************************
   *  Close the cameras and free resources for clean exit 
   */
  void CamCtrlVmbAPI::freeCameras()
  {

    if (!m_cameras.empty()){

      m_cameras.clear(); //Camera destructor implicitly closes the camera beforehand

    }

    if (m_payloadSize != NULL)
      delete m_payloadSize;

    if (m_frames != NULL)
      delete m_frames;

  }


  /*************************************************************************
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

  //Define the capture buffer (frame)
  FramePtr pFrame;
  pFrame.reset( new Frame( m_payloadSize[ m_selectedCameraId ] ) );
  err = pSelectedCamera->AnnounceFrame( pFrame );
  if (err != VmbErrorSuccess ){
    std::cerr << "CamCtrlVmbAPI::captureImage - could not AnnounceFrame for camera" << std::endl;
    return;
  }

  pSelectedCamera->StartCapture();
  pSelectedCamera->QueueFrame( pFrame );

  FeaturePtr pF;
  err = pSelectedCamera->GetFeatureByName( "AcquisitionStart", pF );
  if (err != VmbErrorSuccess){
    std::cerr << "CamCtrlVmbAPI::captureImage - cound not aquire AquisitionStart" << std::endl;
    return;
  }

  err = pF->RunCommand();
  if (err != VmbErrorSuccess){
    std::cerr << "CamCtrlVmbAPI::captureImage - error with AquisitionStart" << std::endl;
    return;    
  }

  m_frames[m_selectedCameraId].push_back(pFrame);  
  std::cout << "captureImage Stub" << std::endl;

}

void CamCtrlVmbAPI::captureStream( void )
{
  std::cout << "captureStream Stub" << std::endl;
}

void CamCtrlVmbAPI::setParameter( camParam_t parameter, void *value, int valueByteSize)
{
  
}


void CamCtrlVmbAPI::getImageSize(int *width, int *height, int *channels, int *bitsPerPixel)
{
  FeaturePtr feature;
  VmbInt64_t value;
  VmbPixelFormat_t pixFmt;

  if ( VmbErrorSuccess == pSelectedCamera->GetFeatureByName( "Width", feature ) ) {
    if ( VmbErrorSuccess == feature->GetValue( value ) ){
      *width = (int)(value);
    }
    else{
      std::cerr << "CamCtrlVmbAPI::getImageSize could not aquire image width" << std::endl;
    }
  }
  if ( VmbErrorSuccess == pSelectedCamera->GetFeatureByName( "Height", feature ) ) {
    if ( VmbErrorSuccess == feature->GetValue( value ) ){
      *height = (int)(value);
    }
    else{
      std::cerr << "CamCtrlVmbAPI::getImageSize could not aquire image width" << std::endl;
    }
  }

  if ( VmbErrorSuccess == pSelectedCamera->GetFeatureByName( "PixelFormat", feature ) ) {
    if ( VmbErrorSuccess != feature->GetValue( value ) ){
      std::cerr << "CamCtrlVmbAPI::getImageSize could not aquire image width" << std::endl;
    }
  }
  
  pixFmt = (VmbPixelFormat_t)(value);

  switch (pixFmt){

  case VmbPixelFormatMono8:
    *channels = 1;
    *bitsPerPixel = 8;
    break;

  case VmbPixelFormatMono10:
    *channels = 1;
    *bitsPerPixel = 10;
    break;

  case VmbPixelFormatMono12:
    *channels = 1;
    *bitsPerPixel = 12;
    break;

  case VmbPixelFormatMono12Packed:
    *channels = 1;
    *bitsPerPixel = 24; //OBS if 1 channel and 24 bits per pixel == packed 2x12 in 24 bits (3*8)
    break;

  case VmbPixelFormatMono14:
    *channels = 1;
    *bitsPerPixel = 14;
    break;

  case VmbPixelFormatMono16:
    *channels = 1;
    *bitsPerPixel = 16;
    break;

  default: 
    std::cerr << "Unsupported pixel depth encountered (only gray scale cameras currently supported" << std::endl;
    *channels = -1;
    break;
  }


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
