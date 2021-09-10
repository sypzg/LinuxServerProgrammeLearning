#include "nethead.h"
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <time.h>
#define _GNU_SOURCE 1
#define  BUFFER_SIZE 64
#define HEAD_MESSAGE_SIZE 50
void write_pipe(int pipe[],char c[], int size)
{
    int rc;
    for( int i = 0; i < size; i++){
        if(c[i]=='\0')break;
        rc = write( pipe[1], &c[i], 1);//写入管道
        assert( rc != -1 );
    }
}
int main( int argc, char* argv[] )
{
    if( argc <= 3 )
    {
        printf("usage:%s ip_address prot_number, user_name\n",basename( argv[0] ));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );


    struct sockaddr_in server_address;
    bzero( &server_address, sizeof(server_address) );
    server_address.sin_family = AF_INET;
    inet_pton( AF_INET , ip, &server_address.sin_addr );
    server_address.sin_port = htons( port );

    int ret = 0;
    int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    assert( sockfd >= 0 );
    ret = connect( sockfd , (struct sockaddr*)&server_address, sizeof(server_address) );
    if( ret == -1 )
    {
        printf("connection failed\n");
        close( sockfd );
        return 1;
    }
    printf("connect success\n");
    pollfd fds[2];
    //注册文件描述符0（标准输入）和文件描述符sockfd上的可读事件
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    fds[1].fd = sockfd;
    fds[1].events = POLLIN | POLLRDHUP;
    fds[1].revents = 0;

    char read_buf[BUFFER_SIZE];
    int pipefd[2];
    ret = pipe(pipefd);

    while( 1 )
    {
        ret = poll( fds, 2, -1 );
        if( ret < 0 )
        {
            printf("poll failure\n");
            break;
        }
        
        if( fds[1].revents &  POLLRDHUP)
        {
            printf("server close the connection\n");
            break;
        }
        else if( fds[1].revents & POLLIN )
        {
            memset( read_buf, '\0', BUFFER_SIZE );
            recv( fds[1].fd, read_buf, BUFFER_SIZE-1, 0 );
            printf( "%s\n", read_buf );
        }

        if( fds[0].revents & POLLIN )
        {
            time_t timep;
            time(&timep);
            char str[HEAD_MESSAGE_SIZE];
            memset(str, '\0', HEAD_MESSAGE_SIZE);
            sprintf(str, "%s\t\t\t%s\n", argv[3], asctime(gmtime(&timep)));   
            write_pipe(pipefd, str, HEAD_MESSAGE_SIZE);
            //使用splice将用户输入的数据直接写到socket上（零拷贝）
            ret = splice( 0, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE );
            ret = splice( pipefd[0], NULL, sockfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE );
        }
    }
    close( sockfd );
    return 0;
}