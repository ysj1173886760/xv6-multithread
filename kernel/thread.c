#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "sleeplock.h"

struct Mutex {
  struct sleeplock lock;
  int allocated;
};

struct Mutex mutex_table[MUTEX_NUM];
struct spinlock mutex_table_lock;

void thread_init() {
  initlock(&mutex_table_lock, "mutex table lock");
  for (int i = 0; i < MUTEX_NUM; i++) {
    initsleeplock(&mutex_table[i].lock, "mutex");
  }
}

uint64 sys_thread_create(void) {
  return thread_create();
}

uint64 sys_mutex_alloc(void) {
  acquire(&mutex_table_lock);
  int id = -1;
  for (int i = 0; i < MUTEX_NUM; i++) {
    if (mutex_table[i].allocated == 0) {
      mutex_table[i].allocated = 1;
      id = i;
      break;
    }
  }
  release(&mutex_table_lock);
  return id;
}

uint64 sys_mutex_lock(void) {
  int id = 0;
  if (argint(0, &id) < 0) {
    panic("failed to get lock id");
  }
  acquiresleep(&mutex_table[id].lock);
  return 0;
}

uint64 sys_mutex_unlock(void) {
  int id = 0;
  if (argint(0, &id) < 0) {
    panic("failed to get lock id");
  }
  releasesleep(&mutex_table[id].lock);
  return 0;
}

uint64 sys_mutex_free(void) {
  int id = 0;
  if (argint(0, &id) < 0) {
    panic("failed to get lock id");
  }
  acquire(&mutex_table_lock);
  mutex_table[id].allocated = 0;
  // do we need to check whether lock is held?
  // for now, i decide to push the responsibility related to safety to user
  // i.e. user is responsible to unlock the mutex and free it before exiting
  // another valid option is instead letting user to free mutex, we track the all the mutex that is 
  // allocated by user, and free them while freeing proc
  release(&mutex_table_lock);
  return 0;
  return 0;
}

uint64 sys_cv_create(void) {
  return 0;
}

uint64 sys_cv_wait(void) {
  return 0;
}

uint64 sys_cv_signal(void) {
  return 0;
}

uint64 sys_cv_free(void) {
  return 0;
}