#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <signal.h>
#include <signal.h>
#include <string.h>
#include "./HEADERFILES/structures.h"
#include "HEADERFILES/password_methods.h"
#include "HEADERFILES/login_logout.h"
#include "HEADERFILES/manager.h"
#include "HEADERFILES/customer.h"
#include "HEADERFILES/employee.h"

int sct;

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clientSignalHandler(int signal_num) {
    printf("\n======================= CLOSING CLIENT SOCKET ============================\n");
    close(sct);
    exit(0);
}

void login_logout_handler(int sct){
    printf("------------------------------------------------------------------------\n");
    printf("--------------Inside login logout handler---------------------\n");
    char read_buffer[500];
    char write_buffer[500];
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
        // printf("====================================================================\n===== WELCOME TO XYZ BANK =====\n====================================================================\nIN WHICH MODE YOU WANT TO LOGIN ?\nSELECT AN OPTION FROM THE FOLLOWING :\n1. Admin\n2. Manager\n3. Employee\n4. Customer\n==========================================\nEnter your choice : ");
        // sleep(1);
        scanf("%s", write_buffer);
        if (send(sct, write_buffer, strlen(write_buffer)+1, 0) == -1) {
            perror("Error sending login data\n");
        }
        // printf("sent -> %s\n", write_buffer);

        // username reading
        if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
            printf("Something Went Wrong.\n");
        }
        printf("%s\n", read_buffer);
        // printf("==========================================================\n======================= LOGIN ==========================\n==========================================================\nEnter Name : ");
        // printf("in 22222 \n============================================\n");
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
        // printf("Enter Password : ");
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
        printf("succ or fail - %s\n", read_buffer);
        if(atoi(read_buffer) == 0) {
            printf("Login Unsuccesful\n");
            continue;
            }
        else {
            printf("Login Succesful\n============================================\n");
            break;
            }
        printf("==========Continue ========\n");
        // printf("in 44444 \n============================================\n");
    }

}

void main(int argc, char *argv[])
{
    signal(SIGINT,clientSignalHandler);
    char read_buffer[500], write_buffer[500];
    size_t read_size, write_size;
    int choice;

    // create socket
    sct = socket(AF_INET, SOCK_STREAM, 0);
    sct = socket(AF_INET, SOCK_STREAM, 0);
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
        // printf("---------------------------starting of the login loop\n");
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
        
        // to get choice
        scanf("%s", write_buffer);
        if (send(sct, write_buffer, strlen(write_buffer)+1, 0) == -1) {
            perror("Error sending login data\n");
        }
        choice = atoi(write_buffer);

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
        // printf("==========Continue ========\n");
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
            printf("---------------------------1-------------------------\n");
            // will show the menu, get the choice
            printf("inside case 1 ===\n");
            memset(read_buffer, '\0', sizeof(read_buffer));
            memset(write_buffer, '\0', sizeof(write_buffer));
            // get menu and show                
            if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
                printf("Something Went Wrong.\n");
            }
            printf("recv of menu loop \n============================================\n");
            printf("Rcvd1 : %s\n", read_buffer);
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
            printf("Rcvd2 : %s\n", read_buffer);
            if(read_buffer=="10") continue;
            else if(read_buffer=="-10") raise(SIGINT);
            // else break;
        }
        else if(temp_choice==2){
            printf("---------------------------2-------------------------\n");
            // will just print the recived msg
            printf("inside case 2 ===\n");
            memset(read_buffer, '\0', sizeof(read_buffer));
            memset(write_buffer, '\0', sizeof(write_buffer));
            // get menu and show                
            if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
                perror("Something Went Wrong.\n");
            }
            printf("recvd msg \n============================================\n");
            printf("Rcvd3 : %s\n", read_buffer);


            // rcv signal to continue the loop   
            printf("Ready to recv CONTI sig \n============================================\n");             
            if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
                perror("Something Went Wrong.\n");
            }
            printf("recv continue sig \n============================================\n");
            printf("Rcvd4 : %s\n", read_buffer);
            if(read_buffer=="10") continue;
            else if(read_buffer=="-10") raise(SIGINT);
            // else break;
        }
        else if(temp_choice==3){
            printf("---------------------------3-------------------------\n");
            // will print the recived msg and get the input
            printf("inside case 2 ===\n");
            memset(read_buffer, '\0', sizeof(read_buffer));
            memset(write_buffer, '\0', sizeof(write_buffer));
            // get msgand show                
            if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
                perror("Something Went Wrong.\n");
            }
            printf("recvd msg \n============================================\n");
            printf("Rcvd5 : %s\n", read_buffer);

            scanf("%s", write_buffer);
            if (send(sct, write_buffer, strlen(write_buffer)+1, 0) == -1) {
                perror("Error sending login data");
            }
            printf("%s\n", write_buffer);
            printf("sent taken input \n============================================\n");

            // rcv signal to continue the loop   
            printf("Ready to recv CONTI sig \n============================================\n");             
            if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
                perror("Something Went Wrong.\n");
            }
            printf("recv continue sig \n============================================\n");
            printf("Rcvdddd : %s\n", read_buffer);
            if(read_buffer=="10") continue;
            else if(read_buffer=="-10") raise(SIGINT);
            // else break;
        }
        else if(temp_choice==4){
            
            // once login you can continue this loop
            // rcv signal to continue the loop   
            printf("Ready to recv CONTI sig \n============================================\n");             
            if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
                perror("Something Went Wrong.\n");
            }
            printf("recv continue sig \n============================================\n");
            printf("Rcvd8 : %s\n", read_buffer);
            if(read_buffer=="10") continue;
            else if(read_buffer=="-10") raise(SIGINT);

            login_logout_handler(sct);
        }
        else if(temp_choice==5){
            printf("---------------------------5-------------------------\n");
            // will print the recived msg and get the input
            printf("inside case 2 ===\n");
            memset(read_buffer, '\0', sizeof(read_buffer));
            memset(write_buffer, '\0', sizeof(write_buffer));
            // get msgand show                
            if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
                perror("Something Went Wrong.\n");
            }
            printf("recvd msg \n============================================\n");
            printf("Rcvd5 : %s\n", read_buffer);

            clear_input_buffer();
            fgets(write_buffer, sizeof(write_buffer), stdin);
            // Remove the trailing newline character added by fgets, if any
            write_buffer[strcspn(write_buffer, "\n")] = '\0'; 

            // Send the feedback through the socket
            if (send(sct, write_buffer, strlen(write_buffer) + 1, 0) == -1) {
                perror("Error sending feedback");
            } else {
                printf("Feedback sent: %s\n", write_buffer);
                printf("sent taken input \n============================================\n");
}
            printf("%s\n", write_buffer);
            printf("sent taken input \n============================================\n");

            // rcv signal to continue the loop   
            printf("Ready to recv CONTI sig \n============================================\n");             
            if(recv(sct, read_buffer, sizeof(read_buffer), 0)==-1){
                perror("Something Went Wrong.\n");
            }
            printf("recv continue sig \n============================================\n");
            printf("Rcvdddd : %s\n", read_buffer);
            if(read_buffer=="10") continue;
            else if(read_buffer=="-10") raise(SIGINT);
            // else break;
        }
    }
    printf("outside switch case \n");
    // break;
    printf("\n======================= CLOSING CLIENT SOCKET ============================\n");
    close(sct);
    exit(0);
}