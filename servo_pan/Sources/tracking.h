#ifndef TRACKING_H
#define TRACKING_H

void update_laser (float latest_sample, float *laser_samples);

void track_object (int tol, float *laser_samples);

int is_tracked(float *dist, int num_samples, float range[2]);

#endif