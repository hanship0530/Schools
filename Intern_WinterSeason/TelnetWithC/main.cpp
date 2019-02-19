//============================================================================
// Name        : StockIndex.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include<stdio.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

#define RESPONSE_RECV_LIMIT 3000
#define SOURCE_START_IDENTIFIER "<!DOCTYPE"
#define SOURCE_START_IDENTIFIER2 "<html>" 		//this is the name of the identifier that the
#define FILENAME "/"		 		// ENTER THE FILENAME HERE
#define PORT	"80"			 		// default for web-browsers

int main()
{
	char response[RESPONSE_RECV_LIMIT+1];  // + 1 is for null
	char *source;
	int sockfd,newfd,err;
	char ip[INET6_ADDRSTRLEN];
	struct addrinfo *p,hints,*res;
	int len,len_s;
	int yes=1;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	void *addr;
	char *ver;
	char request[100];

	sprintf(request,"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n",FILENAME,"computer.kangwon.ac.kr");

	// print the request we are making

	printf("%s\n\n",request);

	memset(&hints,0,sizeof hints);

	hints.ai_socktype=SOCK_STREAM;

	hints.ai_family=AF_UNSPEC;

	if ((err = getaddrinfo("computer.kangwon.ac.kr",PORT, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		return 1;
	}

	for(p=res;p!=NULL;p=p->ai_next)
	{
		if( ( sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol) ) == -1)
		{
			printf("Socket error !!!\n");
			return(0);
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			//close(sockfd);
			perror("client: connect");
			continue;
		}
	}

	if(send(sockfd,request,strlen(request),0) < strlen(request))
	{
		perror("Send Error!!\n");
	}

	freeaddrinfo(res);

	if( recv(sockfd,response,RESPONSE_RECV_LIMIT,0) == 0 )
	{
		perror("Recv : ");
		return(1);
	}

	//close(sockfd); // we dont need it any more

//	printf("%s",response); // for debugging purposes

	/*
    source = strstr(response,SOURCE_START_IDENTIFIER);

	if(source == NULL)
	{
		source = strstr(response,SOURCE_START_IDENTIFIER2);
	}
    */
    source = response ;
	printf("%s\n",source);
	return(0);
}
