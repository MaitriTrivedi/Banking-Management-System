#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/file.h>
#include<unistd.h>
#include "HEADERFILES/structures.h" // Include the structures header

int main() {
    const char *filename = "DATABASE/loan.txt"; // File containing admin data

    struct Loan loan;
    memset(&loan, 0, sizeof(loan));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/loan.txt",O_RDWR);
    if(fd==-1){
        perror("==");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    while((bytesRead = read(fd, &loan, sizeof(loan))) > 0 ){
        // printf("%s %s\n",tempAdmin.u.username, tempAdmin.u.password);
        // printf("%s %s\n",a.u.username, a.u.password);
        // printf("%d\n", (memcmp(tempAdmin.u.password, a.u.password, SHA256_DIGEST_LENGTH)));
        printf("Loan ID: %d, Loan Amount: %.2f, Is Approved: %d, Is Completed: %d, Approving Employee ID: %d Userid: %d username %s\n", loan.loan_id, loan.loan_amount, loan.is_approved, loan.is_completed, loan.approving_employee_id, loan.borrower.u.userid, loan.borrower.u.username);

    }

    close(fd);
    return -1;

    return 0;
}
