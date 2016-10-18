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

	else {
		if((*(process*)a).in_io < (*(process*)b).in_io) {
			return -1;
		}
		else if((*(process*)a).in_io > (*(process*)b).in_io) {
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
}

int compare_io_block(const void * a, const void * b) {
	if((*(io_block*)a).time_unblocked <= (*(io_block*)b).time_unblocked) return -1;
	else return 1;
}