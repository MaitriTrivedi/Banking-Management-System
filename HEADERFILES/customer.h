#include "structures.h"
#include <sys/file.h>
#include<unistd.h>
#include <stdbool.h>
#include<stdio.h>

float view_account_balance(int user_id){
    struct Customer tempCustomer;
    memset(&tempCustomer, 0, sizeof(tempCustomer));
    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }

    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        if(tempCustomer.u.userid==user_id){
            return tempCustomer.account_balance;
        }
    }

    close(fd);
    return -1;
}

void deposite_money(int user_id){
    printf("Deposite Money\n");
    struct Customer tempCustomer;
    memset(&tempCustomer, 0, sizeof(tempCustomer));
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        if(tempCustomer.u.userid==user_id){
            printf("Enter Amount To be Deposited : ");
            float amount;
            scanf("%f",&amount);
            tempCustomer.account_balance = tempCustomer.account_balance + amount;
            lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
            if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                perror("Error writing updated balance");
            }
            close(fd);
            return ;
        }
    }
    close(fd);
}

void withdraw_money(int user_id){
    printf("Withdraw Money\n");
    struct Customer tempCustomer;
    memset(&tempCustomer, 0, sizeof(tempCustomer));
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        if(tempCustomer.u.userid==user_id){
            printf("Enter Amount To be Withdrawn : ");
            float amount;
            scanf("%f", &amount);
            if(tempCustomer.account_balance - amount>1000){
                tempCustomer.account_balance = tempCustomer.account_balance - amount;
                lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                    perror("Error writing updated balance");
                }
                close(fd);
                return;
            }
            else{
                printf("Not Enough Amount is there\n");
            }
            close(fd);
            return ;
        }
    }
    close(fd);
}

void transfer_funds(int sender_user_id, int reciever_user_id){
    printf("transfer funds\n");
     
}

void apply_for_a_loan(int user_id){
    printf("apply for loan\n");
    struct Loan loan;

    printf("Enter Amount : ");
    scanf("%f", &loan.loan_amount);

    struct Customer tempCustomer;
    memset(&tempCustomer, 0, sizeof(tempCustomer));
    int fd, bytesRead;
    fd = open("DATABASE/loan.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        if(tempCustomer.u.userid==user_id){
            tempCustomer.loan_taken = true;
            lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
            if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                perror("Error writing updated customer");
            }
            close(fd);
            return;
        }
    }
    close(fd);

    loan.borrower = tempCustomer;
    loan.is_approved = false;
    memset(&loan, 0, sizeof(loan));
    fd = open("DATABASE/loan.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    lseek(fd, 0, SEEK_END);
    write(fd, &loan, sizeof(loan));
    close(fd);

}

void change_password(int user_id){
    printf("change password\n");
    struct Customer tempCustomer;
    memset(&tempCustomer, 0, sizeof(tempCustomer));
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        if(tempCustomer.u.userid==user_id){
            printf("Enter Old Password : ");
            char password[30];
            scanf("%s",password);
            struct User tempUsr;
            hashPassword(password, tempUsr.password);
            if((memcmp(tempUsr.password, tempCustomer.u.password, SHA256_DIGEST_LENGTH)) == 0){
                printf("Enter New Password : ");
                scanf("%s",password);
                hashPassword(password, tempCustomer.u.password);
                lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                    perror("Error writing updated customer");
                }
            }
            else{
                printf("Sorry You cannot change the password because You have entered WRONG Password ...");
            }
            close(fd);
            return ;
        }
    }
    close(fd);
}

void add_feedback(int user_id){
    printf("add feedback\n");
    struct Customer tempCustomer;
    memset(&tempCustomer, 0, sizeof(tempCustomer));
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        if(tempCustomer.u.userid==user_id){
            int fd2 = open("DATABASE/feedback.txt",O_RDWR);
            if(fd2==-1){
                perror("");
                return;
            }
            struct Feedback fdb;
            fdb.customer = tempCustomer;
            char feedback[200];
            printf("Enter Your Feedback :");
            scanf("%s", feedback);
            strcpy(fdb.feedback, feedback);

            int rating=0;
            printf("Enter Your Rating from 1 to 5 :");
            scanf("%d", &rating);
            fdb.rating = rating;

            lseek(fd2, 0, SEEK_END);
            if (write(fd2, &fdb, sizeof(fdb)) == -1) {
                perror("Error writing updated feedback");
            }
        }
    }
    close(fd);
}

void view_transaction_history(){
    printf("View Transaction History\n");
}

void customer_handler(int user_id){
    printf("==========================================\n");
    printf("Select Your Option :\n1. View Account Balance.\n2. Deposite Money\n3. Withdraw Money\n4. Transfer Funds\n5. Apply for a Loan\n6. Change Password\n7. Add Feedback\n8. View Transaction History\n9. Logout\n");
    printf("Enter your choice :");
    int temp_choice;
    scanf("%d",&temp_choice);
    switch(temp_choice){
        case 1:
        {
            float balance = view_account_balance(user_id);
            printf("Your Account Balance is : %f\n", balance);
            break;
        }
        case 2:
        {
            deposite_money(user_id);
            break;
        }
        case 3:
        {
            withdraw_money(user_id);
            break;
        }
        case 4:
        {
            // transfer_funds();
            break;
        }
        case 5:
        {
            apply_for_a_loan(user_id);
            break;
        }
        case 6:
        {
            change_password(user_id);
            break;
        }
        case 7:
        {
            add_feedback(user_id);
            break;
        }
        case 8:
        {
            view_transaction_history(user_id);
            break;
        }
        case 9:
        {
            logout();
            break;
        }
    }
}