#ifndef _NETHEAD_H_
#define _NETHEAD_H_
#include<sys/socket.h>
#include<libgen.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<netdb.h>
extern int IPv4Sock_Listening(int argc, char* argv[],int pdic_para);

#endif