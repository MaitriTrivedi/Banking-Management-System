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
    printf("================== %d\n", user_id);
    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        printf("===inside %d %d\n", tempCustomer.u.userid, user_id);
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
            return 1;
        }
    }
    close(fd);
    // return continuee(acpt);
    return 1;
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
        }
    }
    close(fd);
    // return continuee(acpt);
    return 1;
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
    return 1;
}

int manager_handler(int acpt, int login_success_user_id) {
    char buffer[500];
    int choice = 2;
    printf("================== in manager handler ==================\n");

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
    strcpy(buffer, "==========================================\nSelect Your Option :\n1. Add a New User.\n2. Activate or Deactivate Customer Account\n3.Assign Loan Application to Employee\n4. Review Customer's FeedBack\n5. Change Password\n");
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
            printf("Case 1: Adding a new user\n");
            return create_new_user(acpt, temp_choice);  // Function to add a new user
            // break;
        case 2:
            printf("Case 2: Activate or Deactivate Customer Account\n");
            char activate[5];
            show_msg_get_data(acpt, activate, "You want to activate or deactivate?\nEnter 1 to activate and 0 to deactivate :");
            return activate_deactivate_customer_account(acpt, temp_choice, atoi(activate)); 
            // Logic for viewing managers here
            break;
        case 3:
            printf("Case 3: Assign Loan Application to Employee\n");
            return assign_loan_applications_to_emp(acpt, temp_choice, login_success_user_id); 
            // Logic for viewing employees here
            break;
        case 4:
            printf("Case 4: Review Customer's FeedBack\n");
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
        default:
            printf("Invalid choice. Please select a valid option.\n");
            break;
    }
    printf("================== End of admin handler ==================\n");
    return 0;
}

void manager_handler2(int acpt){
    printf("==========================================\n");
    printf("Select Your Option :\n1. Add a New User.\n2. View Loans\n3.Assign Loan Application to Employee\n4. View Employees\n5. View Customers\n");
    printf("Enter your choice :");
    int temp_choice;
    // scanf("%d",&temp_choice);
    switch(temp_choice){
        case 1:
        {   
            printf("Select the type of user you want to add :\n1. Admin\n2. Manager\n3. Employee\n4. Customer\n");
            int temp_type;
            printf("Enter type : ");
            scanf("%d", &temp_type);
            create_new_user(acpt, temp_type);
        }
        case 2:
        {
            view_loan_applications();
            break;
        }
        case 3:
        {
            // assign_loan_applications_to_emp();
            break;
        }
        case 4:
        {
            list_of_employees();
            break;
        }
        case 5:
        {
            list_of_customers();
            break;
        }
    }
}