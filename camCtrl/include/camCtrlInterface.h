#ifndef CAM_CTRL_INTERFACE_H 
#define CAM_CTRL_INTERFACE_H 

namespace VisMe{

  class CamCtrlInterface{
  
  public:
    virtual ~CamCtrlInterface() {};

    enum camParam_t{
      EXP_TIME_VALUE, APERTURE_VALUE, GAIN_VALUE, WHITEBALANCE_VALUE,
      EXP_TIME_AUTO, APERTURE_AUTO, GAIN_AUTO, WHITEBALANCE_AUTO
    };
 
    virtual void selectCamera( int id ) = 0;

    virtual void captureImage( void ) = 0;
    virtual void captureStream( void ) = 0;

    virtual void setParameter( camParam_t parameter, void *value, int valueSize) = 0;
    virtual void getImageSize( int *width, int *height, int *channels, int *bitsPerPixel)=0;

  };

}//End namespace VisMe

#endif
