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
 
    typedef char BUFFER_TYPE;

    virtual void openCamera( int camId ) = 0;
    virtual void closeCamera( int camId ) = 0;

    virtual void captureImage( BUFFER_TYPE *buffer ) = 0;
    virtual void captureStream( BUFFER_TYPE *buffer ) = 0;

    virtual void setParameter( camParam_t parameter, void *value, int valueSize) = 0;

    //virtual bool isOpen(int camId);
    //virtual bool isCapturing(int camId);
    

  };

}//End namespace VisMe

#endif
