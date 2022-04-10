#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"


uint64 sys_thread_create(void) {
  return thread_create();
}

uint64 sys_mutex_alloc(void) {
  return 0;
}

uint64 sys_mutex_lock(void) {
  return 0;
}

uint64 sys_mutex_unlock(void) {
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