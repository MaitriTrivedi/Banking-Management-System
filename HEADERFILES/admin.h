#include "structures.h"
#include <sys/file.h>
#include<unistd.h>
#include <stdbool.h>
#include<stdio.h>
#include "create_new_user.h"

void list_of_managers(){
    int fd;

    // open file in read write file
    fd = open("DATABASE/manager.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    
    int bytesRead;
    int i = 1;
    struct Manager m;
    while((bytesRead = read(fd, &m, sizeof(m))) > 0 ){
        printf("%d %s %f\n", i, m.u.username, m.salary);
        i++;
    }

    close(fd); 

    return ;    
}

int logout_admin(int userid){
    printf("----------------------------Inside Logout Admin\n");
    struct Admin tempAdmin;
    memset(&tempAdmin, 0, sizeof(tempAdmin));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/admin.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    while((bytesRead = read(fd, &tempAdmin, sizeof(tempAdmin))) > 0 ){
        if(tempAdmin.u.userid==userid){
                tempAdmin.u.is_logged_in = false;
                lseek(fd, -sizeof(tempAdmin), SEEK_CUR);
                if (write(fd, &tempAdmin, sizeof(tempAdmin)) == -1) {
                    perror("Error writing updated customer");
                }
                close(fd);
                // printf("== == == == end1 Logout admin== == == == ==\n");
                return tempAdmin.u.userid;
            }
        }
    close(fd);
    // printf("== == == == end2 Logout admin== == == == ==\n");
    return -1;

}

int logout_manager(int userid){
    struct Manager tempAdmin;
    memset(&tempAdmin, 0, sizeof(tempAdmin));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/manager.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    while((bytesRead = read(fd, &tempAdmin, sizeof(tempAdmin))) > 0 ){
        if(tempAdmin.u.userid==userid){
                tempAdmin.u.is_logged_in = false;
                lseek(fd, -sizeof(tempAdmin), SEEK_CUR);
                if (write(fd, &tempAdmin, sizeof(tempAdmin)) == -1) {
                    perror("Error writing updated customer");
                }
                close(fd);
                return tempAdmin.u.userid;
            }
        }
    close(fd);
    return -1;

}

int logout_employee(int userid){
    struct Employee tempAdmin;
    memset(&tempAdmin, 0, sizeof(tempAdmin));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/employee.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    while((bytesRead = read(fd, &tempAdmin, sizeof(tempAdmin))) > 0 ){
        if(tempAdmin.u.userid==userid){
                tempAdmin.u.is_logged_in = false;
                lseek(fd, -sizeof(tempAdmin), SEEK_CUR);
                if (write(fd, &tempAdmin, sizeof(tempAdmin)) == -1) {
                    perror("Error writing updated customer");
                }
                close(fd);
                return tempAdmin.u.userid;
            }
        }
    close(fd);
    return -1;

}

int logout_customer(int userid){
    struct Customer tempAdmin;
    memset(&tempAdmin, 0, sizeof(tempAdmin));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    while((bytesRead = read(fd, &tempAdmin, sizeof(tempAdmin))) > 0 ){
        if(tempAdmin.u.userid==userid){
                tempAdmin.u.is_logged_in = false;
                lseek(fd, -sizeof(tempAdmin), SEEK_CUR);
                if (write(fd, &tempAdmin, sizeof(tempAdmin)) == -1) {
                    perror("Error writing updated customer");
                }
                close(fd);
                return tempAdmin.u.userid;
            }
        }
    close(fd);
    return -1;

}

void list_of_employees(){
    int fd;

    // open file in read write file
    fd = open("DATABASE/employee.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    
    int bytesRead;
    int i=1;
    struct Employee e;
    while((bytesRead = read(fd, &e, sizeof(e))) > 0 ){
        printf("%d %s %f\n", i, e.u.username, e.salary);
        i++;
    }

    close(fd); 

    return ;
}

void list_of_customers(){
    int fd;

    // open file in read write file
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    
    int bytesRead;
    int i=1;
    struct Customer c;
    while((bytesRead = read(fd, &c, sizeof(c))) > 0 ){
        char t[6];
        if(c.loan_taken) {
            strcpy(t, "Yes");
        }
        else {
            strcpy(t, "No");
        }
        printf("%d %s %d %f %s %d\n", i, c.u.username, c.account_no, c.account_balance, t, c.loan_taken);
        i++;
    }

    close(fd); 

    return ;
}

int admin_handler(int acpt, int login_success_user_id) {
    char buffer[500];
    int choice = 1;
    int conti;
    printf("----------------------------- In Admin Handler\n");
    // printf("================== in admin handler ==================\n");

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

    // Step 3: Send the admin menu to the client
    strcpy(buffer, "==========================================\nSelect Your Option :\n1. Add a New User\n2. Modify Customer Details\n3. Modify Employee Details\n4. View Customers\n5. Change Password\n6. Exit\n7. Logout\n8. Manage User Roles\nEnter your choice: ");
    // printf("Sending admin menu to client...\n");
    if (send(acpt, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("Error sending admin menu");
    } else {
        printf("----------------------------- Admin Menu Sent\n");
    }
    // printf("Sent admin menu \n============================================\n");

    // Step 4: Receive admin's choice
    // printf("Waiting to receive admin's choice...\n");
    if (recv(acpt, buffer, sizeof(buffer), 0) == -1) {
        perror("Error receiving admin's choice");
    } else {
        // printf("Admin's choice received: %s\n", buffer);
    }
    // printf("Received admin's choice \n============================================\n");

    // Convert choice to integer
    int temp_choice = atoi(buffer);
    // printf("Admin selected choice: %d\n", temp_choice);

    // Step 5: Handle the admin's choice using switch case
    while(1){
        // printf("in while loopoooooooooooooooooooooooooooooooop\n");
        switch (temp_choice) {
            case 1:
                // printf("Case 1: Adding a new user\n");
                int cont = create_new_user(acpt, temp_choice);  // Function to add a new user
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
                // return continuee(acpt);
                return 1;
                // break;
            case 2:
                // printf("Case 2: Modify Customer Details\n");
                cont = modify_customer(acpt, temp_choice);
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
                // Logic for viewing managers here
                break;
            case 3:
                // printf("Case 3: Modify Employee Details\n");
                cont = modify_employee(acpt, temp_choice); 
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
                // Logic for viewing managers here
                break;
            case 4:
                // printf("Case 4: Viewing customers\n");
                cont = view_customers(acpt, temp_choice); 
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
                // Logic for viewing managers here
                break;
            case 5:
                // printf("Case 5: Change Password\n");
                cont = change_password_common(acpt, login_success_user_id, 1);
                // cont = continuee(acpt);  // Exit the admin handler
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
            case 6:
                // printf("Case 6: Exiting the admin handler\n");
                logout_admin(login_success_user_id);
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
                    {
                        logout_admin(login_success_user_id);
                        return 7;
                    }
                    default:
                        break;
                }
                // printf("Case 7: Logout  --ENDDD  %d\n", choice);
                return 7;  // Exit the admin handler
            case 8:
                // printf("Case 8: Manage User Roles\n");
                conti = manage_user_role(acpt);
                if(cont==0) {
                    temp_choice=7;
                    continue;
                }
                else return 1;
                // Logic for viewing managers here
                break;
            default:
                send_message(acpt, "Invalid choice. Please select a valid option.\n",0);
                break;
        }
        break;
    }
    printf("================== End of admin handler ==================\n");
    return 0;
}
