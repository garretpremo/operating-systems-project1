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
FILE *open_file(char *argv[]) {
	check_args(argv);
    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("ERROR: Invalid aruments\nUSAGE: ./a.out <input-file>\n");
		exit(EXIT_FAILURE);
    }
    return fp;
}

/*
	Ensures we have correct arguments for our program
*/
void check_args(char *argv[]) {
    if(!argv[1]) {
        perror("ERROR: Invalid aruments\nUSAGE: ./a.out <input-file>\n");
		exit(EXIT_FAILURE);
	}
}

void print_op(int t, char id, char op[]) {
	if(strcmp(op, "scpu") == 0){
		printf("time %dms: Process %c starts using the CPU\n", t, id);
	} else if (strcmp(op, "fcpu") == 0) {
		printf("time %dms: Process %c finishes using the CPU\n", t, id);
	} else if (strcmp(op, "sio") == 0) {
		printf("time %dms: Process %c starts performing I/O\n", t, id);
	} else if (strcmp(op, "fio") == 0) {
		printf("time %dms: Process %c finishes performing I/O\n", t, id);
	} else if (strcmp(op, "strt") == 0) {
		printf("time %dms: Process %c starts\n", t, id);
	} else if (strcmp(op, "end") == 0) {
		printf("time %dms: Process %c terminates\n", t, id);
	}
}