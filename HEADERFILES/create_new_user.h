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

    // open file in read write file
    fd = open("../DATABASE/manager.txt",O_RDWR);
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
    fd = open("../DATABASE/employee.txt",O_RDWR);
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
    fd = open("../DATABASE/customer.txt",O_RDWR);
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


int create_new_user(int acpt, int type){
    char buffer[500];
    printf("=============================== inside create new user ===============================\n");

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
                return continuee(acpt);
            }
        case 2:
            {
                struct Manager m;
                char temp_username[30];
                // printf("Enter Manager's Name : ");
                take_username(acpt, temp_username);

                char temp_password[30];
                // printf("Enter Manager's Password : ");
                take_password(acpt, temp_password);

                strcpy(m.u.username, temp_username);
                hashPassword( temp_password, m.u.password);
                m.u.userid = show_user_id_by_one()+1;
                create_manager_user(m);
                update_user_id_by_one();
                return continuee(acpt);;
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
                e.u.userid = show_user_id_by_one()+1;
                create_employee_user(e);
                update_user_id_by_one();
                return 0;
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

                printf("Enter Customer's Account Balance : ");
                scanf("%f", &c.account_balance);
                strcpy(c.u.username, temp_name);
                hashPassword( temp_password, c.u.password);
                c.u.userid = show_user_id_by_one()+1;
                create_customer_user(c);
                update_user_id_by_one();
                return 0;
            }
        default:
            {
                printf("Invalid Choice...\n");
                return 0;
            }
    }
    return 0;
}


