#ifndef SINGLETON_H
#define SINGLETON_H

#include <stdlib.h>

#define ALLOC(buffer, type, n) \
(buffer) = (type *) malloc ((n) * sizeof(type)); \
if ((buffer) == NULL) { \
   LOG("%s", "out of memory: malloc() returns NULL ");  \
}

#define SINGLETON_STATIC(type, value) type* Singleton_Static_##type() { \
                  static type instance = value;               \
                  return &instance;                       \
               }
               /*
#define SINGLETON(type, function, ...) type** Singleton_##type() { \
                  static type* instance = NULL;                           \
                  if (NULL == instance) {                   \
                     ALLOC(instance, type, 1);           \
                     if (instance)                          \
                        *instance = function(__VA_ARGS__);      \
                  }                                         \
                  return &instance;                       \
               }
               */
               /*
#define SINGLETON(type, function, ...) type* Singleton_##type() { \
                  static type* instance = NULL;                           \
                  if (NULL == instance) {                   \
                     ALLOC(instance, type, 1);           \
                     if (instance)                          \
                        *instance = function(__VA_ARGS__);      \
                  }                                         \
                  return instance;                       \
               }
*/
               
#define SINGLETON(type, function, ...) type* Singleton_##type() { \
                  static type* instance = NULL;             \
                  if (NULL == instance) {                   \
                     ALLOC(instance, type, 1);           \
                     if (instance)                          \
                        function(instance, __VA_ARGS__);      \
                  }                                         \
                  return instance;                       \
               }

#endif
