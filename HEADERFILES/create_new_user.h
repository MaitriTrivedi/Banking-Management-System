#include <openssl/sha.h>
#include <sys/file.h>
#include<unistd.h>
#include "password_methods.h"
#include "structures.h"
#include<fcntl.h>
#include <sys/stat.h>
#include "last_used_user_id.h"
#include "common_functions.c"

void create_admin_user(struct Admin a){
    int fd;

    // open file in read write file
    fd = open("DATABASE/admin.txt",O_RDWR);
    if(fd==-1){
        perror("create_admin_user : ");
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
    printf("=========== adding manager user ===========\n");
    // open file in read write file
    fd = open("DATABASE/manager.txt",O_RDWR);
    perror("create manager user - open");
    if(fd==-1){
        perror("");
        return;
    }
    // write data to the file
    lseek(fd, 0, SEEK_END);
    write(fd, &m, sizeof(m));
    perror("create manager user - write");
    lseek(fd, -sizeof(m), SEEK_CUR);
    read(fd, &m, sizeof(m));
    perror("create manager user - read");

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
    lseek(fd, 0, SEEK_END);
    write(fd, &e, sizeof(e));
    lseek(fd, -sizeof(e), SEEK_CUR);
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
    perror("");
    lseek(fd, 0, SEEK_SET);
    read(fd, &c, sizeof(c));
    perror("");
    close(fd); 

    return ;
}

int create_new_user(int acpt, int type){
    char buffer[500];
    printf("=============================== inside create new user ===============================\n");

    sleep(1);
    // send loop continue signal
    strcpy(buffer, "10"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send CONTINUE sig \n============================================\n");

    // recv signal
    if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
        printf("Error\n");
    }
    printf("%s\n", buffer);
    printf("recvd of ready sig \n============================================\n");
    // getchar();

    // send signal
    strcpy(buffer, "1"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send of TYPE sig \n============================================\n");
    // getchar();

    // case 1(type of op = 1)
    // send menu
    strcpy(buffer, "==========================================\nSelect Your Option :\n1. Admin\n2. Managers\n3. Employees\n4. Customers\nEnter your choice :");
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("sent menu \n============================================\n");
    // getchar();

    // recv choice
    if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
        printf("Error\n");
    }
    printf("%s\n", buffer);
    printf("recvd choice \n============================================\n");
    // getchar();
    type = atoi(buffer);
    // // send loop continue signal
    // strcpy(buffer, "10"); // type 1
    // if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
    //     perror("Error sending login data");
    // }
    // printf("%s\n", buffer);
    // printf("send CONTINUE sig \n============================================\n");

    
    printf("==========type============= %d\n", type);
    switch(type){
        case 1:
            {   
                // send loop continue signal
                strcpy(buffer, "10"); // type 1
                if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                    perror("Error sending login data");
                }
                printf("%s\n", buffer);
                printf("send CONTINUE sig \n============================================\n");

                printf("=================== inside create new user (ADMIN) ====================\n");
                // recv signal
                if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
                    printf("Error\n");
                }
                printf("%s\n", buffer);
                printf("recvd of ready sig \n============================================\n");
                // getchar();

                // send signal
                strcpy(buffer, "2"); // type 1
                if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                    perror("Error sending login data");
                }
                printf("%s\n", buffer);
                printf("send of TYPE sig \n=====");

                struct Admin a;
                strcpy(a.u.username, "Maitri");
                hashPassword( "1234", a.u.password);
                a.u.userid = show_user_id_by_one()+1;
                a.u.is_active = true;
                a.u.is_logged_in = false;
                create_admin_user(a);
                update_user_id_by_one();
                // return;

                // send message
                strcpy(buffer, "Admin user created successfully\n");
                if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                    perror("Error sending login data");
                }
                printf("%s\n", buffer);
                printf("sent msg \n============================================\n");
                // return continuee(acpt);
                return 1;
            }
        case 2:
            {
                // // send loop continue signal
                // strcpy(buffer, "10"); // type 1
                // if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                //     perror("Error sending login data");
                // }
                // printf("%s\n", buffer);
                // printf("send CONTINUE sig \n============================================\n");

                printf("=================== inside create new user (MANGER) ====================\n");
                struct Manager m;
                char temp_username[30];
                // printf("Enter Manager's Name : ");
                take_username(acpt, temp_username);

                char temp_password[30];
                // printf("Enter Manager's Password : ");
                take_password(acpt, temp_password, NULL);

                strcpy(m.u.username, temp_username);
                hashPassword( temp_password, m.u.password);
                m.u.userid = show_user_id_by_one()+1;
                m.u.is_active = true;
                m.u.is_logged_in = false;
                create_manager_user(m);
                update_user_id_by_one();
                strcpy(buffer, "Manger User Added Successfully.\n");
                send_message(acpt, buffer, 0);
                // return continuee(acpt);
                return 1;
            }

        case 3:
            {
                printf("=================== inside create new user (EMPLOYEE) ====================\n");
                struct Employee e ;
                char temp_username[30];
                // printf("Enter Manager's Name : ");
                take_username(acpt, temp_username);

                char temp_password[30];
                // printf("Enter Manager's Password : ");
                take_password(acpt, temp_password, NULL);

                float temp_salary;
                // printf("Enter Manager's Password : ");
                take_salary(acpt, &temp_salary);

                strcpy(e.u.username, temp_username);
                hashPassword( temp_password, e.u.password);
                e.u.userid = show_user_id_by_one()+1;
                e.u.is_active = true;
                e.u.is_logged_in = false;
                e.salary = temp_salary;
                create_employee_user(e);
                update_user_id_by_one();
                strcpy(buffer, "Employee User Added Successfully.\n");
                send_message(acpt, buffer, 0);
                // return continuee(acpt);
                return 1;
            }
        case 4:
            {
                printf("=================== inside create new user (CUSTOMER) ====================\n");
                struct Customer c ;
                char temp_username[30];
                // printf("Enter Manager's Name : ");
                take_username(acpt, temp_username);

                char temp_password[30];
                // printf("Enter Manager's Password : ");
                take_password(acpt, temp_password, NULL);

                c.account_balance = 1000;
                c.u.is_active = true;
                c.u.is_logged_in = false;
                c.loan_taken = false;

                strcpy(c.u.username, temp_username);
                hashPassword( temp_password, c.u.password);
                c.u.userid = show_user_id_by_one()+1;
                create_customer_user(c);
                update_user_id_by_one();
                strcpy(buffer, "Customer User Added Successfully.\n");
                send_message(acpt, buffer, 0);
                // return continuee(acpt);
                return 1;
            }
        default:
            {
                printf("Invalid Choice...\n");
                return 0;
            }
    }
    return 0;
}

int modify_customer(int acpt, int type){
    char buffer[500];
    printf("=============================== inside create new user ===============================\n");

    sleep(1);
    // send loop continue signal
    strcpy(buffer, "10"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send CONTINUE sig \n============================================\n");

    // recv signal
    if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
        printf("Error\n");
    }
    printf("%s\n", buffer);
    printf("recvd of ready sig \n============================================\n");
    // getchar();

    // send signal
    strcpy(buffer, "1"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send of TYPE sig \n============================================\n");
    // getchar();

    // case 1(type of op = 1)
    // send menu
    strcpy(buffer, "==========================================\nSelect Your Option :\n1. Username\n2. Balance\n3. Loan Taken\nEnter your choice :");
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("sent menu \n============================================\n");
    // getchar();

    // recv choice
    if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
        printf("Error\n");
    }
    printf("%s\n", buffer);
    printf("recvd choice \n============================================\n");
    // getchar();
    type = atoi(buffer);
    // send loop continue signal
    strcpy(buffer, "10"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send CONTINUE sig \n============================================\n");

    
    printf("==========type============= %d\n", type);
    switch(type){
        case 1:
            {  
                struct Customer m;
                char temp_userid[5];
                take_userid(acpt, temp_userid);
                printf("=================== change username ====================\n");
                change_username_common(acpt, atoi(temp_userid), 4);
                return continuee(acpt);
            }
        case 2:
            {
                struct Customer m;
                char temp_username[30]; 
                char temp_userid[5];
                take_username(acpt, temp_userid);
                printf("=================== change balance ====================\n");
                change_customer_balance(acpt, atoi(temp_userid));
                return continuee(acpt);
            }

        case 3:
            {
                struct Customer m;
                char temp_username[30]; 
                char temp_userid[5];
                take_username(acpt, temp_userid);
                printf("=================== loan taken ====================\n");
                change_customer_loan_status(acpt, atoi(temp_userid));
                return continuee(acpt);
            }
        default:
            {
                printf("Invalid Choice...\n");
                return continuee(acpt);
            }
    }
    return continuee(acpt);
}

int modify_employee(int acpt, int type){
    char buffer[500];
    printf("=============================== inside modify employee ===============================\n");

    sleep(1);
    // send loop continue signal
    strcpy(buffer, "10"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send CONTINUE sig \n============================================\n");

    // recv signal
    if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
        printf("Error\n");
    }
    printf("%s\n", buffer);
    printf("recvd of ready sig \n============================================\n");
    // getchar();

    // send signal
    strcpy(buffer, "1"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send of TYPE sig \n============================================\n");
    // getchar();

    // case 1(type of op = 1)
    // send menu
    strcpy(buffer, "==========================================\nSelect Your Option :\n1. Username\n2. Balance\n3. Loan Taken\nEnter your choice :");
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("sent menu \n============================================\n");
    // getchar();

    // recv choice
    if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
        printf("Error\n");
    }
    printf("%s\n", buffer);
    printf("recvd choice \n============================================\n");
    // getchar();
    type = atoi(buffer);
    // send loop continue signal
    strcpy(buffer, "10"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send CONTINUE sig \n============================================\n");

    
    printf("==========type============= %d\n", type);
    switch(type){
        case 1:
            {  
                struct Employee m;
                char temp_username[30]; 
                char temp_userid[5];
                take_username(acpt, temp_userid);
                printf("=================== change username ====================\n");
                change_username_common(acpt, atoi(temp_userid), 3);
                return continuee(acpt);
            }
        case 2:
            {
                struct Employee m;
                char temp_username[30]; 
                char temp_userid[5];
                take_username(acpt, temp_userid);
                printf("=================== change balance ====================\n");
                change_salary(acpt, atoi(temp_userid));
                return continuee(acpt);
            }
        default:
            {
                printf("Invalid Choice...\n");
                return continuee(acpt);
            }
    }
    return continuee(acpt);
}

