#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/file.h>
#include<unistd.h>
#include "../HEADERFILES/structures.h" // Include the structures header

int main() {
    const char *filename = "../DATABASE/customer.txt"; // File containing admin data

    struct Customer tempAdmin;
    memset(&tempAdmin, 0, sizeof(tempAdmin));

    // open admin database file
    int fd, bytesRead;
    fd = open("../DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("==");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    printf("UID ACTIVE LOGGEDIN USERNAME ACC_NO   BALANCE   LOAN_TAKEN            PASSWORD\n");
    while((bytesRead = read(fd, &tempAdmin, sizeof(tempAdmin))) > 0 ){
        // printf("%s %s\n",tempAdmin.u.username, tempAdmin.u.password);
        // printf("%s %s\n",a.u.username, a.u.password);
        // printf("%d\n", (memcmp(tempAdmin.u.password, a.u.password, SHA256_DIGEST_LENGTH)));
        printf("%d      %d       %d        %s      %d     %.2f      %d          %s\n", tempAdmin.u.userid, tempAdmin.u.is_active, tempAdmin.u.is_logged_in, tempAdmin.u.username, tempAdmin.account_no, tempAdmin.account_balance, tempAdmin.loan_taken, tempAdmin.u.password);
    }

    close(fd);
    return -1;

    return 0;
}
