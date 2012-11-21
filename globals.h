#pragma once

#include <time.h>

// Default dimension of OpenGL Window
#define DIM 600
// Maximum coordinate of town
#define MAX_COORD 1000.0f
#define DEFAULT_TOWNS 100
#define DEFAULT_MI_CONSTANT 10000
#define DEFAULT_M_CONSTANT 10000

#ifdef _WIN32

#else
	#define sprintf_s(buffer,size,...) snprintf (buffer,size, __VA_ARGS__)
#endif

#define M_MI (mi_constant+m_constant)

// Global iteration counter
unsigned long global_iteration_counter;
clock_t global_start_time;
// Minimum main for loop execute time
extern int global_benchmark;

extern int best_index;
extern float best_value;

// Parameters from command line
int towns_count;
int mi_constant;
int m_constant;

struct town {
	float x;
	float y;
};

// Globals
struct town *towns; // []
int **population; // [][]
float **weights; // [][]
float *overall_lengths; // []

float *overall_lengths_weights; // []
float overall_lengths_sum;
float overall_lengths_weights_sum;
int is_dirty;
