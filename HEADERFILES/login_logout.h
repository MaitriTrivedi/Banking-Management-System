#include "structures.h"
#include "password_methods.h"
#include "admin.h"
// #include "create_new_user.h"

int login_admin(struct Admin a){
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
        // printf("%s %s\n",tempAdmin.u.username, tempAdmin.u.password);
        // printf("%s %s\n",a.u.username, a.u.password);
        // printf("%d\n", (memcmp(tempAdmin.u.password, a.u.password, SHA256_DIGEST_LENGTH)));
        if(strcmp(tempAdmin.u.username, a.u.username)==0){
            if( (memcmp(tempAdmin.u.password, a.u.password, SHA256_DIGEST_LENGTH)) == 0){
                close(fd);
                // printf("===========login_admin %d ============\n",tempAdmin.u.userid);
                return tempAdmin.u.userid;
            }
        }
    }

    close(fd);
    return -1;

}

int login_manager(struct Manager a){
    struct Manager tempCustomer;
    memset(&tempCustomer, 0, sizeof(tempCustomer));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/manager.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        // printf("%s %s\n",tempAdmin.u.username, tempAdmin.u.password);
        // printf("%s %s\n",a.u.username, a.u.password);
        // printf("%d\n", (memcmp(tempAdmin.u.password, a.u.password, SHA256_DIGEST_LENGTH)));
        if(strcmp(tempCustomer.u.username, a.u.username)==0){
            if( (memcmp(tempCustomer.u.password, a.u.password, SHA256_DIGEST_LENGTH)) == 0){
                close(fd);
                return tempCustomer.u.userid;
            }
        }
    }

    close(fd);
    return -1;

}

int login_employee(struct Employee a){
    struct Employee tempCustomer;
    memset(&tempCustomer, 0, sizeof(tempCustomer));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/employee.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        // printf("%s %s\n",tempAdmin.u.username, tempAdmin.u.password);
        // printf("%s %s\n",a.u.username, a.u.password);
        // printf("%d\n", (memcmp(tempAdmin.u.password, a.u.password, SHA256_DIGEST_LENGTH)));
        if(strcmp(tempCustomer.u.username, a.u.username)==0){
            if( (memcmp(tempCustomer.u.password, a.u.password, SHA256_DIGEST_LENGTH)) == 0){
                close(fd);
                return tempCustomer.u.userid;
            }
        }
    }

    close(fd);
    return -1;

}

int login_customer(struct Customer a){
    struct Customer tempCustomer;
    memset(&tempCustomer, 0, sizeof(tempCustomer));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return -1;
    }

    // search for the availability of the Admin userx
    // printf("Start checking for the user...\n");
    while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
        // printf("%s %s\n",tempAdmin.u.username, tempAdmin.u.password);
        // printf("%s %s\n",a.u.username, a.u.password);
        // printf("%d\n", (memcmp(tempAdmin.u.password, a.u.password, SHA256_DIGEST_LENGTH)));
        if(strcmp(tempCustomer.u.username, a.u.username)==0){
            if( (memcmp(tempCustomer.u.password, a.u.password, SHA256_DIGEST_LENGTH)) == 0){
                close(fd);
                return tempCustomer.u.userid;
            }
        }
    }

    close(fd);
    return -1;

}

int login(struct User u, int type){
    // printf("=================\n");
    // printf("===== LOGIN =====\n");
    // printf("=================\n");
    printf("Type %d\n", type);
    printf("Uname %s\n", u.username);
    printf("Password %s\n", u.password);
    switch (type){
        case 1:
            {
                struct Admin a;
                a.u = u;
                int t= login_admin(a);
                // printf("Inside switch admin login : %d\n", t);
                return t;
            }
        case 2:
            {
                struct Manager mngr;
                mngr.u = u;
                int t= login_manager(mngr);
                // printf("Inside switch : %d\n", t);
                return t;
            }
        case 3:
            {
                struct Employee emp;
                emp.u = u;
                int t= login_employee(emp);
                // printf("Inside switch : %d\n", t);
                return t;
            }
        case 4:
            {
                struct Customer cust;
                cust.u = u;
                int t= login_customer(cust);
                // printf("Inside switch : %d\n", t);
                return t
                ;
            }
    }
    return -1;
}

void login_handler(){

}

bool logout(){

}