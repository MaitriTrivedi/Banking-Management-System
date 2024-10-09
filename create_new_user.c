#include <openssl/sha.h>
#include <sys/file.h>
#include<unistd.h>
#include "password_methods.h"
#include "structures.h"
#include<fcntl.h>
#include <sys/stat.h>

void create_admin_user(struct Admin a){
    int fd;

    // open file in read write file
    fd = open("DATABASE/admin.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    // write data to the file
    write(fd, &a, sizeof(a));
    lseek(fd, 0, SEEK_SET);
    read(fd, &a, sizeof(a));

    close(fd); 

    return ;
}

void create_manager_user(struct Manager m){
    int fd;

    // open file in read write file
    fd = open("DATABASE/manager.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    // write data to the file
    write(fd, &m, sizeof(m));
    lseek(fd, 0, SEEK_SET);
    read(fd, &m, sizeof(m));

    close(fd); 

    return ;
}

void create_employee_user(struct Employee e){
    int fd;

    // open file in read write file
    fd = open("DATABASE/employee.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    // write data to the file
    write(fd, &e, sizeof(e));
    lseek(fd, 0, SEEK_SET);
    read(fd, &e, sizeof(e));

    close(fd); 

    return ;
}

void create_customer_user(struct Customer c){
    int fd;

    // open file in read write file
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return;
    }
    struct stat fileState;
    if(fstat(fd, &fileState)<0){
        perror("");
        close(fd);
        return;
    }

    int filesize = fileState.st_size;
    int number_of_account_holder = filesize/sizeof(c);

    printf("Number of acc holders = %d\n",number_of_account_holder);

    // go to last customer to get the last account number
    if(number_of_account_holder!=0){
        struct Customer ctemp;
        lseek(fd, sizeof(c)*(number_of_account_holder-1), SEEK_SET);
        read(fd, &ctemp, sizeof(ctemp));
        c.account_no = ctemp.account_no + 1;
    }
    else{
        // if first customer
        c.account_no = 1;
    }

    // write data to the file
    write(fd, &c, sizeof(c));
    lseek(fd, 0, SEEK_SET);
    read(fd, &c, sizeof(c));

    close(fd); 

    return ;
}

void create_new_user(int type){
    switch(type){
        case 1:
            {
                struct Admin a;
                strcpy(a.u.username, "Maitri");
                hashPassword( "1234", a.u.password);
                create_admin_user(a);
                return;
            }
        case 2:
            {
                struct Manager m;
                char temp_name[30];
                printf("Enter Manager's Name : ");
                scanf("%s", temp_name);

                char temp_password[30];
                printf("Enter Manager's Password : ");
                scanf("%s", temp_password);

                strcpy(m.u.username, temp_name);
                hashPassword( temp_password, m.u.password);
                create_manager_user(m);
                return;
            }

        case 3:
            {
                struct Employee e ;
                char temp_name[30];
                printf("Enter Employee's Name : ");
                scanf("%s", temp_name);

                char temp_password[30];
                printf("Enter Employee's Password : ");
                scanf("%s", temp_password);

                printf("Enter Employee's Salary : ");
                scanf("%f", &e.salary);

                printf("Enter Employee's Salary : ");
                scanf("%f", &e.salary);

                strcpy(e.u.username, temp_name);
                hashPassword( temp_password, e.u.password);
                create_employee_user(e);
                return;
            }
        case 4:
            {
                struct Customer c ;
                char temp_name[30];
                printf("Enter Customer's Name : ");
                scanf("%s", temp_name);

                char temp_password[30];
                printf("Enter Customer's Password : ");
                scanf("%s", temp_password);

                // printf("Enter Customer's Account Number : ");
                // scanf("%d", &c.account_no);

                printf("Enter Customer's Account Balance : ");
                scanf("%f", &c.account_balance);

                char temp_loan[5];
                printf("Has this customer taken loan?\nType 'Yes or Y or yes or y or 1' if taken anf 'No or no or n or 0' if not taken : ");
                scanf("%s", temp_loan);
                if(temp_loan=="Yes" || temp_loan=="Y" || temp_loan=="yes" || temp_loan=="y" || temp_loan=="1" ){
                    c.loan_taken = true;
                    printf("Enter Customer's Loan's Loan ID : ");
                    scanf("%d", &c.loan_id);
                }
                else{
                    c.loan_taken = false;
                    c.loan_id = -1;
                }

                strcpy(c.u.username, temp_name);
                hashPassword( temp_password, c.u.password);
                create_customer_user(c);
                return;
            }
        default:
            {
                printf("Invalid Choice...\n");
                return;
            }
    }
}


