#include "structures.h"
#include <sys/file.h>
#include<unistd.h>
#include <stdbool.h>
#include<stdio.h>
#include "loan.h"

int activate_deactivate_customer_account(int acpt, int type, int activate){
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
    show_msg_get_data(acpt, buffer, "Enter User Id You want to activate or deactivate :\n");
    int user_id = atoi(buffer); 
    // printf("================== %d\n", user_id);
    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        // printf("===inside %d %d\n", tempCustomer.u.userid, user_id);
        if(tempCustomer.u.userid==user_id){
            // tempCustomer.u.is_active = tempCustomer.u.is_active ? 0 : 1;
            tempCustomer.u.is_active = activate;
            lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
            if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                perror("Error writing updated customer");
            }
            if(activate==1)
                send_message(acpt, "User Activated Successfully ...\n", 0);
            else
                send_message(acpt, "User Deactivated Successfully ...\n", 0);
            // return continuee(acpt);
            int temp = continuee(acpt);
            // printf("=====temp %d\n",temp);
            return temp;
        }
    }
    close(fd);
    // return continuee(acpt);
    int temp = continuee(acpt);
    // printf("=====temp %d\n",temp);
    return temp;
}

int assign_loan_applications_to_emp(int acpt, int type, int user_id){
    struct Loan loan;
    memset(&loan, 0, sizeof(loan));
    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/loan.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }
    char buffer[5];
    show_msg_get_data(acpt, buffer, "Enter Loan Id You want to assign to an Employee :\n");
    int loan_id = atoi(buffer); 

    show_msg_get_data(acpt, buffer, "Enter Employee Id You want to assign this Loan Application :\n");
    int emp_id = atoi(buffer); 
    printf("================== %d\n", user_id);
    while((bytesRead = read(fd, &loan, sizeof(loan))) > 0 ){
        // printf("===inside %d %d\n", tempCustomer.u.userid, user_id);
        if(loan.loan_id==loan_id){
            // tempCustomer.u.is_active = tempCustomer.u.is_active ? 0 : 1;
            loan.approving_employee_id = emp_id;
            lseek(fd, -sizeof(loan), SEEK_CUR);
            if (write(fd, &loan, sizeof(loan)) == -1) {
                perror("Error writing updated customer");
            }
            char message[150];
            sprintf(message, "Employee %d assigned to loan %d...\n", emp_id, loan.loan_id);
            send_message(acpt, message, 0);
            // return continuee(acpt);
            int temp = continuee(acpt);
            // printf("=====temp %d\n",temp);
            return temp;
        }
    }
    close(fd);
    // return continuee(acpt);
    int temp = continuee(acpt);
    // printf("=====temp %d\n",temp);
    return temp;
}

int review_customers_feedback(int acpt, int type){
    struct Feedback feedback;
    memset(&feedback, 0, sizeof(feedback));
    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/feedback.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }
    while((bytesRead = read(fd, &feedback, sizeof(feedback))) > 0 ){
        // printf("===inside %d %d\n", tempCustomer.u.userid, user_id);
        char message[500];
        sprintf(message, "%d - %s - %d - %s", feedback.customer.u.userid, feedback.customer.u.username, feedback.rating, feedback.feedback);
        send_message(acpt, message, 0);
    }
    close(fd);
    // return continuee(acpt);
    int temp = continuee(acpt);
    // printf("=====temp %d\n",temp);
    return temp;
}

int manager_handler(int acpt, int login_success_user_id) {
    char buffer[500];
    int choice = 2;
    // printf("================== in manager handler ==================\n");

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

    // Step 3: Send the manager menu to the client
    strcpy(buffer, "==========================================\nSelect Your Option :\n1. Add a New User.\n2. Activate or Deactivate Customer Account\n3. Assign Loan Application to Employee\n4. Review Customer's FeedBack\n5. Change Password\n6. Exit\n7. Logout\nEnter Your Choice : ");
    // printf("Sending MANAGER menu to client...\n");
    if (send(acpt, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("Error sending admin menu");
    } else {
        // printf("Manager menu sent: %s\n", buffer);
    }
    // printf("Sent MANAGER menu \n============================================\n");

    // Step 4: Receive admin's choice
    // printf("Waiting to receive manager's choice...\n");
    if (recv(acpt, buffer, sizeof(buffer), 0) == -1) {
        perror("Error receiving manager's choice");
    } else {
        // printf("manager's choice received: %s\n", buffer);
    }
    // printf("Received manager's choice \n============================================\n");

    // Convert choice to integer
    int temp_choice = atoi(buffer);
    // printf("manager selected choice: %d\n", temp_choice);
    int cont;
    // Step 5: Handle the admin's choice using switch case
    while(1){
        switch (temp_choice) {
        case 1:
            printf("----------------------------- Adding a new user\n");
            cont = create_new_user_by_manager(acpt, temp_choice);  // Function to add a new user
            if(cont==0) {
                temp_choice=7;
                continue;
            }
            else return 1;
            // return continuee(acpt);
            return 1;

        case 2:
            printf("----------------------------- Activate or Deactivate Customer Account\n");
            char activate[5];
            show_msg_get_data(acpt, activate, "You want to activate or deactivate?\nEnter 1 to activate and 0 to deactivate :");
            cont = activate_deactivate_customer_account(acpt, temp_choice, atoi(activate)); 
            if(cont==0) {
                temp_choice=7;
                continue;
            }
            else return 1;
            break;
        case 3:
            printf("----------------------------- Assign Loan Application to Employee\n");
            cont =  assign_loan_applications_to_emp(acpt, temp_choice, login_success_user_id); 
            if(cont==0) {
                temp_choice=7;
                continue;
            }
            else return 1;
            break;
        case 4:
            printf("----------------------------- Review Customer's FeedBack\n");
            cont = review_customers_feedback(acpt, temp_choice); 
            // Logic for viewing customers here
            if(cont==0) {
                temp_choice=7;
                continue;
            }
            else return 1;
            break;
        case 5:
            printf("----------------------------- Change Password\n");
                cont = change_password_common(acpt, login_success_user_id, 2);
                // cont = continuee(acpt);  // Exit the admin handler
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
        case 6:
            printf("----------------------------- Exiting the manager handler\n");
            logout_manager(login_success_user_id);
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
            // printf("----------------------------- Logged Out");
            return 7;  // Exit the admin handler
        default:
            send_message(acpt, "Invalid Choice...\n", 0);
            break;
        }
        break;
    }
    printf("================== End of manager handler ==================\n");
    return 0;
}
