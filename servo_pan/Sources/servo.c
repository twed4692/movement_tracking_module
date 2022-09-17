#include "derivative.h"
#include <math.h> 
#include "servo.h"

#define ZERO_ELEVATION_DUTY 4550
#define ZERO_AZIMUTH_DUTY 4200


void PWMinitialise(void){
    // set PP5 and PP7 for pwm output 
    PWMCLK = 0; // select clock A
    PWMPOL = 0xA0; // PWM5 and PWM7 output start high
    PWMCTL = 0xC0; // 16-bit PWM: use PWM4 and PWM5 for PWM5, PWM6 and PWM7 for PWM7
    PWMCAE = 0; // Center aligned
    PWMPRCLK = 0x33; // PWM Clock prescaler to 8 

    // set the PWM period appropriate for servos
    PWMPER45 = 0xEA6A;
    PWMPER67 = 0xEA6A;

    // set the initial duty cycle for both servos
    PWMDTY45 = ZERO_ELEVATION_DUTY;
    PWMDTY67 = ZERO_AZIMUTH_DUTY;
    
    PWME  |= 0xFF;      // enable PWM0
}

void Init_TC6 (void) {
  TSCR1_TEN = 1;
  
  TSCR2 = 0x00;   // prescaler 1, before 32 = 0x04
  TIOS_IOS6 = 1;   // set channel 6 to output compare
    
  TCTL1_OL6 = 1;    // Output mode for ch6
  TIE_C6I = 1;   // enable interrupt for channel 6

  TFLG1 |= TFLG1_C6F_MASK;
}

/************************************************************

ALL CODE BELOW THIS LINE HAS BEEN EITHER ADDED OR MODIFIED FROM THE
BASE CODE PROVIDED FOR THE ASSIGNMENT

************************************************************/

long iterator_counter = 0;
int toggle = 0;
int rotate = 0;

/************************************************************
Swtiches the pan direction.
************************************************************/

void switch_dir (void) {
  
  toggle ^= 1;
}

/************************************************************
Returns the current pan direction, where '1' is left and '0' 
is right.
************************************************************/

int current_dir (void) {
  
  return toggle;
}

/************************************************************
Returns the current angle for the servo based on the PWM 
signal.
************************************************************/

float crnt_angle (void) {
  
  float gradient = // calculate value
  float offset = // calculate value
  float angle;
  
  angle = gradient*(float)iterator_counter+offset;
  
  return angle;
}


/************************************************************
Sets rotation to either on or off.
************************************************************/

void set_rotation (int rotation_true) {
  
  rotate = rotation_true;
}

/************************************************************
Pans the servo given a PWM signal length.
************************************************************/

void setServoPose(int azimuth){  
    PWMDTY45 = (int)(ZERO_ELEVATION_DUTY);  // Sets elevation to duty cycle using PWM 45
    PWMDTY67 = (int)(ZERO_AZIMUTH_DUTY+rotate*azimuth);   // Sets azimuth to duty cycle using PWM 67
} 

// the interrupt for timer 6 which is used for cycling the servo
#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
__interrupt void TC6_ISR(void) { 
   
  TC6 = TCNT + 20000;   // interrupt delay depends on the prescaler
  TFLG1 |= TFLG1_C6F_MASK;

  if (toggle == 0)
    iterator_counter++;
  else
    iterator_counter--;
  
  if (iterator_counter > 2500) {
    toggle = 1;
  } else if (iterator_counter < -2500) {
    toggle = 0;
  }
  
  setServoPose(iterator_counter);    
}
