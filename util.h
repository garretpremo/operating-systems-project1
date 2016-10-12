#ifndef UTIL_H_
#define UTIL_H_

typedef struct process process;
typedef struct process_list p_list;

void parse(FILE *f, p_list *pl);

FILE *open_file(char *argv[]);

void check_args(char *argv[]);

void print_op(int t, char id, char op[]);

#endif // UTIL_H_