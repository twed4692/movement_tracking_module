#include <hidef.h>      /* common defines and macros */
#include <assert.h>
#include "derivative.h"      /* derivative-specific definitions */

// need this for string functions
#include <stdio.h>

#include "gyro.h"
#include "l3g4200d.h"
#include "laser.h"
#include "pll.h"
#include "servo.h"
#include "simple_serial.h"
#include "tracking.h"


void printErrorCode(IIC_ERRORS error_code) {
  char buffer[128];  
  switch (error_code) {
    case NO_ERROR: 
      sprintf(buffer, "IIC: No error\r\n");
      break;
    
    case NO_RESPONSE: 
      sprintf(buffer, "IIC: No response\r\n");
      break;
    
    case NAK_RESPONSE:
      sprintf(buffer, "IIC: No acknowledge\r\n");
      break;
    
    case IIB_CLEAR_TIMEOUT:
      sprintf(buffer, "IIC: Timeout waiting for reply\r\n");
      break;
    
    case IIB_SET_TIMEOUT: 
      sprintf(buffer, "IIC: Timeout not set\r\n");
      break;
    
    case RECEIVE_TIMEOUT:
      sprintf(buffer, "IIC: Received timeout\r\n");
      break;
    
    case IIC_DATA_SIZE_TOO_SMALL:
      sprintf(buffer, "IIC: Data size incorrect\r\n");
      break;

    default:
      sprintf(buffer, "IIC: Unknown error\r\n");
      break;
  }
    
  SerialOutputString(buffer, &SCI1);
}


void main(void) {

  int i;
  int pan_dir;
  AccelRaw read_accel;
  AccelScaled scaled_accel;
  GyroRaw read_gyro;
  unsigned long single_sample;
  
  float dist;
  float laser_dists[20];
  int tol = 2;
  IIC_ERRORS error_code = NO_ERROR;
  char buffer[300];  


  // make sure the board is set to 24MHz
  //  this is needed only when not using the debugger
  PLL_Init();

  // initialise PWM
  PWMinitialise();
  setServoPose(100, 100);
  
  // initialise the simple serial
  SerialInitialise(BAUD_9600, &SCI1);

  // initialise the sensor suite
  error_code = iicSensorInit();
  
  // write the result of the sensor initialisation to the serial
  if (error_code == NO_ERROR) {
    sprintf(buffer, "NO_ERROR\r\n");
    SerialOutputString(buffer, &SCI1);
  } else {
    sprintf(buffer, "ERROR %d\r\n");
    SerialOutputString(buffer, &SCI1);
  }

  laserInit();

  Init_TC6();
  
	EnableInterrupts;
  //COPCTL = 7;
  _DISABLE_COP();
    
  for(;;) {
      
      GetLatestLaserSample(&single_sample);
      sprintf(buffer, "%lu         ", single_sample);
      SerialOutputString(buffer, &SCI1);
      
      dist = conv_dist(single_sample);
      update_laser(single_sample, &laser_dists[0]);
      /*
      for (i = 0; i < 20; i++) {
        sprintf(buffer, "%lu ", laser_samples[i]);
        SerialOutputString(buffer, &SCI1);
      }
      sprintf(buffer, "\r\n");
      SerialOutputString(buffer, &SCI1);
      */
      track_object(tol, &laser_dists[0]);
      pan_dir = current_dir();
      
      sprintf(buffer, "%f m\r\n", dist);
      SerialOutputString(buffer, &SCI1);      
      
    
    //_FEED_COP(); /* feeds the dog */
  } /* loop forever */
  
  /* please make sure that you never leave main */
}
