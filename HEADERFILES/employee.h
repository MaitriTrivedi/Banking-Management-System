#include "structures.h"
#include <sys/file.h>
#include<unistd.h>
#include <stdbool.h>
#include<stdio.h>


void employee_handler(int acpt){
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
            create_new_user(acpt, temp_type);
        }
        case 2:
        {
            view_loan_applications();
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