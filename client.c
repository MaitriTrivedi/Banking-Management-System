#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <string.h>
#include "structures.h"
#include "password_methods.h"
#include "login_logout.h"
#include "create_new_user.c"
#include "manager.h"
#include "customer.h"
#include "employee.h"

void main(int argc, char *argv[])
{
    // create socket
    int sct = socket(AF_INET, SOCK_STREAM, 0);
    if(sct==-1){
        perror("");
        return;
    }
    // printf("======================= CREATED SOCKET ============================\n");

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
    while(1){
        printf("====================================================================\n");
        printf("===== WELCOME TO XYZ BANK =====\n");
        printf("====================================================================\n");
        printf("\nIN WHICH MODE YOU WANT TO LOGIN ?\n");
        printf("SELECT AN OPTION FROM THE FOLLOWING :\n1. Admin\n2. Manager\n3. Employee\n4. Customer\n");
        int choice;
        printf("==========================================\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        struct User u;
        printf("Enter Name : ");
        scanf("%s", u.username);

        char password[50];
        printf("Enter Password : ");
        scanf("%s",password);

        hashPassword(password, u.password);

        int login_success = login(u, choice);
        if(login_success)
            printf("Login Successful !\n");
        else {
            printf("Login Unsuccessful !\n");
        }
        int t1=1;
        while(t1!=2){
            switch (choice){
                case 1:
                {
                    admin_handler();
                    break;
                }
                case 2:
                {
                    manager_handler();
                    break;
                }
                case 3:
                {
                    employee_handler();
                    break;
                }
                case 4:
                {
                    customer_handler();
                    break;
                }
            }
            printf("Do you want to continue or logout?\n1.Continue\n2.Logout\n");
            scanf("%d",&t1);
            printf("==========================================\n");
        }

        // if(t1==2){
        //     logout();
        // }
    }

    // close
    int cls = close(sct);
    if(cls == -1){
        perror("");
        return;
    }
    return;
}
