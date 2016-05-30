
#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

/*
	需求：
		使用epoll创建服务器
		提供

	socket FD 类型
	server FD
	连接其它server FD
	其它server连接本服务FD
	client FD

 */

enum  EFDType
{
	FD_TYPE_ACCEPT 		= 0, // 
	FD_TYPE_SERVER 		= 1, // 
	FD_TYPE_MAILBOX 	= 2, // 
	FD_TYPE_ERROR		= 3, // 错误的类型
};

class ITimerCallback
{
public:
	ITimerCallback(){}
	virtual ~ITimerCallback(){}
	virtual void OnTimer(unsigned long long timerID) =0;
};


		

class EchoServer
{
public:
	EchoServer();
	~EchoServer();

public:
	virtual bool OnAccept(ITcpSocket * s);
	virtual bool OnClose(int fd);
public:
	// 与其它服务器通讯
	bool OnSvrReady(int serverID);
	bool OnSvrClose(int serverID);
	bool ConnectOtherSvr(const char * ip, unsigned short port);
	bool SendToOtherSvr(int serverID, char * buf, unsigned int len);
	virtual bool OnSvrAccept(ITcpSocket * s);
	virtual bool OnSvrClose(int fd);

	// 与客户端通讯
	virtual bool OnAccept(ITcpSocket * s);
	virtual bool OnClose(int fd);
	bool SendToClient(int fd, char * buf, unsigned int len);
	bool SendToGroup(std::vector<int> fdlist,char * buf, unsigned int len);
	bool SendToAll(char * buf, unsigned int len);
	bool CloseClient(fd);

public:	// 定时器
	virtual unsigned long long CreateTimer(unsigned int delayms, ITimerCallback * cb);
	virtual bool CancelTimer(unsigned long long timerID);

private:
	
	// 
	
	int m_epfd;
	std::map<int, TcpSocket*> map;
	bool m_bShutdown;
};

#endif