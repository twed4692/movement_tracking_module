#include "derivative.h"
#include "movement.h"

#include <math.h>

#include "misc.h"

/************************************************************
Updates the movement struct with a new value, given the 
component (distance, time, angle) to update.
************************************************************/

int dist_count = 0;
int time_count = 0;
int angl_count = 0;

void update_movement (MoveParam movement, float value, char type) {

    int full_length = 40;
    int i;
    
    switch type {
      
      case 'd':
      
      dist_count = update_array(&movement.dist[0], value, dist_count, full_length);
      break;
      
      case 't':
      
      time_count = update_array(&movement.time[0], value, time_count, full_length);
      break;
      
      case 'a':
      
      angl_count = update_array(&movement.angl[0], value, angl_count, full_length);
      break;
    }
}      
    
/************************************************************
Updates an array with a value given the current element 
location in the array and its maximum length. If the current 
element would exceed the maximum length, shifts values down 
to make room for the new value. Returns the new element 
location.
************************************************************/
  
int update_array (float *array, float value, int current_element, int max_length) {
  
  int i;
  
  if current_element < max_length {
    
    *array[current_element] = value;
    current_element++;
    
    return current_element;
  } 
  
  else {
    
    for (i = 0; i < max_length-1; i++) {
      
      array[i] = array[i+1];
    }
    
    array[max_length-1] = value;
    
    return max_length;
  }
}
  

/************************************************************
Converts a clock count to the corresponding time interval
in s.
************************************************************/

float clock_to_time (unsigned long clock_cycles) {

	float clock_speed = 24000000;
	float time;
	float prescale_fact = pow(2, prescaler);

	time = clock_cycles*(clock_speed/prescale_fact);
	
	return time;
}

/************************************************************
Given a signal from the LIDAR sensor, returns the 
corresponding distance value in m.
************************************************************/

float lidar_to_dist(unsigned long laser_sample) {
  
  float conv_fact = 1000;  
  float time;
  float dist;
  
  time = clock_to_time(laser_sample);
  dist = time*conv_fact;   
  
  return dist;
}

/************************************************************
Given an array of distance and time values, returns the 
average movement speed in m/s.
************************************************************/

float movement_speed (float *dist, float *time) {

	float first_dist;
	float second_dist;
	float total_time;
	float movement_speed;

	int total_length = 20;
	int half_length = total_length/2;

	first_dist = average(half_length, &dist[0]);
	second_dist = average(half_length, &dist[full_length]);
	total_time = sum(total_length, &time[0]);
	movement_speed = (second_dist-first_dist)/total_time;

	return movement_speed;
}  
  

/************************************************************
Given the distances and corresponding angles at the edges of 
a tracked object, returns the angle at which the midpoint
between those edges is directed in radians. 
************************************************************/

float distanceAngleGeometry (float leftAngle, float rightAngle, float leftDistance, float rightDistance) {
    
    // considering origin is taken from the left
    float midpointAngle;
    float midpointDistance; // left = a right = b
    
    // values for calculation
    float DOC;
    float CD;
    float CM;
    float DCO;
    float COM;
       
    // geometry
    DOC = rightAngle - leftAngle;
    CD = sqrtf(powf(leftDistance,2) + powf(rightDistance,2)-2*leftDistance*rightDistance*cosf(DOC));
    CM = CD/2;
    DCO = asinf((rightDistance*sinf(DOC))/CD);
    midpointDistance = sqrtf(powf(leftDistance,2)+powf(CM,2) - 2*leftDistance*CM*cosf(DCO));

    COM = asinf((CM*sinf(DCO))/midpointDistance);
    midpointAngle = COM + leftAngle;

    return midpointAngle;
}