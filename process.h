#ifndef PROCESS_H_
#define PROCESS_H_

typedef struct process process;
typedef struct process_list p_list;

struct process_list {
	process *processes;
	int size;
};

//  times are in milliseconds
struct process {
	char process_id;
	int arrival_time;
	int cpu_burst_time;
	int num_bursts;
	int io_time;
	int wait_time;
	int turnaround_time;
}; 

void print_process_list(process *pl, int size);

void print_process(process p);

int compare_process_by_arrival(const void * a, const void * b);

int compare_process_by_id(const void * a, const void * b);

#endif //PROCESS_H_
