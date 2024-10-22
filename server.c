#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include "HEADERFILES/structures.h"
#include "HEADERFILES/login_logout.h"
#include "HEADERFILES/manager.h"
#include "HEADERFILES/customer.h"
#include "HEADERFILES/employee.h"

#define PORT 8060
#define LISTEN_BACKLOG 50
#define MAX_CONCURRENT_CONNECTIONS 5

volatile sig_atomic_t server_running = 1;
static volatile int keep_running = 1;
int server_socket;

void ignoreSIGINT(int signum) {
    printf("\nServer received SIGINT, ignoring...\n");
}

// void handle_sigterm(int sig) {
//     printf("Caught SIGTERM signal. Cleaning up and exiting...\n");
//     // Perform cleanup if needed
//     exit(0);
// }

// void serverSignalHandler(int signum) {
//     if (signum == SIGINT) {
//         printf("\nReceived Ctrl+C. Shutting down server...\n");
//         server_running = 0;
//         keep_running = 0;  // Use existing keep_running variable
        
//         // Close the server socket to unblock accept()
//         if (server_socket != -1) {
//             shutdown(server_socket, SHUT_RDWR);
//             close(server_socket);
//         }
//     }
// }

void *handleClient(void *client_socket)
{
    // signal(SIGINT,serverSignalHandler);
    char read_buffer[500], write_buffer[500];
    size_t read_size, write_size;
    /*
    This is called after creating new thread for each new client when a new client sends the request to handle each client concurrently.
    */
    printf("\n----------------------------- a new client connected\n");
    int acpt = *(int *)client_socket;

    int choice;
    struct User u;
    char msg[500];
    int brk=0;
    int c=0;
    int t=0;
    while(1){
        // printf("=========== %d ==============\n", c);
        // Receive login data from client
        memset(read_buffer, '\0', sizeof(read_buffer));
        memset(write_buffer, '\0', sizeof(write_buffer));
        memset(msg, '\0', sizeof(msg));

        printf("ready to recv login choice\n");
        if (recv(acpt, &read_buffer, sizeof(read_buffer), 0) == -1) {
            perror("Error receiving login choice");
            close(acpt);
            pthread_exit(NULL);
        }

        // if(t==1){
        //     printf("managed by t^^^^^^^^^^^^^^^^^^^^^^^^^\n");
        //     printf("ready to recv login choice\n");
        //     if (recv(acpt, &read_buffer, sizeof(read_buffer), 0) == -1) {
        //         perror("Error receiving login choice");
        //         close(acpt);
        //         pthread_exit(NULL);
        //     }
        //     t=0;
        // }
        // printf("************************** = %s\n", read_buffer);
        strcpy(msg, "====================================================================\n===== WELCOME TO XYZ BANK =====\n====================================================================\nIN WHICH MODE YOU WANT TO LOGIN ?\nSELECT AN OPTION FROM THE FOLLOWING :\n1. Admin\n2. Manager\n3. Employee\n4. Customer\n==========================================\nEnter your choice : ");
        if (send(acpt, msg, strlen(msg)+1, 0) == -1) {
            perror("Error sendiread_bufferng login data");
            // continue;
        }

        if (recv(acpt, &read_buffer, sizeof(read_buffer), 0) == -1) {
            perror("Error receiving login choice");
            close(acpt);
            pthread_exit(NULL);
        }
        // printf("choice %s\n",read_buffer);
        choice = atoi(read_buffer);

        strcpy(msg, "==========================================================\n======================= LOGIN ==========================\n==========================================================\nEnter Name : ");
        if (send(acpt, msg, strlen(msg)+1, 0) == -1) {
            perror("Error sending login data");
        }
        if (recv(acpt, &u.username, sizeof(u.username), 0) == -1) {
            perror("Error receiving login data");
            close(acpt);
            pthread_exit(NULL);
        }
        // printf("uname %s\n",u.username);
        strcpy(msg, "Enter Password : ");
        if (send(acpt, msg, strlen(msg)+1, 0) == -1) {
            perror("Error sending login data");
        }
        

        if (recv(acpt, &read_buffer, sizeof(read_buffer), 0) == -1) {
            perror("Error receiving login data");
            close(acpt);
            pthread_exit(NULL);
        }
        // printf("pass %s\n",read_buffer);
        hashPassword(read_buffer, u.password);

        int login_success_user_id = login(u, choice, acpt);
        // if(login_success_user_id==-1){
        //     continue;
        // }
        // else 
        if(login_success_user_id==-2 || login_success_user_id==-1){
            printf("----------------------------- handling multiple login tries\n");
            choice = 9;
            strcpy(msg, "0");
            // getchar();
            if (send(acpt, msg, strlen(msg)+1, 0) == -1) {
                perror("Sent login data");
            }
        }
        else {
            strcpy(msg, "1");
            // getchar();
            if (send(acpt, msg, strlen(msg)+1, 0) == -1) {
                perror("Sent login data");
            }
            printf("----------------------------- Login Successful\n");
            // continue;
        }

        
        // handle further :
        int t1=1;
        int conti;
        while(t1!=2){
            switch (choice){
                 case 9:{
                    // printf("In multiple login handler\n");
                    while(1){
                        conti = 7;
                        // printf("contiiii============== %d\n",conti);
                        if(conti==1){
                            continue;
                        }
                        else if(conti==0){
                            t=1;
                            t1=2;
                            break;
                        }
                        else if(conti==6){
                            // printf("contiiii brk brk brk\n");
                            brk=1;
                            sleep(1);
                            // // send loop continue signal
                            strcpy(read_buffer, "-10"); // type 1
                            if (send(acpt, read_buffer, strlen(read_buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", read_buffer);
                            // printf("send CONTINUE sig \n============================================\n");
                            // printf("=============================== client connection closed ===============================\n\n");
                            close(acpt);
                            pthread_exit(NULL);
                            break;
                        }
                        else if(conti==7){
                            // printf("Returning to login menu...\n");
                            printf("------------t----------------- Logged Out\n");
                            
                            char buffer[500];

                            sleep(1);
                            // // send loop continue signal
                            strcpy(buffer, "10"); // type 1
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send CONTINUE sig \n============================================\n");

                            // recv signal
                            if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
                                printf("Error\n");
                            }
                            // printf("%s\n", buffer);
                            // printf("recvd of ready sig \n============================================\n");
                            // getchar();

                            // send signal
                            strcpy(buffer, "4"); // type 4
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send of TYPE sig \n============================================\n");

                            sleep(1);
                            // // send loop continue signal
                            strcpy(buffer, "10"); // type 1
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send CONTINUE sig \n============================================\n");
                            t1 = 2;
                            brk=1;
                            break; 
                        }
                    }
                    // manager_handler(acpt, login_success_user_id);
                    break;
                }
                case 1:
                {   
                    
                    while(1){
                        conti = admin_handler(acpt,login_success_user_id);
                        // printf("contiiii============== %d\n",conti);
                        if(conti==1){
                            continue;
                        }
                        else if(conti==0){
                            t=1;
                            t1=2;
                            break;
                        }
                        else if(conti==6){
                            // printf("contiiii brk brk brk\n");
                            brk=1;
                            sleep(1);
                            // // send loop continue signal
                            strcpy(read_buffer, "-10"); // type 1
                            if (send(acpt, read_buffer, strlen(read_buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", read_buffer);
                            // printf("send CONTINUE sig \n============================================\n");
                            printf("============================ CLIENT CONNECTION CLOSED ============================\n");
                            close(acpt);
                            pthread_exit(NULL);
                            break;
                        }
                        else if(conti==7){
                            // printf("Returning to login menu...\n");
                            printf("----------------------------- Logged Out\n");
                            
                            char buffer[500];

                            sleep(1);
                            // // send loop continue signal
                            strcpy(buffer, "10"); // type 1
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send CONTINUE sig \n============================================\n");

                            // recv signal
                            if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
                                printf("Error\n");
                            }
                            // printf("%s\n", buffer);
                            // printf("recvd of ready sig \n============================================\n");
                            // getchar();

                            // send signal
                            strcpy(buffer, "4"); // type 4
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send of TYPE sig \n============================================\n");

                            sleep(1);
                            // // send loop continue signal
                            strcpy(buffer, "10"); // type 1
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send CONTINUE sig \n============================================\n");
                            t1 = 2;
                            brk=1;
                            break; 
                        }
                    }
                    // manager_handler(acpt, login_success_user_id);
                    break;
                }
                case 2:
                {   
                    printf("----------------------------- In manager handler\n");
                    while(1){
                        conti = manager_handler(acpt,login_success_user_id);
                        // printf("contiiii============== %d\n",conti);
                        if(conti==1){
                            continue;
                        }
                        else if(conti==0){
                            t1=2;
                            break;
                        }
                        else if(conti==6){
                            // printf("contiiii brk brk brk\n");
                            brk=1;
                            sleep(1);
                            // // send loop continue signal
                            strcpy(read_buffer, "-10"); // type 1
                            if (send(acpt, read_buffer, strlen(read_buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", read_buffer);
                            // printf("send CONTINUE sig \n============================================\n");
                            // printf("=============================== client connection closed ===============================\n\n");
                            close(acpt);
                            pthread_exit(NULL);
                            break;
                        }
                        else if(conti==7){
                            // printf("Returning to login menu...\n");
                            printf("----------------------------- Logged Out\n");
                            
                            char buffer[500];

                            sleep(1);
                            // // send loop continue signal
                            strcpy(buffer, "10"); // type 1
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send CONTINUE sig \n============================================\n");

                            // recv signal
                            if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
                                printf("Error\n");
                            }
                            // printf("%s\n", buffer);
                            // printf("recvd of ready sig \n============================================\n");
                            // getchar();

                            // send signal
                            strcpy(buffer, "4"); // type 4
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send of TYPE sig \n============================================\n");

                            sleep(1);
                            // // send loop continue signal
                            strcpy(buffer, "10"); // type 1
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send CONTINUE sig \n============================================\n");
                            t1 = 2;
                            brk=1;
                            break; 
                        }
                    }
                    // manager_handler(acpt, login_success_user_id);
                    break;
                }
                case 3:
                {
                    printf("===========================In Employee handler\n");
                    while(1){
                        conti = employee_handler(acpt,login_success_user_id);
                        // printf("contiiii============== %d\n",conti);
                        if(conti==1){
                            continue;
                        }
                        else if(conti==0){
                            // printf("====11111111111111111111111111111\n");
                            t1=2;
                            break;
                        }
                        else if(conti==6){
                            // printf("contiiii brk brk brk\n");
                            brk=1;
                            sleep(1);
                            // // send loop continue signal
                            strcpy(read_buffer, "-10"); // type 1
                            if (send(acpt, read_buffer, strlen(read_buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", read_buffer);
                            // printf("send CONTINUE sig \n============================================\n");
                            // printf("=============================== client connection closed ===============================\n\n");
                            close(acpt);
                            pthread_exit(NULL);
                            break;
                        }
                        else if(conti==7){
                            // printf("Returning to login menu...\n");
                            printf("----------------------------- Logged Out\n");
                            
                            char buffer[500];

                            sleep(1);
                            // // send loop continue signal
                            strcpy(buffer, "10"); // type 1
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send CONTINUE sig \n============================================\n");

                            // recv signal
                            if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
                                printf("Error\n");
                            }
                            // printf("%s\n", buffer);
                            // printf("recvd of ready sig \n============================================\n");
                            // getchar();

                            // send signal
                            strcpy(buffer, "4"); // type 4
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send of TYPE sig \n============================================\n");

                            sleep(1);
                            // // send loop continue signal
                            strcpy(buffer, "10"); // type 1
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send CONTINUE sig \n============================================\n");
                            t1 = 2;
                            brk=1;
                            break; 
                        }
                    }
                    break;
                }
                case 4:
                {   
                    printf("===========================In Customer handler\n");
                    while(1){
                        conti = customer_handler(acpt, login_success_user_id);
                        // printf("contiiii============== %d\nPress Enter : ",conti);
                        // getchar();
                        if(conti==1){
                            continue;
                        }
                        else if(conti==0){
                            // printf("----made t1 = 288888888888888888888888888\n");
                            t1=2;
                            logout_customer(login_success_user_id);
                            break;
                        }
                        else if(conti==6){
                            // printf("contiiii brk brk brk\n");
                            brk=1;
                            sleep(1);
                            // // send loop continue signal
                            strcpy(read_buffer, "-10"); // type 1
                            if (send(acpt, read_buffer, strlen(read_buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", read_buffer);
                            // printf("send CONTINUE sig \n============================================\n");
                            // printf("=============================== client connection closed ===============================\n\n");
                            close(acpt);
                            pthread_exit(NULL);
                            break;
                        }
                        else if(conti==7){
                            // printf("Returning to login menu...\n");
                            printf("----------------------------- Logged Out\n");
                            
                            char buffer[500];

                            sleep(1);
                            // // send loop continue signal
                            strcpy(buffer, "10"); // type 1
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send CONTINUE sig \n============================================\n");

                            // recv signal
                            if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
                                printf("Error\n");
                            }
                            // printf("%s\n", buffer);
                            // printf("recvd of ready sig \n============================================\n");
                            // getchar();

                            // send signal
                            strcpy(buffer, "4"); // type 4
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send of TYPE sig \n============================================\n");

                            sleep(1);
                            // // send loop continue signal
                            strcpy(buffer, "10"); // type 1
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            // printf("%s\n", buffer);
                            // printf("send CONTINUE sig \n============================================\n");
                            t1 = 2;
                            brk=1;
                            break; 
                        }
                    }
                    // customer_handler(acpt, login_success_user_id);
                    break;
                }
            }
            if(brk==1) {
                char buffer[500];
                // printf("----=-------------=---> inside break\n");
                strcpy(buffer, "10"); // type 1
                if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                    perror("Error sending login data");
                }
                // printf("%s\n", buffer);
                // printf("send CONTINUE sig \n============================================\n");
                            
                break;
            }
        }
        c++;
        // printf("=--------=-----> %d\n", c);
    }
    printf("=============================== client connection closed ===============================\n\n");
    close(acpt);
    pthread_exit(NULL);
    exit(0);
}

int main(int argc, char const *argv[])
{
     // Set up SIGINT handler
    // signal(SIGINT, ignoreSIGINT);
    // signal(SIGTERM, ignoreSIGINT);
    socklen_t l;

    // create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("");
        return 0;
    }
    printf("============================ CRAETED SERVER SOCKET ============================");

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // bind that socket to port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int bnd = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bnd == -1)
    {
        perror("");
        return 0;
    }

    // listen
    int lsn = listen(server_socket, LISTEN_BACKLOG);
    if (lsn == -1)
    {
        perror("");
        return 0;
    }


    while (keep_running)
    {
        
        if(!keep_running) break;

        // accept connection requests
        struct sockaddr_in client_addr;
        l = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &l);
        if (client_socket == -1)
        {
            perror("");
            return 0;
        }

        if(!keep_running) break;
        // new thread for each client
        pthread_t thread_id;
        int pthrd = pthread_create(&thread_id, NULL, (void *)handleClient, (void *)&client_socket);
        if (pthrd == -1)
        {
            perror("");
            return 0;
        }
        printf("-########################testing\n");
    }
    

    // close
    int cls = close(server_socket);
    if (cls == -1)
    {
        perror("");
        return 0;
    }
    printf("======================= CLOSED SERVER SOCKET ============================\n");

    return 0;
}
