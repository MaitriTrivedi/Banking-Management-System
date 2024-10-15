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