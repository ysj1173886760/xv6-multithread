#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int flag = 0;
int main() {
    int mutex = mutex_alloc();
    int cv = cv_alloc();
    if (thread_create() == 0) {
        int x = 0;
        for (int i = 0; i < 1000; i++) {
            x++;
        }
        printf("x = %d\n", x);
        mutex_lock(mutex);
        flag = 1;
        mutex_unlock(mutex);
        cv_signal(cv);
        exit(0);
    }
    mutex_lock(mutex);
    while (flag == 0) {
        cv_wait(cv, mutex);
    }
    printf("flag = %d\n", flag);
    mutex_unlock(mutex);

    wait((int *)0);

    mutex_free(mutex);
    cv_free(cv);
    exit(0);
}