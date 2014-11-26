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

#include <signal.h> //for SIGUSR1

#include "settings.h"
#include "experiments.h"
#include "fileIO.h"
#include "commonImage.h"

#ifdef COMPILE_WITH_GUI
 #include <opencv2/core/core.hpp>       // OpenCV is used only for 
 #include <opencv2/highgui/highgui.hpp> // displaying images (ok bit bloated but easy...)
#endif

using namespace commonImage;

namespace VisMe {

///////////////////////////////////////////////////////////////////////
//Global variables for experiment settings
///////////////////////////////////////////////////////////////////////

const std::string DEFAULT_SETUP_FILE_NAME = "setup.ini";
Settings::saveSettings_t saveSettings;
Settings::experimentSettings_t experimentSettings;
std::vector<std::string> cameraIds;
std::string setupFileName = "undefined";
Settings::adaptiveSettings_t adaptiveSettings;

CamCtrlVmbAPI *camCtrl = NULL; //The camera controller

/////////////////////////////////////////////////
//Set of local variables
/////////////////////////////////////////////////
const int FILENAME_BUFFER_LENGTH = 1024;
const int PATHNAME_BUFFER_LENGTH = 1024;

//OBS never freed if caught CTRL+C signal to end program (trust in OS)...
char **pathNameBuffer;
char **fileNameBuffer;
commonImage_t *imgBuffer;
//END FIX

int currentOutImageIndex = 0;
int currentOutImageFolderIndex = 0;

bool saveAutoExposureFrame = false;

bool running = false;

///////////////////////////////////////////////////////////////////////
//Experiment API
///////////////////////////////////////////////////////////////////////

/**
 * calculate average of sub region in image
 * @param *buf pointer to data
 * @param *dataLength the bytes of data ( char 1, short 2, int 4)
 * @param numPoints the number of data points used for calculation (continuous strip of data)
 * @param offset offset for the pointer buf ie
 *
 * for example if 16bit image having size 1024*768 then calling
 * unsinged int mean = subAverage( &imagedata, 2, 1024*10, 1024*340);
 * would calculate the mean value for ten rows of image data at the middle of image (starting row 340)
 */
unsigned int subAverage(void *buf, int dataLength = 1, int numPoints = 1000, int offset = 0) {
	double sum = 0;
	unsigned int count = numPoints;
	switch (dataLength) {
	case 1: {
		unsigned char *ptIn = (unsigned char *) (buf)
				+ (offset * sizeof(unsigned char));
		while (count-- > 0)
			sum += *ptIn++;
		break;
	}
	case 2: {
		unsigned short *ptIn = (unsigned short *) (buf)
				+ (offset * sizeof(unsigned short));
		while (count-- > 0)
			sum += *ptIn++;
		break;
	}
	default: {
		unsigned int *ptIn = (unsigned int *) (buf)
				+ (offset * sizeof(unsigned int));
		while (count-- > 0)
			sum += *ptIn++;
		break;
	}

	}
	return sum / numPoints;

}

/**
 * Capture a stack of images for HDR experiments. The exposuretimes used are given in the
 * ini-file.
 */
void run_image_stack_capture() {
	Settings::cameraSettings_t *p_CamSet;

	struct timeval starttime, endtime, timediff, captTime;
	int sleepTime_sec;

	int numCameras = cameraIds.size();

	char windowNameBuffer[numCameras][128];

	imgBuffer = new commonImage_t[numCameras]; //size, type, data

	pathNameBuffer = new char*[numCameras];
	fileNameBuffer = new char*[numCameras];

	int cvImageType;
	unsigned int nPix;

	//Init settings for each camera:
	for (int camId = 0; camId < numCameras; camId++) {

		//Make names buffer
		pathNameBuffer[camId] = new char[PATHNAME_BUFFER_LENGTH];
		fileNameBuffer[camId] = new char[FILENAME_BUFFER_LENGTH];

		//Use the first setting of image stack
		camCtrl->selectCamera(camId);
		camCtrl->setCameraToSettings(&(experimentSettings.imageStack[0]));

		int c, bpp;
		camCtrl->getImageSize(&imgBuffer[camId].width, &imgBuffer[camId].height, &c, &bpp);

		nPix = imgBuffer[camId].width * imgBuffer[camId].height;
		//std::cout << "Allocating memory for image " << imgBuffer[camId].width << "x" << imgBuffer[camId].height << " c:" << c << " bpp:" << bpp << std::endl;

		//Allocate memory for each data buffer
		if (c == 1 && bpp < 9) {
			imgBuffer[camId].mode = commonImage::Gray8bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char));
#ifdef COMPILE_WITH_GUI
			cvImageType = CV_8UC1;
#endif
		} else if (c == 1 && bpp < 17) {
			imgBuffer[camId].mode = commonImage::Gray16bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char) * 2);
#ifdef COMPILE_WITH_GUI
			cvImageType = CV_16UC1;
#endif
		} else if (c == 3 && bpp == 8) {
			imgBuffer[camId].mode = commonImage::RGB8bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char) * 3);
#ifdef COMPILE_WITH_GUI
			cvImageType = CV_8UC3;
#endif
		} else if (c == 4 && bpp == 8) {
			imgBuffer[camId].mode = commonImage::RGBA8bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char) * 4);
#ifdef COMPILE_WITH_GUI
			cvImageType = CV_8UC4;
#endif
		} else {
			std::cerr << "Unsupported image format encountered : " << c
					<< " channels with " << bpp << "bits per pixel"
					<< std::endl;
			return;
		}

		if (imgBuffer[camId].data == NULL) {
			std::cerr << "Error while allocating image buffers for experiment"
					<< std::endl;
			return;
		}

		//Open one window per camera (if preview set)
#ifdef COMPILE_WITH_GUI
		if (experimentSettings.preview) {
			sprintf(windowNameBuffer[camId], "Camera %u", camId + 1);
			cv::namedWindow(windowNameBuffer[camId], CV_WINDOW_AUTOSIZE);
			std::cout << "Preview window(s) created" << std::endl;
		}
#endif 
	}

	unsigned int *sumImgBuffer[numCameras];

	for (int camId = 0; camId < numCameras; camId++) {
		sumImgBuffer[camId] = (unsigned int*) malloc(
				nPix * sizeof(unsigned int));
		if (sumImgBuffer[camId] == NULL) {
			std::cerr << "Error while allocating image buffers for experiment"
					<< std::endl;
			return;
		}
	}

	char *pCurrPath;
	char *pCurrName;

	int fileNameId;
	unsigned int frame = 0;
	const bool mfalse = false;
	const bool mtrue = true;

	std::cout << "Capturing..." << std::endl;

	unsigned int meanValue[experimentSettings.imageStack.size()];

	running = true;
	while (1) {

		std::cout << "frame:" << ++frame << std::endl;

		gettimeofday(&starttime, NULL);
		fileNameId = 0; //start naming each image set from 0 (assume own folders)

		//Generate new folder for this set of images (for each camera)
		for (int cid = 0; cid < numCameras; cid++) {
			generateImageDir(cid + 1, pathNameBuffer[cid]);
			camCtrl->selectCamera(cid);
			//GT1290Camera::AcquisitionModeEnum val = GT1290Camera::AcquisitionMode_SingleFrame;
			//camCtrl->setParameter(CamCtrlInterface::PARAM_ACQUISITION_MODE,(void*)(&val), sizeof(bool));
			camCtrl->setParameter(CamCtrlInterface::PARAM_EXPTIME_AUTO,
					(void*) &mfalse, sizeof(bool));
		}

		if (experimentSettings.saveSumImage){
			//Zero the sum image(s)
			for (int camId = 0; camId < numCameras; camId++) {
				unsigned int* pt = sumImgBuffer[camId];
				int count = nPix;
				while (count-- > 0) {
					*pt++ = 0;
				}
			}
		}

		//Capture each exposure time
		//TODO	Should fork for each camera...
		for (int imageId = 0; imageId < experimentSettings.imageStack.size();
				imageId++) {

			p_CamSet = &(experimentSettings.imageStack[imageId]);

			//Capture at each camera
			for (int camId = 0; camId < numCameras; camId++) {

				camCtrl->selectCamera(camId);
				camCtrl->setParameter(CamCtrlInterface::PARAM_EXPTIME_VALUE,
						(void*) &p_CamSet->exposureTime, sizeof(double));
				camCtrl->captureImage(imgBuffer[camId].data); //Blocking call to capture image
			}

			//From one camera (first)
			meanValue[imageId] = subAverage(imgBuffer[0].data, 2, 1280 * 50,
					38400); //line 1280*50  //Estimate of the whole image from the sky row
			//std::cout << "meanValue:" << meanValue[imageId] << std::endl;

			//store result (Now blocking for each camera)
			for (int camId = 0; camId < numCameras; camId++) {

				if (experimentSettings.saveSumImage) {
					int count = nPix;
					unsigned int *ptOut = sumImgBuffer[camId];
					
					switch (imgBuffer[camId].mode) {
					case commonImage::Gray8bpp: {
						unsigned char *ptIn =
								(unsigned char *) imgBuffer[camId].data;
						while (count-- > 0) {
							*ptOut++ += (unsigned int) (*ptIn++);
						}
						break;
					}
					case commonImage::Gray16bpp: {
						unsigned short *ptIn =
								(unsigned short *) imgBuffer[camId].data;
						while (count-- > 0) {
							*ptOut++ += (unsigned int) (*ptIn++);
						}
						break;

					}
					case  commonImage::RGB8bpp: {
						unsigned char *ptIn =
								(unsigned char *) imgBuffer[camId].data;
						while (count-- > 0) {
							*ptOut++ += (unsigned int) (*ptIn++);
							*ptOut++ += (unsigned int) (*ptIn++);
							*ptOut++ += (unsigned int) (*ptIn++);
						}
						break;

					}
					case commonImage::RGBA8bpp: {
						unsigned char *ptIn =
								(unsigned char *) imgBuffer[camId].data;
						while (count-- > 0) {
							*ptOut++ += (unsigned int) (*ptIn++);
							*ptOut++ += (unsigned int) (*ptIn++);
							*ptOut++ += (unsigned int) (*ptIn++);
							*ptOut++ += (unsigned int) (*ptIn++);
						}
						break;
					}
					}

				}
				if (experimentSettings.saveStackImages) {
					generateImageName(pathNameBuffer[camId],
							fileNameBuffer[camId], &fileNameId);
					//commonImage::saveTIFF(fileNameBuffer[camId], &imgBuffer[camId], commonImage::COMPRESSION_ZIP, true); //true = verbose
					saveImage(fileNameBuffer[camId], &imgBuffer[camId], saveSettings.compression, true);
				}
			}

		}

		//Save sum image
		if (experimentSettings.saveSumImage) {
			commonImage_t sumImg;
			sumImg.mode = Gray32bpp;
			sumImg.width = imgBuffer[0].width;
			sumImg.height = imgBuffer[0].height;

			for (int camId = 0; camId < numCameras; camId++) {
				sumImg.data = sumImgBuffer[camId];
				if (saveSettings.imageDirectoryPrefixType
						== VisMe::Settings::NONE) {
					snprintf(fileNameBuffer[camId], 256, "%ssumImage%06d.tif",
							pathNameBuffer[camId], frame); //path /-ended
				} else {
					snprintf(fileNameBuffer[camId], 256, "%ssumImage.tif",
							pathNameBuffer[camId]); //path /-ended
				}
				//commonImage::saveTIFF(fileNameBuffer[camId], &sumImg, commonImage::COMPRESSION_ZIP);
				saveImage(fileNameBuffer[camId], &sumImg, saveSettings.compression, false );

			}
		}

		int okExpId = 0;
		unsigned int th = 6000; // 2 to 14 = 16384; (mean gray image) XXX (hard coded)
		while (okExpId < experimentSettings.imageStack.size()
				&& meanValue[okExpId] < th)
			okExpId++;

		//Restore reasonable exposure time
		for (int camId = 0; camId < numCameras; camId++) {

			camCtrl->selectCamera(camId);
			//camCtrl->setParameter(CamCtrlInterface::PARAM_EXPTIME_ONCE,(void*) &mtrue, sizeof(bool));
			//double setVal = (experimentSettings.imageStack[0].exposureTime + experimentSettings.imageStack[experimentSettings.imageStack.size()].exposureTime)/2;
			double setVal = experimentSettings.imageStack[okExpId].exposureTime;
			//double setVal = 50; //OBS 12 min
			camCtrl->setParameter(CamCtrlInterface::PARAM_EXPTIME_VALUE,
					&setVal, sizeof(double));

		}

		gettimeofday(&captTime, NULL);
		timersub(&captTime, &starttime, &timediff);
		int captTime_sec = timediff.tv_sec + 1; //+1 for ensure marginal

#ifdef COMPILE_WITH_GUI
		//test if there is time left
		bool runPreview;
		gettimeofday(&endtime, NULL);
		timersub(&endtime, &starttime, &timediff);
		int timeRemaining = experimentSettings.captureInterval
				- timediff.tv_sec;

		if (timeRemaining < captTime_sec) {
			runPreview = false;
		} else {
			runPreview = true;
		}

		//Show preview
		int countPreviewFrames = 0;
	
		if (experimentSettings.preview && runPreview) {

			while (runPreview) {

				for (int camId = 0; camId < numCameras; camId++) {

					camCtrl->selectCamera(camId);

					//std::cout << "Capturing preview..." << std::endl;
					camCtrl->captureImage(imgBuffer[camId].data);

					cv::Mat image8bpp(imgBuffer[camId].height,
							imgBuffer[camId].width, CV_8UC1);

					//Scale data
					short *in = (short*) (imgBuffer[camId].data);
					unsigned char *out = image8bpp.data;
					int count = imgBuffer[camId].height
							* imgBuffer[camId].width;
					while (count-- > 0) {
						*out++ = (char) (*in++
								* 0.0155649148507599340780076908991); //magic_num = 255/16383 (14 bit -> 8 bit)
					}

					sprintf(windowNameBuffer[camId], "Camera %u", camId + 1);
					cv::imshow(windowNameBuffer[camId], image8bpp);
					cv::waitKey(50); //Force image window update by waiting XXX ms (obs must be large enough so that there is time for rendering...)

				}

				//test if there is time left
				gettimeofday(&endtime, NULL);
				timersub(&endtime, &starttime, &timediff);

				if ((experimentSettings.captureInterval - timediff.tv_sec)
						< captTime_sec + 1) { //1 sec margin
					runPreview = false;
				}

			} //While runPreview

			//TODO	Should join from each camera for sync...

			//Wait for given interval
			gettimeofday(&endtime, NULL);
			timersub(&endtime, &starttime, &timediff);
			sleepTime_sec = experimentSettings.captureInterval
					- timediff.tv_sec;

			if (sleepTime_sec < 0) {
				std::cout
						<< "Warning! The stack capture time overflow (try to increase the image capture interval)"
						<< std::endl << "set: "
						<< experimentSettings.captureInterval
						<< "s  time elapsed: " << timediff.tv_sec << "s"
						<< std::endl;
			}
		}//if prewiew

#else
		gettimeofday(&endtime, NULL);
		timersub(&endtime, &starttime, &timediff);
		sleepTime_sec = experimentSettings.captureInterval - timediff.tv_sec;
#endif	//COMPILE_WITH_GUI	


		//Sleep the time left (either after preview or between captures
		if (sleepTime_sec >0){
		  sleep(sleepTime_sec);
		}
		if (timediff.tv_usec>0){
		  usleep(timediff.tv_usec);
		}
	}

	//OBS imgBuffer[camId] is not freed ever! (for ever loop expected)

}

/**
 * Capture just single image from a given camera using given exposureTime
 * if exposure time is not given time from the first image stack settings
 * is used as well as other settings are defaulted to that.
 */
void run_single_capture(int camId, unsigned int expTime_us) {

	if (camId > camCtrl->getNumberOfCameras() - 1)
		return;

	commonImage_t imBuff;
	char pathBuff[1024];
	char nameBuff[1024];
	int c, bpp, cvImageType;

	camCtrl->getImageSize(&imBuff.width, &imBuff.height, &c, &bpp);

	if (c == 1 && bpp < 9) {
		imBuff.mode = commonImage::Gray8bpp;
		imBuff.data = (void*) malloc(
				imBuff.width * imBuff.height * sizeof(char));
	} else if (c == 1 && bpp < 17) {
		imBuff.mode = commonImage::Gray16bpp;
		imBuff.data = (void*) malloc(
				imBuff.width * imBuff.height * sizeof(char) * 2);
	}
	if (!imBuff.data) {
		std::cerr << "run_single_capture:: error allocating memory"
				<< std::endl;
		return;
	}

	// Take a single image with a camera
	camCtrl->selectCamera(camId);
	camCtrl->setCameraToSettings(&(experimentSettings.imageStack[0]));
	bool mfalse = false;

	double setVal;
	if (expTime_us==0){
		setVal = experimentSettings.imageStack[0].exposureTime;
	}else{
		setVal =expTime_us;
	}
	camCtrl->setParameter(CamCtrlInterface::PARAM_EXPTIME_AUTO, (void*) &mfalse, sizeof(bool));
	camCtrl->setParameter(CamCtrlInterface::PARAM_EXPTIME_VALUE, (void*) &setVal, sizeof(double));
	camCtrl->captureImage(imBuff.data);

	generateImageDir(camId + 1, pathBuff);
	int fileNameCount = 0;
	generateImageName(pathBuff, nameBuff, &fileNameCount);

	saveImage(nameBuff, &imBuff, saveSettings.compression, true);

	free(imBuff.data);

}


/**
 * Save image data in tiff-file
 * @param nameBuff the name of the image
 * @parma *imBuff the commonImage_t image containing the size, bitdepth and data of the image
 * @param compression a VisMe::Settings type compression selector for the file
 * @param verbose
 */
void saveImage( char *nameBuff, commonImage_t *imBuff,
				Settings::fileCompressionType_t compression, bool verbose )
{
	switch (compression)
	{

	case Settings::NO: {
		commonImage::saveTIFF(nameBuff, imBuff, commonImage::COMPRESSION_NONE,
				true);
		break;
	}
	case Settings::LZW: {
		commonImage::saveTIFF(nameBuff, imBuff, commonImage::COMPRESSION_LZW,
				true);
		break;
	}
	case Settings::ZIP: {
		commonImage::saveTIFF(nameBuff, imBuff, commonImage::COMPRESSION_ZIP,
				true);
		break;
	}
	case Settings::JPEG: {
		commonImage::saveTIFF(nameBuff, imBuff, commonImage::COMPRESSION_JPG,
				true);
		break;
	}
	case Settings::PACKBITS: {
		commonImage::saveTIFF(nameBuff, imBuff,
				commonImage::COMPRESSION_PACKBITS, true);
		break;
	}
	}
}
/**
 * Capture HDR images (32b) using adaptive exposure time sampling mode
 * No normalization is done for the data (ie min max may vary a lot)
 */
void run_adaptive_hdr_capture() {
	Settings::cameraSettings_t *p_CamSet;

	struct timeval starttime, endtime, timediff, captTime;
	int sleepTime_sec;

	int numCameras = cameraIds.size();

	char windowNameBuffer[numCameras][128]; //Name of openCV image window for displaying images

	imgBuffer = new commonImage_t[numCameras]; //size, type, data

	pathNameBuffer = new char*[numCameras];
	fileNameBuffer = new char*[numCameras];

	int cvImageType;
	unsigned int nPix;

	//Init settings for each camera:
	for (int camId = 0; camId < numCameras; camId++) {

		//Make names buffer
		pathNameBuffer[camId] = new char[PATHNAME_BUFFER_LENGTH];
		fileNameBuffer[camId] = new char[FILENAME_BUFFER_LENGTH];

		//Use the first setting of image stack
		camCtrl->selectCamera(camId);
		camCtrl->setCameraToSettings(&(experimentSettings.imageStack[0]));

		int c, bpp;
		camCtrl->getImageSize(&imgBuffer[camId].width, &imgBuffer[camId].height, &c, &bpp);

		nPix = imgBuffer[camId].width * imgBuffer[camId].height;

		//Allocate memory for each data buffer
		if (c == 1 && bpp < 9) {
			imgBuffer[camId].mode = commonImage::Gray8bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char));
#ifdef COMPILE_WITH_GUI
			cvImageType = CV_8UC1;
#endif
		} else if (c == 1 && bpp < 17) {
			imgBuffer[camId].mode = commonImage::Gray16bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char) * 2);
#ifdef COMPILE_WITH_GUI
			cvImageType = CV_16UC1;
#endif
		} else if (c == 3 && bpp == 8) {
			imgBuffer[camId].mode = commonImage::RGB8bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char) * 3);
#ifdef COMPILE_WITH_GUI
			cvImageType = CV_8UC3;
#endif
		} else if (c == 4 && bpp == 8) {
			imgBuffer[camId].mode = commonImage::RGBA8bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char) * 4);
#ifdef COMPILE_WITH_GUI
			cvImageType = CV_8UC4;
#endif
		} else {
			std::cerr << "Unsupported image format encountered : " << c
					<< " channels with " << bpp << "bits per pixel"
					<< std::endl;
			return;
		}

		if (imgBuffer[camId].data == NULL) {
			std::cerr << "Error while allocating image buffers for experiment"
					<< std::endl;
			return;
		}

#ifdef COMPILE_WITH_GUI
		//Open one window per camera (if preview set)
		if (experimentSettings.preview) {
			sprintf(windowNameBuffer[camId], "Camera %u", camId + 1);
			cv::namedWindow(windowNameBuffer[camId], CV_WINDOW_AUTOSIZE);
			std::cout << "Preview window(s) created" << std::endl;
		}
#endif
	}

	unsigned int *sumImgBuffer[numCameras];

	for (int camId = 0; camId < numCameras; camId++) {
		sumImgBuffer[camId] = (unsigned int*) malloc( nPix * sizeof(unsigned int) );
		if (sumImgBuffer[camId] == NULL) {
			std::cerr << "Error while allocating image buffers for experiment"
					<< std::endl;
			return;
		}
	}

	char *pCurrPath;
	char *pCurrName;

	int fileNameId;
	unsigned int frame = 0;
	const bool mfalse = false;
	const bool mtrue = true;

	unsigned int meanValue[experimentSettings.imageStack.size()];
	running = true;
	double expTime0[numCameras];

	//Solve suitable exposure time for set
	for (int camId = 0; camId < numCameras; camId++) {
		camCtrl->selectCamera(camId);
		double eTime = adaptiveHDRautoexp( adaptiveSettings.maxExpTime,
										   adaptiveSettings.expTargetMeanValue,
											imgBuffer[camId].data);
		eTime = eTime / adaptiveSettings.expTimeFraction; //Aim for HDR
		if (eTime < adaptiveSettings.minExpTime )
			eTime = adaptiveSettings.minExpTime ; //HW minimum
		expTime0[camId] = eTime;
		std::cout << "-setting exposure time to " << expTime0[camId] << " µs" << std::endl;
	}
	unsigned int stackCount;

	std::cout << "Capturing..." << std::endl;
	while (1) {

		std::cout << "frame:" << ++frame << std::endl;

		gettimeofday(&starttime, NULL);

		fileNameId = 0; //start naming each image set from 0 (assume own folders)

		//Generate new folder for this set of images (for each camera)
		for (int cid = 0; cid < numCameras; cid++) {
			generateImageDir(cid + 1, pathNameBuffer[cid]);
			camCtrl->selectCamera(cid);
			camCtrl->setParameter(CamCtrlInterface::PARAM_EXPTIME_AUTO,
					(void*) &mfalse, sizeof(bool));
		}

		//std::cout << "*WARNING* Some adaptive method parameters hard coded" << std::endl;

		int imageId = 0;
		p_CamSet = &(experimentSettings.imageStack[imageId]);

		//Zero the sum image(s)
		for (int camId = 0; camId < numCameras; camId++) {
			unsigned int* pt = sumImgBuffer[camId];
			int count = nPix;
			while (count-- > 0) {
				*pt++ = 0;
			}

		}

		//Capture at each camera //TODO fork for each camera
		for (int camId = 0; camId < numCameras; camId++) {

			camCtrl->selectCamera(camId);
			camCtrl->setParameter(CamCtrlInterface::PARAM_EXPTIME_VALUE,
					(void*) &expTime0[camId], sizeof(double));

			//Max val not very good - mean appears to be better
			double mVal = 0;
			double target = adaptiveSettings.integrationTargetMean; //aim for 10x improvement in range (old max pow(2,14)) (mean gray)
			int ret=0;
			stackCount=0;
			while (mVal < target) {

				if( camCtrl->captureImage(imgBuffer[camId].data) != 0 ) //frame not receiveved for some reason -> repeat
					continue;

				stackCount++;
				unsigned int *ptOut = sumImgBuffer[camId];
				int count = nPix;

				switch (imgBuffer[camId].mode) {
				case commonImage::Gray8bpp: {
					unsigned char *ptIn =  (unsigned char *) imgBuffer[camId].data;
					while (count-- > 0) {
						*ptOut += (unsigned int) (*ptIn++);
						mVal += *ptOut++;
					}
					break;
				}
				case commonImage::Gray16bpp: {
					unsigned short *ptIn = (unsigned short *) imgBuffer[camId].data;
					while (count-- > 0) {
						*ptOut += (unsigned int) (*ptIn++);
						mVal += *ptOut++;

					}
					break;

				}
				case commonImage::RGB8bpp: {
					unsigned char *ptIn = (unsigned char *) imgBuffer[camId].data;
					while (count-- > 0) {
						*ptOut++ += (unsigned int) (*ptIn++);
						*ptOut++ += (unsigned int) (*ptIn++);
						*ptOut += (unsigned int) (*ptIn++);
						mVal = *ptOut++;

					}
					break;

				}
				case commonImage::RGBA8bpp: {
					unsigned char *ptIn =
							(unsigned char *) imgBuffer[camId].data;
					while (count-- > 0) {
						*ptOut += (unsigned int) (*ptIn++);
						mVal = *ptOut++;
						*ptOut++ += (unsigned int) (*ptIn++);
						*ptOut++ += (unsigned int) (*ptIn++);
						*ptOut++ += (unsigned int) (*ptIn++);

					}
					break;
				}
			  } //switch
				mVal /= nPix;
			} //while maxVal < target
		} //for camId

		//Save sum image
		if (experimentSettings.saveSumImage) {
			commonImage_t sumImg;
			sumImg.mode = Gray32bpp;
			sumImg.width = imgBuffer[0].width;
			sumImg.height = imgBuffer[0].height;

			for (int camId = 0; camId < numCameras; camId++) {
				sumImg.data = sumImgBuffer[camId];
				if (saveSettings.imageDirectoryPrefixType == VisMe::Settings::NONE) {
					snprintf(fileNameBuffer[camId], 256, "%ssumImage%06d.tif",
							pathNameBuffer[camId], frame); //path /-ended
				} else {
					snprintf(fileNameBuffer[camId], 256, "%ssumImage.tif",
							pathNameBuffer[camId]); //path /-ended
				}
				//commonImage::saveTIFF(fileNameBuffer[camId], &sumImg, commonImage::COMPRESSION_ZIP);
				saveImage(fileNameBuffer[camId], &sumImg, saveSettings.compression,true );

				std::cout << "Saved stack of " << stackCount << " images" << std::endl;
			}
		}


		gettimeofday(&captTime, NULL);
		timersub(&captTime, &starttime, &timediff);
		int captTime_sec = timediff.tv_sec + 5; //+5 to ensure marginal


		//Find suitable exposure time for next set
		for (int camId = 0; camId < numCameras; camId++) {
			camCtrl->selectCamera(camId);
			double eTime = adaptiveHDRautoexp(adaptiveSettings.maxExpTime,
											  adaptiveSettings.expTargetMeanValue,
											  imgBuffer[camId].data);
			eTime = eTime / adaptiveSettings.expTimeFraction; //Aim for HDR
			if (eTime < adaptiveSettings.minExpTime)
				eTime = adaptiveSettings.minExpTime; //HW minimum
			expTime0[camId] = eTime;
			std::cout << "-setting exposure time to " << expTime0[camId]  << " µs" << std::endl;
		}

		gettimeofday(&endtime, NULL);
		timersub(&endtime, &starttime, &timediff);
		int timeRemaining = experimentSettings.captureInterval - timediff.tv_sec;

#ifdef COMPILE_WITH_GUI
		//test if there is time left
		bool runPreview;

		if (timeRemaining < captTime_sec) {
			runPreview = false;
		} else {
			runPreview = true;
		}

		//Show preview
		int countPreviewFrames = 0;

		if (experimentSettings.preview && runPreview) {

			while (runPreview) {

				for (int camId = 0; camId < numCameras; camId++) {

					camCtrl->selectCamera(camId);

					//std::cout << "Capturing preview..." << std::endl;
					camCtrl->captureImage(imgBuffer[camId].data);

					cv::Mat image8bpp(imgBuffer[camId].height,
							imgBuffer[camId].width, CV_8UC1);

					//Scale data
					short *in = (short*) (imgBuffer[camId].data);
					unsigned char *out = image8bpp.data;
					int count = imgBuffer[camId].height
							* imgBuffer[camId].width;
					while (count-- > 0) {
						*out++ = (char) (*in++
								* 0.0155649148507599340780076908991); //magic_num = 255/16383 (14 bit -> 8 bit)
					}

					sprintf(windowNameBuffer[camId], "Camera %u", camId + 1);
					cv::imshow(windowNameBuffer[camId], image8bpp);
					cv::waitKey(50); //Force image window update by waiting XXX ms (obs must be large enough so that there is time for rendering...)

				}

				//test if there is time left
				gettimeofday(&endtime, NULL);
				timersub(&endtime, &starttime, &timediff);

				if ((experimentSettings.captureInterval - timediff.tv_sec)
						< captTime_sec + 1) { //1 sec margin
					runPreview = false;
				}

			} //While runPreview

			//TODO	Should join from each camera for sync..

			//Wait for given interval
			gettimeofday(&endtime, NULL);
			timersub(&endtime, &starttime, &timediff);
			sleepTime_sec = experimentSettings.captureInterval
					- timediff.tv_sec;

			if (sleepTime_sec < 0) {
				std::cout
						<< "Warning! The stack capture time overflow (try to increase the image capture interval)"
						<< std::endl << "set: "
						<< experimentSettings.captureInterval
						<< "s  time elapsed: " << timediff.tv_sec << "s"
						<< std::endl;
			}
		}

#else
		gettimeofday(&endtime, NULL);
		timersub(&endtime, &starttime, &timediff);
		sleepTime_sec = experimentSettings.captureInterval - timediff.tv_sec;
#endif // 
		if (sleepTime_sec>0)
		  sleep(sleepTime_sec);
		if (timediff.tv_usec)
		  usleep(timediff.tv_usec);
	}
	

	//OBS imgBuffer[camId] is not freed ever! (for ever loop expected)
}

/**
 * adaptiveHDRautoexp estimates the exposure time limited to maxExpTime targeting to have
 * average value targetVal (estimated from 20 lines in middle of image for speed)
 */
unsigned int adaptiveHDRautoexp(int maxExpTime, int targetVal, void *imgBuffer) {
	double exptime = 50;
	double subMean = 0;
	while (exptime < maxExpTime && subMean < targetVal) {
		camCtrl->setParameter(CamCtrlInterface::PARAM_EXPTIME_VALUE,
				(void*) &exptime, sizeof(double));
		camCtrl->captureImage(imgBuffer);
		subMean = subAverage(imgBuffer, 2, 1280 * 100, 1280 * 50); //line 50 100 lines
		exptime = exptime * 2.5;
	}
	return exptime;
}


/**
 * Capture images from cameras and show on the screen. No saving is done!
 */
#ifdef COMPILE_WITH_GUI
void run_streaming_view() {

	Settings::cameraSettings_t *p_CamSet;

	int numCameras = cameraIds.size();

	char windowNameBuffer[numCameras][128];

	imgBuffer = new commonImage_t[numCameras]; //size, type, data

	pathNameBuffer = new char*[numCameras];
	fileNameBuffer = new char*[numCameras];

	int cvImageType;
	unsigned int nPix;

	//Init settings for each camera:
	for (int camId = 0; camId < numCameras; camId++) {

		//Make names buffer
		pathNameBuffer[camId] = new char[PATHNAME_BUFFER_LENGTH];
		fileNameBuffer[camId] = new char[FILENAME_BUFFER_LENGTH];

		//Use the first setting of image stack
		camCtrl->selectCamera(camId);
		camCtrl->setCameraToSettings(&(experimentSettings.imageStack[0]));

		int c, bpp;
		camCtrl->getImageSize(&imgBuffer[camId].width, &imgBuffer[camId].height,
				&c, &bpp);

		nPix = imgBuffer[camId].width * imgBuffer[camId].height;
		//std::cout << "Allocating memory for image " << imgBuffer[camId].width << "x" << imgBuffer[camId].height << " c:" << c << " bpp:" << bpp << std::endl;

		//Allocate memory for each data buffer
		if (c == 1 && bpp < 9) {
			imgBuffer[camId].mode = commonImage::Gray8bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char));
			cvImageType = CV_8UC1;
		} else if (c == 1 && bpp < 17) {
			imgBuffer[camId].mode = commonImage::Gray16bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char) * 2);
			cvImageType = CV_16UC1;
		} else if (c == 3 && bpp == 8) {
			imgBuffer[camId].mode = commonImage::RGB8bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char) * 3);
			cvImageType = CV_8UC3;
		} else if (c == 4 && bpp == 8) {
			imgBuffer[camId].mode = commonImage::RGBA8bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char) * 4);
			cvImageType = CV_8UC4;
		} else {
			std::cerr << "Unsupported image format encountered : " << c
					<< " channels with " << bpp << "bits per pixel"
					<< std::endl;
			return;
		}

		if (imgBuffer[camId].data == NULL) {
			std::cerr << "Error while allocating image buffers for experiment"
					<< std::endl;
			return;
		}

		//Open one window per camera 

		sprintf(windowNameBuffer[camId], "Camera %u", camId + 1);
		cv::namedWindow(windowNameBuffer[camId], CV_WINDOW_AUTOSIZE);
		std::cout << "Preview window(s) created" << std::endl;

	while (1) {

		for (int camId = 0; camId < numCameras; camId++) {

			camCtrl->selectCamera(camId);

			//std::cout << "Capturing preview..." << std::endl;
			camCtrl->captureImage(imgBuffer[camId].data);

			cv::Mat image8bpp(imgBuffer[camId].height,
					imgBuffer[camId].width, CV_8UC1);

			//Scale data
			short *in = (short*) (imgBuffer[camId].data);
			unsigned char *out = image8bpp.data;
			int count = imgBuffer[camId].height
					* imgBuffer[camId].width;
			while (count-- > 0) {
				*out++ = (char) (*in++
						* 0.0155649148507599340780076908991); //magic_num = 255/16383 (14 bit -> 8 bit)
			}

			sprintf(windowNameBuffer[camId], "Camera %u", camId + 1);
			cv::imshow(windowNameBuffer[camId], image8bpp);
			cv::waitKey(150); //Force image window update by waiting XX ms (obs must be large enough so that there is time for rendering...)

		}


		}

	} //While runPreview

}
#else

void run_streaming_view() {
  std::cout << "Program compiled without GUI - cannot show stream from the camera."  << std::endl;
}

#endif



/*
 * Handler for external signal SIGUSR
 */
bool flag_trigger_capture=false;
void signal_SIGUSR1_callback_handler(int signum){
	flag_trigger_capture=true;
}

/**
 * Capture single images using SIGUSR1 as tricker ( stack settings)
 */
void run_external_signal_capture()
{
	signal(SIGUSR1, signal_SIGUSR1_callback_handler);

	Settings::cameraSettings_t *p_CamSet;

	struct timeval starttime, endtime, timediff, captTime;

	int numCameras = cameraIds.size();

	char windowNameBuffer[numCameras][128];

	imgBuffer = new commonImage_t[numCameras]; //size, type, data

	pathNameBuffer = new char*[numCameras];
	fileNameBuffer = new char*[numCameras];

	int cvImageType;
	unsigned int nPix;

	//Init settings for each camera:
	for (int camId = 0; camId < numCameras; camId++) {

		//Make names buffer
		pathNameBuffer[camId] = new char[PATHNAME_BUFFER_LENGTH];
		fileNameBuffer[camId] = new char[FILENAME_BUFFER_LENGTH];

		//Use the first setting of image stack
		camCtrl->selectCamera(camId);
		camCtrl->setCameraToSettings(&(experimentSettings.imageStack[0]));

		int c, bpp;
		camCtrl->getImageSize(&imgBuffer[camId].width, &imgBuffer[camId].height,
				&c, &bpp);

		nPix = imgBuffer[camId].width * imgBuffer[camId].height;
		//std::cout << "Allocating memory for image " << imgBuffer[camId].width << "x" << imgBuffer[camId].height << " c:" << c << " bpp:" << bpp << std::endl;

		//Allocate memory for each data buffer
		if (c == 1 && bpp < 9) {
			imgBuffer[camId].mode = commonImage::Gray8bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char));
#ifdef COMPILE_WITH_GUI
			cvImageType = CV_8UC1;
#endif
		} else if (c == 1 && bpp < 17) {
			imgBuffer[camId].mode = commonImage::Gray16bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char) * 2);
#ifdef COMPILE_WITH_GUI
			cvImageType = CV_16UC1;
#endif
		} else if (c == 3 && bpp == 8) {
			imgBuffer[camId].mode = commonImage::RGB8bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char) * 3);
#ifdef COMPILE_WITH_GUI
			cvImageType = CV_8UC3;
#endif
		} else if (c == 4 && bpp == 8) {
			imgBuffer[camId].mode = commonImage::RGBA8bpp;
			imgBuffer[camId].data = (void*) malloc(nPix * sizeof(char) * 4);
#ifdef COMPILE_WITH_GUI
			cvImageType = CV_8UC4;
#endif
		} else {
			std::cerr << "Unsupported image format encountered : " << c
					<< " channels with " << bpp << "bits per pixel"
					<< std::endl;
			return;
		}

		if (imgBuffer[camId].data == NULL) {
			std::cerr << "Error while allocating image buffers for experiment"
					<< std::endl;
			return;
		}

		//Open one window per camera (if preview set)
#ifdef COMPILE_WITH_GUI
		if (experimentSettings.preview) {
			sprintf(windowNameBuffer[camId], "Camera %u", camId + 1);
			cv::namedWindow(windowNameBuffer[camId], CV_WINDOW_AUTOSIZE);
			std::cout << "Preview window(s) created" << std::endl;
		}
#endif
	}//End init capture buffers


	unsigned int *sumImgBuffer[numCameras];
	for (int camId = 0; camId < numCameras; camId++) {
		sumImgBuffer[camId] = (unsigned int*) malloc(
				nPix * sizeof(unsigned int));
		if (sumImgBuffer[camId] == NULL) {
			std::cerr << "Error while allocating image buffers for experiment"
					<< std::endl;
			return;
		}
	}

	char *pCurrPath;
	char *pCurrName;

	int fileNameId;
	unsigned int frame = 0;
	const bool mfalse = false;
	const bool mtrue = true;

	std::cout << "Waiting for SIGUSR1..." << std::endl;

	unsigned int meanValue[experimentSettings.imageStack.size()];


	while(1){

		if (flag_trigger_capture){
			std::cout << "frame:" << ++frame << std::endl;

			fileNameId = 0; //start naming each image set from 0 (assume own folders)

			//Generate new folder for this set of images (for each camera)
			for (int cid = 0; cid < numCameras; cid++) {
				generateImageDir(cid + 1, pathNameBuffer[cid]);
				camCtrl->selectCamera(cid);
				//GT1290Camera::AcquisitionModeEnum val = GT1290Camera::AcquisitionMode_SingleFrame;
				//camCtrl->setParameter(CamCtrlInterface::PARAM_ACQUISITION_MODE,(void*)(&val), sizeof(bool));
				camCtrl->setParameter(CamCtrlInterface::PARAM_EXPTIME_AUTO,
						(void*) &mfalse, sizeof(bool));
			}

			//Capture each exposure time
			//TODO	Should fork for each camera...
			for (int imageId = 0;
					imageId < experimentSettings.imageStack.size(); imageId++) {

				p_CamSet = &(experimentSettings.imageStack[imageId]);

				//Capture at each camera
				for (int camId = 0; camId < numCameras; camId++) {

					camCtrl->selectCamera(camId);
					camCtrl->setParameter(CamCtrlInterface::PARAM_EXPTIME_VALUE,
							(void*) &p_CamSet->exposureTime, sizeof(double));
					camCtrl->captureImage(imgBuffer[camId].data); //Blocking call to capture image
				}

				//From one camera (first)
				meanValue[imageId] = subAverage(imgBuffer[0].data, 2, 1280 * 2, 512000); //line 400  1280*400=512000 //Estimate of the whole image from the middle row

				//store result (Now blocking for each camera)
				for (int camId = 0; camId < numCameras; camId++) {

					if (experimentSettings.saveSumImage) {
						int count = nPix;
						unsigned int *ptOut = sumImgBuffer[camId];
						switch (imgBuffer[camId].mode) {
						case commonImage::Gray8bpp: {
							unsigned char *ptIn = (unsigned char *) imgBuffer[camId].data;
							while (count-- > 0) {
								*ptOut++ += (unsigned int) (*ptIn++);
							}
							break;
						}
						case commonImage::Gray16bpp: {
							unsigned short *ptIn = (unsigned short *) imgBuffer[camId].data;
							while (count-- > 0) {
								*ptOut++ += (unsigned int) (*ptIn++);
							}
							break;

						}
						case commonImage::RGB8bpp: {
							unsigned char *ptIn = (unsigned char *) imgBuffer[camId].data;
							while (count-- > 0) {
								*ptOut++ += (unsigned int) (*ptIn++);
								*ptOut++ += (unsigned int) (*ptIn++);
								*ptOut++ += (unsigned int) (*ptIn++);
							}
							break;

						}
						case commonImage::RGBA8bpp: {
							unsigned char *ptIn = (unsigned char *) imgBuffer[camId].data;
							while (count-- > 0) {
								*ptOut++ += (unsigned int) (*ptIn++);
								*ptOut++ += (unsigned int) (*ptIn++);
								*ptOut++ += (unsigned int) (*ptIn++);
								*ptOut++ += (unsigned int) (*ptIn++);
							}
							break;
						}
						}

					}
					if (experimentSettings.saveStackImages) {
						generateImageName(pathNameBuffer[camId], fileNameBuffer[camId], &fileNameId);
						//commonImage::saveTIFF(fileNameBuffer[camId], &imgBuffer[camId], commonImage::COMPRESSION_ZIP,	true); //true = verbose
						saveImage(fileNameBuffer[camId], &imgBuffer[camId], saveSettings.compression, true);
					}
				}

			}

			//Save sum image
			if (experimentSettings.saveSumImage) {
				commonImage_t sumImg;
				sumImg.mode = Gray32bpp;
				sumImg.width = imgBuffer[0].width;
				sumImg.height = imgBuffer[0].height;

				for (int camId = 0; camId < numCameras; camId++) {
					sumImg.data = sumImgBuffer[camId];
					if (saveSettings.imageDirectoryPrefixType
							== VisMe::Settings::NONE) {
						snprintf(fileNameBuffer[camId], 256,
								"%ssumImage%06d.tif", pathNameBuffer[camId],
								frame); //path /-ended
					} else {
						snprintf(fileNameBuffer[camId], 256, "%ssumImage.tif",
								pathNameBuffer[camId]); //path /-ended
					}
					//commonImage::saveTIFF(fileNameBuffer[camId], &sumImg, commonImage::COMPRESSION_ZIP);
					saveImage(fileNameBuffer[camId], &sumImg, saveSettings.compression, true);

				}
			}

			int okExpId = 0;
			unsigned int th = 6000; // 2 to 14 = 16384; (mean gray image) XXX (hard coded)
			while (okExpId < experimentSettings.imageStack.size() && meanValue[okExpId] < th)
				okExpId++;

			//Restore reasonable exposure time (for preview)
			for (int camId = 0; camId < numCameras; camId++) {
				camCtrl->selectCamera(camId);
				double setVal =	experimentSettings.imageStack[okExpId].exposureTime;
				camCtrl->setParameter(CamCtrlInterface::PARAM_EXPTIME_VALUE,
						&setVal, sizeof(double));
			}

			//Prepare buffer for the next capture.
			if (experimentSettings.saveSumImage){
				//Zero the sum image(s)
				for (int camId = 0; camId < numCameras; camId++) {
					unsigned int* pt = sumImgBuffer[camId];
					int count = nPix;
					while (count-- > 0) {
						*pt++ = 0;
					}
				}
			}

			flag_trigger_capture  = false;
		}

#ifdef COMPILE_WITH_GUI
		//Show preview
		if (experimentSettings.preview ) {

			for (int camId = 0; camId < numCameras; camId++) {

				camCtrl->selectCamera(camId);

				//std::cout << "Capturing preview..." << std::endl;
				camCtrl->captureImage(imgBuffer[camId].data);

				cv::Mat image8bpp(imgBuffer[camId].height,
						imgBuffer[camId].width, CV_8UC1);

				//Scale data
				short *in = (short*) (imgBuffer[camId].data);
				unsigned char *out = image8bpp.data;
				int count = imgBuffer[camId].height * imgBuffer[camId].width;
				while (count-- > 0) {
					*out++ = (char) (*in++ * 0.0155649148507599340780076908991); //magic_num = 255/16383 (14 bit -> 8 bit)
				}

				sprintf(windowNameBuffer[camId], "Camera %u", camId + 1);
				cv::imshow(windowNameBuffer[camId], image8bpp);
				cv::waitKey(50); //Force image window update by waiting XXX ms (obs must be large enough so that there is time for rendering...)

			}

		}//if preview
		else {
			sleep(50);
		}
#else 
		sleep(50);
#endif

	}
}

/****************************
 * Do cleaning up prior exit
 ****************************/
void cleanExit(std::string msg) {

	std::cout << msg << std::endl;
	delete camCtrl;

}

///////////////////////////////////////////////////////////////////////////////////////////
//OBS be sure that the savesettings struct is populated before calling generateX functions
//It is also expected that the saving path is created when the save settings is populated

//////////////////////////////////////////////////////////////////////////////////////////
// Generate a cam directory under the main path
// If directory already exist do nothing
bool generateCamDir(int idx, char *pathNameBuffer) {

	if (snprintf(pathNameBuffer, PATHNAME_BUFFER_LENGTH, "%s%s%d\0",
			saveSettings.outPath.c_str(),
			saveSettings.cameraDirectoryPrefix.c_str(), idx) > 0) {

		int rval = FileIO::makeDirectory(pathNameBuffer, false, true);
		if (rval == FileIO::DIR_ERROR)
			exit(-1);

		return true;
	}
	return false;
}

char* generateImageDir(int camId, char *pathOut) {

	switch (saveSettings.imageDirectoryPrefixType) {

	case Settings::DATETIME: {
		char timeBuf[64];
		time_t now = time(NULL);
		struct tm *p_tm = localtime(&now);
		strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d_%Hh%Mm%Ss", p_tm);

		if (snprintf(pathOut, PATHNAME_BUFFER_LENGTH, "%s%s%d/%s/\0",
				saveSettings.outPath.c_str(),
				saveSettings.cameraDirectoryPrefix.c_str(), camId, timeBuf)
				> 0) {
			int rval = FileIO::makeDirectory(pathOut, false, true);
			if (rval < 0)
				return NULL;
			else
				return pathOut;
		}
		break;
	}
	case Settings::RUNNING: {
		bool uniqueName = false;

		//Iterate through suggested filenames until a free one is found
		// -OBS be wary that the currentOutImageFilderIndex is common for _EVERY_ camera
		//  that is - it is expected that _ALL_ cameras follow the same saving schema
		while (!uniqueName) {

			if (snprintf(pathOut, PATHNAME_BUFFER_LENGTH, "%s%s%d/%s%06d/",
					saveSettings.outPath.c_str(),
					saveSettings.cameraDirectoryPrefix.c_str(), camId,
					saveSettings.imageDirectoryPrefix.c_str(),
					currentOutImageFolderIndex) > 0) {

				int rval = FileIO::makeDirectory(pathOut, false, false);

				if (rval == 0) {
					uniqueName = true;
					return pathOut;
				} else if (rval == FileIO::DIR_EXISTS)
					currentOutImageFolderIndex++;

				else if (rval == FileIO::DIR_ERROR) {
					std::cout << "Error creating image directory: " << pathOut
							<< std::endl;
					return NULL;
				}

			}
		}
		break;
	}
	default: //NONE
	{
		if (snprintf(pathOut, PATHNAME_BUFFER_LENGTH, "%s%s%d/",
				saveSettings.outPath.c_str(),
				saveSettings.cameraDirectoryPrefix.c_str(), camId) > 0) {
			return pathOut;
		} else
			return NULL;
		break;
	}
	} //end switch

}

char* generateImageName(char *path, char *nameOut, int *p_lastIndex) {
	if (!FileIO::dirExist(path)) {
		return NULL;
	}

	int idx = 0;
	if (p_lastIndex != NULL) {
		idx = *p_lastIndex + 1;
	}

	bool uniqueName = false;
	char bufNumberedPrefix[256];
	nameOut[0] = 0;
	//Iterate through suggested filenames until a free one is found
	while (!uniqueName) {

		snprintf(bufNumberedPrefix, 256, saveSettings.filenamePrefix.c_str(),
				idx);

		if (snprintf(nameOut, FILENAME_BUFFER_LENGTH, "%s%s%s\0", path, // a slash terminated path is expected
				bufNumberedPrefix, saveSettings.filenameSuffix.c_str()) > 0) {

			if (!FileIO::fileExist(nameOut)) {
				uniqueName = true;
				if (p_lastIndex != NULL)
					*p_lastIndex = idx;

				return nameOut;
			} else
				idx++;
		}
	}
	return nameOut;
} //end generateImageName

//########################################
//FEEL FREE TO CLEAN UP
void run_debug_filenames() {
	std::cout << "***** DEBUG filenames *****" << std::endl;

	char pathBuffer[1024];
	char nameBuffer[1024];

	char *p_CurrPath;
	char *p_CurrName;
	while (1) {

		for (int camId = 1; camId < cameraIds.size() + 1; camId++) {
			p_CurrPath = generateImageDir(camId, pathBuffer);
			if (p_CurrPath == NULL) {
				std::cerr << "A NULL path encountered [generateImageDir]";
				exit(-1);
			}

			currentOutImageIndex = 0;

			//	for (int imId =0; imId < experimentSettings.imageStack.size(); imId++){
			for (int imId = 0; imId < 4; imId++) {

				p_CurrName = generateImageName(p_CurrPath, nameBuffer,
						&currentOutImageIndex);

				if (p_CurrName == NULL) {
					std::cerr
							<< "A NULL file name encountered [generateImageName]";
					exit(-1);
				}

				std::ofstream outFile(p_CurrName);
				outFile << "DEBUG testFile" << std::endl;
				outFile.close();
				std::cout << "Created file: " << p_CurrName << std::endl;

			}
		}

		sleep(experimentSettings.captureInterval);

	} //end while 1
}

void run_test_tiff_saving() {
	short data[100][100];
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
			data[i][j] = i * j * 1;

	commonImage::commonImage_t image;
	image.width = 100;
	image.height = 100;
	image.mode = commonImage::Gray14bpp;
	image.data = (void*) (data);

	std::string path = "./debug100x100.tiff";
	commonImage::saveTIFF(path.c_str(), &image, commonImage::COMPRESSION_NONE,
			true);

}

void run_test_tiff_load(char * fileIn) {
	commonImage::commonImage_t image;
	commonImage::readTIFF(fileIn, &image);

	if (!image.data)
		std::cout << "Error loading image: " << fileIn << std::endl;
	else {

		if (image.mode == Gray16bpp) {
			unsigned short *pData = (unsigned short*) (image.data);
			int count = image.height * image.width;
			while (count-- > 0)
				*pData = 65535 - *pData++;
		}

		if (image.mode == Gray8bpp) {
			char *pData = (char*) (image.data);
			int count = image.height * image.width;
			while (count-- > 0)
				*pData = 255 - *pData++;
		}

		if (image.mode == RGB8bpp) {
			char *pData = (char*) (image.data);
			int count = image.height * image.width * 3;
			while (count-- > 0)
				*pData = 255 - *pData++;
		}

		commonImage::saveTIFF("./debug_load_invert_save.tiff", &image,
				commonImage::COMPRESSION_ZIP);

	}

}

//END FEEL FREE TO CLEAN UP
//########################################

}//end namespace VisMe
