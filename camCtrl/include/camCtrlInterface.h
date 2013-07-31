/**
 * @file camCtrlInterface.h
 *
 * @section DESCRIPTION
 *
 * A pure virtual interface for controlling and capturing using HW cameras.
 * Inherent the class for real implementation of camera controllers.
 *
 * namespace:  VisMe::
 *          
 * @author Sami Varjo 2013
 ********************************************************************************************/


#ifndef CAM_CTRL_INTERFACE_H 
#define CAM_CTRL_INTERFACE_H 

namespace VisMe{

  /**
   * A pure abstract interface class for all camera control implementations. The common
   * functionalities for camera controllers are defined here. 
   */
  class CamCtrlInterface{
  
  public:
    virtual ~CamCtrlInterface() {};

    enum camParam_t{
      PARAM_GAIN_AUTO, 
      PARAM_EXPTIME_AUTO, 
      PARAM_WHITEBALANCE_AUTO,
      PARAM_IRIS_AUTO,

      PARAM_GAMMA_VALUE,
      PARAM_GAIN_VALUE, 
      PARAM_IRIS_VALUE,
      PARAM_EXPTIME_VALUE, 

    };

    /**
     * Find the cameras using ID string, open them and init for usage
     * @param IDlist camera IDs that are to be initialized (eg IP, serial number,...)
     */
    virtual int InitByIds( std::vector<std::string> IDlist ) = 0;  
    
    /**
     * Select camera that is controlled by index of populated list
     * @param id the index of camera in vector of found cameras
     */
    virtual void selectCamera( int id ) = 0;

    /**
     * Select camera that is controlled by camera specific identifier string (eg IP or serial number)
     * @param pStrId a pointer to the camera Id char string that is to be selected
     */
    virtual void selectCamera( const char *pStrId ) = 0;

    /**
     * Capture a single image 
     * @param buffer a preallocated buffer for the image data
     */
    virtual void captureImage( void *buffer ) = 0;

    /**
     * Capture a stream
     */
    virtual void captureStream( void ) = 0;

    /**
     * Set a camera parameter for currently selected camera
     * @param parameter a enumeration of the parameter to be set
     * @param value a pointer to the data to be set
     * @param valueSize the length of the value in bytes
     */
    virtual void setParameter( camParam_t parameter, void *value, int valueSize) = 0;

    
    /**
     * Get the image dimensions that the selected camera provides (generally is not the same
     * as the frame size from IP camera).
     * @param width a pointer to storage of image width
     * @param height a pointer to storage of image height
     * @param channels a pointer to storage of image data channels 
     * @param bitsPerPixel a pointer to storage of number of bits per Pixel
     */
    virtual void getImageSize( int *width, int *height, int *channels, int *bitsPerPixel)=0;


    /**
     * Release the resources used by the cameras and close the cameras for clean exit
     */
    virtual void freeCameras()=0;
 
  };

}//End namespace VisMe

#endif
