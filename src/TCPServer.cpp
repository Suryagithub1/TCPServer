#include "TCPServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>


using namespace TCP;

int TCP::TCPServer::createSocket()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        puts("Failed to create Socket\n");
        exit(1);
    }
    puts("Socket Created\n");
    return sock_fd;
};
void TCP::TCPServer::setSockAddrIn(struct sockaddr_in server)
{
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(9999);
    server.sin_family = AF_INET;
};
void TCP::TCPServer::binding(int sock_fd, struct sockaddr_in server)
{
    if (bind(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("Bind failed\n");
        exit(1);
    }
    puts("Bind successful\n");
}
void TCP::TCPServer::listenForConnection(int sock_fd, int n)
{
    // listen(sock_fd, 100);
    if (listen(sock_fd, 100) == 0)
    {
        puts("listening...........");
    }
    puts("Not Listening..");
}
void TCP::TCPServer::createConnection(int sock_fd, void *connection_handler(void *))
{
    int c;
    struct sockaddr_in client;
    c = sizeof(struct sockaddr_in);
    //Establish connection with client
    while ((new_sock = accept(sock_fd, (struct sockaddr *)&client, (socklen_t *)&c)) > 0)
    {
        puts("Connection accepted\n");
        //Memory Allocation
        int *new_sockp = NULL;
        new_sockp = (int *)malloc(sizeof(int));
        *new_sockp = new_sock;
        //Thread based communication
        pthread_t process_con;
        if (pthread_create(&process_con, NULL, connection_handler, (void *)new_sockp) < 0)
        {
            puts("Thread creation failed\n");
        }
        puts("Connection Handler Assigned\n");
        pthread_join(process_con, NULL);
    }
}
