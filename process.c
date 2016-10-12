#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

void print_process_list(process *pl, int size) {
	int i;
	for(i = 0; i < size; i++) {
		print_process(pl[i]);
	}
}

void print_process(process p) {
	printf("|--- PROCESS -----------------\n");
	printf("|\tprocess_id: %c\n", p.process_id);
	printf("|\tarrival_time: %d\n", p.arrival_time);
	printf("|\tcpu_burst_time: %d\n", p.cpu_burst_time);
	printf("|\tnum_bursts: %d\n", p.num_bursts);
	printf("|\tio_time: %d\n", p.io_time);	
}

int compare_process_by_arrival(const void * a, const void * b) {
	if((*(process*)a).arrival_time <= (*(process*)b).arrival_time) return -1;
	else return 1;
}

int compare_process_by_id(const void * a, const void * b) {
	if((*(process*)a).process_id <= (*(process*)b).process_id) return -1;
	else return 1;
}

int compare_process_by_burst(const void * a, const void * b) {
	if((*(process*)a).arrival_time + (*(process*)a).cpu_burst_time <= (*(process*)b).arrival_time + (*(process*)b).cpu_burst_time) return -1;
	else return 1;
}