#ifndef SETTINGS_HEADER
#define SETTINGS_HEADER

typedef struct s_cameraSettings{

  bool autogain;
  bool autoexpsure;
  bool autowhitebalance;
  bool autoiris;

  

  double gamma;
  double gain;
  double iris;
  double exposureTime;


}cameraSettings_t;


#endif
