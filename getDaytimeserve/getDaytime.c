#include "../nethead.h"

int main(int argc, char* argv[]){
    printf("%d\n",argc);
    assert(argc==2);
    char* host = argv[1];
    //获得目标主机地址
    struct hostent* hostinfo = gethostbyname(host);
    assert(hostinfo);
    //获取daytime服务信息
    struct servent* servinfo = getservbyname("daytime","tcp");
    assert(servinfo);
    printf("daytime prot is %d\n", ntohs(servinfo->s_port));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr = *((struct in_addr* )(*(hostinfo->h_addr_list)));
    for( int i = 0 ; i < sizeof(*hostinfo->h_addr_list); i++){
      printf("%c",(*hostinfo->h_addr_list)[i]);  
    }printf("\n");
    
    address.sin_port = servinfo->s_port;

    int sockfd = socket( AF_INET, SOCK_STREAM, 0);
    int result = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
    assert(result != -1);

    char buffer[128];
    result = read(sockfd, buffer,sizeof(buffer));
    assert(result > 0);
    buffer[result]='\0';
    printf("the day time is : %s",buffer);
    close(sockfd);
    return 0;
}