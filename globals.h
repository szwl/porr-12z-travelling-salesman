#pragma once

#include <time.h>

// Default dimension of OpenGL Window
#define DIM 600
// Maximum coordinate of town
#define MAX_COORD 1000.0f
#define DEFAULT_TOWNS 60
#define DEFAULT_MI_CONSTANT 500
#define DEFAULT_M_CONSTANT 500

#ifdef _WIN32

#else
	#define sprintf_s(buffer,size,...) snprintf (buffer,size, __VA_ARGS__)
#endif

#define M_MI (mi_constant+m_constant)

// Global iteration counter
unsigned long global_iteration_counter;
clock_t global_start_time;

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
