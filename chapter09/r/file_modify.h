#ifndef FILE_EDIT_H
#define FILE_EDIT_H

#include <stdio.h>

struct File_modify_t;

typedef struct File_modify_t File_modify_t;

File_modify_t* File_modify_malloc();

int File_modify_init(File_modify_t**, const char * filename);

void File_modify_destroy(File_modify_t**);

int edit_makefile(File_modify_t*);

int File_modify_set(File_modify_t * object, const char * filename);

int File_modify_get_filename(File_modify_t * object, char ** filename);

typedef int bool_t;

bool_t exist_file(const char * const file_name);
FILE*  open_file( const char * const filename, const char * const mode );
char * read_line(const FILE * const file);

#endif
