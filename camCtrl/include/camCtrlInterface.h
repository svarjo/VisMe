/******************************************************************************************
 * file: camCtrlInterface.h
 *
 * A pure virtual interface for controlling and capturing using HW cameras.
 * Inheret the class for real implementation of camera controllers.
 *
 * namespace:  VisMe::
 *          
 * 2013 Sami Varjo
 ********************************************************************************************/


#ifndef CAM_CTRL_INTERFACE_H 
#define CAM_CTRL_INTERFACE_H 

namespace VisMe{

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

    /*
     * Find the cameras using ID string, open them and init for usage
     */
    virtual int InitByIds( std::vector<std::string> IDlist ) = 0;  
    
    /*
     * Select camera that is controlled by index of populated list
     */
    virtual void selectCamera( int id ) = 0;

    /*
     * Select camera that is controlled by camera specific identifier string (eg IP or serial number)
     */
    virtual void selectCamera( const char *pStrId ) = 0;

    /*
     * Capture a single image 
     */
    virtual void captureImage( void *buffer ) = 0;

    /*
     * Capture a stream
     */
    virtual void captureStream( void ) = 0;

    /*
     * Set a camera parameter for selected camera
     */
    virtual void setParameter( camParam_t parameter, void *value, int valueSize) = 0;

    
    /*
     * Get the image dimensions that the selected camera provides (generally is not the same
     * as the frame size from IP camera).
     */
    virtual void getImageSize( int *width, int *height, int *channels, int *bitsPerPixel)=0;


    /*
     * Release the resources used by the cameras and close the cameras for clean exit
     */
    virtual void freeCameras()=0;
 
  };

}//End namespace VisMe

#endif
