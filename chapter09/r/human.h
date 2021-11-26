#ifndef DEMO_HPP
#define DEMO_HPP

#include <string>
#include <regex.h>

struct Human_t;

typedef struct Human_t Human_t;

extern const regex_t * const NAME_REGEX;

Human_t* Human_malloc();

int Human_init(Human_t**, const char * filename);

void Human_destroy(Human_t**);

int edit_makefile(Human_t*);

int Human_set(Human_t * object, const char * filename);

int Human_get_filename(Human_t * object, char ** filename);

typedef int bool_t;

namespace demo {

class Demo {
   char * name;
   
   static const regex & set_regex();
public:
   Demo(const char *name);
   char * get_name() const;
   void set_name(const char * name);
   ~Demo();
      
   static const regex NAME_REGEX;
};

}

#endif
