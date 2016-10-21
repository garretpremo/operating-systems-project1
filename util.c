/*
	Utility file for Operating Systems - Project 1
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "process.h"

#define DEFAULT_NUM_PROCESSES 4

char INVALID_ARGS[] = "ERROR: Invalid arguments\nUSAGE: ./a.out <input-file>  <stats-output-file>\n";

/*
	Parse input file and return a list of processes

	See process.h
*/
void parse(FILE *f, p_list *proclist) {
	char c;
	char buffer[5];
	int buffer_c = 0;
	bool comment = false;
	bool newline = false;
	int proc_def = 0;
	int max_procs = DEFAULT_NUM_PROCESSES;	
	process p;

	/*
		reads each character from file and populates
		pl.processes dynamically.

		also keeps track of the number of processes via pl.size
	*/
    while ((c = getc(f)) != EOF) {
		switch(c) {
			case '#':
				comment = true;
				continue;
			case '\n':
				if(comment || newline) {
					comment = false;
					newline = true;
					continue;
				}
				newline = true;
				if(proc_def == 4) {
					// set io time
					p.io_time = atoi(buffer);

				}
				// add process to array
				if(proclist->size == max_procs-2) {
					max_procs = max_procs * 2;
					proclist->processes = (process *)realloc(proclist->processes, max_procs*sizeof(process));
				}
				p.arrived = false;
				p.in_io = false;
				proclist->processes[proclist->size] = p;
				proclist->size += 1;
				memset(&buffer[0], '\0', sizeof(buffer));
				buffer_c = 0;
				proc_def = 0;
				continue;
			default:
				newline = false;
				if(comment) continue;
				
				if(c == '|') {
					// populate current process
					switch(proc_def) {
						case 0:
							p.process_id = buffer[buffer_c-1];
						case 1:
							p.arrival_time = atoi(buffer);
						case 2:
							p.cpu_burst_time = atoi(buffer);
							p.remaining_time = p.cpu_burst_time;
						case 3:
							p.num_bursts = atoi(buffer);
						default: // reset buffer
							memset(&buffer[0], '\0', sizeof(buffer));
							buffer_c = 0;
					}
					proc_def += 1;	
				} else {
					buffer[buffer_c] = c;
					buffer_c += 1;
				}
		}
	}
}


/*
	Opens inputted file
*/
FILE *open_file(char arg[], char p[]) {
    FILE *fp;
    fp = fopen(arg, p);
    if (fp == NULL) {
        perror(INVALID_ARGS);
		exit(EXIT_FAILURE);
    }
    return fp;
}

/*
	Ensures we have correct arguments for our program
*/
void check_args(char *argv[]) {
    if(!argv[1] || !argv[2]) {
        perror(INVALID_ARGS);
		exit(EXIT_FAILURE);
	}
}

void print_op(int t, process p, char op[], process *ps, int ready) {
	// printf("%d\n", ready);
	char prnt[(ready*2+1)];
	int i;
	for(i = 0; i < ready; i++) {
		prnt[i*2] = ' ';
		prnt[i*2+1] = ps[i].process_id;
	}
	prnt[ready*2] = '\0';

	if (strcmp(op, "prmp") == 0) {
		fprintf(stdout, "time %dms: Time slice expired; process %c preempted with %dms to go",
						t, p.process_id, p.remaining_time);
	} else if(strcmp(op, "nprmp") == 0) {
		fprintf(stdout, "time %dms: Time slice expired; no preemption because ready queue is empty",
						t);
	} else {
		fprintf(stdout, "time %dms: Process %c ", t, p.process_id);
	}
	if(strcmp(op, "scpu") == 0){
		fprintf(stdout, "started using the CPU");
	} else if (strcmp(op, "fcpu") == 0) {
		fprintf(stdout, "completed a CPU burst; %d to go", p.num_bursts);
	} else if (strcmp(op, "sio") == 0) {
		fprintf(stdout, "blocked on I/O until time %dms", t + p.io_time);
	} else if (strcmp(op, "fio") == 0) {
		fprintf(stdout, "completed I/O");
	} else if (strcmp(op, "rdy") == 0) {
		fprintf(stdout, "arrived");
	} else if (strcmp(op, "end") == 0) {
		fprintf(stdout, "terminated");
	}
	if(ready == 0) {
		fprintf(stdout, " [Q empty]\n");
	} else
		fprintf(stdout, " [Q%s]\n", prnt);
}

bool in_array(process p, process *processes, int size) {
	int i;
	for(i = 0; i < size; i++) {
		if(p.process_id == processes[i].process_id) {
			return true;
		}
	}
	return false;
}
