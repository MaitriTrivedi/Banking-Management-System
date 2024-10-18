#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"

// Locking function
int lock_record(int fd, int type, ssize_t record_size) {
    struct flock fl;

    fl.l_start = -record_size;  
    fl.l_len = record_size;     
    fl.l_type = type;           
    fl.l_whence = SEEK_CUR;     
    fl.l_pid = getpid();        

    return fcntl(fd, F_SETLKW, &fl);  // Apply the lock, wait if needed
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
