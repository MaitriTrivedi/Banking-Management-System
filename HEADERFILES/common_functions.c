#include <unistd.h>

int continuee(int acpt){
    char buffer[500];

    sleep(1);
    // // send loop continue signal
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
    strcpy(buffer, "==========================================\nSelect Your Option :\n1. Continue\n2. Logout\nEnter your choice :");
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
    int choice = atoi(buffer);
    // send loop continue signal
    strcpy(buffer, "10"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send CONTINUE sig \n============================================\n");

    
    printf("========== choice ============= %d\n", choice);
    switch(choice){
        case 1:{
            return 1;
        }
        case 2:{
            return 0;
        }
    }
}

void take_username(int acpt, char* username_buffer){
    char buffer[500];

    // sleep(1);
    // // // send loop continue signal
    // strcpy(buffer, "10"); // type 1
    // if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
    //     perror("Error sending login data");
    // }
    // printf("%s\n", buffer);
    // printf("send CONTINUE sig \n============================================\n");

    // recv signal
    if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
        printf("Error\n");
    }
    printf("%s\n", buffer);
    printf("recvd of ready sig \n============================================\n");
    // getchar();

    // send signal
    strcpy(buffer, "3"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send of TYPE sig \n============================================\n");
    // getchar();

    // case 3(type of op = 3)
    // send msg
    strcpy(buffer, "Enter Username : ");
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("sent msg \n============================================\n");
    // getchar();

    // recv input
    if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
        printf("Error\n");
    }
    printf("%s\n", buffer);
    printf("recvd input \n============================================\n");

    strcpy(username_buffer, buffer);
}

void take_password(int acpt, char* username_buffer, char *s){
    char buffer[500];
    if(s==NULL) s = "";

    sleep(1);
    // // send loop continue signal
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
    strcpy(buffer, "3"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send of TYPE sig \n============================================\n");
    // getchar();

    // case 3(type of op = 3)
    // send msg
    snprintf(buffer, sizeof(buffer), "Enter%s Password: ", s);
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("sent msg \n============================================\n");
    // getchar();

    // recv input
    if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
        printf("Error\n");
    }
    printf("%s\n", buffer);
    printf("recvd input \n============================================\n");

    strcpy(username_buffer, buffer);
}

void take_salary(int acpt, float *temp_salary){
    char buffer[500];

    sleep(1);
    // // send loop continue signal
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
    strcpy(buffer, "3"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send of TYPE sig \n============================================\n");
    // getchar();

    // case 3(type of op = 3)
    // send msg
    strcpy(buffer, "Enter Salary : ");
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("sent msg \n============================================\n");
    // getchar();

    // recv input
    if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
        printf("Error\n");
    }
    printf("%s\n", buffer);
    printf("recvd input \n============================================\n");

    *temp_salary = atof(buffer);
}

void send_message(int acpt, char* write_buffer){
    char buffer[500];

    sleep(1);
    // // send loop continue signal
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
    strcpy(buffer, "2"); // type 1
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("send of TYPE sig \n============================================\n");
    // getchar();

    // case 2(type of op = 2)
    // send msg
    strcpy(buffer, write_buffer);
    if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
        perror("Error sending login data");
    }
    printf("%s\n", buffer);
    printf("sent msg \n============================================\n");
    // getchar();
}

int change_password_common(int acpt, int uid, int type){
    printf("change password\n");
    char buffer[500];
    switch(type){
        case 1:{
            struct Admin tempCustomer;
            memset(&tempCustomer, 0, sizeof(tempCustomer));
            int fd, bytesRead;
            fd = open("DATABASE/admin.txt",O_RDWR);
            if(fd==-1){
                perror("");
                return continuee(acpt);
            }
            while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
                printf("====in while\n");
                printf("%d %d \n",tempCustomer.u.userid,uid );
                if(tempCustomer.u.userid==uid){
                    printf("====in if\n");
                    char password[30];
                    take_password(acpt, password, " Old");
                    struct User tempUsr;
                    hashPassword(password, tempUsr.password);
                    if((memcmp(tempUsr.password, tempCustomer.u.password, SHA256_DIGEST_LENGTH)) == 0){
                        take_password(acpt, password, " New");
                        hashPassword(password, tempCustomer.u.password);
                        lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                        if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                            perror("Error writing updated customer");
                        }
                        send_message(acpt, "Password Changes Successfully ...\n");
                    }
                    else{
                        send_message(acpt, "Sorry You cannot change the password because You have entered WRONG Password ...\n");
                    }
                    close(fd);
                    int temp = continuee(acpt);
                    printf("=====temp %d\n",temp);
                    return temp;
                }
                printf("change password admin\n");
            }
            close(fd);
            break;
        }
        case 2:{
            struct Manager tempCustomer;
            memset(&tempCustomer, 0, sizeof(tempCustomer));
            int fd, bytesRead;
            fd = open("DATABASE/manager.txt",O_RDWR);
            if(fd==-1){
                perror("");
                return continuee(acpt);
            }
            while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
                if(tempCustomer.u.userid==uid){
                    char password[30];
                    take_password(acpt, password, " Old");
                    struct User tempUsr;
                    hashPassword(password, tempUsr.password);
                    if((memcmp(tempUsr.password, tempCustomer.u.password, SHA256_DIGEST_LENGTH)) == 0){
                        take_password(acpt, password, " New");
                        hashPassword(password, tempCustomer.u.password);
                        tempCustomer.u = tempUsr;
                        lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                        if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                            perror("Error writing updated customer");
                        }
                        send_message(acpt, "Password Changes Successfully ...\n");
                    }
                    else{
                        send_message(acpt, "Sorry You cannot change the password because You have entered WRONG Password ...\n");
                    }
                    close(fd);
                    return continuee(acpt);
                }
            }
            close(fd);
            break;
        }
        case 3:{
            struct Employee tempCustomer;
            memset(&tempCustomer, 0, sizeof(tempCustomer));
            int fd, bytesRead;
            fd = open("DATABASE/employee.txt",O_RDWR);
            if(fd==-1){
                perror("");
                return continuee(acpt);
            }
            while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
                if(tempCustomer.u.userid==uid){
                    char password[30];
                    take_password(acpt, password, " Old");
                    struct User tempUsr;
                    hashPassword(password, tempUsr.password);
                    if((memcmp(tempUsr.password, tempCustomer.u.password, SHA256_DIGEST_LENGTH)) == 0){
                        take_password(acpt, password, " New");
                        hashPassword(password, tempCustomer.u.password);
                        tempCustomer.u = tempUsr;
                        lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                        if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                            perror("Error writing updated customer");
                        }
                        send_message(acpt, "Password Changes Successfully ...\n");
                    }
                    else{
                        send_message(acpt, "Sorry You cannot change the password because You have entered WRONG Password ...\n");
                    }
                    close(fd);
                    return continuee(acpt);
                }
            }
            close(fd);
            break;
        }
        case 4:{
            struct Customer tempCustomer;
            memset(&tempCustomer, 0, sizeof(tempCustomer));
            int fd, bytesRead;
            fd = open("DATABASE/customer.txt",O_RDWR);
            if(fd==-1){
                perror("");
                return continuee(acpt);
            }
            while((bytesRead = read(fd, &tempCustomer, sizeof(tempCustomer))) > 0 ){
                if(tempCustomer.u.userid==uid){
                    char password[30];
                    take_password(acpt, password, " Old");
                    struct User tempUsr;
                    hashPassword(password, tempUsr.password);
                    if((memcmp(tempUsr.password, tempCustomer.u.password, SHA256_DIGEST_LENGTH)) == 0){
                        take_password(acpt, password, " New");
                        hashPassword(password, tempCustomer.u.password);
                        tempCustomer.u = tempUsr;
                        lseek(fd, -sizeof(tempCustomer), SEEK_CUR);
                        if (write(fd, &tempCustomer, sizeof(tempCustomer)) == -1) {
                            perror("Error writing updated customer");
                        }
                        send_message(acpt, "Password Changes Successfully ...\n");
                    }
                    else{
                        send_message(acpt, "Sorry You cannot change the password because You have entered WRONG Password ...\n");
                    }
                    close(fd);
                    return continuee(acpt);
                }
            }
            close(fd);
            break;
        }
    }
    return continuee(acpt);
}
