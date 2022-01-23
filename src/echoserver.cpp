/*
* echoserver.cpp - A Multi-threaded TCP Echo Server 
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#define BUFSIZE 1024
char buf[BUFSIZE + 1]; /* message buffer */

void *handle_client(void *arg);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
void string_return(int m_sockfd)
{

    int n;
    char msg[2048] = "Hi Client You connected to server";
    char buf[BUFSIZE];
    bzero(&buf, BUFSIZE);
again:
    while (n = recv(m_sockfd, buf, BUFSIZE, 0) > 0)
    {
        //   send(m_sockfd, &msg, strlen(msg), 0);
        send(m_sockfd, buf, sizeof(buf), 0);
        bzero(&buf, sizeof(buf));
    }
    bzero(&buf, sizeof(buf));
    // we do this to make sure there is a '\0' at the end, all the times.

    if (n < 0 && errno == EINTR) // Interrupt occured
    {
        if (errno == EINTR)
            goto again;
        else
            error("Echo error : ");
    }
}

void *handle_client(void *arg)
{
    int child_fd = *(int *)arg;
    delete (int *)arg;
    pthread_detach(pthread_self());

    string_return(child_fd);

    close(child_fd);

    return NULL;
}

int main()
{

    int parentfd;
    int *clientfd;
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    struct hostent *hostp;
    char *hostaddrp;
    int n;
    int optval;
    int portno;
    int clientlen;
    pthread_t thread;

    portno = htons(8787);
    //create socket connection
    parentfd = socket(AF_INET, SOCK_STREAM, 0);
    if (parentfd < 0)
    {
        error("ERROR OPENING SOCKET");
    }
    optval = 1;
    //Eliminates "ERROR on binding: Address already in use" error
    setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
    //build server internet addrs
    bzero((char *)&serveraddr, sizeof(serveraddr));

    //Internet addrss
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8787);

    //bind associated socket with port
    if (bind(parentfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        error("ERROR ON BINDING");
    }
    //listenining
    if (listen(parentfd, 0) < 0)
        error("ERROR ON LISTEINING");

    clientlen = sizeof(clientaddr);
    while (true)
    {

        clientfd = new int;
        puts("Waiting for Connections....\n");

        *clientfd = accept(parentfd, (struct sockaddr *)&clientaddr, (socklen_t *)&clientlen);
        if (clientfd < 0)
        {
            if (errno != EINTR)
                error("ERROR on accept");
            else
                continue;
        }
        puts("Connected\n");
        pthread_create(&thread, NULL, &handle_client, clientfd);
    }
}