#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <string.h>
#include "./HEADERFILES/structures.h"
#include "HEADERFILES/password_methods.h"
#include "HEADERFILES/login_logout.h"
#include "HEADERFILES/manager.h"
#include "HEADERFILES/customer.h"
#include "HEADERFILES/employee.h"

void main(int argc, char *argv[])
{
    char read_buffer[500], write_buffer[500];
    size_t read_size, write_size;

    // create socket
    int sct = socket(AF_INET, SOCK_STREAM, 0);
    if(sct==-1){
        perror("");
        return;
    }

    // connect
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8060);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int cnt = connect(sct, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(cnt == -1){
        perror("");
        return;
    }

    // LOGIN HANDLING :
    while(1){
        memset(read_buffer, '\0', sizeof(read_buffer));
        memset(write_buffer, '\0', sizeof(write_buffer));
        
        // login
        // choice reading

        strcpy(write_buffer, "1");
        if (send(sct, write_buffer, strlen(write_buffer)+1, 0) == -1) {
            perror("Error sending login data\n");
        }

        if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
            // error
            printf("Something Went Wrong.\n");
        }
        
        printf("%s\n", read_buffer);

        scanf("%s", write_buffer);
        if (send(sct, write_buffer, strlen(write_buffer)+1, 0) == -1) {
            perror("Error sending login data\n");
        }

        // username reading
        if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
            printf("Something Went Wrong.\n");
        }
        printf("%s\n", read_buffer);
        scanf("%s", write_buffer);
        if (send(sct, write_buffer, strlen(write_buffer)+1, 0) == -1) {
            perror("Error sending login data");
        }

        // password reading
        if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
            // error
            printf("Something Went Wrong.\n");
        }
        printf("%s\n", read_buffer);
        scanf("%s", write_buffer);
        if (send(sct, write_buffer, strlen(write_buffer)+1, 0) == -1) {
            perror("Error sending login data");
        }

        // login success or failure
        int tempp;
        if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
            // error
            printf("Something Went Wrong.\n");
        }
        // printf("===%s\n", read_buffer);
        if(atoi(read_buffer) == 0) {
            printf("Login Unsuccesful\n");
            continue;
            }
        else {
            printf("Login Succesful\n============================================\n");
            break;
            }
        printf("==========Continue =====");
    }


    // AFTER SUCCESSFUL LOGIN
    // admin_handler()
    // employee_handler()
    // manager_handler()
    // cutomer_handler()
    while(1){
        // empty buffer
        memset(read_buffer, '\0', sizeof(read_buffer));
        int choice;
        printf("Start of loop \n============================================\n");
        // getchar();

        // send ready recv 
        strcpy(write_buffer, "1");
        if (send(sct, write_buffer, strlen(write_buffer)+1, 0) == -1) {
            perror("Error sending login data");
        }
        printf("%s\n", write_buffer);
        printf("send ready sig \n============================================\n");
        // getchar();

        // rcv signal
        if(recv(sct, &read_buffer, sizeof(read_buffer), 0)==-1){
            perror("");
        }
        printf("%s\n", read_buffer);
        int temp_choice = atoi(read_buffer);
        printf("recvd TYPE of operation sig \n============================================\n");
        // getchar();

        if(temp_choice==1){
            // will show the menu, get the choice
            printf("inside case 1 ===\n");
            memset(read_buffer, '\0', sizeof(read_buffer));
            memset(write_buffer, '\0', sizeof(write_buffer));
            // get menu and show                
            if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
                printf("Something Went Wrong.\n");
            }
            printf("recv of menu loop \n============================================\n");
            printf("Rcvd : %s\n", read_buffer);
            // getchar();
            // printf("%s\n", read_buffer);

            // send choice
            scanf("%s", write_buffer);
            if (send(sct, write_buffer, strlen(write_buffer)+1, 0) == -1) {
                perror("Error sending login data");
            }
            printf("%s\n", write_buffer);
            printf("sent choice \n============================================\n");
            // getchar();
            // continue;
            // break;

            // rcv signal to continue the loop                
            if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
                printf("Something Went Wrong.\n");
            }
            printf("recv of menu loop \n============================================\n");
            printf("Rcvd : %s\n", read_buffer);
            if(read_buffer=="10") continue;
        }
        else if(temp_choice==2){
            // will just print the recived msg
            printf("inside case 2 ===\n");
            memset(read_buffer, '\0', sizeof(read_buffer));
            memset(write_buffer, '\0', sizeof(write_buffer));
            // get menu and show                
            if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
                printf("Something Went Wrong.\n");
            }
            printf("recv of messagcontinue signale \n============================================\n");
            printf("Rcvd : %s\n", read_buffer);

            // rcv signal to continue the loop                
            if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
                printf("Something Went Wrong.\n");
            }
            printf("recv of menu loop \n============================================\n");
            printf("Rcvd : %s\n", read_buffer);
            if(read_buffer=="10") continue;
        }
        printf("outside switch case \n");
        // break;
    }
}