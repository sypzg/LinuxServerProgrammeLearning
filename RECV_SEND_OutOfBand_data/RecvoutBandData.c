#include "../nethead.h"

#define BUF_SIZE  1024

int main(int argc, char* argv[]){
    int sock = IPv4Sock_Listening(argc,argv,2);
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept( sock, (struct sockaddr*)&client, &client_addrlength );
    if( connfd < 0){
        printf("errno is :%d",errno);
    }
    else{
        char buffer[BUF_SIZE];

        memset( buffer, '\0', BUF_SIZE );
    int ret = recv( connfd, buffer, BUF_SIZE-1, 0);
        printf("got %d bytes of normal data '%s'\n",ret,buffer);

        memset( buffer, '\0', BUF_SIZE );
        ret = recv( connfd, buffer, BUF_SIZE-1, MSG_OOB);
        printf("got %d bytes of normal data '%s'\n",ret,buffer);

        memset( buffer, '\0', BUF_SIZE );
        ret = recv( connfd, buffer, BUF_SIZE-1, 0);
        printf("got %d bytes of normal data '%s'\n",ret,buffer);

        close(connfd);
    }
    close(sock);
    return 0;
} 