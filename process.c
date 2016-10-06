#include <stdio.h>
#include <stdlib.h>
#include "process.h"

void print_process(process p) {
	printf("PROCESS -----------------\n");
	printf("|\tprocess_id: %c\n", p.process_id);
	printf("|\tinit_arrival_time: %d\n", p.init_arrival_time);
	printf("|\tcpu_burst_time: %d\n", p.cpu_burst_time);
	printf("|\tnum_bursts: %d\n", p.num_bursts);
	printf("|\tio_time: %d\n", p.io_time);	
}
