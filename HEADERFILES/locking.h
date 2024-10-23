#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"
#include <errno.h>

// Locking function
int lock_record(int fd, int type, ssize_t record_size) {
    struct flock fl;

    fl.l_start = -record_size;  
    fl.l_len = record_size;     
    fl.l_type = type;           
    fl.l_whence = SEEK_CUR;     
    fl.l_pid = getpid();        

    printf("Attempting to acquire lock...\n");

    // Loop until the lock is acquired successfully
    while (fcntl(fd, F_SETLKW, &fl) == -1) {
        // If lock acquisition fails due to an interrupt, print a message and retry
        if (errno == EINTR) {
            printf("Interrupted while waiting for lock. Retrying...\n");
            continue;
        } else {
            perror("Error acquiring lock");
            return -1;
        }
    }

    printf("Lock acquired successfully.\n");
    return 0;
}



int unlock_record(int fd, ssize_t record_size) {
    struct flock fl;

    // Configure the flock structure for unlocking
    fl.l_start = -record_size;
    fl.l_len = record_size;   
    fl.l_type = F_UNLCK;      
    fl.l_whence = SEEK_CUR;   
    fl.l_pid = getpid();

    return fcntl(fd, F_SETLK, &fl);
}
