#include "structures.h"
#include <sys/file.h>
#include<unistd.h>
#include <stdbool.h>
#include<stdio.h>


int approve_or_reject_loan_application(int acpt, int approve){
    struct Loan loan;
    memset(&loan, 0, sizeof(loan));
    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/loan.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }
    char loan_id_buf[5];
    show_msg_get_data(acpt, loan_id_buf, "Enter Loan Id You want to approve or reject :\n"); 
    // printf("================== %d\n", user_id);
    while((bytesRead = read(fd, &loan, sizeof(loan))) > 0 ){
        // printf("===inside %d %d\n", tempCustomer.u.userid, user_id);
        if(loan.loan_id==atoi(loan_id_buf)){
            // tempCustomer.u.is_active = tempCustomer.u.is_active ? 0 : 1;
            loan.is_approved = approve;
            lseek(fd, -sizeof(loan), SEEK_CUR);
            if (write(fd, &loan, sizeof(loan)) == -1) {
                perror("Error writing updated customer");
            }
            if(approve==1)
                send_message(acpt, "Loan Approved Successfully ...\n", 0);
            else
                send_message(acpt, "Loan Rejected Successfully ...\n", 0);
            // return continuee(acpt);
            return 1;
        }
    }
    close(fd);
    // return continuee(acpt);
    return 1;
}

int view_assigned_loan(int acpt, int emp_id){
    const char *filename = "DATABASE/loan.txt"; // File containing admin data

    struct Loan loan;
    char message[500];
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
        if(loan.approving_employee_id==emp_id)
        {    
            sprintf(message, "Loan ID: %d, Loan Amount: %.2f, Is Approved: %d, Is Completed: %d, Approving Employee ID: %d Userid: %d Username: %s\n", loan.loan_id, loan.loan_amount, loan.is_approved, loan.is_completed, loan.approving_employee_id, loan.borrower.u.userid, loan.borrower.u.username);
            send_message(acpt, message, 0);
        }
    }
    // return continuee(acpt);
    return 1;
}

int view_customer_transaction_passbook(int acpt, int cust_id){
    const char *filename = "DATABASE/transaction.txt"; // File containing admin data

    struct Transaction transaction;
    char message[500];
    memset(&transaction, 0, sizeof(transaction));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/loan.txt",O_RDWR);
    if(fd==-1){
        perror("==");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    while((bytesRead = read(fd, &transaction, sizeof(transaction))) > 0 ){
        if(transaction.sender_uid==cust_id)
        {    
            char type_str[50];
            char *time_str = ctime(&transaction.transaction_time);
            time_str[strlen(time_str) - 1] = '\0'; 
            if(transaction.transaction_type==1) strcpy(type_str, "Deposite");
            else if(transaction.transaction_type==2) strcpy(type_str, "Withdraw");
            else if(transaction.transaction_type==3) strcpy(type_str, "Transfer");
            sprintf(message, "%d %d %.2f %s %d \n", transaction.sender_uid, transaction.reciever_uid, transaction.amount, time_str, transaction.transaction_type);
            send_message(acpt, message, 0);
        }
    }
    // return continuee(acpt);
    return 1;
}

int employee_handler(int acpt, int login_success_user_id) {
    char buffer[500];
    int choice = 2;
    printf("================== in employee handler ==================\n");

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

    // Step 3: Send the manager menu to the client
    strcpy(buffer, "==========================================\nSelect Your Option :\n1. Add a New Customer.\n2. Modify Customer Data\n3.Process Loan Application\n4. Approve or Reject Laon\n5. Change Password\n6. Exit\n7. Logout\n8. View Assigned Loans\n9. View Customer Transaction\n");
    printf("Sending MANAGER menu to client...\n");
    if (send(acpt, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("Error sending admin menu");
    } else {
        printf("Manager menu sent: %s\n", buffer);
    }
    printf("Sent MANAGER menu \n============================================\n");

    // Step 4: Receive admin's choice
    printf("Waiting to receive manager's choice...\n");
    if (recv(acpt, buffer, sizeof(buffer), 0) == -1) {
        perror("Error receiving manager's choice");
    } else {
        printf("manager's choice received: %s\n", buffer);
    }
    printf("Received manager's choice \n============================================\n");

    // Convert choice to integer
    int temp_choice = atoi(buffer);
    printf("manager selected choice: %d\n", temp_choice);

    // Step 5: Handle the admin's choice using switch case
    switch (temp_choice) {
        case 1:
            printf("Case 1: Adding a new Customer\n");
            return create_new_customer(acpt);  // Function to add a new user
            // break;
        case 2:
            printf("Case 2: Modify Customer Data\n");
            // Logic for viewing managers here
            break;
        case 3:
            printf("Case 3: Process Loan Application\n");
            return assign_loan_applications_to_emp(acpt, temp_choice, login_success_user_id); 
            // Logic for viewing employees here
            break;
        case 4:
            printf("Case 4: Approve or Reject Loan Application\n");
            char activate[5];
            show_msg_get_data(acpt, activate, "You want to approve or reject?\nEnter 1 to approve and 0 to reject :");
            return approve_or_reject_loan_application(acpt, atoi(activate));
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
            printf("Case 8: View Assigned Loans\n");
            return view_assigned_loan(acpt, login_success_user_id);
        case 9:
            printf("Case 9: View Customer Transaction\n");
            char cust_id_buff[5];
            show_msg_get_data(acpt, cust_id_buff, "Enter User Id who's Transaction History You want to see :");
            return view_customer_transaction_passbook(acpt, atoi(cust_id_buff));
            return 6;  // Exit the admin handler
        default:
            printf("Invalid choice. Please select a valid option.\n");
            break;
    }
    printf("================== End of admin handler ==================\n");
    return 0;
}