#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

void *connection_handler(void *);
int main(){
int sock_fd,new_sock,c;
struct sockaddr_in server,client;
char *message, server_reply[2048];
int *new_sockp;
//Socket creation
sock_fd=socket(AF_INET,SOCK_STREAM,0);
if(sock_fd==-1){
    puts("Socket creation failed\n");
    exit(1);
}
puts("Socket created Successfully");
//Server address and port setting IP_V4
server.sin_addr.s_addr=INADDR_ANY;
server.sin_port = htons(9999);
server.sin_family=AF_INET;

//Binding socket and server
if(bind(sock_fd, (struct sockaddr*)&server, sizeof(server)) < 0 ){
    puts("Bind failed\n");
    exit(1);
}
puts("Bind successful\n");
listen(sock_fd,100);

//Waiting for connection 
c=sizeof(struct sockaddr_in);
while((new_sock=accept(sock_fd, (struct sockaddr *)&client, (socklen_t *)&c)) > 0){
puts("Connection accepted\n");

//Memory Allocation thread creation
new_sockp=(int *)malloc(sizeof(int));
*new_sockp=new_sock;
pthread_t process_con;
if(pthread_create(&process_con, NULL , connection_handler,(void *)new_sockp) < 0){
    puts("Thread creation failed\n");
    return -1;
}
puts("Connection Handler Assigned\n");
pthread_join(process_con,NULL);
}
return 0;
}


void *connection_handler(void *socket_desc){
int sock=*(int *)socket_desc,read_size;
char client_msg[2048];
char msg[2048]="Hi client connection handler assigned\n";
write (sock,msg,strlen(msg));
while((read_size==recv(sock, client_msg ,2048 ,0)) > 0)
    write (sock, client_msg, strlen(client_msg));
    if(read_size==0){
        puts("Client disconnected\n");
        fflush(stdout);
    }
    else if(read_size==-1){
        puts("reconnection failed\n");
    }
    free(socket_desc);

}