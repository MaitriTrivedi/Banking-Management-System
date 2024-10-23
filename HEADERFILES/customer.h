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
        // printf("Last Used User Id : %d\n", t);
        t = t+1;
        
        lseek(fd, 0, SEEK_SET);
        write(fd, &t, sizeof(t));

        lseek(fd, 0, SEEK_SET);
        read(fd, &t, sizeof(t));
        // printf("Updated User Id : %d\n", t);
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
        // printf("Last Used User Id : %d\n", t);
    }
    else{
        
        lseek(fd, 0, SEEK_SET);
        read(fd, &t, sizeof(t));
        // printf("Last Used User Id : %d\n", t);
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
        // printf("%d %d----------------\n", userid, tempCustomer.u.userid);
        if(tempCustomer.u.userid==userid){
            char message[100];
            sprintf(message, "Your Account Balance is : %.2f\n", tempCustomer.account_balance);
            send_message(acpt, message, 0);
            int temp = continuee(acpt);
            // printf("=====temp %d\n",temp);
            return temp;
        }
    }
    close(fd);
    // return continuee(acpt);
    return 1;
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

int add_feedback(int acpt, int user_id) {
    struct Feedback feedback;
    memset(&feedback, 0, sizeof(feedback));

    struct Customer tempCustomer;
    memset(&tempCustomer, 0, sizeof(tempCustomer));
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        int temp = continuee(acpt);
        // printf("=====temp %d\n",temp);
        return temp;
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
        int temp = continuee(acpt);
        // printf("=====temp %d\n",temp);
        return temp;
    }
    char temp_buffer[500];
    show_msg_get_whole_line_as_data(acpt, temp_buffer, "Give Your Feedback : ");
    strcpy(feedback.feedback, temp_buffer);

    char rating[20];
    show_msg_get_data(acpt, rating, "Give Rating between 1(lowest) to 5(highest) : ");
    feedback.rating = atoi(rating);
    feedback.customer =  tempCustomer;

    // Move to the end of the file
    lseek(fd, 0, SEEK_END);

    // Write the transaction to the file
    if (write(fd, &feedback, sizeof(feedback)) == -1) {
        perror("Error writing transaction");
        unlock_record(fd, 1);  // Unlock in case of error
        close(fd);
        int temp = continuee(acpt);
        // printf("=====temp %d\n",temp);
        return temp;
    }
    // printf("----------------------kk------------------------\n");
    close(fd);  // Close the file descriptor
    int temp = continuee(acpt);
    // printf("=====temp %d\n",temp);
    return temp;
}

int deposite_money(int acpt, int userid, size_t recordsize, int loan_amount){
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
    float amount;
    if(loan_amount==-1){
        show_msg_get_data(acpt, buff, "Enter the amount you want to deposit: ");
        amount = atof(buff);
    }
    else{
        // printf("----------loan amt------------------------------------\n");
        amount = loan_amount;
    }
    // Convert the entered amount to float
    

    // Read through the file to find the user
    while ((bytesRead = read(fd, &tempAdmin, sizeof(tempAdmin))) > 0) {
        printf("%d %d\n", tempAdmin.u.userid, userid);
        if (tempAdmin.u.userid == userid) {
            // printf("----------------------------------------------\n");
            // Lock the record for writing
            printf("Locking Record......................\n");
            if (lock_record(fd, F_WRLCK, recordsize) == -1) {
                perror("Error locking record");
                close(fd);
                return -1;
            }
            printf("Locked Record......................\n");

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
            printf("Unlocking Record......................\n");
            if (unlock_record(fd, recordsize) == -1) {
                perror("Error unlocking record");
            }
            printf("Unlocked Record......................\n");

            close(fd);
            if(loan_amount!=-1){
                // printf("loan approval---------99999-------------\n");
                return 0;
            }
            int temp = continuee(acpt);
            // printf("=====temp %d\n",temp);
            return temp;  // Return userid on success
        }
    }

    close(fd);

    int temp = continuee(acpt);
    // printf("=====temp %d\n",temp);
    return temp;
    // return continuee(acpt);  // Call continuee function if user not found
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
            printf("Locking Record......................\n");
            if (lock_record(fd, F_WRLCK, recordsize) == -1) {
                perror("Error locking record");
                close(fd);
                return -1;
            }
            printf("Locked Record......................\n");

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
            printf("Unlocking Record......................\n");
            if (unlock_record(fd, recordsize) == -1) {
                perror("Error unlocking record");
            }
            printf("Unlocked Record......................\n");

            close(fd);
            int temp = continuee(acpt);
            // printf("=====temp %d\n",temp);
            return temp;// Return userid on success
        }
    }

    close(fd);
    int temp = continuee(acpt);
    // printf("=====temp %d\n",temp);
    return temp;
    // return continuee(acpt);  // Call continuee function if user not found
}

int transfer_funds(int acpt, int sender_userid, size_t recordsize){
    struct Customer sender, reciver, user;
    memset(&sender, 0, sizeof(sender));
    memset(&reciver, 0, sizeof(reciver));

    int fd;
    fd = open("DATABASE/customer.txt", O_RDWR);
    if (fd == -1) {
        perror("Error opening customer database");
        return -1;
    }

    char uid_reciever[70];
    char buff[500];
    show_msg_get_data(acpt, uid_reciever, "Enter the receiver's user ID: ");
    show_msg_get_data(acpt, buff, "Enter the amount you want to transfer: ");

    int reciver_userid = atoi(uid_reciever);

    int sender_found=0, reciever_found=0, sender_cnt=1, reciever_cnt=1;

    while(read(fd, &user, sizeof(user)) > 0 && (sender_found==0 || reciever_found==0)){
        if(user.u.userid!=sender_userid && sender_found==0){
            sender_cnt+=1;
        }
        else{
            sender = user;
            sender_found = 1;
        }
        if(user.u.userid!=reciver_userid && reciever_found==0){
            reciever_cnt+=1;
        }
        else{
            reciver = user;
            reciever_found = 1;
        }
    }

    lseek(fd, 0, SEEK_SET);

    // lock sender and reciever
    struct flock lock_sender, lock_reciever;
    lock_sender.l_type = F_WRLCK;
    lock_sender.l_whence = SEEK_SET;
    lock_sender.l_start = sizeof(user)*(sender_cnt-1);
    lock_sender.l_len = sizeof(user);
    lock_sender.l_pid = getpid();
    printf("Before locking sender...\n");
    while (fcntl(fd, F_SETLKW, &lock_sender) == -1) {
        printf("waiting...\n");
        // If lock acquisition fails due to an interrupt, print a message and retry
        if (errno == EINTR) {
            printf("Interrupted while waiting for lock. Retrying...\n");
            continue;
        } else {
            // For other errors, print the error and return failure
            perror("Error acquiring lock");
            return -1;
        }
    }
    printf("Locked sender...\n");

    lock_reciever.l_type = F_WRLCK;
    lock_reciever.l_whence = SEEK_SET;
    lock_reciever.l_start = sizeof(user)*(reciever_cnt-1);
    lock_reciever.l_len = sizeof(user);
    lock_reciever.l_pid = getpid();
    printf("Before locking reciever...\n");
    while (fcntl(fd, F_SETLKW, &lock_reciever) == -1) {
        printf("waiting...\n");
        // If lock acquisition fails due to an interrupt, print a message and retry
        if (errno == EINTR) {
            printf("Interrupted while waiting for lock. Retrying...\n");
            continue;
        } else {
            // For other errors, print the error and return failure
            perror("Error acquiring lock");
            return -1;
        }
    }
    fcntl(fd, F_SETLKW, &lock_reciever);
    printf("Locked reciever...\n");

    // updating both
    lseek(fd, sizeof(sender)*(sender_cnt-1), SEEK_SET);
    read(fd, &sender, sizeof(sender));
    sender.account_balance -= atof(buff);
    lseek(fd, sizeof(sender)*(sender_cnt-1), SEEK_SET);
    write(fd, &sender, sizeof(sender));
    lseek(fd, sizeof(sender)*(sender_cnt-1), SEEK_SET);
    read(fd, &sender, sizeof(sender));

    lseek(fd, sizeof(reciver)*(reciever_cnt-1), SEEK_SET);
    read(fd, &reciver, sizeof(reciver));
    reciver.account_balance += atof(buff);
    lseek(fd, sizeof(reciver)*(reciever_cnt-1), SEEK_SET);
    write(fd, &reciver, sizeof(reciver));
    lseek(fd, sizeof(reciver)*(reciever_cnt-1), SEEK_SET);
    read(fd, &reciver, sizeof(reciver));

    // unlock both
    printf("Unlocking sender...\n");
    lock_sender.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock_sender);
    printf("Unlocked sender...\n");

    printf("Unlocking reciever...\n");
    lock_reciever.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock_reciever);
    printf("Unlocked reciever...\n");

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
            printf("Locking Record......................\n");
            if (lock_record(fd, F_WRLCK, record_size) == -1) {
                perror("Error locking record");
                close(fd);
                int temp = continuee(acpt);
                // printf("=====temp %d\n",temp);
                return temp;  // Return userid on success
            }
            printf("Locked Record......................\n");
            // 
            // printf("Locking Record......................\n");
            // if (lock_record(fd2, F_WRLCK, record_size2) == -1) {
            //     perror("Error locking record");
            //     close(fd);
            //     int temp = continuee(acpt);
            //     printf("=====temp %d\n",temp);
            //     return temp;  // Return userid on success
            // }
            // printf("Locked Record......................\n");

            // Update the balance
            tempAdmin.loan_taken = true;

            // Move the file pointer back to update the record
            lseek(fd, -sizeof(tempAdmin), SEEK_CUR);
            if (write(fd, &tempAdmin, sizeof(tempAdmin)) == -1) {
                perror("Error writing updated customer");
                unlock_record(fd, record_size);  // Unlock in case of error
                close(fd);
                int temp = continuee(acpt);
            // printf("=====temp %d\n",temp);
            return temp;  // Return userid on success
            }

            loan.approving_employee_id = -1;
            loan.borrower = tempAdmin;
            loan.is_completed = false;
            loan.is_approved = false;
            loan.loan_amount = amount;
            loan.loan_id = show_user_id_by_one() + 1;
            update_user_id_by_one();

            lseek(fd2, 0, SEEK_END);

            if (write(fd2, &loan, sizeof(loan)) == -1) {
                perror("Error writing updated balance");
            }
            send_message(acpt, "Applied for Loan Successfully....\n", userid);
            
            // Unlock the record after updating
            // printf("Unlocking Record......................\n");
            // if (unlock_record(fd2, record_size2) == -1) {
            //     perror("Error unlocking record");
            // }
            // printf("Unlocked Record......................\n");

            printf("Unocking Record......................\n");
            // Unlock the record after updating
            if (unlock_record(fd, record_size) == -1) {
                perror("Error unlocking record");
            }
            printf("Unlocked Record......................\n");

            close(fd);
            close(fd2);
            int temp = continuee(acpt);
            // printf("=====temp %d\n",temp);
            return temp;  // Return userid on success
        }
    }
    // printf("================>>>>");
    close(fd);
    int temp = continuee(acpt);
    // printf("=====temp %d\n",temp);
    return temp;
    // return continuee(acpt);  
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

int view_transaction_history(int acpt, int cust_id){
    printf("===================inside transaction history==============\n");
    const char *filename = "DATABASE/transaction.txt"; // File containing admin data

    struct Transaction transaction;
    char message[500];
    memset(&transaction, 0, sizeof(transaction));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/transaction.txt",O_RDWR);
    if(fd==-1){
        perror("==");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    while((bytesRead = read(fd, &transaction, sizeof(transaction))) > 0 ){
        printf("%d %d\n", transaction.sender_uid, cust_id);
        if(transaction.sender_uid==cust_id || transaction.reciever_uid==cust_id)
        {    
            char type_str[50];
            char *time_str = ctime(&transaction.transaction_time);
            time_str[strlen(time_str) - 1] = '\0'; 
            if(transaction.transaction_type==1) strcpy(type_str, "Deposite");
            else if(transaction.transaction_type==2) strcpy(type_str, "Withdraw");
            else if(transaction.transaction_type==3) strcpy(type_str, "Transfer");
            else if(transaction.transaction_type==4) strcpy(type_str, "Credited");
            sprintf(message, "%d %d %.2f %s %s", transaction.sender_uid, transaction.reciever_uid, transaction.amount, time_str, type_str);
            send_message(acpt, message, 0);
        }
    }
    // return continuee(acpt);
    int temp = continuee(acpt);
    // printf("=====temp %d\n",temp);
    return temp;
}


int customer_handler(int acpt, int login_success_user_id) {
    char buffer[500];
    int choice = 4;
    printf("----------------------------- in customer handler\n");

    // Step 1: Receive ready signal from client
    // printf("Waiting to receive ready signal from client...\n");
    if (recv(acpt, buffer, sizeof(buffer), 0) == -1) {
        perror("Error receiving ready signal");
    } else {
        // printf("Ready signal received: %s\n", buffer);
    }
    // printf("Received ready signal from client \n============================================\n");

    // Step 2: Send type signal back to the client
    strcpy(buffer, "1"); // Type 1 operation
    // printf("Sending type signal to client...\n");
    if (send(acpt, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("Error sending type signal");
    } else {
        // printf("Type signal sent: %s\n", buffer);
    }
    // printf("Sent type signal to client \n============================================\n");

    // Step 3: Send the customer menu to the client
    strcpy(buffer, "==========================================\nSelect Your Option :\n1. View My account Balance\n2. Deposite Money\n3. Withdraw Money\n4. Transfer Funds\n5. Change Password\n6. Exit\n7. Logout\n8. Apply for a loan\n9. Add Feedback\n10. View Transaction History\nEnter Your Choice : ");
    // printf("Sending customer menu to client...\n");
    if (send(acpt, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("Error sending admin menu");
    } else {
        // printf("customer menu sent: %s\n", buffer);
    }
    // printf("Sent customer menu \n============================================\n");

    // Step 4: Receive admin's choice
    // printf("Waiting to receive customer's choice...\n");
    if (recv(acpt, buffer, sizeof(buffer), 0) == -1) {
        perror("Error receiving customer's choice");
    } else {
        // printf("customer's choice received: %s\n", buffer);
    }
    // printf("Received customer's choice \n============================================\n");

    // Convert choice to integer
    int temp_choice = atoi(buffer);
    // printf("customer selected choice: %d\n", temp_choice);
    int cont;
    // Step 5: Handle the admin's choice using switch case
    while(1){
        switch (temp_choice) {
            case 1:
                char userid_buffer[10];
                // printf("Case 1: View My Account Balance\n");
                // show_msg_get_data(acpt, userid_buffer, "Enter your user id :");
                // printf("entert uid %d\n",atoi(userid_buffer));
                cont = view_account_balance(acpt, temp_choice, login_success_user_id);  
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
                // Logic for viewing managers here
                break;
            case 2:
                // printf("Case 2: Deposite Money\n");
                cont = deposite_money(acpt, login_success_user_id, sizeof(struct Customer), -1);
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
                // Logic for viewing managers here
                break;
            case 3:
                // printf("Case 3: Withdraw Money\n");
                cont = withdraw_money(acpt, login_success_user_id, sizeof(struct Customer));
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
                // Logic for viewing managers here
                break;
            case 4:
                // printf("Case 4: Transfer Funds\n");
                cont = transfer_funds(acpt, login_success_user_id, 4);
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
            case 5:
                // printf("Case 5: Change Password\n");
                cont = change_password_common(acpt, login_success_user_id, 4);
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
            case 6:
                // printf("Case 6: Exiting the manager handler\n");
                logout_customer(login_success_user_id);
                return 6;  // Exit the admin handler
            case 7:
                // printf("Case 7: Logout  -- %d\n", choice);
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
                // printf("Case 7: Logout  --ENDDD  %d\n", choice);
                return 7;  // Exit the admin handler
            case 8:
                printf("----------------------------- Apply for a loan\n");
                cont = apply_for_a_loan(acpt, login_success_user_id, sizeof(struct Customer));
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
            case 9:
                printf("----------------------------- Add Feedback\n");
                cont = add_feedback(acpt, login_success_user_id);
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
            case 10:
                printf("----------------------------- View Transaction History\n");
                cont = view_transaction_history(acpt, login_success_user_id);
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
            default:
                send_message(acpt, "Invalid Choice...\n", 0);
                break;
        }
    break;
    }
    printf("================== End of customer handler ==================\n");
    return 0;
}