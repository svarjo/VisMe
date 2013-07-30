/* experiments.cpp
 *
 * impementation for experiments.h
 *
 * @author S.Varjo 2013
 */

#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "settings.h"
#include "experiments.h"
#include "fileIO.h"
#include "commonImage.h"

#include <opencv2/core/core.hpp>       // OpenCV is used only for 
#include <opencv2/highgui/highgui.hpp> // displaying images (ok bit bloated but easy...)

namespace VisMe{

  ///////////////////////////////////////////////////////////////////////
  //Global variables for experiment settings 
  ///////////////////////////////////////////////////////////////////////

  const std::string DEFAULT_SETUP_FILE_NAME = "setup.ini";
  Settings::saveSettings_t saveSettings;
  Settings::experimentSettings_t experimentSettings;
  std::vector<std::string> cameraIds;
  std::string setupFileName="undefined";

  CamCtrlVmbAPI *camCtrl = NULL;   //The camera controller

  /////////////////////////////////////////////////
  //Set of local variables
  /////////////////////////////////////////////////
  const int FILENAME_BUFFER_LENGTH = 1024;
  const int PATHNAME_BUFFER_LENGTH = 1024;

  //TODO FIX:
  //OBS never freed if chatched CTRL+C signal to end program...
  char **pathNameBuffer;
  char **fileNameBuffer;
  commonImage_t *imgBuffer;
  //END FIX

  int currentOutImageIndex=0;
  int currentOutImageFolderIndex=0;

  bool saveAutoExposureFrame = false;

  bool running = false;

  ///////////////////////////////////////////////////////////////////////
  //Experiment API
  ///////////////////////////////////////////////////////////////////////

  void run_image_stack_capture()
  {
    Settings::cameraSettings_t *p_CamSet;
    
    struct timeval starttime, endtime, timediff;

    int numCameras = cameraIds.size();  

    char windowNameBuffer[numCameras][128];
 
    imgBuffer = new commonImage_t[numCameras]; //size, type, data

    pathNameBuffer = new char*[numCameras];
    fileNameBuffer = new char*[numCameras];

    int cvImageType;

    //Init settings for each camera:
    for (int camId=0; camId < numCameras; camId++ ){
      
      //Make names buffer
      pathNameBuffer[camId] = new char[PATHNAME_BUFFER_LENGTH];
      fileNameBuffer[camId] = new char[FILENAME_BUFFER_LENGTH];

      //Use the first setting of image stack
      camCtrl->selectCamera(camId);
      camCtrl->setCameraToSettings(  &(experimentSettings.imageStack[0]) );     

      int w,h,c,bpp;
      camCtrl->getImageSize( &imgBuffer[camId].width,  &imgBuffer[camId].height, &c, &bpp );

      //Allocate memory for each data buffer, OBS possible memory leak with ctrl+c
      if (c==1 && bpp < 9){
	imgBuffer[camId].mode = commonImage::Gray8bpp;
	imgBuffer[camId].data = (void*)malloc( imgBuffer[camId].width*imgBuffer[camId].height*sizeof(char) );
	cvImageType = CV_8UC1;
      }
      else if (c==1 && bpp < 17){
	imgBuffer[camId].mode = commonImage::Gray16bpp;
	imgBuffer[camId].data = (void*)malloc( imgBuffer[camId].width*imgBuffer[camId].height*sizeof(char)*2 );
	cvImageType = CV_16UC1;
      }
      else if (c==3 && bpp ==8){
	imgBuffer[camId].mode = commonImage::RGB8bpp;
	imgBuffer[camId].data =  (void*)malloc( imgBuffer[camId].width*imgBuffer[camId].height*sizeof(char)*3 );
	cvImageType = CV_8UC3;
      }
      else if (c==4 && bpp ==8){
	imgBuffer[camId].mode = commonImage::RGBA8bpp;
	imgBuffer[camId].data = (void*)malloc( imgBuffer[camId].width*imgBuffer[camId].height*sizeof(char)*4 );
	cvImageType = CV_8UC4;
      }
      else{
	std::cerr << "Unsupported image format encountered : " << c 
		  << " channels with " << bpp << "bits per pixel" << std::endl;
	return;
      }

      if (imgBuffer[camId].data == NULL){
	std::cerr << "Error while allocating image buffers for experiment" << std::endl;
	return;
      }

      //Open one window per camera (if preview set)
      if (experimentSettings.preview){
	sprintf( windowNameBuffer[camId], "Camera %u", camId ); 
	cv::namedWindow( windowNameBuffer[camId] , CV_WINDOW_AUTOSIZE );
      }
    }

    char *pCurrPath;
    char *pCurrName;

    int fileNameId;
    
    while(1){
       
      gettimeofday( &starttime, NULL );
      fileNameId=0; //start naming each image set from 0 (assume own folders)

      //Generate new folder for this set of images (for each camera)
      for (int camId=0; camId < numCameras; camId++){
	generateImageDir( camId, pathNameBuffer[camId] );
      }
      
      //TODO
      //Should fork for each camera...
      for (int imageId=0; imageId < experimentSettings.imageStack.size(); imageId++ ) {
	p_CamSet = &(experimentSettings.imageStack[imageId]);

	for (int camId=0; camId < numCameras; camId++ ) {
	  camCtrl->selectCamera(camId);
	  camCtrl->setParameter( CamCtrlInterface::PARAM_EXPTIME_AUTO,  (void*)&p_CamSet->autoexposure, sizeof(bool) );
	  camCtrl->captureImage( imgBuffer[camId].data ); //Blocking call to capture image
	}

	//Wait for capture and store results (Now blocking for each camera)
	for (int camId=0; camId < numCameras; camId++ ) {
	  generateImageName( pathNameBuffer[camId], fileNameBuffer[camId], &fileNameId );
	  commonImage::saveTIFF( fileNameBuffer[camId], 
				 &imgBuffer[camId], 
				 commonImage::COMPRESSION_ZIP, true); //true = verbose
	}
      }

      //Show the last capture from the stack (could be the middle one...)
      if (experimentSettings.preview){
	for (int camId =0; camId < numCameras; camId++){
	  sprintf( windowNameBuffer[camId], "Camera %u", camId ); 
	  cv::namedWindow( windowNameBuffer[camId] , CV_WINDOW_AUTOSIZE );
		
	  cv::Mat imageIn( imgBuffer[camId].width, imgBuffer[camId].height, cvImageType, 
			   imgBuffer[camId].data, cv::Mat::AUTO_STEP);

	  cv::Mat image8bpp( imgBuffer[camId].width, imgBuffer[camId].height, CV_8UC1);
	
	  if (imgBuffer[camId].mode == Gray16bpp)
	    imageIn.convertTo(image8bpp, CV_8UC1, 255.0/65535.0); //OBS if underlying data is not really 16bit -> dark
	  else
	    imageIn.convertTo(image8bpp, CV_8UC1);

	  cv::imshow( windowNameBuffer[camId], image8bpp);
	}
	cv::waitKey(1); //Force image window update by waiting 1ms

      }

      //Wait for given interval
      gettimeofday( &endtime, NULL);
      timersub(&endtime, &starttime, &timediff);
      int sleepTime_sec = experimentSettings.captureInterval-timediff.tv_sec;

      if (sleepTime_sec < 0){	
	std::cout << "Warning! The image stack capture time is longer than the set captureInterval" << std::endl
		  << "set: " << experimentSettings.captureInterval <<  "s  time elapsed: " << timediff.tv_sec 
		  << "s" << std::endl;
      }
      else{
	sleep( sleepTime_sec );
	usleep ( timediff.tv_usec );
      }

    }
  }

  /**********************************************************************
   * Capture just single image from a gicen camera
   */
  void run_single_capture( int camId )
  {
    if (camId > camCtrl->getNumberOfCameras()-1) 
      return;

    commonImage_t imBuff;
    char pathBuff[1024];
    char nameBuff[1024];
    int c,bpp,cvImageType;
   
    camCtrl->getImageSize( &imBuff.width,  &imBuff.height, &c, &bpp );

    if (c==1 && bpp < 9){
      imBuff.mode = commonImage::Gray8bpp;
      imBuff.data = (void*)malloc( imBuff.width*imBuff.height*sizeof(char) );
      cvImageType = CV_8UC1;
    }
    else if (c==1 && bpp < 17){
      imBuff.mode = commonImage::Gray16bpp;
      imBuff.data = (void*)malloc( imBuff.width*imBuff.height*sizeof(char)*2 );
      cvImageType = CV_16UC1;
    }
    if (!imBuff.data){
      std::cerr << "run_single_capture:: error allocating memory" << std::endl;
      return;
    }

    // Take a single image with a camera    
    camCtrl->selectCamera(camId);
    camCtrl->setCameraToSettings( &(experimentSettings.imageStack[0]) );
    camCtrl->captureImage( imBuff.data );
     
    generateImageDir( camId, pathBuff );
    int fileNameCount = 0;
    generateImageName( pathBuff, nameBuff, &fileNameCount );
   
    switch(saveSettings.compression){

      case Settings::NO:{
	commonImage::saveTIFF( nameBuff, &imBuff, commonImage::COMPRESSION_NONE, true); //true = verbose
	break;
      }
      case Settings::LZW:{
	commonImage::saveTIFF( nameBuff, &imBuff, commonImage::COMPRESSION_LZW, true); 
	break;
      }
      case Settings::ZIP:{
	commonImage::saveTIFF( nameBuff, &imBuff, commonImage::COMPRESSION_ZIP, true); 
	break;
      }
      case Settings::JPEG:{
	commonImage::saveTIFF( nameBuff, &imBuff, commonImage::COMPRESSION_JPG, true); 
	break;
      }
      case Settings::PACKBITS:{
	commonImage::saveTIFF( nameBuff, &imBuff, commonImage::COMPRESSION_PACKBITS, true); 
	break;
      }
    }
    free(imBuff.data);
  
  }


  void run_streaming_view()
  {    
    std::cout << "run_streaming_view() stub" << std::endl;
  }

  
  /****************************
   * Do cleaning up prior exit
   ****************************/
  void cleanExit(std::string msg){
    
    std::cout << msg << std::endl;    
    camCtrl->freeCameras();
    delete camCtrl;

  }

  ///////////////////////////////////////////////////////////////////////////////////////////
  //OBS be sure that the savesettings struct is populated before calling generateX functions
  //It is also expected that the saving path is created when the save settings is populated

  //////////////////////////////////////////////////////////////////////////////////////////
  // Generate a cam directory under the main path
  // If directory already exist do nothing
  bool generateCamDir(int idx, char *pathNameBuffer){    

    if (snprintf( pathNameBuffer, PATHNAME_BUFFER_LENGTH, "%s%s%d\0", 
		  saveSettings.outPath.c_str(), 
		  saveSettings.cameraDirectoryPrefix.c_str(), 
		  idx) > 0) {
      
      int rval = FileIO::makeDirectory( pathNameBuffer, false, true );
      if (rval == FileIO::DIR_ERROR)
	exit(-1);
      
      return true;
    }    
    return false;
  }

  char* generateImageDir( int camId, char *pathOut )
  {

    switch (saveSettings.imageDirectoryPrefixType) {

    case Settings::DATETIME :
      {
	char timeBuf[64];
	time_t now = time(NULL);
	struct tm *p_tm = localtime( &now );
	strftime( timeBuf, sizeof(timeBuf), "%Y-%m-%d_%Hh%Mm%Ss", p_tm );

	if (snprintf( pathOut, PATHNAME_BUFFER_LENGTH, "%s%s%d/%s/\0", 
		      saveSettings.outPath.c_str(), 
		      saveSettings.cameraDirectoryPrefix.c_str(), 
		      camId,
		      timeBuf
		      ) > 0  )
	  {
	    int rval = FileIO::makeDirectory( pathOut, false, true );
	    if (rval < 0)
	      return NULL;
	    else
	      return pathOut;
	  }
	break;
      }
    case Settings::RUNNING :
      {
	bool uniqueName=false;

	//Iterate through suggested filenames until a free one is found
	// -OBS be wary that the currentOutImageFilderIndex is common for _EVERY_ camera
	//  that is - it is expected that _ALL_ cameras follow the same saving schema
	while (!uniqueName){
	  
	  if (snprintf( pathOut, PATHNAME_BUFFER_LENGTH, "%s%s%d/%s%06d/", 
			saveSettings.outPath.c_str(), 
			saveSettings.cameraDirectoryPrefix.c_str(),
			camId,			
			saveSettings.imageDirectoryPrefix.c_str(),
			currentOutImageFolderIndex
			) > 0) {

	    int rval = FileIO::makeDirectory( pathOut, false, false );

	    if (rval == 0){
	      uniqueName = true;
	      return pathOut;
	    }	  
	    else if (rval == FileIO::DIR_EXISTS)
	      currentOutImageFolderIndex++;

	    else if (rval == FileIO::DIR_ERROR){
	      std::cout << "Error creating image directory: " << pathOut << std::endl;
	      return NULL;
	    }

	  }
	}
	break;
      }
    default ://NONE
      {
	if (snprintf( pathOut, PATHNAME_BUFFER_LENGTH, "%s%s%d/", 
		      saveSettings.outPath.c_str(), 
		      saveSettings.cameraDirectoryPrefix.c_str(), 
		      camId
		      ) > 0) {
	  return pathOut;
	}else
	  return NULL;
	break;
      }
    }//end switch

  }

  char* generateImageName(char *path, char *nameOut, int *p_lastIndex)
  {
    if (! FileIO::dirExist(path) ){ return NULL; }

    int idx=0;
    if (p_lastIndex != NULL) {
      idx = *p_lastIndex + 1;
    }

    bool uniqueName=false;      
    char bufNumberedPrefix[256];
    
    //Iterate through suggested filenames until a free one is found   
    while (!uniqueName){

      snprintf( bufNumberedPrefix, 256, saveSettings.filenamePrefix.c_str(), idx );

      if (snprintf( nameOut, FILENAME_BUFFER_LENGTH, "%s%s%s\0", 
		    path, // a slash terminated path is expected
		    bufNumberedPrefix,
		    saveSettings.filenameSuffix.c_str() ) > 0)
	{

	  if ( !FileIO::fileExist(nameOut) ) {
	    uniqueName = true;
	    if (p_lastIndex != NULL)
	      *p_lastIndex = idx;

	    return nameOut;
	  }	  
	  else 
	    idx++;	    	    
	}
    }  
    
  }//end generateImageName



  //########################################
  //FEEL FREE TO CLEAN UP
  void run_debug_filenames()
  {
    std::cout << "***** DEBUG filenames *****" << std::endl;
        
    char pathBuffer[1024];
    char nameBuffer[1024];

    char *p_CurrPath;
    char *p_CurrName;
    while(1){
      
      for (int camId = 1; camId < cameraIds.size()+1 ; camId++){
	p_CurrPath = generateImageDir(camId, pathBuffer);
	if (p_CurrPath == NULL){
	  std::cerr << "A NULL path encountered [generateImageDir]";
	  exit(-1);
	}

	currentOutImageIndex = 0;

	//	for (int imId =0; imId < experimentSettings.imageStack.size(); imId++){	     
	for (int imId =0; imId < 4; imId++){	     

	  p_CurrName = generateImageName(p_CurrPath, nameBuffer,  &currentOutImageIndex );

	  if (p_CurrName == NULL){
	    std::cerr << "A NULL file name encountered [generateImageName]";
	    exit(-1);
	  }

	  std::ofstream outFile( p_CurrName );	  
	  outFile << "DEBUG testFile" << std::endl;
	  outFile.close();
	  std::cout << "Created file: " << p_CurrName << std::endl;

	}
      }
      
      sleep( experimentSettings.captureInterval );

    }//end while 1
  }
 
  void run_test_tiff_saving()
  {
    short data[100][100];
    for (int i=0;i<100;i++)
      for (int j=0;j<100;j++)
	data[i][j]=i*j*1;

    commonImage::commonImage_t image;
    image.width = 100;
    image.height = 100;
    image.mode = commonImage::Gray14bpp;
    image.data = (void*)(data);

    std::string path = "./debug100x100.tiff";
    commonImage::saveTIFF(path.c_str(), &image, commonImage::COMPRESSION_NONE, true);
    
    
  }

  void run_test_tiff_load(char * fileIn)
  {
    commonImage::commonImage_t image;
    commonImage::readTIFF( fileIn, &image );

    if (! image.data )
      std::cout << "Error loading image: " << fileIn << std::endl;
    else{
      
      if (image.mode == Gray16bpp){
	unsigned short *pData = (unsigned short*)(image.data);
	int count = image.height * image.width;
	while (count-- > 0)
	  *pData = 65535-*pData++;      
      }

      if (image.mode == Gray8bpp){
	char *pData = (char*)(image.data);
	int count = image.height * image.width;
	while (count-- > 0)
	  *pData = 255-*pData++;
      }

      if (image.mode == RGB8bpp){
	char *pData = (char*)(image.data);
	int count = image.height * image.width * 3;
	while (count-- > 0)
	  *pData = 255-*pData++;
      }

      commonImage::saveTIFF( "./debug_load_invert_save.tiff", &image, commonImage::COMPRESSION_ZIP );

    }

    

  }

  //END FEEL FREE TO CLEAN UP
  //########################################

}//end namespace VisMe
