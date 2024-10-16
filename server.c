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

static volatile int keep_running = 1;
int server_socket;

void serverSignalHandler(int signal_num){
    int cls = close(server_socket);
    if (cls == -1)
    {
        perror("");
        exit(1);
    }
    printf("======================= CLOSED SERVER SOCKET ============================\n");
    exit(1);
}


void *handleClient(void *client_socket)
{
    char read_buffer[500], write_buffer[500];
    size_t read_size, write_size;
    /*
    This is called after creating new thread for each new client when a new client sends the request to handle each client concurrently.
    */
    printf("\n=============================== a new client connected ===============================\n");
    int acpt = *(int *)client_socket;

    int choice;
    struct User u;
    char msg[500];
    int brk=0;
    int c=0;
    while(1){
        printf("=========== %d ==============\n", c);
        // Receive login data from client
        memset(read_buffer, '\0', sizeof(read_buffer));
        memset(write_buffer, '\0', sizeof(write_buffer));
        memset(msg, '\0', sizeof(msg));

        if (recv(acpt, &read_buffer, sizeof(read_buffer), 0) == -1) {
            perror("Error receiving login choice");
            close(acpt);
            pthread_exit(NULL);
        }

        strcpy(msg, "====================================================================\n===== WELCOME TO XYZ BANK =====\n====================================================================\nIN WHICH MODE YOU WANT TO LOGIN ?\nSELECT AN OPTION FROM THE FOLLOWING :\n1. Admin\n2. Manager\n3. Employee\n4. Customer\n==========================================\nEnter your choice : ");
        if (send(acpt, msg, strlen(msg)+1, 0) == -1) {
            perror("Error sending login data");
            // continue;
        }

        if (recv(acpt, &read_buffer, sizeof(read_buffer), 0) == -1) {
            perror("Error receiving login choice");
            close(acpt);
            pthread_exit(NULL);
        }
        printf("choice %s\n",read_buffer);
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
        printf("uname %s\n",u.username);
        strcpy(msg, "Enter Password : ");
        if (send(acpt, msg, strlen(msg)+1, 0) == -1) {
            perror("Error sending login data");
        }
        

        if (recv(acpt, &read_buffer, sizeof(read_buffer), 0) == -1) {
            perror("Error receiving login data");
            close(acpt);
            pthread_exit(NULL);
        }
        printf("pass %s\n",read_buffer);
        hashPassword(read_buffer, u.password);

        int login_success_user_id = login(u, choice);
        if(login_success_user_id==-1){
            strcpy(msg, "0");
            if (send(acpt, msg, strlen(msg)+1, 0) == -1) {
                perror("Error sending login data");
                // continue;
            }
            printf("Login Unsuccessful !\n");
            continue;
        }
        else {
            strcpy(msg, "1");
            // getchar();
            if (send(acpt, msg, strlen(msg)+1, 0) == -1) {
                perror("Sent login data");
            }
            // printf("senttt %s\n", msg);
            // getchar();
            // printf("============= Login Successful =============\n");
            // getchar();
            // printf("88888888888888888\n");
        }

        
        // handle further :
        int t1=1;
        while(t1!=2){
            switch (choice){
                case 1:
                {   
                    printf("In admin handler\n");
                    while(1){
                        int conti = admin_handler(acpt,login_success_user_id);
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
                            strcpy(read_buffer, "6"); // type 1
                            if (send(acpt, read_buffer, strlen(read_buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            printf("%s\n", read_buffer);
                            printf("send CONTINUE sig \n============================================\n");
                            printf("=============================== client connection closed ===============================\n\n");
                            close(acpt);
                            pthread_exit(NULL);
                            break;
                        }
                        else if(conti==7){
                            printf("Returning to login menu...\n");
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
                            strcpy(buffer, "4"); // type 4
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            printf("%s\n", buffer);
                            printf("send of TYPE sig \n============================================\n");

                            sleep(1);
                            // // send loop continue signal
                            strcpy(buffer, "10"); // type 1
                            if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
                                perror("Error sending login data");
                            }
                            printf("%s\n", buffer);
                            printf("send CONTINUE sig \n============================================\n");
                            t1 = 2;
                            brk=1;
                            break; 
                        }
        
                    }
                    break;
                }
                case 2:
                {
                    // manager_handler(login_success_user_id);
                    break;
                }
                case 3:
                {
                    employee_handler(login_success_user_id);
                    break;
                }
                case 4:
                {
                    customer_handler(login_success_user_id);
                    break;
                }
            }
            if(brk==1) {
                printf("----=-------------=---> inside break\n");
                break;
                }
        }
        c++;
        printf("=--------=-----> %d\n", c);
    }
    
    printf("=============================== client connection closed ===============================\n\n");
    close(acpt);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    socklen_t l;

    // create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("");
        return 0;
    }
    printf("======================= CRAETED SERVER SOCKET ============================\n");

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
    
    signal(SIGINT,serverSignalHandler);

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
