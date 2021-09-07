#include"nethead.h"
int IPv4Sock_Listening(int argc, char* argv[],int pdic_para){
 if( argc <= pdic_para)
	{
		printf("usage: %s  ip_address port_number backlong\n",basename(argv[0]));
	return 1;
	}
	const char* ip = argv[1];
	int port = atoi( argv[2] );
	
	int sock = socket( PF_INET, SOCK_STREAM, 0 );
	assert( sock >= 0 );
	//create a IPv4 socket
	struct sockaddr_in address;
	bzero( &address, sizeof( address ));
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &address.sin_addr );
	address.sin_port = htons( port );

	int ret = bind(sock, ( struct sockaddr* )&address, sizeof(address));
	if(ret == -1){
        printf("bind error: errno:%d ",errno);
        exit(0);
    }
	ret = listen(sock, 5);
	assert(ret != -1);
    return sock;
}
