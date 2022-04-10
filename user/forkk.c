#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main() {
    for (int i = 0; i < 2; i++) {
        fork();
        printf("-");
    }
    exit(0);
}