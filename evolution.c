#include <stdlib.h> // rand
#include <stdio.h> // printf
#include <time.h> // time
#include <math.h> // sqrt
#include <string.h>
#include <float.h> // FLT_MAX

#include "globals.h"
#include "evolutionLib.hpp"
#include "evolution.h" // This header

#define M_MI (mi_constant+m_constant)

// ----------------------------------------------------------------------------

void generate_population() {

	int i, k, j, temp;

	population = (int**)malloc(M_MI * sizeof(int*));

	for (i = 0; i < M_MI; i++) {	

		population[i] = (int*)malloc(towns_count * sizeof(int));

		for (k = 0; k < towns_count; k++){
			population[i][k] = k;
		}
	    
		for (k = towns_count-1; k > 0; k--) {
			j = rand() % (k+1);
			temp = population[i][j];
			population[i][j] = population[i][k];
			population[i][k] = temp;
		}
	}
}

// ----------------------------------------------------------------------------

float calculate_weight(int i, int j) {
	
	return sqrt( pow(towns[i].x - towns[j].x, 2) + 
		pow(towns[i].y - towns[j].y, 2) );
}

// ----------------------------------------------------------------------------

float calculate_overall_length(int index) {

	int i,x,y;
	float v = 0;

	// for ( i = 0; i < M_MI; ++i)
	// {
	// 	for ( x = 0; i < towns_count; ++x)
	// 		{
	// 				printf("%d ", population[i][x]);
	// 		}
	// 		printf("\n");
	// }

	// Sum of weights
	for(i = 0; i < towns_count-1; i++){
		x = population[index][i];
		y = population[index][i+1];		
		v += weights[population[index][i]][population[index][i+1]];
	}
	//TODO last first also?
	
	return v;
}

// ----------------------------------------------------------------------------

void find_best(){
	int i;
	best_value = FLT_MAX;

	for(i = 0; i < mi_constant; ++i){
		if(best_value > overall_lengths[i]){
			best_value = overall_lengths[i];
			best_index = i;
		} 
	}
}

// ----------------------------------------------------------------------------

void destroy_population() {
	
	int i;

	for (i = 0; i < M_MI; i++)
		free(population[i]);
	
	free(population);
}

// ----------------------------------------------------------------------------

void print_best() {
	
	int i;
	float v = 0.0;
	float t = 0.0;

	fprintf(stderr, "%f [%d]", best_value, best_index);
	for(i = 0; i < towns_count; i++)
		fprintf(stderr, " %d", population[best_index][i]);
	
	fprintf(stderr, "\n");
}

// ----------------------------------------------------------------------------

void print_population_info() {

	fprintf(stderr, "Iteration %lu\n", global_iteration_counter);
	print_best();
	return;
}

// ----------------------------------------------------------------------------

void generate_weight_matrix() {
	
	int i, j;
	float tmp;

	// Allocate memory
	weights = (float**)malloc(towns_count * sizeof(float*));
	
	for (i = 0; i < towns_count; i++)
		weights[i] = (float*)malloc(towns_count * sizeof(float));

	for (i = 0; i < towns_count; i++) {	
		
		for (j = 0; j < towns_count; j++) {
			tmp = calculate_weight(i, j);
			weights[i][j] = tmp;
			weights[j][i] = tmp;
		} // for j
	} // for i
}

// ----------------------------------------------------------------------------

void destroy_weight_matrix() {
	
	int i;

	for (i = 0; i < towns_count; i++)
		free(weights[i]);
		
	free(weights);
}

// ----------------------------------------------------------------------------

void generate_towns() {

	int i;
	
	// Allocate memory
	towns = (struct town*)malloc(sizeof(struct town)*towns_count);

	// Generate towns
	for (i = 0; i < towns_count; i++) {
		towns[i].x = - MAX_COORD + (float)rand()/((float)RAND_MAX/MAX_COORD/2);
		towns[i].y = - MAX_COORD + (float)rand()/((float)RAND_MAX/MAX_COORD/2);
	}
}

// ----------------------------------------------------------------------------

void destroy_towns() {

	free(towns);
}

// ----------------------------------------------------------------------------
void generate_population_overall_length(){
	int i;

	overall_lengths = (float*)malloc(M_MI * sizeof(float));
	for(i = 0; i < M_MI; ++i){
		overall_lengths[i] = calculate_overall_length(i);
	}
}

// ----------------------------------------------------------------------------
void destroy_population_overall_length(){
	free(overall_lengths);
}

// ----------------------------------------------------------------------------

void init(int argc, char **argv) {
		
	towns_count = 0;
	mi_constant = 0;
	m_constant = 0;

	// Process execute parameters
	if (argc == 4) {
		towns_count = atoi(argv[1]);
		mi_constant = atoi(argv[2]);
		m_constant = atoi(argv[3]);
	}
	else {
		fprintf(stderr, "argc: %d\n", argc);
	}
	if (towns_count==0) towns_count = 30;
	if (mi_constant==0) mi_constant = 1000;
	if (m_constant==0) m_constant = 10;
	
	generate_towns();

	// Generate connectivity weight matrix
	generate_weight_matrix();

	// Generate population
	generate_population();
	
	// Reset iteration counter
	global_iteration_counter = 0;

	//Each element holds current path length
	generate_population_overall_length();

} // init()

// ----------------------------------------------------------------------------


void terminate() {
	print_population_info();
	fprintf(stderr, "Quiting");
	destroy_towns();
	destroy_weight_matrix();
	destroy_population();
	destroy_population_overall_length();
	fprintf(stderr, ".\n");
	exit(0);
} // terminate()

void evo_iter(void) {
	//TODO Evolve - Iteration step
	int i,x,y,tmp;

		
	//dla wszystkich dzieci
	for(i = mi_constant; i < M_MI; ++i){
		//wylosuj rodziców (można by coś mądrzejszego)
		x = rand() % mi_constant;
		y = rand() % mi_constant;

		//printf("X: %d , Y: %d\n",x,y);
		//zrob dziecko
		pmx(x,y,i,-1);
		
		//policz jego odleglosc
		overall_lengths[i] = calculate_overall_length(i);

		//jesli lepsza niz ojca to zamien (trzeba cos madrzejszego)
		if(overall_lengths[i] < overall_lengths[x]){
			swapRows(population[i],population[x]);
			swapf(&overall_lengths[i], &overall_lengths[x]);
		}
	}
	
}