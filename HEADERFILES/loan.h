#include "structures.h"
#include <sys/file.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

void approve_or_disapprove_loan_applications(){

}

void view_loan_applications(){
    int fd;

    // open file in read write file
    fd = open("DATABASE/loan.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    
    int bytesRead;
    int i=1;
    struct Loan loan;
    while((bytesRead = read(fd, &loan, sizeof(loan))) > 0 ){
        printf("%d %s %d %f %d\n", i, loan.borrower.u.username, loan.borrower.account_no, loan.borrower.account_balance, loan.loan_id);
        i++;
    }

    close(fd); 

    return ;
}
