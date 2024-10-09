#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>

#define PORT 8060
#define LISTEN_BACKLOG 50
#define MAX_CONCURRENT_CONNECTIONS 5

static volatile int keep_running = 1;
int server_socket;

void signalHandler(int signal_num){
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
    /*
    This is called after creating new thread for each new client when a new client sends the request to handle each client concurrently.
    */
    printf("\n=============================== a new client connected ===============================\n");
    int acpt = *(int *)client_socket;
    char st[20];
    // printf("%d\n", acpt);
    int rd = read(acpt, &st, sizeof(st));
    if (rd == -1)
    {
        perror("");
        return 0;
    }

    int wr = write(acpt, &st, sizeof(st));
    if (wr == -1)
    {
        perror("");
        return 0;
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
    
    signal(SIGINT, signalHandler);

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
