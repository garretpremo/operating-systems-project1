/*
		Operating Systems - Poject 1
	
		Process Simulation Framework


	contributors: Garret Premo, Morgan Larkin
*/
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "process.h"

void simulate(int n, int m, int t_cs);
void simulate_fcfs(int n, int m, int t_cs);
void simulate_sjf(int n, int m, int t_cs);
void simulate_rr(int n, int m, int t_cs, int t_slice);


int main(int argc, char *argv[]) {
	
	FILE *f;
	f = open_file(argv); 

	parse(f);

	return EXIT_SUCCESS;
}



/*
	Simulates a processor (should probably have multiple
			of these functions i.e. 'simulate_fcfs()' )

	int n: number of processes to simulate
	
	int m: number of processors available within the CPU
			(default: 1)

	int t_cs: the time (in milliseconds) that it takes
			to perform a context switch. Context switches
			occur each time a process leaves the CPU and is
			replaced by another process.
*/
void simulate(int n, int m, int t_cs) {

}

/*
	Simulates First-Come-First-Serve (FCFS)
*/
void simulate_fcfs(int n, int m, int t_cs) {

}

/*
	Simulates Shortest Job First (SJF)
*/
void simulate_sjf(int n, int m, int t_cs) {

}

/*
	Simulates Round Robin (RR)
*/
void simulate_rr(int n, int m, int t_cs, int t_slice) {
	
}












