#include "structures.h"
#include <sys/file.h>
#include<unistd.h>
#include <stdbool.h>
#include<stdio.h>
#include "locking.h"
#include<time.h>

#define TRANSACTION_TYPE_SIZE 100

void update_loan_id_by_one(){
    int fd;
    struct stat file_stat;

    // open file in read write file
    fd = open("DATABASE/last_used_loan_id.txt",O_RDWR);
    if(fd==-1){
        perror("update_user_id_by_one : ");
        return;
    }

    // check if the file is empty
    fstat(fd, &file_stat);
    if(file_stat.st_size == 0){
        // file is empty, initialize with 0
        int init_val = 0;
        write(fd, &init_val, sizeof(init_val));
        lseek(fd, 0, SEEK_SET);  // reset file pointer to start
    }
    else{
        int t;
        // write data to the file
        lseek(fd, 0, SEEK_SET);
        read(fd, &t, sizeof(t));
        printf("Last Used User Id : %d\n", t);
        t = t+1;
        
        lseek(fd, 0, SEEK_SET);
        write(fd, &t, sizeof(t));

        lseek(fd, 0, SEEK_SET);
        read(fd, &t, sizeof(t));
        printf("Updated User Id : %d\n", t);
    }

    close(fd); 
    return;
}

int show_loan_id_by_one(){
    int fd;
    int t=0;
    struct stat file_stat;

    // open file in read write file
    fd = open("DATABASE/last_used_loan_id.txt",O_RDWR);
    if(fd==-1){
        perror("show_loan_id_by_one : ");
        return t;
    }

    // check if the file is empty
    fstat(fd, &file_stat);
    if(file_stat.st_size == 0){
        // file is empty, initialize with 0
        int init_val = 0;
        printf("Last Used User Id : %d\n", t);
    }
    else{
        
        lseek(fd, 0, SEEK_SET);
        read(fd, &t, sizeof(t));
        printf("Last Used User Id : %d\n", t);
    }

    close(fd); 
    return t;
}

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
    char buffer[50];
    // int user_id = atoi(buffer); 
    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        printf("%d %d----------------\n", userid, tempCustomer.u.userid);
        if(tempCustomer.u.userid==userid){
            char message[100];
            sprintf(message, "Your Account Balance is : %.2f\n", tempCustomer.account_balance);
            send_message(acpt, message, 0);
            return tempCustomer.account_balance;
        }
    }
    close(fd);
    return continuee(acpt);
}

void add_transaction_history(int sender_uid, int reciever_uid, float amount, int type) {
    struct Transaction transaction;
    memset(&transaction, 0, sizeof(transaction));
    // transaction.transaction_type = malloc(TRANSACTION_TYPE_SIZE * sizeof(char)); // Allocate memory

    // Open customer database file
    int fd = open("DATABASE/transaction.txt", O_RDWR | O_APPEND);
    if (fd == -1) {
        perror("Error opening transaction database");
        return;
    }
    printf("--------------------jj--------------------------\n");
    // Prepare the transaction data
    transaction.amount = amount;
    transaction.reciever_uid = reciever_uid;
    transaction.sender_uid = sender_uid;
    transaction.transaction_time = time(NULL);
    transaction.transaction_type = type; 

    printf("----------------------pp------------------------\n");
    // Lock the record for writing (using custom function)
    // if (lock_record(fd, F_WRLCK, 1) == -1) { // Assuming user type is 1 for transactions
    //     perror("Error locking record");
    //     close(fd);
    //     return;
    // }

    // Move to the end of the file
    lseek(fd, 0, SEEK_END);

    // Write the transaction to the file
    if (write(fd, &transaction, sizeof(transaction)) == -1) {
        perror("Error writing transaction");
        unlock_record(fd, 1);  // Unlock in case of error
        close(fd);
        return;
    }
    printf("----------------------kk------------------------\n");

    // Unlock the record after writing
    // if (unlock_record(fd, 1) == -1) {
    //     perror("Error unlocking record");
    // }

    close(fd);  // Close the file descriptor
}


void add_feedback(int acpt, int user_id) {
    struct Feedback feedback;
    memset(&feedback, 0, sizeof(feedback));

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
            break;
        }
    }
    close(fd);

    // Open customer database file
    fd = open("DATABASE/feedback.txt", O_RDWR | O_APPEND);
    if (fd == -1) {
        perror("Error opening transaction database");
        return;
    }
    char temp_buffer[500];
    show_msg_get_whole_line_as_data(acpt, temp_buffer, "Give Your Feedback : ");
    strcpy(feedback.feedback, temp_buffer);

    char rating[20];
    show_msg_get_data(acpt, rating, "Give Rating between 1(lowest) to 5(highest) : ");
    feedback.rating = atoi(rating);

    // Move to the end of the file
    lseek(fd, 0, SEEK_END);

    // Write the transaction to the file
    if (write(fd, &feedback, sizeof(feedback)) == -1) {
        perror("Error writing transaction");
        unlock_record(fd, 1);  // Unlock in case of error
        close(fd);
        return;
    }
    printf("----------------------kk------------------------\n");
    close(fd);  // Close the file descriptor
}

int deposite_money(int acpt, int userid, size_t recordsize){
    struct Customer tempAdmin;
    memset(&tempAdmin, 0, sizeof(tempAdmin));

    // Open customer database file
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt", O_RDWR);
    if (fd == -1) {
        perror("Error opening customer database");
        return -1;
    }
    char buff[500];
    show_msg_get_data(acpt, buff, "Enter the amount you want to deposit: ");
    
    // Convert the entered amount to float
    float amount = atof(buff);

    // Read through the file to find the user
    while ((bytesRead = read(fd, &tempAdmin, sizeof(tempAdmin))) > 0) {
        if (tempAdmin.u.userid == userid) {
            printf("----------------------------------------------\n");
            // Lock the record for writing
            if (lock_record(fd, F_WRLCK, recordsize) == -1) {
                perror("Error locking record");
                close(fd);
                return -1;
            }

            // Send the current balance to the client
            char message[100];
            sprintf(message, "Current Balance is: %.2f\n", tempAdmin.account_balance);
            send_message(acpt, message, 0);
            printf("----------------------------------------------\n");
            add_transaction_history(userid, -1, amount, 1);
            printf("----------------------------------------------\n");
            // Update the balance
            tempAdmin.account_balance += amount;

            // Move the file pointer back to update the record
            lseek(fd, -sizeof(tempAdmin), SEEK_CUR);
            if (write(fd, &tempAdmin, sizeof(tempAdmin)) == -1) {
                perror("Error writing updated customer");
                unlock_record(fd, recordsize);  // Unlock in case of error
                close(fd);
                return -1;
            }

            // Send the updated balance to the client
            sprintf(message, "Updated Balance is: %.2f\n", tempAdmin.account_balance);
            send_message(acpt, message, 0);

            // Unlock the record after updating
            if (unlock_record(fd, recordsize) == -1) {
                perror("Error unlocking record");
            }

            close(fd);
            return tempAdmin.u.userid;  // Return userid on success
        }
    }

    close(fd);
    return continuee(acpt);  // Call continuee function if user not found
}

int withdraw_money(int acpt, int userid, size_t recordsize){
    struct Customer tempAdmin;
    memset(&tempAdmin, 0, sizeof(tempAdmin));

    // Open customer database file
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt", O_RDWR);
    if (fd == -1) {
        perror("Error opening customer database");
        return -1;
    }
    char buff[500];
    show_msg_get_data(acpt, buff, "Enter the amount you want to withdraw : ");
    
    // Convert the entered amount to float
    float amount = atof(buff);

    // Read through the file to find the user
    while ((bytesRead = read(fd, &tempAdmin, sizeof(tempAdmin))) > 0) {
        if (tempAdmin.u.userid == userid) {
            // Lock the record for writing
            if (lock_record(fd, F_WRLCK, recordsize) == -1) {
                perror("Error locking record");
                close(fd);
                return -1;
            }

            add_transaction_history(userid, -1, amount, 2);

            // Send the current balance to the client
            char message[100];
            sprintf(message, "Current Balance is: %.2f\n", tempAdmin.account_balance);
            send_message(acpt, message, 0);

            // Update the balance
            tempAdmin.account_balance -= amount;

            // Move the file pointer back to update the record
            lseek(fd, -sizeof(tempAdmin), SEEK_CUR);
            if (write(fd, &tempAdmin, sizeof(tempAdmin)) == -1) {
                perror("Error writing updated customer");
                unlock_record(fd, recordsize);  // Unlock in case of error
                close(fd);
                return -1;
            }

            // Send the updated balance to the client
            sprintf(message, "Updated Balance is: %.2f\n", tempAdmin.account_balance);
            send_message(acpt, message, 0);

            // Unlock the record after updating
            if (unlock_record(fd, recordsize) == -1) {
                perror("Error unlocking record");
            }

            close(fd);
            return tempAdmin.u.userid;  // Return userid on success
        }
    }

    close(fd);
    return continuee(acpt);  // Call continuee function if user not found
}

int transfer_funds(int acpt, int userid, size_t recordsize) {
    struct Customer tempAdmin, tempAdmin_reciver;
    memset(&tempAdmin, 0, sizeof(tempAdmin));
    memset(&tempAdmin_reciver, 0, sizeof(tempAdmin_reciver));

    // Open customer database file
    int fd, bytesRead;
    int fd2, bytesRead2;
    fd = open("DATABASE/customer.txt", O_RDWR);
    if (fd == -1) {
        perror("Error opening customer database");
        return -1;
    }

    fd2 = open("DATABASE/customer.txt", O_RDWR);
    if (fd2 == -1) {
        perror("Error opening customer database");
        close(fd);  // Close the first file descriptor before returning
        return -1;
    }

    char uid_reciever[70];
    char buff[500];
    show_msg_get_data(acpt, uid_reciever, "Enter the receiver's user ID: ");
    show_msg_get_data(acpt, buff, "Enter the amount you want to transfer: ");

    // Convert the entered amount to float
    float amount = atof(buff);

    // Read through the file to find the sender
    while ((bytesRead = read(fd, &tempAdmin, sizeof(tempAdmin))) > 0) {
        if (tempAdmin.u.userid == userid) {
            // Find the receiver in the second file descriptor
            while ((bytesRead2 = read(fd2, &tempAdmin_reciver, sizeof(tempAdmin_reciver))) > 0) {
                if (tempAdmin_reciver.u.userid == atoi(uid_reciever)) {
                    // Lock both records for writing
                    if (lock_record(fd, F_WRLCK, recordsize) == -1 || lock_record(fd2, F_WRLCK, recordsize) == -1) {
                        perror("Error locking records");
                        close(fd);
                        close(fd2);
                        return -1;
                    }

                    // Check if sender has enough balance
                    if (tempAdmin.account_balance < amount) {
                        char message[100];
                        sprintf(message, "Insufficient funds. Your current balance is: %.2f\n", tempAdmin.account_balance);
                        send_message(acpt, message, 0);

                        // Unlock both records before returning
                        unlock_record(fd, recordsize);
                        unlock_record(fd2, recordsize);
                        close(fd);
                        close(fd2);
                        return -1;
                    }


                    // Update the receiver's balance
                    char message[100];
                    sprintf(message, "Current Balance of Receiver is: %.2f\n", tempAdmin_reciver.account_balance);
                    send_message(acpt, message, 0);

                    tempAdmin_reciver.account_balance += amount;

                    // Move the file pointer back to update the receiver record
                    lseek(fd2, -sizeof(tempAdmin_reciver), SEEK_CUR);
                    if (write(fd2, &tempAdmin_reciver, sizeof(tempAdmin_reciver)) == -1) {
                        perror("Error writing updated receiver data");
                        // Unlock both records in case of error
                        unlock_record(fd, recordsize);
                        unlock_record(fd2, recordsize);
                        close(fd);
                        close(fd2);
                        return -1;
                    }

                    // Update the sender's balance
                    sprintf(message, "Your Current Balance is: %.2f\n", tempAdmin.account_balance);
                    send_message(acpt, message, 0);

                    tempAdmin.account_balance -= amount;

                    // Move the file pointer back to update the sender record
                    lseek(fd, -sizeof(tempAdmin), SEEK_CUR);
                    if (write(fd, &tempAdmin, sizeof(tempAdmin)) == -1) {
                        perror("Error writing updated sender data");
                        // Unlock both records in case of error
                        unlock_record(fd, recordsize);
                        unlock_record(fd2, recordsize);
                        close(fd);
                        close(fd2);
                        return -1;
                    }

                    // Send the updated balances to both the sender and receiver
                    sprintf(message, "Updated Balance of Receiver is: %.2f\n", tempAdmin_reciver.account_balance);
                    send_message(acpt, message, 0);

                    sprintf(message, "Updated Balance of Yours is: %.2f\n", tempAdmin.account_balance);
                    send_message(acpt, message, 0);

                    add_transaction_history(userid, atoi(uid_reciever), amount, 3);

                    // Unlock both records after updating
                    if (unlock_record(fd, recordsize) == -1 || unlock_record(fd2, recordsize) == -1) {
                        perror("Error unlocking records");
                    }

                    close(fd);
                    close(fd2);
                    return tempAdmin.u.userid;  // Return sender's userid on success
                }
            }
        }
    }

    // Close file descriptors if user not found
    close(fd);
    close(fd2);
    return continuee(acpt);  // Call continuee function if user not found
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

int apply_for_a_loan(int acpt, int userid, size_t record_size){
    struct Customer tempAdmin;
    struct Loan loan;

    record_size = sizeof(struct Customer);
    size_t record_size2 = sizeof(struct Loan);
    
    memset(&tempAdmin, 0, sizeof(tempAdmin));

    // Open customer database file
    int fd, fd2, bytesRead, bytesRead2;
    fd = open("DATABASE/customer.txt", O_RDWR);
    if (fd == -1) {
        perror("Error opening customer database");
        return -1;
    }
    fd2 = open("DATABASE/loan.txt", O_RDWR);
    if (fd2 == -1) {
        perror("Error opening customer database");
        return -1;
    }
    char buff[500];
    show_msg_get_data(acpt, buff, "Enter the amount for which you want to take loan: ");
    
    // Convert the entered amount to float
    float amount = atof(buff);

    // Read through the file to find the user
    while ((bytesRead = read(fd, &tempAdmin, sizeof(tempAdmin))) > 0) {
        if (tempAdmin.u.userid == userid) {
            // Lock the record for writing
            if (lock_record(fd, F_WRLCK, record_size) == -1) {
                perror("Error locking record");
                close(fd);
                return -1;
            }
            if (lock_record(fd2, F_WRLCK, record_size2) == -1) {
                perror("Error locking record");
                close(fd);
                return -1;
            }

            // Update the balance
            tempAdmin.loan_taken = true;

            // Move the file pointer back to update the record
            lseek(fd, -sizeof(tempAdmin), SEEK_CUR);
            if (write(fd, &tempAdmin, sizeof(tempAdmin)) == -1) {
                perror("Error writing updated customer");
                unlock_record(fd, record_size);  // Unlock in case of error
                close(fd);
                return -1;
            }

            loan.approving_employee_id = -1;
            loan.borrower = tempAdmin;
            loan.is_completed = false;
            loan.is_approved = false;
            loan.loan_amount = amount;
            loan.loan_id = show_user_id_by_one() + 1;

            lseek(fd2, 0, SEEK_END);

            if (write(fd2, &loan, sizeof(loan)) == -1) {
                perror("Error writing updated balance");
            }
            
            // Unlock the record after updating
            if (unlock_record(fd2, record_size2) == -1) {
                perror("Error unlocking record");
            }

            // Unlock the record after updating
            if (unlock_record(fd, record_size) == -1) {
                perror("Error unlocking record");
            }

            close(fd);
            close(fd2);
            return tempAdmin.u.userid;  // Return userid on success
        }
    }
    printf("================>>>>");
    close(fd);
    return continuee(acpt);  
}// Call continuee function if user not fou

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

// void add_feedback(int user_id){
//     printf("add feedback\n");
//     struct Customer tempCustomer;
//     memset(&tempCustomer, 0, sizeof(tempCustomer));
//     int fd, bytesRead;
//     fd = open("DATABASE/customer.txt",O_RDWR);
//     if(fd==-1){
//         perror("");
//         return;
//     }
//     while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
//         if(tempCustomer.u.userid==user_id){
//             int fd2 = open("DATABASE/feedback.txt",O_RDWR);
//             if(fd2==-1){
//                 perror("");
//                 return;
//             }
//             struct Feedback fdb;
//             fdb.customer = tempCustomer;
//             char feedback[200];
//             printf("Enter Your Feedback :");
//             scanf("%s", feedback);
//             strcpy(fdb.feedback, feedback);

//             int rating=0;
//             printf("Enter Your Rating from 1 to 5 :");
//             scanf("%d", &rating);
//             fdb.rating = rating;

//             lseek(fd2, 0, SEEK_END);
//             if (write(fd2, &fdb, sizeof(fdb)) == -1) {
//                 perror("Error writing updated feedback");
//             }
//         }
//     }
//     close(fd);
// }

void view_transaction_history(){
    printf("View Transaction History\n");
}

int customer_handler(int acpt, int login_success_user_id) {
    char buffer[500];
    int choice = 4;
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
    printf("customer selected choice: %d\n", temp_choice);

    // Step 5: Handle the admin's choice using switch case
    switch (temp_choice) {
        case 1:
            char userid_buffer[10];
            printf("Case 1: View My Account Balance\n");
            show_msg_get_data(acpt, userid_buffer, "Enter your user id :");
            printf("entert uid %d\n",atoi(userid_buffer));
            view_account_balance(acpt, temp_choice, atoi(userid_buffer));  
            if(continuee(acpt)==0){
                return 7;
            }
            return 0;
            // break;
        case 2:
            printf("Case 2: Deposite Money\n");
            deposite_money(acpt, login_success_user_id, sizeof(struct Customer));
            if(continuee(acpt)==0){
                return 7;
            }
            return 0;
            break;
        case 3:
            printf("Case 3: Withdraw Money\n");
            withdraw_money(acpt, login_success_user_id, sizeof(struct Customer));
            if(continuee(acpt)==0){
                return 7;
            }
            return 0;
            break;
        case 4:
            printf("Case 4: Transfer Funds\n");
            transfer_funds(acpt, login_success_user_id, 4);
            if(continuee(acpt)==0){
                return 7;
            }
            return 0;
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
                    {    printf("press enter to logout\n");
                    getchar();
                    logout_customer(login_success_user_id);
                    break;
                    }
                default:
                    break;
            }
            printf("Case 7: Logout  --ENDDD  %d\n", choice);
            return 7;  // Exit the admin handler
        case 8:
            printf("Case 8: Apply for a loan\n");
            apply_for_a_loan(acpt, login_success_user_id, sizeof(struct Customer));
            if(continuee(acpt)==0){
                return 7;
            }
            return 0;
            break;
        case 9:
            printf("Case 9: Add Feedback\n");
            add_feedback(acpt, login_success_user_id);
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