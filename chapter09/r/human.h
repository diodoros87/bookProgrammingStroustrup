#include <regex.h>

struct Human_t;

typedef struct Human_t Human_t;

extern regex_t * NAME_REGEX;

Human_t* Human_malloc();

int Human_init(Human_t**, const char * filename);

void Human_destroy(Human_t**);

int Human_set(Human_t * object, const char * filename);

int Human_get_name(Human_t * object, char ** filename);

typedef int bool_t;
