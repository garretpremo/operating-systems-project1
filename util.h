#ifndef UTIL_H_
#define UTIL_H_

typedef struct process process;
typedef struct process_list p_list;

void parse(FILE *f, p_list *pl);

FILE *open_file(char *argv[]);

void check_args(char *argv[]);

void print_op(int t, process p, char op[], process *ps, int ready);

bool in_array(process p, process *ps, int size);

#endif // UTIL_H_