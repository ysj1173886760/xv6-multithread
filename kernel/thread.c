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

struct CV {
  int allocated;
};

struct Mutex mutex_table[MUTEX_NUM];
struct spinlock mutex_table_lock;

struct CV cv_table[CV_NUM];
struct spinlock cv_table_lock;

void thread_init() {
  initlock(&mutex_table_lock, "mutex table lock");
  initlock(&cv_table_lock, "cv table lock");
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
}

uint64 sys_cv_alloc(void) {
  int id = -1;
  acquire(&cv_table_lock);
  for (int i = 0; i < CV_NUM; i++) {
    if (cv_table[i].allocated == 0) {
      cv_table[i].allocated = 1;
      id = i;
      break;
    }
  }
  release(&cv_table_lock);
  return id;
}

uint64 sys_cv_wait(void) {
  int cv_id;
  int mutex_id;
  if (argint(0, &cv_id) < 0) {
    panic("failed to get cv id");
  }
  if (argint(1, &mutex_id) < 0) {
    panic("failed to get mutex id");
  }

  struct proc *p = myproc();
  acquire(&p->lock);
  releasesleep(&mutex_table[mutex_id].lock);

  p->chan = &cv_table[cv_id];
  p->state = SLEEPING;

  sched();

  p->chan = 0;

  release(&p->lock);
  acquiresleep(&mutex_table[mutex_id].lock);
  
  return 0;
}

uint64 sys_cv_signal(void) {
  int cv_id;
  if (argint(0, &cv_id) < 0) {
    panic("failed to get cv id");
  }
  wakeup(&cv_table[cv_id]);
  return 0;
}

uint64 sys_cv_free(void) {
  int id = 0;
  if (argint(0, &id) < 0) {
    panic("failed to get cv id");
  }
  acquire(&cv_table_lock);
  cv_table[id].allocated = 0;
  release(&cv_table_lock);
  return 0;
}