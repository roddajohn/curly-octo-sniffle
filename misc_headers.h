#ifndef MISC_HEADERS_H
#define MISC_HEADERS_H

int num_frames;
char name[128];

struct vary_node {
  
  char name[128];
  double value;
  struct vary_node *next;
};

void my_main();
void print_knobs();
void process_knobs();
void first_pass();
struct vary_node ** second_pass();

#endif
