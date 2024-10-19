#include <sys/socket.h>
#include <string.h>
#include "../HEADERFILES/structures.h"
#include<fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../HEADERFILES/password_methods.h"


int logout_common(int uid, int type){
    char buffer[500];
    switch(type){
        case 1:{
            struct Admin tempCustomer;
            memset(&tempCustomer, 0, sizeof(tempCustomer));
            int fd, bytesRead;
            fd = open("../DATABASE/admin.txt",O_RDWR);
            if(fd==-1){
                perror("");
                return 0;
            }
            while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
                if(tempCustomer.u.userid==uid){
                    tempCustomer.u.is_logged_in = false;
                    lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                    if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                        perror("Error writing updated customer");
                    close(fd);
                    return 0;
                }
            }
            close(fd);
            break;
        }
        case 2:{
            struct Manager tempCustomer;
            memset(&tempCustomer, 0, sizeof(tempCustomer));
            int fd, bytesRead;
            fd = open("../DATABASE/manager.txt",O_RDWR);
            if(fd==-1){
                perror("");
                return 0;
            }
            while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
                if(tempCustomer.u.userid==uid){
                    tempCustomer.u.is_logged_in = false;
                    lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                    if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                        perror("Error writing updated customer");
                    close(fd);
                    return 0;
                    }
                    printf("done\n");

            }
            close(fd);
            break;
        case 3:{
            struct Employee tempCustomer;
            memset(&tempCustomer, 0, sizeof(tempCustomer));
            int fd, bytesRead;
            fd = open("../DATABASE/employee.txt",O_RDWR);
            if(fd==-1){
                perror("");
                return 0;
            }
            while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
                if(tempCustomer.u.userid==uid){
                    tempCustomer.u.is_logged_in = false;
                    lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                    if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                        perror("Error writing updated customer");
                    close(fd);
                    return 0;
                }
            }
            close(fd);
            break;
        case 4:{
            printf("---- %d \n", uid);
            struct Customer tempCustomer;
            memset(&tempCustomer, 0, sizeof(tempCustomer));
            int fd, bytesRead;
            fd = open("../DATABASE/customer.txt",O_RDWR);
            if(fd==-1){
                perror("");
                return 0;
            }
            while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
                printf("%d %d\n", tempCustomer.u.userid, uid);
                if(tempCustomer.u.userid==uid){
                    tempCustomer.u.is_logged_in = false;
                    lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                    if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                        perror("Error writing updated customer");
                        close(fd);
                        return 0;
                    }
                    else {
                        printf("done\n");
                        return 0;
                    }
                    close(fd);
                    break;
                }
            }
            }
        }
    }
    return 0;
}}}}}


void main(int argc, char *argv[]){
    int type;
    int uid;
    if(argc>1){
        type = atoi(argv[1]);
    }
    else{
        printf("Enter Type of User You want to add :\n1. Admin\n2. Manager\n3. Employee\n4. Customer\n");
        scanf("%d", &type);
    }
    printf("%d\n", type);
    printf("Enter User Id : ");
    scanf("%d", &uid);
    logout_common(uid, type);
    return;
}

// gcc create_new_user.c -o create_new_user -lssl -lcrypto
// gcc create_new_user_script.c -lssl -lcrypto