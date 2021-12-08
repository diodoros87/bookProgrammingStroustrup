#ifndef HUMAN_PRIVATE_H
#define HUMAN_PRIVATE_H

struct Human_t {
   char * name;
};

void Human_destroy_protected(Human_t * const object);

#endif
