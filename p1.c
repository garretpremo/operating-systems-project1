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
process *queue_fcfs(process *pl);
process *queue_sjf(process *pl);
process *queue_rr(process *pl);
void simulate_fcfs(process *queue);
void check_finish_io(int time, io_block *blocks, int *num_blocks);
void delete_first_process(process *processes);
void delete_first_block(io_block *blocks, int *num_blocks);
void check_process_arrived(int time, process *queue, int *ready);


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
	n = process_list.size;

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
		if(a != 0) { continue; }
		switch(a) {
			case 0: 
				queue = queue_fcfs(process_list->processes);
				simulate_fcfs(queue);
				break;
			case 1:
				queue = queue_sjf(process_list->processes);
				printf("time 0ms: Start of simulation %s [Q <queue-contents>]\n", SJF);
				// simulate_sjf(queue);
				break;
			case 2:  
				queue = queue_rr(process_list->processes);
				printf("time 0ms: Start of simulation %s [Q <queue-contents>]\n", RR);
				// simulate_rr(queue);
				break;
		}

		// clean up
		free(queue);
	}
	printf("simulate(): end simulate\n");
}

/* NOTE: Queue functions below currently only copy the inputted
		process list. Algorithms not yet implemented. */

/*
	Create First-Come-First-Serve (FCFS) queue
*/
process *queue_fcfs(process *pl) {
	p_list queue;
	queue.processes = (process *)calloc(n, sizeof(process));
	int i;
	for(i = 0; i < n; i++) {
		queue.processes[i] = pl[i];
	}
	qsort(queue.processes, n, sizeof(process), compare_process_by_arrival);

	return queue.processes;
}

/*
	Create Shortest Job First (SJF) queue
*/
process *queue_sjf(process *pl) {
	p_list queue;
	queue.processes = (process *)calloc(n, sizeof(process));
	int i;
	for(i = 0; i < n; i++) {
		queue.processes[i] = pl[i];
	}
	qsort(queue.processes, n, sizeof(process), compare_process_by_burst);

	return queue.processes;
}

/*
	Create Round Robin (RR) queue 
*/
process *queue_rr(process *pl) {
	p_list queue;
	queue.processes = (process *)calloc(n, sizeof(process));
	int i;
	for(i = 0; i < n; i++) {
		queue.processes[i] = pl[i];
	}
	qsort(queue.processes, n, sizeof(process), compare_process_by_arrival);

	return queue.processes;
}

/*
	Simulate FCFS algorithm
*/
void simulate_fcfs(process *queue) {
	// bool done = false;
	int time = 0;
	// int next_end_io_time = 0;
	int i = 0;
	int j;

	// process ready_queue[n];
	int ready = 0;
	// process queue[n];
	// int queue_size = 0;

	io_block blocks[n];
	int num_blocks = 0;



	printf("time %dms: Simulator started for %s [Q empty]\n", time, FCFS);

	while(n != 0) {	
		// print_process_list(queue, n);
		check_process_arrived(time, queue, &ready);
		// print_process_list(queue, ready);



		io_block b;
		process p = queue[0];
		
		if(p.arrival_time > time)
			time = p.arrival_time;

		check_process_arrived(time, queue, &ready);
		// track wait time

		// if(next_end_io_time < time) {
		// 	num_blocks -= check_finish_io(time, blocks, num_blocks, &next_end_io_time);
		// }

		// check_finish_io(time, blocks, &num_blocks);

		// context switch(starting process)
		time += t_cs/2;
		p.wait_time += (time - p.arrival_time);

		check_process_arrived(time, queue, &ready);

		// if(next_end_io_time < time) {
		// 	num_blocks -= check_finish_io(time, blocks, num_blocks, &next_end_io_time);
		// }

		// check if process has any more bursts
		// if no, remove from queue,
		// if yes, stick onto end of queue.

		queue[0].num_bursts -= 1;
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
		print_op(time, p.process_id, "scpu", queue, ready);
		time += p.cpu_burst_time;

		check_process_arrived(time, queue, &ready);

		// finish burst
		print_op(time, p.process_id, "fcpu", queue, ready);

		// start io		
		print_op(time, p.process_id, "sio", queue, ready);


		// // keep track of block time
		// if(p.io_time > 0) {
		// 	b.process_id = p.process_id;
		// 	b.time_unblocked = (time + p.io_time);
		// 	blocks[num_blocks] = b;
		// 	num_blocks += 1;
		// 	if(num_blocks > 1) {
		// 		// sort io_blocks
		// 		for(j = 0; j < num_blocks; j++)
		// 			printf("block {%d, %c}\n", blocks[i].time_unblocked, blocks[i].process_id);
		// 		qsort(blocks, num_blocks, sizeof(io_block), compare_io_block);
		// 	}
		// }

		// // add process back to queue if it's not finished
		// if(queue[0].num_bursts > 0) {
		// 	queue[0].arrival_time = b.time_unblocked;
		// 	qsort(queue, n, sizeof(process), compare_process_by_arrival);
		// 	// print_process_list(queue, n);
		// } else {
		// 	ready -= 1;
		// 	// remove process from queue
		// 	// delete_first_process(queue);
		// }

		// context switch (exiting process)
		time += t_cs/2;

		check_process_arrived(time, queue, &ready);
		
		// if(next_end_io_time < time) {
		// 	num_blocks -= check_finish_io(time, blocks, num_blocks, &next_end_io_time);
		// } 

		// if(n == 0 && num_blocks == 0) {
		// 	done = true;
		// }
		i++;
	}
	printf("time %dms: Simulator finished for %s\n", time, FCFS);
}

// void check_finish_io(int time, io_block *blocks, int *num_blocks) {
// 	if(*num_blocks == 0) { return; }

// 	printf("%d\n", *num_blocks);
// 	while(blocks[0].time_unblocked < time) {
// 		print_op(blocks[0].time_unblocked, blocks[0].process_id, "fio");
// 		delete_first_block(blocks, num_blocks);
// 	}
// }

void check_process_arrived(int time, process *queue, int *ready) {
	int i;
	for(i = 0; i < n; i++) {
		if(queue[i].arrival_time <= time && !in_array(queue[i], queue, *ready)) {
			*ready += 1;
			print_op(queue[i].arrival_time, queue[i].process_id, "rdy", queue, *ready);
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

#if 0
void simulate_fcfs(process **queue) {
	printf("time 0ms: Start of simulation %s [Q <queue-contents>]\n", FCFS);
	char *process_ids = (char *)calloc(n, sizeof(char));
	int *cpu_burst_times = (int *)calloc(n, sizeof(int));
	int *context_switch_times = (int *)calloc(n, sizeof(int));
	int *io_times = (int *)calloc(n, sizeof(int));
	int *wait_times = (int *)calloc(n, sizeof(int));
	int i, j;
	bool io_blocked = false;
	//int io_unblock_time = 0;
	int total_time = 0;

	for(i = 0; i < 1; i++) {
		//int total_time = 0;
		process *p = queue[i];
		process_ids[i] = p->process_id;
		for(j = 0; j < p->num_bursts; j++) {
			// start process
			print_op(total_time, p->process_id, "scpu");

			// cpu burst
			cpu_burst_times[i] += p->cpu_burst_time;
			total_time += p->cpu_burst_time;
			print_op(total_time, p->process_id, "fcpu");
			
			// context switch
			context_switch_times[i] += t_cs;
			total_time += (p->cpu_burst_time + t_cs);

			// start io
			io_times[i] += p->io_time;
			if(!io_blocked) {
				print_op(total_time, p->process_id, "sio");
				io_blocked = true;
			}
		}
		print_op(total_time, p->process_id, "end");



		// if(i != 0) {
		// 	if(cpu_burst_times[i-1] + wait_times[i-1] + 
		// 		context_switch_times[i-1] > p->arrival_time) {

		// 		wait_times[i] = (cpu_burst_times[i-1] - p->arrival_time 
		// 						+ wait_times[i-1] + context_switch_times[i-1]);

		// 		p->wait_time = wait_times[i];
		// 		printf("%c wait time: %d\n", p->process_id, p->wait_time);
		// 	}
		// } else {
		// 	wait_times[i] = p->arrival_time;
		// }			

		//printf("total cpu_burst_time %d\n", total_cpu_burst_time);

		//print_process(p);
	}
	printf("time %dms: Simulator ended for %s\n", total_time, FCFS);
	free(process_ids);
	free(cpu_burst_times);
	free(context_switch_times);
	free(io_times);
	free(wait_times);
}

bool check_finish_io(int t, int unblock_t) {
	return t > unblock_t;
}
#endif


