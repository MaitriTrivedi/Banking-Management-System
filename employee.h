#include "structures.h"
#include <sys/file.h>
#include<unistd.h>
#include <stdbool.h>
#include<stdio.h>
#include "admin.h"

void loan_applications(){
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
            printf("%d %s %d %f %s %d\n", i, c.u.username, c.account_no, c.account_balance, t, c.loan_id);
        }
        i++;
    }

    close(fd); 

    return ;
}

void approve_or_disapprove_loan_applications(){

}

void employee_handler(){
    printf("==========================================\n");
    printf("Select Your Option :\n1. Add a New User.\n2. View Loans\n3. View Employees\n4. View Customers\n");
    printf("Enter your choice :");
    int temp_choice;
    scanf("%d",&temp_choice);
    switch(temp_choice){
        case 1:
        {   
            printf("Select the type of user you want to add :\n1. Admin\n2. Manager\n3. Employee\n4. Customer\n");
            int temp_type;
            printf("Enter type : ");
            scanf("%d", &temp_type);
            create_new_user(temp_type);
        }
        case 2:
        {
            loan_applications();
            break;
        }
        case 3:
        {
            list_of_employees();
            break;
        }
        case 4:
        {
            list_of_customers();
            break;
        }
        case 5:
        {
            approve_or_disapprove_loan_applications();
            break;
        }
    }
}