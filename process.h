#ifndef PROCESS_H_
#define PROCESS_H_

typedef struct process process;
typedef struct process_list p_list;
typedef struct process_averages p_avgs;

struct process_list {
	process *processes;
	int size;
};

//  times are in milliseconds
struct process {
	int arrival_time;
	int cpu_burst_time;
	int remaining_time;
	int num_bursts;
	int io_time;
	int wait_time;
	int turnaround_time;
	bool arrived;
	bool in_io;
	char process_id;
}; 

struct process_averages {
	float cpu_burst_time;
	float wait_time;
	float turnaround_time;
	int total_cs;
	int preemptions;
};

void print_process_list(process *pl, int size);

void print_process(process p);

void calculate_stats(int total_procs, int N, process *queue, process *pl, p_avgs *averages);

void queue_tostr(process *p, char *str, int size);

int compare_process_by_arrival(const void * a, const void * b);

int compare_process_by_id(const void * a, const void * b);

int compare_process_by_arrival_then_burst(const void * a, const void * b);

int compare_process_by_burst(const void * a, const void * b);

#endif //PROCESS_H_
