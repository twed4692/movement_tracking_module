#ifndef MOVEMENT_H
#define MOVEMENT_H

typedef struct MoveParam {
  
  float dist[40];
  float time[40];
  float angl[40];
}

void update_movement (MoveParam array, float value, char type);

int update_array (float *array, float value, int current_element, int max_length);

float clock_to_time (unsigned long clock_cycles);

float lidar_to_dist(unsigned long laser_sample);

float movement_speed (float *dist, float *time);

float distanceAngleGeometry (float leftAngle, float rightAngle, float leftDistance, float rightDistance);

#endif