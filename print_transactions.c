#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/file.h>
#include<unistd.h>
#include "HEADERFILES/structures.h" // Include the structures header

int main() {
    const char *filename = "DATABASE/transaction.txt"; // File containing admin data

    struct Transaction tempAdmin;
    memset(&tempAdmin, 0, sizeof(tempAdmin));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/transaction.txt",O_RDWR);
    if(fd==-1){
        perror("==");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    while((bytesRead = read(fd, &tempAdmin, sizeof(tempAdmin))) > 0 ){
        char type_str[50];
        char *time_str = ctime(&tempAdmin.transaction_time);
        time_str[strlen(time_str) - 1] = '\0'; 
        if(tempAdmin.transaction_type==1) strcpy(type_str, "Deposite");
        else if(tempAdmin.transaction_type==2) strcpy(type_str, "Withdraw");
        else if(tempAdmin.transaction_type==3) strcpy(type_str, "Transfer");
        printf("%d %d %.2f %s %d \n", tempAdmin.sender_uid, tempAdmin.reciever_uid, tempAdmin.amount, time_str, tempAdmin.transaction_type);
    }

    close(fd);
    return -1;

    return 0;
}
