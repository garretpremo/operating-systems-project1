/*
	Utility file for Operating Systems - Project 1
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "process.h"

/*
	Parse input file
*/
process parse(FILE *f) {
	char c;
	char buffer[5];
	int buffer_c = 0;
	bool comment = false;
	int proc_def = 0;
	process p;	

    while ((c = getc(f)) != EOF) {
		switch(c) {
			case '#':
				comment = true;
				continue;
			case '\n':
				comment = false;
				continue;
			default:
				if(comment) continue;
				
				if(c == '|') {
					switch(proc_def) {
						case 0:
							p.process_id = buffer[buffer_c-1];
						case 1:
							p.init_arrival_time = atoi(buffer);
						case 2:
							p.cpu_burst_time = atoi(buffer);
						case 3:
							p.num_bursts = atoi(buffer);
						case 4:
							p.io_time = atoi(buffer);
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
	return p;
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
