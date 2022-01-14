#ifndef __TCP_Server_H_
#define __TCP_Server_H_
#include <sys/socket.h>
#include <iostream>

namespace TCP
{
    using namespace std;
    class TCPServer
    {
    private:
        int sock_fd, new_sock, c;
        char *message, server_reply[2048];
        int *new_sockp;
     //   struct sockaddr_in server, client;

    public:
        int createSocket();
        void setSockAddrIn(struct sockaddr_in server);
        void binding(int sock_fd, struct sockaddr_in server);
        void listenForConnection(int sock_fd, int n);
        void createConnection(int sock_fd, void *connection_handler(void *));
        void *connection_handler(void *);
    };

} //namespace TCP

#endif