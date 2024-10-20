#include "structures.h"
#include "password_methods.h"
#include "admin.h"
// #include "create_new_user.h"

int login_admin(struct Admin a, int acpt){
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
        if(strcmp(tempAdmin.u.username, a.u.username)==0){
            if( (memcmp(tempAdmin.u.password, a.u.password, SHA256_DIGEST_LENGTH)) == 0){
                printf("-------------0000000000000---------- %d\n",tempAdmin.u.is_logged_in);
                if(tempAdmin.u.is_logged_in==1) {
                    send_message(acpt, "You are already logged in somewhere.....!",1);
                    raise(SIGINT);
                    return -1;
                } //already loggedin
                tempAdmin.u.is_logged_in = true;
                lseek(fd, -sizeof(tempAdmin), SEEK_CUR);
                if (write(fd, &tempAdmin, sizeof(tempAdmin)) == -1) {
                    perror("Error writing updated customer");
                }
                close(fd);
                // printf("===========login_admin %d ============\n",tempAdmin.u.userid);
                return tempAdmin.u.userid;
            }
        }
    }
    close(fd);
    return -1;

}

int login_manager(struct Manager a, int acpt){
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
        if(strcmp(tempCustomer.u.username, a.u.username)==0){
            if( (memcmp(tempCustomer.u.password, a.u.password, SHA256_DIGEST_LENGTH)) == 0){
                if(tempCustomer.u.is_logged_in==1) {
                    send_message(acpt, "You are already logged in somewhere.....!",1);
                    raise(SIGINT);
                    return -1;
                } //already loggedin
                tempCustomer.u.is_logged_in = true;
                lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                    perror("Error writing updated customer");
                }
                close(fd);
                return tempCustomer.u.userid;
            }
        }
    }

    close(fd);
    return -1;

}

int login_employee(struct Employee a, int acpt){
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
        printf("%d %d", tempCustomer.u.userid, a.u.userid);
        if(strcmp(tempCustomer.u.username, a.u.username)==0){
            if( (memcmp(tempCustomer.u.password, a.u.password, SHA256_DIGEST_LENGTH)) == 0){
                if(tempCustomer.u.is_logged_in==1){
                    send_message(acpt, "You are already logged in somewhere.....!",1);
                    raise(SIGINT);
                    return -1;
                } //already loggedin
                tempCustomer.u.is_logged_in = true;
                lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                    perror("Error writing updated customer");
                }
                close(fd);
                return tempCustomer.u.userid;
            }
        }
    }

    close(fd);
    return -1;

}

int login_customer(struct Customer a, int acpt){
    printf("--inside Customer login\n");
    printf("%s %d\n", a.u.username, a.u.userid);
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
            printf("%s %d %s %s\n", a.u.username, a.u.userid, tempCustomer.u.username, tempCustomer.u.password);
            char pass[70];
            // hashedPasswordToHex(tempCustomer.u.password, pass, SHA256_DIGEST_LENGTH);
            printf("%s\n",pass);
            if( (memcmp(tempCustomer.u.password, a.u.password, SHA256_DIGEST_LENGTH)) == 0){
                printf("-------------0000000000000---------- %d\n",tempCustomer.u.is_logged_in);
                if(tempCustomer.u.is_logged_in==1){
                    send_message(acpt, "You are already logged in somewhere.....!",1);
                    // raise(SIGINT);
                    return -2;
                } //already loggedin
                tempCustomer.u.is_logged_in = true;
                lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                    perror("Error writing updated customer");
                }
                close(fd);
                return tempCustomer.u.userid;
            }
        }
    }

    close(fd);
    return -1;

}

int login(struct User u, int type, int acpt){
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
                int t= login_admin(a, acpt);
                // printf("Inside switch admin login : %d\n", t);
                return t;
            }
        case 2:
            {
                struct Manager mngr;
                mngr.u = u;
                int t= login_manager(mngr, acpt);
                // printf("Inside switch : %d\n", t);
                return t;
            }
        case 3:
            {
                struct Employee emp;
                emp.u = u;
                int t= login_employee(emp, acpt);
                // printf("Inside switch : %d\n", t);
                return t;
            }
        case 4:
            {
                struct Customer cust;
                cust.u = u;
                int t= login_customer(cust, acpt);
                // printf("Inside switch : %d\n", t);
                return t
                ;
            }
        default:
        {
            send_message(acpt, "Invalid Input ...\n", 0);
            return -1;
        }
    }
    return -1;
}

void login_handler(){

}

bool logout(){

}