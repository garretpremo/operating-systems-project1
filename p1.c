/*
		Operating Systems - Poject 1
	
		Process Simulation Framework


	contributors: Garret Premo, Morgan Larkin
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "process.h"

/* function definitions */
void simulate(p_list *process_list);
void queue_fcfs(process *pl, process *queue);
process *queue_sjf(process *pl);
process *queue_rr(process *pl);
void simulate_fcfs(process *pl);
void simulate_sjf(process *pl);
void simulate_rr(process *pl);
void check_finish_io(int time, io_block *blocks, int *num_blocks);
void delete_first_process(process *processes);
void delete_first_block(io_block *blocks, int *num_blocks);
void check_process_arrived(int time, process *queue, int *ready);
void check_process_arrived_sjf(int time, process *queue, int *ready);


#define DEFAULT_NUM_PROCESSES 4

/* constant definitions */

/* number of processors ( DEFAULT 1 ) */
#define m 1

/* time (in ms) to perform a context switch ( DEFAULT 8 ) 
			Context switches occur each time a process leaves 
			the CPU and is replaced by another process.	*/
#define t_cs 8

/* time (in ms) to perform a slice for RR ( DEFAULT 84 ) */
#define t_slice 84

/* easily accessible strings for algorithm names */
char FCFS[] = "FCFS";
char SJF[] = "SJF";
char RR[] = "RR";

/* number of processes to simulate ( equivalent to p_list.size ) */
int n = 0;
int N = 0;


int main(int argc, char *argv[]) {
	
	FILE *f;
	f = open_file(argv); 

	// parse all processes from the file
	p_list process_list;

	process_list.processes = (process *)calloc(DEFAULT_NUM_PROCESSES, sizeof(process));
	process_list.size = 0;

	// parse input file
	parse(f, &process_list);

	// set global variable
	N = process_list.size;
	n = N;

	simulate(&process_list);


	// clean up
	printf("cleanup\n");
	fclose(f);
	free(process_list.processes);
	printf("exit successfully\n");
	return EXIT_SUCCESS;
}

/*
	Simulates a processor using three different ready
	queue methods; FCFS, SJF, RR

	p_list pl: list of processes (parsed from input file)
			which is stored in a struct (see process.h)

*/
void simulate(p_list *process_list) {
	printf("simulate(): begin simulate\n");
	process *queue;

	/* a for algorithm */
	int a;
	for(a = 0; a < 3; a++) {

		// copy process list into a ready queue
		queue = (process *)calloc(n, sizeof(process));
		memcpy(queue, process_list->processes, n*sizeof(process));

		if(a == 2) {
			switch(a) {
				case 0: 
					// sort ready queue by arrival time
					qsort(queue, n, sizeof(process), compare_process_by_arrival);
					simulate_fcfs(queue);
					break;
				case 1:
					qsort(queue, n, sizeof(process), compare_process_by_burst);
					simulate_sjf(queue);
					// simulate_sjf(queue);
					break;
				case 2:  
					qsort(queue, n, sizeof(process), compare_process_by_arrival);
					simulate_rr(queue);
					// simulate_rr(queue);
					break;
			}
		}
		// clean up
		n = N;
		free(queue);
	}
	printf("simulate(): end simulate\n");
}

/*
	Simulate FCFS algorithm
*/
void simulate_fcfs(process *pl) {

	int time = 0;

	process *queue = (process *)calloc(n, sizeof(process));
	memcpy(queue, pl, n*sizeof(process));

	int ready = 0;


	printf("time %dms: Simulator started for %s [Q empty]\n", time, FCFS);

	while(n != 0) {	
		check_process_arrived(time, queue, &ready);

		process p = queue[0];
		
		if(p.arrival_time > time)
			time = p.arrival_time;

		check_process_arrived(time, queue, &ready);

		// context switch(starting process)
		time += t_cs/2;

		// track wait time
		p.wait_time += (time - p.arrival_time);

		check_process_arrived(time, queue, &ready);

		// check if process has any more bursts
		// if no, remove from queue,
		// if yes, stick onto end of queue.

		queue[0].num_bursts -= 1;
		p.num_bursts -= 1;
		if(ready != 0)
			ready -= 1;
		if(queue[0].num_bursts > 0) {
			queue[0].arrival_time = p.arrival_time + p.wait_time + p.cpu_burst_time + p.io_time;
			qsort(queue, n, sizeof(process), compare_process_by_arrival);
			// print_process_list(queue, n);
		} else {
			// remove process from queue
			delete_first_process(queue);
		}

		// start process
		print_op(time, p, "scpu", queue, ready);
		time += p.cpu_burst_time;

		check_process_arrived(time, queue, &ready);

		if(p.num_bursts == 0) {
			print_op(time, p, "end", queue, ready);
		} else {

			// finish burst
			print_op(time, p, "fcpu", queue, ready);

			// start io		
			print_op(time, p, "sio", queue, ready);

		}

		// context switch (exiting process)
		time += t_cs/2;

		check_process_arrived(time, queue, &ready);

	}
	free(queue);
	printf("time %dms: Simulator ended for %s\n", time, FCFS);
}

void simulate_sjf(process *pl) {

	int time = 0;
	//int i = 0;

	process *queue = (process *)calloc(n, sizeof(process));
	memcpy(queue, pl, n*sizeof(process));

	int ready = 0;


	printf("time %dms: Simulator started for %s [Q empty]\n", time, SJF);

	while(n != 0) {	

		check_process_arrived_sjf(time, queue, &ready);
		

		process p = queue[0];
		
		if(p.arrival_time > time) {
			time = p.arrival_time;
		}


		check_process_arrived_sjf(time, queue, &ready);
		
		

		// context switch(starting process)
		time += t_cs/2;

		// track wait time
		p.wait_time += (time - p.arrival_time);

		check_process_arrived_sjf(time, queue, &ready);


		// check if process has any more bursts
		// if no, remove from queue,
		// if yes, stick onto end of queue.

		queue[0].num_bursts -= 1;
		p.num_bursts -= 1;
		if(ready != 0)
			ready -= 1;
		if(queue[0].num_bursts > 0) {
			queue[0].arrival_time = p.arrival_time + p.wait_time + p.cpu_burst_time + p.io_time;
			queue[0].in_io = true;
			qsort(queue, n, sizeof(process), compare_process_by_burst);
			// print_process_list(queue, n);
		} else {
			// remove process from queue
			delete_first_process(queue);
		}

		// start process
		print_op(time, p, "scpu", queue, ready);
		time += p.cpu_burst_time;

		check_process_arrived_sjf(time, queue, &ready);

		if(p.num_bursts == 0) {
			print_op(time, p, "end", queue, ready);
		} else {

			// finish burst
			print_op(time, p, "fcpu", queue, ready);

			// start io		
		
			print_op(time, p, "sio", queue, ready);

		}

		// context switch (exiting process)
		time += t_cs/2;

		check_process_arrived_sjf(time, queue, &ready);
		//i++;
	}

	free(queue);
	printf("time %dms: Simulator ended for %s\n", time, SJF);
}

void simulate_rr(process *pl) {
	int time = 0;
	int i = 0;

	process *queue = (process *)calloc(n, sizeof(process));
	memcpy(queue, pl, n*sizeof(process));

	int ready = 0;

	char prev_id;
	
	bool just_ended = false;

	printf("time %dms: Simulator started for %s [Q empty]\n", time, FCFS);

	while(n != 0) {	
		check_process_arrived(time, queue, &ready);

		process p = queue[0];

		bool no_preemp = true;
		
		if(p.arrival_time > time)
			time = p.arrival_time;

		check_process_arrived(time, queue, &ready);

		// context switch(starting process)
		time += t_cs/2;

		// track wait time
		p.wait_time += (time - p.arrival_time);


		// check if process has any more bursts
		// if no, remove from queue,
		// if yes, stick onto end of queue.
		while(no_preemp) {
			check_process_arrived(time, queue, &ready);
			if(queue[0].remaining_time - t_slice <= 0) {
				// printf("before: proc %c; %dms\n", queue[0].process_id, queue[0].remaining_time);
				queue[0].num_bursts -= 1;
				p.num_bursts -= 1;
				int rem = queue[0].remaining_time;
				if(ready != 0)
					ready -= 1;
				if(queue[0].num_bursts > 0) {
					queue[0].arrival_time = time + rem + p.io_time;
					queue[0].remaining_time = queue[0].cpu_burst_time;
					queue[0].in_io = true;
					qsort(queue, n, sizeof(process), compare_process_by_arrival);
					// print_process_list(queue, n);
				} else {
					// remove process from queue
					delete_first_process(queue);
				}

				// start process
				print_op(time, p, "scpu", queue, ready);
				
				time += rem;				

				check_process_arrived(time, queue, &ready);

				if(p.num_bursts == 0) {
					print_op(time, p, "end", queue, ready);
					if(n != 0){
						just_ended = true;
						print_op(time + t_cs, queue[0], "scpu", queue, ready-1);
					}
				} else {

					// finish burst
					print_op(time, p, "fcpu", queue, ready);

					// start io		
					print_op(time, p, "sio", queue, ready);
					//printf("%d\n", p.arrival_time);

				}
				check_process_arrived(time, queue, &ready);
			} else {
				queue[0].in_io = false;
				queue[0].remaining_time -= t_slice;
				p.remaining_time -= t_slice;
				queue[0].arrival_time = time + t_slice;

				qsort(queue, n, sizeof(process), compare_process_by_arrival);

				if(time + t_slice != queue[0].arrival_time && !just_ended)
					print_op(time, p, "scpu", queue, ready-1);
				else if(time + t_slice != queue[0].arrival_time && just_ended)
					just_ended = false;



				time = time + t_slice;
				if(ready != 0)
					ready -= 1;

				check_process_arrived(time, queue, &ready);

				if(time == queue[0].arrival_time) {
					print_op(time, p, "nprmp", queue, ready-1);
					continue;
				} else {
					print_op(time, p, "prmp", queue, ready);
				}
			}
			no_preemp = false;
		}


		// context switch (exiting process)
		time += t_cs/2;

		check_process_arrived(time, queue, &ready);
		i++;

	}
	free(queue);
	printf("time %dms: Simulator ended for %s\n", time, RR);
}

void check_process_arrived(int time, process *queue, int *ready) {
	int i;
	for(i = 0; i < n; i++) {
		if(queue[i].arrival_time <= time && !in_array(queue[i], queue, *ready)) {
			// printf("time %dms: Process %c ARRIVED: %d\n", time, queue[i].process_id, queue[i].arrived);
			*ready += 1;

			if(!queue[i].arrived) {
				queue[i].arrived = true;
				print_op(queue[i].arrival_time, queue[i], "rdy", queue, *ready);
			} else if(queue[i].in_io) {
				queue[i].in_io = false;
				print_op(queue[i].arrival_time, queue[i], "fio", queue, *ready);
			} 
		} 
	}
}

void check_process_arrived_sjf(int time, process *queue, int *ready) {
	int i;
	process tmp;
	for(i = 0; i < n; i++) {
		if(queue[i].arrival_time <= time && !in_array(queue[i], queue, *ready)) {
			*ready += 1;
			if(!queue[i].arrived) {
				queue[i].arrived = true;
				tmp = queue[i];
				qsort(queue, n, sizeof(process), compare_process_by_burst);
				print_op(tmp.arrival_time, tmp, "rdy", queue, *ready);
			} else {
				queue[i].in_io = false;
				tmp = queue[i];
				qsort(queue, n, sizeof(process), compare_process_by_burst);
				print_op(tmp.arrival_time, tmp, "fio", queue, *ready);
			}
		} else {
			break;
		}
	}
}

void delete_first_process(process *processes) {
	int i;
	if(n == 1) {
		n-=1;
		return;
	}
	for(i = 0; i < n - 1; i++) {
		processes[i] = processes[i+1];
	}
	n-=1;
}

void delete_first_block(io_block *blocks, int *num_blocks) {
	if(*num_blocks == 1) {
		*num_blocks -= 1;
		return;
	}
	int i;
	for(i = 0; i < *num_blocks-1; i++) {
		blocks[i] = blocks[i+1];
	}
	*num_blocks -= 1;
}
