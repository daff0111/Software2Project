#include "event.h"
#include <atomic>

sem_t *mango::Event::semaphore=NULL;
std::atomic<int> mango::Event::id_gen;
