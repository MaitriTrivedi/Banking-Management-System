#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/file.h>
#include<unistd.h>
#include "HEADERFILES/structures.h" // Include the structures header

int main() {
    const char *filename = "DATABASE/customer.txt"; // File containing admin data

    struct Customer tempAdmin;
    memset(&tempAdmin, 0, sizeof(tempAdmin));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }

    int id;
    printf("Enter cust id : ");
    scanf("%d",&id);

    while((bytesRead = read(fd, &tempAdmin, sizeof(tempAdmin))) > 0 ){
        if(tempAdmin.u.userid==id){
            int balance;
            printf("Enter New Balance : ");
            scanf("%d",&balance);
            tempAdmin.account_balance = balance;
            lseek(fd, -sizeof(tempAdmin), SEEK_CUR);
            if (write(fd, &tempAdmin, sizeof(tempAdmin)) == -1) {
                perror("Error writing updated customer");
            }
            printf("DONE\n");
            break;
        }
    }

    close(fd);
    return -1;

    return 0;
}
