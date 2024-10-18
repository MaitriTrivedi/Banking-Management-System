#include "structures.h"
#include <sys/file.h>
#include<unistd.h>
#include <stdbool.h>
#include<stdio.h>

int view_account_balance(int acpt, int type, int userid){
    printf("In view acc balance ---- %d\n", userid);
    struct Customer tempCustomer;
    memset(&tempCustomer, 0, sizeof(tempCustomer));
    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }
    char buffer[5];
    // show_msg_get_data(acpt, buffer, "Enter User Id You want to activate or deactivate :\n");
    int user_id = atoi(buffer); 
    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        if(tempCustomer.u.userid==user_id){
            return tempCustomer.account_balance;
        }
    }
    close(fd);
    return continuee(acpt);
}

float view_account_balance2(int user_id){
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

int customer_handler(int acpt, int login_success_user_id) {
    char buffer[500];
    int choice = 2;
    printf("================== in customer handler ==================\n");

    // Step 1: Receive ready signal from client
    printf("Waiting to receive ready signal from client...\n");
    if (recv(acpt, buffer, sizeof(buffer), 0) == -1) {
        perror("Error receiving ready signal");
    } else {
        printf("Ready signal received: %s\n", buffer);
    }
    printf("Received ready signal from client \n============================================\n");

    // Step 2: Send type signal back to the client
    strcpy(buffer, "1"); // Type 1 operation
    printf("Sending type signal to client...\n");
    if (send(acpt, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("Error sending type signal");
    } else {
        printf("Type signal sent: %s\n", buffer);
    }
    printf("Sent type signal to client \n============================================\n");

    // Step 3: Send the customer menu to the client
    strcpy(buffer, "==========================================\nSelect Your Option :\n1. View My account Balance\n2. Deposite Money\n3. Withdraw Money\n4. Transfer Funds\n5. Change Password\n6. Exit\n7. Logout\n8. Apply for a loan\n9. Add Feedback\n10. View Transaction History\n");
    printf("Sending customer menu to client...\n");
    if (send(acpt, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("Error sending admin menu");
    } else {
        printf("customer menu sent: %s\n", buffer);
    }
    printf("Sent customer menu \n============================================\n");

    // Step 4: Receive admin's choice
    printf("Waiting to receive customer's choice...\n");
    if (recv(acpt, buffer, sizeof(buffer), 0) == -1) {
        perror("Error receiving customer's choice");
    } else {
        printf("customer's choice received: %s\n", buffer);
    }
    printf("Received customer's choice \n============================================\n");

    // Convert choice to integer
    int temp_choice = atoi(buffer);
    printf("manager selected choice: %d\n", temp_choice);

    // Step 5: Handle the admin's choice using switch case
    switch (temp_choice) {
        case 1:
            char buffer[10];
            printf("Case 1: View My Account Balance\n");
            show_msg_get_data(acpt, buffer, "Enter your user id :");
            return view_account_balance(acpt, temp_choice, atoi(buffer));  // Function to add a new user
            // break;
        case 2:
            printf("Case 2: Deposite Money\n");
            // return activate_deactivate_customer_account(acpt, temp_choice, ); 
            // Logic for viewing managers here
            break;
        case 3:
            printf("Case 3: Withdraw Money\n");
            return assign_loan_applications_to_emp(acpt, temp_choice); 
            // Logic for viewing employees here
            break;
        case 4:
            printf("Case 4: Transfer Funds\n");
            return review_customers_feedback(acpt, temp_choice); 
            // Logic for viewing customers here
            break;
        case 5:
            printf("Case 5: Change Password\n");
            return change_password_common(acpt, login_success_user_id, 2);
            return 0;  // Exit the admin handler
        case 6:
            printf("Case 6: Exiting the manager handler\n");
            return 6;  // Exit the admin handler
        case 7:
            printf("Case 7: Logout  -- %d\n", choice);
            switch (choice){
                case 1:
                    logout_admin(login_success_user_id);
                    break;
                case 2:
                    logout_manager(login_success_user_id);
                    break;
                case 3:
                    logout_employee(login_success_user_id);
                    break;
                case 4:
                    logout_customer(login_success_user_id);
                    break;
                default:
                    break;
            }
            printf("Case 7: Logout  --ENDDD  %d\n", choice);
            return 7;  // Exit the admin handler
        case 8:
            printf("Case 8: Apply for a loan\n");
            return change_password_common(acpt, login_success_user_id, 2);
            return 0;  // Exit the admin handler
        case 9:
            printf("Case 9: Add Feedback\n");
            return change_password_common(acpt, login_success_user_id, 2);
            return 0;  // Exit the admin handler
        case 10:
            printf("Case 10: View Transaction History\n");
            return change_password_common(acpt, login_success_user_id, 2);
            return 0;  // Exit the admin handler
        default:
            printf("Invalid choice. Please select a valid option.\n");
            break;
    }
    printf("================== End of customer handler ==================\n");
    return 0;
}

void customer_handler2(int user_id){
    printf("==========================================\n");
    printf("Select Your Option :\n1. View Account Balance.\n2. Deposite Money\n3. Withdraw Money\n4. Transfer Funds\n5. Apply for a Loan\n6. Change Password\n7. Add Feedback\n8. View Transaction History\n9. Logout\n");
    printf("Enter your choice :");
    int temp_choice;
    scanf("%d",&temp_choice);
    switch(temp_choice){
        case 1:
        {
            float balance = view_account_balance2(user_id);
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