#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/file.h>
#include<unistd.h>
#include "../HEADERFILES/structures.h" // Include the structures header

int main() {
    const char *filename = "../DATABASE/feedback.txt"; // File containing admin data

    struct Feedback feedback;
    memset(&feedback, 0, sizeof(feedback));

    // open admin database file
    int fd, bytesRead;
    fd = open("../DATABASE/feedback.txt",O_RDWR);
    if(fd==-1){
        perror("==");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    printf("UID USERNAME RATING              FEEDBACK\n");
    while((bytesRead = read(fd, &feedback, sizeof(feedback))) > 0 ){
        printf("%d     %s    %d    %s\n", feedback.customer.u.userid, feedback.customer.u.username, feedback.rating, feedback.feedback);
    }
    close(fd);
    return 0;
}
