#include <iostream>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define LISTENQ 20

using namespace std;

int main()
{
    std::cout << "Server Begin..." << std::endl;

    ////////
    socklen_t _sockLen;

    struct epoll_event ev;
    struct epoll_event  events[20];	

    int _epfd = epoll_create(256);

    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;

    int _listen = socket( AF_INET, SOCK_STREAM, 0);

    ev.data.fd = _listen;
    ev.events = EPOLLIN|EPOLLET;

    epoll_ctl(_epfd, EPOLL_CTL_ADD, _listen, &ev);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    const char* _local_IP = "127.0.0.1";
    inet_aton(_local_IP, &(server_addr.sin_addr));

    server_addr.sin_port = htons(4321);

    bind(_listen, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(_listen, LISTENQ);

    ////////
    //epoll work
    //
    int _connfd = 0;   

    for(;;)
    {
	////////
	int _nfds = epoll_wait(_epfd, events, 20, 500);

        for( int i=0; i<_nfds; i++ )
	{
	    if( events[i].data.fd = _listen )
	    {
		_connfd = accept(_listen, (sockaddr*)&client_addr, &_sockLen);

    		if( _connfd < 0 )
		{
		    perror("error: connfd < 0");
		    //exit(1);
		}

		char* str = inet_ntoa(client_addr.sin_addr);

    		std::cout << "accept a connection from" << str << std::endl;

		ev.data.fd = _connfd;
  	  	ev.events = EPOLLIN|EPOLLET;

		epoll_ctl(_epfd, EPOLL_CTL_ADD, _connfd, &ev);

	    }		

	}

    
    }
    


    return 0;
}



