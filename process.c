#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>		
#include "process.h"

void print_process_list(process *pl, int size) {
	int i;
	for(i = 0; i < size; i++) {
		print_process(pl[i]);
	}
}

void print_process(process p) {
	// A|0|168|5|600
	printf("%c|%d|%d|%d|%d\n", p.process_id, p.arrival_time, p.cpu_burst_time, p.num_bursts, p.io_time);
}

void calculate_stats(int N, process *queue, process *pl, p_avgs *averages) {
	int i = 0;
	int total_procs = 0;
	int total_cpu_burst_time = 0;
	int total_wait_time = 0;
	int total_turnaround_time = 0;
	for(i = 0; i < N; i++) {
		total_procs += pl[i].num_bursts;
		total_cpu_burst_time += pl[i].cpu_burst_time * pl[i].num_bursts;
		total_turnaround_time += queue[i].turnaround_time;
		total_wait_time += queue[i].wait_time;
	}

	averages->cpu_burst_time = (float)total_cpu_burst_time/total_procs;
	averages->wait_time = (float)total_wait_time/total_procs;
	averages->turnaround_time = (float)total_turnaround_time/total_procs;
}

void queue_tostr(process *p, char *str, int size) {
	int i;
	for(i = 0; i < size; i++) {
		str[2*i] = ' ';
		str[2*i+1] = p[i].process_id; 
	}
}

int compare_process_by_arrival(const void * a, const void * b) {
	if((*(process*)a).arrival_time <= (*(process*)b).arrival_time) return -1;
	else return 1;
}

int compare_process_by_id(const void * a, const void * b) {
	if((*(process*)a).process_id <= (*(process*)b).process_id) return -1;
	else return 1;
}

int compare_process_by_arrival_then_burst(const void * a, const void * b) {
	if((*(process*)a).arrival_time < (*(process*)b).arrival_time) {
		return -1;
	}
	else if ((*(process*)a).arrival_time > (*(process*)b).arrival_time) {
		return 1;
	}
	else {
		if((*(process*)a).cpu_burst_time <= (*(process*)b).cpu_burst_time) {
			return -1;
		}
		else 
			return 1;
	}
}

int compare_process_by_burst (const void * a, const void * b) {
	if((*(process*)a).arrived < (*(process*)b).arrived) {
		return 1;
	}
	else if((*(process*)a).arrived > (*(process*)b).arrived) {
		return -1;
	}
	else if((*(process*)a).arrived == false) {
		if((*(process*)a).arrival_time <= (*(process*)b).arrival_time) {
			return -1;
		}
		else if ((*(process*)a).arrival_time > (*(process*)b).arrival_time) {
			return 1;
		}
		else {
			if((*(process*)a).cpu_burst_time <= (*(process*)b).cpu_burst_time) {
				return -1;
			}
			else 
				return 1;
		}
	}

	else {
		if((*(process*)a).in_io < (*(process*)b).in_io) {
			return -1;
		}
		else if((*(process*)a).in_io > (*(process*)b).in_io) {
			return 1;
		}
		else if((*(process*)a).in_io == true){
			if((*(process*)a).arrival_time <= (*(process*)b).arrival_time) return -1;
			else return 1;
			
		}
		
		else {
			if((*(process*)a).cpu_burst_time <= (*(process*)b).cpu_burst_time) {
				return -1;
			}
			else 
				return 1;
		}
	}
}