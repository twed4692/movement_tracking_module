#include "derivative.h"
#include "misc.h"

unsigned long clock_start;
unsigned long clock_end;


/************************************************************
Used to start a timing a section of code.
************************************************************/

void start_count (void) {

	clock_start = TCNT;
	
	return;
}

/************************************************************
Used to finish timing a section of code. Returns the 
time to complete in s.
************************************************************/

float end_count (void) {

	float time;
	unsigned long total_count;
	
	clock_end = TCNT;
	
	if (clock_end < clock_start) {
	
		clock_end += 65536;
	}

	total_count = clock_end-clock_start;
	time = clock_to_time(prescaler, total_count);
	
	return time;
}

/************************************************************
Calculates and returns the average of a dataset given the 
number of samples.
************************************************************/

float average (int num_samples, float *array) {
  
  int i;
  float average;
  float sum = 0;
  
  for (i = 0; i < num_samples; i++) {
    
    sum += array[i];
  }
  
  average = sum/num_samples;
  return average;
}

/************************************************************
Calculates and returns the sum of a dataset given the
number of samples.
************************************************************/

float sum (int num_samples, float *array) {

	int i;
	float sum = 0;

	for (i = 0; i < num_samples; i++) {

		sum += array[i];
	}

	return sum;
}