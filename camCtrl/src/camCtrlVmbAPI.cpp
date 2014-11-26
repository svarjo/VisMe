/******************************************************************************************
 * file: camCtrlVmbAPI.cpp
 *
 * Implement the camCtrlVmbAPI.h          
 *
 * 2013 Sami Varjo
 *
 ********************************************************************************************/
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstring>

#include "camCtrlVmbAPI.h"
#include "experiments.h"
#include "GT1290Camera.h"


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

  /**
   * First thing to do with the cam controller. Initializes Vimba-system and assigns
   *  the camera factory for the sytem.
   */
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

    if ( !m_cameras.empty() ){

      for (int i = 0; i<m_cameras.size(); i++){
    	  selectCamera(i);
    	  std::string strID;
    	  m_cameras[i]->GetID(strID);
    	  cameraIds.push_back(strID);
      }
    }
    else{
      std::cout << "Not a single suitable camera was found!" << std::endl;
      return -1;
    }

    return 0;
  }

  /*********************************************************************************
   * Set the pixel format to gray 14 bbp (or fall back to 12,10
   * or 8 bits per pixel if not successful at 14 bbp)
   * Also populate m_payloadSize[id] array containing the size of captured frame for
   * each camera in m_cameras vector. In addition FramePtrVector array is created for 
   * cameras at m_frames[id] 
   */
  int CamCtrlVmbAPI::setupGrayModeCameras(void)
  {
    m_payloadSize = new VmbInt64_t[m_cameras.size()];
    
    // Initialize (all) the found cameras
    //    for ( CameraPtrVector::iterator iter = m_cameras.begin(); m_cameras.end() != iter;  iter++)
    for ( int id = 0; id < m_cameras.size(); id++)
      {
    	//ALREADY OPEN CAMERAS EXPECTED!
    	std::string strID;
    	m_cameras[id]->GetID(strID);
    	//VmbAccessModeType allowedAccessType;
    	//m_cameras[id]->GetPermittedAccess( allowedAccessType );
    	//std::cout << "DBG::Trying to open camera " << strID << " using " << allowedAccessType << std::endl;
    	//std::cout << "DBG::Trying to open camera " << strID << " using " << VmbAccessModeFull << std::endl;
    	//err = m_cameras[id]->Open( allowedAccessType ); //Now camera should be open and usable.
    	//err = m_cameras[id]->Open( VmbAccessModeFull ); //Now camera should be open and usable.
    	//if (err != VmbErrorSuccess ){
    	//	std::cerr << "camCtrlVmbAPI::Init - could not open camera " << strID <<  " err: " << err << std::endl;
    	//	return(-3);
    	//}
	
    	FeaturePtr feature;
    	VmbInt64_t value;

		err = m_cameras[id]->GetFeatureByName("PixelFormat", feature);
		if (err != VmbErrorSuccess) {
			std::cerr
					<< "camCtrlVmbAPI::Init - could not get pixelFormat feature"
					<< std::endl;
		}

		//Try to set as high precision Mono as possible
		err = feature->SetValue(VmbPixelFormatMono14);
		if (err != VmbErrorSuccess) {
			err = feature->SetValue(VmbPixelFormatMono12);
			if (err != VmbErrorSuccess) {
				err = feature->SetValue(VmbPixelFormatMono10);
				if (err != VmbErrorSuccess) {
					err = feature->SetValue(VmbPixelFormatMono8);
					if (err != VmbErrorSuccess) {
						std::cerr
								<< "Could not set camera pixel format to gray (mono8-mono16)"
								<< std::endl;
					} else {
						std::cout << "\tgray 8 bpp camera set" << std::endl;
					}
				} else {
					std::cout << "\tgray 10 bpp camera set" << std::endl;
				}
			} else {
				std::cout << "\tgray 12 bpp camera set" << std::endl;
			}
		} else {
			std::cout << "\tgray 14 bpp camera set" << std::endl;
		}

		err = m_cameras[id]->GetFeatureByName("PayloadSize", feature); //Required when aquiring frames
		if (err != VmbErrorSuccess) {
			std::cerr << "camCtrlVmbAPI::Init - payloadSize not received from "
					<< strID << " err: " << err << std::endl;
			return (-3);
		}
		feature->GetValue(value);
		m_payloadSize[id] = value;
	}

	m_frames = new FramePtrVector[m_cameras.size()];
	return 0;
  }

  /********************************************************************
   * Go through the camera vector containing N cameras and populate
   * m_cameras vector with GigE and USB cameras. Open them and set the
   * initial mode to gray...
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
	    	  GT1290Camera_t gcam = SP_DYN_CAST( *iter, GT1290Camera );
	    	  if(gcam != NULL) {
	    		  m_cameras.push_back(gcam);
	    	  }

	          // Set the GeV packet size to the highest possible value
	          FeaturePtr pCommandFeature;
	          if ( VmbErrorSuccess == gcam->GetFeatureByName( "GVSPAdjustPacketSize", pCommandFeature ))
	          {
	              if ( VmbErrorSuccess == pCommandFeature->RunCommand() )
	              {
	                  bool bIsCommandDone = false;
	                  do
	                  {
	                      if ( VmbErrorSuccess != pCommandFeature->IsCommandDone( bIsCommandDone ))
	                      {
	                          break;
	                      }
	                  } while ( false == bIsCommandDone );
	              }
	          }else{
	        	  cleanExit("Could not auto adjust GigE camera packet size!");
	          }



	    	  /*
	    	  FeaturePtr pFeature;
	    	  err = gcam->GetFeatureByName( "StreamBytesPerSecond", pFeature);
              if ( VmbErrorSuccess == err )
              {
                  VmbInt64_t nMin, nMax, nValue;
                  double rfBandwidth;

                  err = pFeature->GetValue( nValue );
                  if ( VmbErrorSuccess == err ) {

                	  err = pFeature->GetRange( nMin, nMax );
					  if ( VmbErrorSuccess == err )
					  {
						  rfBandwidth = (double)nValue / nMax;

						  //nValue = (VmbUint64_t)(rfBandwidth * nMax);
						  //err = pFeature->SetValue( nValue );
					  }
                  }
              }
              */
	    /*
		GigECamera_t gcam = SP_DYN_CAST( *iter, GigECamera );
		if(gcam != NULL){
		  gcam->addonGigE(strInfo);
		  m_cameras.push_back(gcam);
		}
		*/
		break;
	      }
	    case VmbInterfaceUsb: 
	      {
		USBCamera_t ucam = SP_DYN_CAST( *iter, USBCamera );
		if(ucam != NULL){
		  ucam->addonUSB(strInfo);
		  m_cameras.push_back(ucam);
		  std::cout << "Added camera:"<<std::endl;
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

    }

    //Set defaults and populate other essentials
    if ( setupGrayModeCameras() != 0){
    	cleanExit("Error while setting up gray mode for cameras");
    }

    std::cout << "\nFound " << m_cameras.size() << " AVT Vimba camera(s) " << std::endl;
    }


  /*********************************************************************************
   * Use a list (std::vector<std::string>) of camera ids to initialize cameras
   * The cameras in IDlist are opened
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
        

    std::cout << "Cameras in CameraVector: " << m_cameras.size() << std::endl;

    int ok = 0;
    //    int ok = openGrayModeCameras();

    return ok;
  }



  /********************************************************************
   *  Close the cameras and free resources for clean exit 
   */
  void CamCtrlVmbAPI::freeCameras()
  {

    if (!m_cameras.empty()){
    	for (int camId = 0; camId < m_cameras.size(); camId++ ){
    		m_cameras[camId]->Close();
    	}
      m_cameras.clear(); //Camera destructor implicitly should close the camera
    }

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
  
  /*****************************************************************************
   * Get pointer of selected camera 
   */
  CameraPtr CamCtrlVmbAPI::getSelectedCamera(void){return pSelectedCamera;}

  /*****************************************************************************
   * Get number of cameras
   */
  int CamCtrlVmbAPI::getNumberOfCameras(void){ return m_cameras.size(); }
  

  ///////////////////////////////////////////////////////////////////////////////
  //
  //  APIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPIAPI

  /*****************************************************************************/
  /* Override the virtual interface functions                                  */
  /*****************************************************************************/

  /*****************************************************************************
   * Select the active camera by enumeration id (int) 0 - (Ncams-1)
   */
  void CamCtrlVmbAPI::selectCamera( int id )
  {
    if ( id > m_cameras.size()-1 || id < 0 )  { return; }

    m_selectedCameraId = id;
    pSelectedCamera = m_cameras[id];

  }

  /*****************************************************************************
   * Send request to capture an image from selected camera at FramePter pFrame
   * Synchronous action (ie blocking call)
   */
int CamCtrlVmbAPI::captureImage( void *buffer  )
{

	if (buffer == NULL)
		return -1;

	//Why I cannot pass own buffer to capture Image
	//std::cout << "CaptureImage - payoload: " << m_payloadSize[m_selectedCameraId] << std::endl;
  	//Frame newFrame( (VmbUchar_t *)(buffer), m_payloadSize[m_selectedCameraId] );
  	//FramePtr pOwnBuff;
  	//SP_SET(pOwnBuff, &newFrame);


	//WORKS:
	FramePtr pF; //!? auto pointer only - mem allocated automagically? - how to use own buff?

	std::string sCamModel;
	err = pSelectedCamera->GetModel(sCamModel);

	if (err != VmbErrorSuccess) {
		std::cerr << "Selected camera not found!" << std::endl;
		return err;
	}

	//Implement setParameter for GT1290 gray
	if (sCamModel.compare(0, 6, "GT1290") == 0) {

		GT1290Camera_t cam = SP_DYN_CAST( pSelectedCamera, GT1290Camera );
		err = cam->SetAcquisitionMode(GT1290Camera::AcquisitionMode_SingleFrame);

		if (err == VmbErrorSuccess){
			err = cam->AcquisitionStart();
			if (err == VmbErrorSuccess){
				double val;
				cam->GetExposureTimeAbs( val );
				int timeOut = 2000+(val/1000);  //Wait for frame exposure time + 1000 ms (50 ms skips many frames)
				err = cam->AcquireSingleImage( pF, timeOut);
			}
		}
		else{
			std::cerr << "Error while capturing single image. Error:" << err <<std::endl;
			std::cerr << "TODO find out problem - Quiting " << std::endl;
			exit( err );
			//return err;
		}


	}
	else{
		std::cerr << "captureImage :: unsupported camera model" << std::endl;
		return err;

	}

  if ( err == VmbErrorSuccess ) {
    // See if frame is not corrupt
    VmbFrameStatusType eReceiveStatus;
    err = pF->GetReceiveStatus( eReceiveStatus );

    //WORKS:
    if (    VmbErrorSuccess == err && VmbFrameStatusComplete == eReceiveStatus ){

      VmbUint32_t buffSize;
      pF->GetBufferSize( buffSize );

      VmbUchar_t *pIn;
      err = pF->GetImage( pIn );
	
      memcpy(buffer, pIn, buffSize);

      return 0;
    }

    else
    	std::cerr << "Frame skipped" << std::endl;
    	return -2;

  }

}


/*
void CamCtrlVmbAPI::captureImageAsyc( void ) 
 {

  FramePtr pFrame;

  IFrameObserverPtr pFO( new FrameObserver(pC, camId) );       
  err = pC->RegisterObserver(pC);

  //Define the capture buffer (frame)
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

  //  m_frames[m_selectedCameraId].push_back(pFrame);  
  //  std::cout << "captureImage Stub" << std::endl;

}
  */
void CamCtrlVmbAPI::stopCapture( void ) {
	pSelectedCamera->EndCapture();
	//pSelectedCamera->StopContinuousImageAcquisition();
};

void CamCtrlVmbAPI::captureStream( void  )
{
	//STUB DUMMY
	std::cout << "captureStream Implementation missing TODO" << std::endl;
/*
	GT1290Camera::AcquisitionModeEnum val = GT1290Camera::AcquisitionMode_Continuous;
	camCtrl->setParameter(CamCtrlInterface::PARAM_ACQUISITION_MODE,(void*)(&val), sizeof(bool));
	pSelectedCamera->StartCapture();
*/
/*
	FrameObserver myObs(pSelectedCamera);
	AVT::VmbAPI::shared_ptr<FrameObserver> pFO;
	SP_SET(pFO,&myObs);
	pSelectedCamera->StartContinuousImageAcquisition(2, pFO);
*/
}
  

void CamCtrlVmbAPI::setParameter(camParam_t parameter, void *value,	int valueByteSize) {

	std::string sCamModel;
	err = pSelectedCamera->GetModel(sCamModel);

	if (err != VmbErrorSuccess) {
		std::cerr << "Selected camera not found!" << std::endl;
		return;
	}

	//Implement setParameter for GT1290 gray
	if (sCamModel.compare(0, 6, "GT1290") == 0) {

		GT1290Camera_t cam = SP_DYN_CAST( pSelectedCamera, GT1290Camera );

		//std::cout << "setting parameter for " << sCamModel << std::endl;

		switch (parameter) {
		case PARAM_GAIN_AUTO:{

			bool setVal = *((bool*) (value));
			if (setVal){
				err = cam->SetGainAuto(GT1290Camera::GainAuto_Continuous);
			}else{
				err = cam->SetGainAuto(GT1290Camera::GainAuto_Off);
			}
			break;
		}
		case PARAM_EXPTIME_AUTO:{

			//OBS! Auto Appears to work only with continuous mode...
			bool setVal = *((bool*)(value));
			if (setVal){
				err=cam->SetExposureAuto(GT1290Camera::ExposureAuto_Continuous);
				//std::cout << "Set ExposureAuto_Continuous" << std::endl;
			}
			else{
				err=cam->SetExposureAuto(GT1290Camera::ExposureAuto_Off);
				//std::cout << "Set ExposureAuto_Off" << std::endl;
			}

			break;
		}
		case PARAM_WHITEBALANCE_AUTO:{
			//Gray camera - nothing to do
			break;
		}
		case PARAM_IRIS_AUTO:{

			bool setVal = *((bool*) (value));
			if (setVal)
				err = cam->SetIrisMode(GT1290Camera::IrisMode_PIrisAuto);
			else
				err = cam->SetIrisMode(GT1290Camera::IrisMode_Disabled);
			break;
		}
		case PARAM_GAMMA_VALUE:{

			double setVal = *((double*) (value));
			err = cam->SetGamma(setVal);
			break;
		}
		case PARAM_GAIN_VALUE:{

			err = cam->SetGainAuto(GT1290Camera::GainAuto_Off);

			double setVal = *((double*) (value));
			err = cam->SetGain(setVal);

			break;
		}
		case PARAM_IRIS_VALUE:{
			err = cam->SetIrisMode(GT1290Camera::IrisMode_PIrisManual);
			double dVal =  *((double*) (value)) ; //Note rounding to int
			err = cam->SetIrisAutoTarget((VmbInt64_t)(dVal+0.5));
			break;
		}
		case PARAM_EXPTIME_VALUE:{

			double setVal = (*((double*) (value)));
			err = cam->SetExposureTimeAbs (setVal);
			break;
		}
		case PARAM_EXPTIME_ONCE:{
			err = cam->SetExposureAuto(GT1290Camera::ExposureAuto_Once);
			std::cout << "Set ExposureAuto_Once" << std::endl;
			break;
		}

		case PARAM_ACQUISITION_MODE:{
			GT1290Camera::AcquisitionModeEnum val = (*(GT1290Camera::AcquisitionModeEnum*)(value));
			err = cam->SetAcquisitionMode(val);
			break;
		}
		default:{
			std::cerr << "camCtrlVmbAPI - unsupported parameter encountered: "
					<< parameter << std::endl;
			break;
		}
	  }
	  if (err != VmbErrorSuccess) {
			std::cerr << "setParameter::Error while setting parameter "	<< parameter << std::endl;
	  }


	} else {
		std::cout
				<< "CamCtrlVmbAPI::setParameter *IMPLEMENTATION MISSING* - unsupported camera model"
				<< std::endl;
		return;
	}

}


  /** 
   * Set selected camera to given set of parameters
   *
   * @param p_camSet pointer to the cameras settings struct 
   */  
  void CamCtrlVmbAPI::setCameraToSettings ( Settings::cameraSettings_t *p_CamSet )
  {

	setParameter(CamCtrlInterface::PARAM_GAMMA_VALUE, (void*) &p_CamSet->gamma, sizeof(double));
	setParameter(CamCtrlInterface::PARAM_GAIN_VALUE, (void*) &p_CamSet->gain, sizeof(double));
	setParameter(CamCtrlInterface::PARAM_IRIS_VALUE, (void*) &p_CamSet->iris, sizeof(double));
	setParameter(CamCtrlInterface::PARAM_EXPTIME_VALUE,	(void*) &p_CamSet->exposureTime, sizeof(double));

	setParameter(CamCtrlInterface::PARAM_GAIN_AUTO, (void*) &p_CamSet->autogain, sizeof(bool));
	setParameter(CamCtrlInterface::PARAM_EXPTIME_AUTO, (void*) &p_CamSet->autoexposure, sizeof(bool));
	setParameter(CamCtrlInterface::PARAM_WHITEBALANCE_AUTO, (void*) &p_CamSet->autowhitebalance, sizeof(bool));
	setParameter(CamCtrlInterface::PARAM_IRIS_AUTO, (void*) &p_CamSet->autoiris, sizeof(bool));

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
	  /*
      return GigECamera_t( new GigECamera(pCameraID, 
					  pCameraName, 
					  pCameraModel, 
					  pCameraSerialNumber, 
					  pInterfaceID, 
					  interfaceType, 
					  pInterfaceName, 
					  pInterfaceSerialNumber, 
					  interfacePermittedAccess));
      */

      return GT1290Camera_t( new GT1290Camera(  pCameraID,
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
