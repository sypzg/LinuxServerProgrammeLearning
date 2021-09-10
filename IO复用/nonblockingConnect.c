/*非阻塞connect：通过select监听connect的可写事件，提取状态码SO_ERROR判断connect是否成功*/
#include "../nethead.h"
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>

#define BUFFER_SIZE 1023

int setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}
//超时连接函数，参数分别是服务器IP地址，端口号和超时时间（毫秒）。函数成功时返回已处于连接状态的socket，失败返回-1
int unblock_connect( const char* ip, int port, int time )
{
    int ret = 0;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = atoi( port );
    inet_pton( AF_INET, ip, address.sin_addr);

    int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    assert( sockfd != -1 );
    int fdopt = setnonblocking( sockfd )；
    ret = connect( sockfd, (struct sockaddr*)&address, sizeof(address) );
    if( ret == 0)
    {
        //如果连接成功，则恢复sockfd属性，并立即返回
        printf("connect with server imediately\n");
        fcntl( sockfd, F_SETFL, fdopt);
        return sockfd;
    }
    else if( errno != EINPROGRESS )
    {
        //如果连接没有立即建立，那么只有errno是EINPROGRESS时表示连接还在进行，否则出错返回
        printf("unblock connect not support\n");
        return -1;
    }
    
    fd_set readfds;
    fd_set writefds;
    struct timeval timeout;

    FD_ZERO( &writefds );
    FD_SET( sockfd, &writefds );
    timeout.tv_sec = time;
    timeout.tv_usec = 0;

    ret = select( sockfd + 1, NULL, &writefds, NULL, &timeout );
    if( ret <= 0 )
    {
        /*select超时或则出错，返回*/
        printf("connection time out\n");
        close( sockfd );
        return -1;
    }

    if( !FD_ISSET( sockfd, &writefds ) )
    {
        printf("no events on sokfd found\n");
        close( sockfd );
        return -1;
    }

    int error = 0;
    socklen_t lenght = sizeof( error );
    //调用getsockopt来获得并清除sockfd上的错误
    if( getsockopt( sockfd, SOL_SOCKET, SO_ERROR, &error, &lenght) < 0 )
    {
        printf("get socket option failed\n");
        close( sockfd );
        return -1;
    }
    //错误号不为0表示连续出错
    if( error != 0 )
    {
        printf("connection faile after select with the error:%d \n");
        close( sockfd );
        return -1;
    }
    printf("connection ready after select with the socket:%d\n", sockfd);
    fcntl( sockfd, F_SETFL, fdopt );
    return sockfd;
}
int main( int argc, char* argv[] )
{
    if( argc <= 2)
    {
        printf("usage:%s ip_address port_number\n",basename( argv[0] ));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    
    int sockfd = unblock_connect( ip, prot, 10 );
    if( sockfd < 0)
    {
        return 1;
    }
    close( sockfd );
    return 0;
}



















