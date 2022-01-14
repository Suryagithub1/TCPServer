#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "TCPServer.h"

using namespace TCP;
void *connection_handler(void *);
int main()
{
    int sock_fd, new_sock, c;
    struct sockaddr_in server, client;
    char *message, server_reply[2048];
    int *new_sockp;
    //create socket
    TCPServer *s = NULL;
    sock_fd = s->TCP::TCPServer::createSocket();
    //set server properties
    s->setSockAddrIn(server);
    //bind
    s->binding(sock_fd, server);
    //listening
    s->TCP::TCPServer::listenForConnection(sock_fd, 100);
    //Establish connection
    s->createConnection(sock_fd, connection_handler);
}

void *connection_handler(void *socket_desc)
{
    int sock = *(int *)socket_desc, read_size;
    char client_msg[2048];
    char msg[2048] = "Hi client connection handler assigned\n";
    write(sock, msg, strlen(msg));
    while ((read_size == recv(sock, client_msg, 2048, 0)) > 0)
        write(sock, client_msg, strlen(client_msg));

    if (read_size == 0)
    {
        puts("Client disconnected\n");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        puts("reconnection failed\n");
    }
    free(socket_desc);
}