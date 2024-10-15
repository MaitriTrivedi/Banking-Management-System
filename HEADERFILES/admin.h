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
void admin_handler(int acpt, int login_success_user_id) {
    char buffer[200];
    int choice;
    printf("================== in admin handler ==================\n");

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

    // Step 3: Send the admin menu to the client
    strcpy(buffer, "==========================================\nSelect Your Option :\n1. Add a New User\n2. View Managers\n3. View Employees\n4. View Customers\n5. Exit\nEnter your choice: ");
    printf("Sending admin menu to client...\n");
    if (send(acpt, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("Error sending admin menu");
    } else {
        printf("Admin menu sent: %s\n", buffer);
    }
    printf("Sent admin menu \n============================================\n");

    // Step 4: Receive admin's choice
    printf("Waiting to receive admin's choice...\n");
    if (recv(acpt, buffer, sizeof(buffer), 0) == -1) {
        perror("Error receiving admin's choice");
    } else {
        printf("Admin's choice received: %s\n", buffer);
    }
    printf("Received admin's choice \n============================================\n");

    // Convert choice to integer
    int temp_choice = atoi(buffer);
    printf("Admin selected choice: %d\n", temp_choice);

    // Step 5: Handle the admin's choice using switch case
    switch (temp_choice) {
        case 1:
            printf("Case 1: Adding a new user\n");
            create_new_user(acpt, temp_choice);  // Function to add a new user
            break;
        case 2:
            printf("Case 2: Viewing managers\n");
            // Logic for viewing managers here
            break;
        case 3:
            printf("Case 3: Viewing employees\n");
            // Logic for viewing employees here
            break;
        case 4:
            printf("Case 4: Viewing customers\n");
            // Logic for viewing customers here
            break;
        case 5:
            printf("Case 5: Exiting the admin handler\n");
            return;  // Exit the admin handler
        default:
            printf("Invalid choice. Please select a valid option.\n");
            break;
    }
    printf("================== End of admin handler ==================\n");
}
