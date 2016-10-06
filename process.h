#ifndef PROCESS_H_
#define PROCESS_H_

typedef struct process process;

//  times are in milliseconds
struct process {
	char process_id;
	int init_arrival_time;
	int cpu_burst_time;
	int num_bursts;
	int io_time;
}; 

void print_process(process p);

#endif //PROCESS_H_
