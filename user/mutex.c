#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int x = 0;
int main() {
    int mutex = mutex_alloc();
    if (thread_create() == 0) {
        for (int i = 0; i < 500; i++) {
            mutex_lock(mutex);
            x++;
            mutex_unlock(mutex);
        }
        exit(0);
    } else {
        for (int i = 0; i < 500; i++) {
            mutex_lock(mutex);
            x++;
            mutex_unlock(mutex);
        }
    }
    // join child thread
    wait((int *)0);

    printf("%d\n", x);
    mutex_free(mutex);
    exit(0);
}