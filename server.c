#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

#define LISTEN_BACKLOG 50

int main(int argc, char const *argv[])
{
    const struct sockaddr s;
    struct sockaddr c;
    socklen_t l;

    // create socket
    int sct = socket(AF_INET, SOCK_STREAM, 0);
    if(sct==-1){
        perror("");
        return 0;
    }
    printf("======================= CRAETED SOCKET ============================\n");

    // bind that socket to port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8060);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int bnd = bind(sct, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(bnd==-1){
        perror("");
        return 0;
    }

    // listen
    int lsn = listen(sct, LISTEN_BACKLOG);
    if(lsn==-1){
        perror("");
        return 0;
    }

    // accept connection requests
    struct sockaddr_in client_addr;
    l = sizeof(client_addr);
    int acpt = accept(sct, (struct sockaddr *)&client_addr, &l);
    if(acpt==-1){
        perror("");
        return 0;
    }

    // printf("Testing.............\n");
    char st[20];
    int rd = read(acpt, &st, sizeof(st));
    if(rd==-1){
        perror("");
        return 0;
    }
    printf("Client Message : %s\n", st);

    int wr = write(acpt, &st, sizeof(st));
    if(wr==-1){
        perror("");
        return 0;
    }

    // close
    int cls = close(sct);
    if(cls == -1){
        perror("");
        return 0;
    }

    return 0;
}
