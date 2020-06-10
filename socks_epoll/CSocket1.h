#include <iostream>
#include<sys/socket.h>
#include<sys/un.h>
#include<netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <netinet/tcp.h>
 
class CServSocket
{
public:
    CServSocket();
    virtual ~CServSocket();
	std::string  getLocalAddr();
	std::string getCliAddr();
	int getAcceptSock();
	bool Accept();
    bool Listen(int port);
	bool Send(char* sendbuf, int len);
	bool Recv(char* recvbuf, int len, int timeout);
private:
	bool Close();
	bool setSoLinger(bool dolinger, int seconds);
	bool setIntOptions(int option, int value);
	bool setTimeout(int option, int milliseconds);
	bool setNonBlock(bool isnonsocket);
	bool setNoDelay(bool nodelay);
private:
    char ip[20];
	int port;
	int sockfd;
	int connsockfd;
	struct sockaddr_in servAddr, cliAddr;
};
 
CServSocket::CServSocket():port(0),sockfd(-1),connsockfd(-1)
{
	memset(ip, 0, 20);
}
 
CServSocket::~CServSocket()
{
	if(!Close())
	{
		std::cout << "close err\n";
	}
}
 
bool CServSocket::Close()
{
	if(sockfd)
		close(sockfd);
	else
		return false;
	if(connsockfd)
	    close(connsockfd);
	else
		return false;
	sockfd = connsockfd = port = -1;
	return true;
}
 
bool CServSocket::Listen(int port)
{
	if(port < 0)
	{
		std::cout <<"the port err\n";
		return false;
	}
	sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		std::cout << "create socket err\n";
		return false;
	}
	getLocalAddr();
	servAddr.sin_family = AF_INET;
	if(ip)
	    servAddr.sin_addr.s_addr = inet_addr(ip);
	else
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);
	setSoLinger(false, 0);
	setIntOptions(SO_REUSEADDR,1);
	setIntOptions(SO_KEEPALIVE, 1);
	setIntOptions(SO_SNDBUF, 640000);
	setIntOptions(SO_RCVBUF, 640000);
	setNoDelay(true);
	if(::bind(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
	{
		std::cout << "bind err\n";
		return false;
	}
	if(::listen(sockfd, 0) < 0)
	{
		std::cout << "listen err\n";
		return false;		
	}
	return true;
}
 
bool CServSocket::Accept()
{
	socklen_t len = sizeof(cliAddr);
	if((connsockfd = ::accept(sockfd, (struct sockaddr*)&cliAddr, &len)) <0)
	{
		return false;
	}
	return true;
}
 
std::string  CServSocket::getLocalAddr()
{
	char ipaddr[20]={'\0'};
	const char* shellstr = "ifconfig | sed -n '2p' | awk -F'[ :]+' '{printf $4}'";  
    FILE *fp = popen(shellstr, "r");
	fread(ipaddr, sizeof(char), sizeof(ipaddr), fp);
	if(ipaddr)
	{
		strcpy(ip, ipaddr);
	}
	pclose(fp);
	return std::string(ip);
}
 
std::string CServSocket::getCliAddr()
{
	char cliip[16];
	socklen_t size = sizeof(cliAddr);
	if(getpeername(sockfd, (sockaddr*)&cliAddr, &size))
	{
		strcpy(cliip, "0.0.0.0");
	}
	else
	{
        sprintf(cliip, "%d.%d.%d.%d", ((unsigned char*)&cliAddr.sin_addr)[0],
				((unsigned char*)&cliAddr.sin_addr)[1],
				((unsigned char*)&cliAddr.sin_addr)[2],
				((unsigned char*)&cliAddr.sin_addr)[3]);
	}
	return cliip;
}
 
int CServSocket::getAcceptSock()
{
	return connsockfd;
}
bool CServSocket::setIntOptions(int option, int value)
{
	bool res = false;
	if(sockfd)
	{
		res = (setsockopt(sockfd, SOL_SOCKET, option, (const void*)&value, sizeof(value)) == 0);
	}
	return res;
}
 
bool CServSocket::setSoLinger(bool dolinger, int seconds)
{
	bool res = false;
	if(sockfd)
	{
		struct linger ling;
		ling.l_onoff = dolinger?1:0;
		ling.l_linger = seconds;
		res = (setsockopt(sockfd,SOL_SOCKET, SO_LINGER, (const char *)&ling, sizeof(struct linger)) == 0);
		res = true;		
	}
	return res;
}
 
bool CServSocket::setTimeout(int option, int milliseconds)
{
	bool res = false;
	if(sockfd)
	{
		struct timeval timeout;
		timeout.tv_sec = milliseconds/1000;
		timeout.tv_usec = (milliseconds%1000)*1000000;
		res = (setsockopt(sockfd, SOL_SOCKET, option, (const void*)&timeout, sizeof(timeout)) == 0);
		res = true;		
	}
	return res;
}
 
bool CServSocket::setNonBlock(bool isnonsocket)
{
	bool res = false;
	if(sockfd)
	{
		int oldfd = fcntl(sockfd, F_GETFL);
		res = (fcntl(sockfd, F_SETFL, oldfd | O_NONBLOCK) <0);
		res = true;		
	}
	return res;
}
 
bool CServSocket::setNoDelay(bool nodelay)
{
	bool res = false;
	if(sockfd)
	{
		int ndelay = nodelay?1:0;
		res = (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY,(const void*)&ndelay, sizeof(ndelay)) == 0);
		res =true;		
	}
	return res;
}
 
bool CServSocket::Send(char *sendbuf, int len)
{
	if(sockfd <0 || sendbuf==NULL || len < 0)
		return false;
	int dataleft = len, total = 0, ret =0;
	for(;dataleft >0;)
	{
		ret = ::send(connsockfd, sendbuf+total, dataleft, 0);
		if(ret < 0)
		{
			if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
			{
				usleep(50000);
				ret = 0;
			}
		}
		total += ret;
		dataleft = len-total;
	}
	return total == len;
}
 
bool CServSocket::Recv(char *recvbuf, int len, int timeout)
{
	if(sockfd <0 || recvbuf==NULL || len < 0)
		return false;
	fd_set fds;
	struct timeval interval;
	interval.tv_sec = timeout;
	interval.tv_usec = 0;
	int recvlen = 0;
	for(;;)
	{
		FD_ZERO(&fds);
		FD_SET(connsockfd, &fds);
		int res = ::select(connsockfd+1, &fds, NULL, NULL, &interval);
		if(res == 0)
			continue;
		if(res < 0)
		{
			::close(connsockfd);
			connsockfd = -1;
			return false;
		}			
		else
		{
			if(FD_ISSET(connsockfd, &fds))
			{
				recvlen = ::recv(connsockfd, recvbuf, len, 0);
				break;
			}
		}
	}
	return recvlen == len;
}
 
class CCliSocket
{
public:
    CCliSocket();
    virtual ~CCliSocket();
	bool Connect(const char* ip, int port);
	bool Send(char* sendbuf, int len);
	bool Recv(char* recvbuf, int len);
private:
	bool Close();
private:
	int clisockfd;	
};
 
CCliSocket::CCliSocket()
{
	if((clisockfd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "client socket err\n";
	}
}
 
CCliSocket::~CCliSocket()
{
	if(!Close())
	{
		std::cout << "close err\n";
	}
}
 
bool CCliSocket::Close()
{
	if(clisockfd < 0)
		return false;
	else
	{
		::close(clisockfd);
		clisockfd = -1;
	}
		
	return true;
}
 
bool CCliSocket::Connect(const char* ip, int port)
{
	if(ip==NULL || ip=="" || port < 0)
		return false;
	struct sockaddr_in sddr;
	sddr.sin_family = AF_INET;
	sddr.sin_addr.s_addr = inet_addr(ip);
	sddr.sin_port = htons(port);
	if(::connect(clisockfd, (struct sockaddr*)&sddr, sizeof(sddr)) < 0)
	{
		std::cout << "connnect to server err\n";
		return false;
	}
	return true;
}
 
bool CCliSocket::Send(char* sendbuf, int len)
{
	int ret = 0;
	do
	{
		ret = ::send(clisockfd, sendbuf, len, 0);
		
	} while (ret < 0 && (errno == EAGAIN || errno == EINTR));
    return ret == len; 
}
 
bool CCliSocket::Recv(char* recvbuf, int len)
{
	int ret = 0;
	do
	{
		ret = ::recv(clisockfd, recvbuf, len, 0);
	} while (ret < 0 && (errno == EAGAIN || errno == EINTR));
    return ret == len; 
}