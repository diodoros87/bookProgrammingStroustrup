#ifndef DEMO_HPP
#define DEMO_HPP

#include <string>
#include <regex.h>

struct Human_t;

typedef struct Human_t Human_t;

const regex_t NAME_REGEX = set_regex();

Human_t* Human_malloc();

int Human_init(Human_t*, const char * filename);

void Human_destroy(Human_t*);

int edit_makefile(Human_t*);

int Human_set(Human_t * object, const char * filename);

int Human_get_filename(Human_t * object, char * filename);

typedef int bool_t;

bool_t exist_file(const char * const file_name);
FILE*  open_file( const char * const filename, const char * const mode );
char * read_line(const FILE * const file);
char * concatenate(const char * first, const char * second);

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
