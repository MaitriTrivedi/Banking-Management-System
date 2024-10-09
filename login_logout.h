#include "structures.h"
#include "password_methods.h"
#include "admin.h"

bool login_admin(struct Admin a){
    struct Admin tempAdmin;
    memset(&tempAdmin, 0, sizeof(tempAdmin));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/admin.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return false;
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
                return true;
            }
        }
    }

    close(fd);
    return false;

}

bool login_customer(struct Customer a){
    struct Customer tempCustomer;
    memset(&tempCustomer, 0, sizeof(tempCustomer));

    // open admin database file
    int fd, bytesRead;
    fd = open("DATABASE/customer.txt",O_RDWR);
    if(fd==-1){
        perror("");
        return false;
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
                return true;
            }
        }
    }

    close(fd);
    return false;

}

bool login(struct User u, int type){
    printf("=================\n");
    printf("===== LOGIN =====\n");
    printf("=================\n");
    switch (type){
        case 1:
            {
                struct Admin a;
                a.u = u;
                int t= login_admin(a);
                // printf("Inside switch : %d\n", t);
                return t;
            }
        case 2:
        case 3:
        case 4:
            {
                struct Customer c;
                c.u = u;
                int t= login_customer(c);
                // printf("Inside switch : %d\n", t);
                return t;
            }
    }
    return 0;
}

bool logout(){

}