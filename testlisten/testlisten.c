#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h>
#include<assert.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

static int  stop = 0;

//signal handler
static void handle_term(int sig)
{
	stop = 1;
}

int main( int argc, char* argv[] )
{
	signal( SIGTERM, handle_term );

	if( argc <= 3)
	{
		printf("usage: %s  ip_address port_number backlong\n",basename(argv[0]));
	return 1;
	}
	const char* ip = argv[1];
	int port = atoi( argv[2] );
	int backlog = atoi( argv[3] );
	
	int sock = socket( PF_INET, SOCK_STREAM, 0 );
	assert( sock >= 0 );
	//create a IPv4 socket
	struct sockaddr_in address;
	bzero( &address, sizeof( address ));
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &address.sin_addr );
	address.sin_port = htons( port );

	int ret = bind(sock, ( struct sockaddr* )&address, sizeof(address));
	assert( ret != -1 );

	ret = listen( sock, backlog );
	assert( ret != -1 );
	
	int cnt = 1;
	while(!stop)
	{
	printf("sleep %d-th.",cnt++);
	sleep(1);
	}
	close(sock);
	return 0;
}
