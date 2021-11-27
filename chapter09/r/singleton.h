#define SINGLETON(t, inst, init) t* Singleton_##t() { \
                 static t inst = init;               \
                 return &inst;                       \
                }
