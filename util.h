#ifndef UTIL_H_
#define UTIL_H_

typedef struct process process;

process parse(FILE *f);

FILE *open_file(char *argv[]);

void check_args(char *argv[]);
 
#endif // UTIL_H_
