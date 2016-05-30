#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

const int MAXCONNECTION=5;
const int MAXRECEIVE = 500;

#include <string>

class SocketException
{
    public:
        SocketException ( std::string description ) : m_description( description ) {};
        ~SocketException (){};

        std::string Description() { return m_description; }

 private:
        std::string m_description;
};

class CSocket
{
public:
	CSocket();
	virtual ~CSocket();
public:
	bool Create();
	bool Bind(unsigned short port);
	bool Listen() const;
	bool OnAccept(CSocket& clientSocket) const;

	bool Connect(const char * ip, unsigned short port);
	bool Send(CSocket& socket, char * buf, unsigned int len);
	int OnRecv(CSocket& socket, char * buf, unsigned int len)

	void SetNonBlocking(const bool flag);
	bool IsValid() const;

private:
	int m_sockfd;
	struct sockaddr_in m_address;
};

#endif