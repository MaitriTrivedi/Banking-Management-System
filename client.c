#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    // create socket
    int sct = socket(AF_INET, SOCK_STREAM, 0);
    if(sct==-1){
        perror("");
        return 0;
    }
    // printf("======================= CRAETED SOCKET ============================\n");

    // connect
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8060);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int cnt = connect(sct, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(cnt == -1){
        perror("");
        return 0;
    }
    // printf("========================= CONNECTION ESTABLISHED ==========================\n");

    char *st = "TESTING";
    if(write(sct, st, strlen(st)) == -1){
        perror("");
        return 0;
    }
    char st2[20];
    int rd = read(sct, &st2, sizeof(st2));
    if(rd==-1){
        perror("");
        return 0;
    }
    printf("Server Message : 'I got your message : \"%s\"'\n", st2);

    // close
    int cls = close(sct);
    if(cls == -1){
        perror("");
        return 0;
    }
    // printf("========================= CONNECTION CLOSED ==========================\n");
    getchar();
    return 0;
}
