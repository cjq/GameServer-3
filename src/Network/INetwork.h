
#ifndef INETWORK_H
#define INETWORK_H 

namespace network
{

	// 定义定时器回调接口
	class ITimerCallback
	{
	public:
		ITimerCallback() {}
		virtual ~ITimerCallback() {}

		virtual void OnTimer(unsigned long long timerID) = 0;
	};


	// IO处理器回调
	class IIOServerCallback
	{
	public:
		IIOServerCallback() {}
		virtual ~IIOServerCallback() {}

		virtual bool OnPost(void *pUser) = 0;
	};

		//! IO处理器
		class IIOServer
		{
		public:
			IIOServer(){}
			virtual ~IIOServer() {}
			//! some initialization for runing.
			virtual bool Initialize(IIOServerCallback *cb) = 0;
			//! user block call.
			virtual void RunOnce() = 0;
			//! provide Thread's message queue.
			virtual void Post(void *pUser) = 0;
			//! create timer
			virtual unsigned long long CreateTimer(unsigned int delayms, ITimerCallback * cb) = 0;
			virtual bool CancelTimer(unsigned long long timerID) = 0;
		};


	class ITcpSocketCallback
	{
	public:
		ITcpSocketCallback(){}
		virtual ~ITcpSocketCallback(){}
		virtual bool OnConnect(bool bConnected) = 0;
		virtual bool OnRecv(unsigned int nRecvedLen) = 0;
		virtual bool OnSend(unsigned int nSentLen) = 0;
		virtual bool OnClose() = 0;
	};

	class ITcpSocket
	{
	public:
		ITcpSocket(){}
		virtual ~ITcpSocket(){}
		virtual bool Initialize(IIOServer * ios, ITcpSocketCallback * cb) = 0;
		virtual bool DoConnect(const char *ip, unsigned short port) = 0;
		virtual bool DoSend(char * buf, unsigned int len) = 0;
		virtual bool DoRecv(char * buf, unsigned int len) =0;
		virtual bool GetPeerInfo(unsigned int * addr, unsigned short *port)=0;
		virtual bool Close() = 0;
	};

	class ITcpAcceptCallback
	{
	public:
		ITcpAcceptCallback(){}
		virtual ~ITcpAcceptCallback(){}
		virtual bool OnAccept(ITcpSocket * s) = 0;
		virtual bool OnClose() = 0;
	};

	class ITcpAccept
	{
	public:
		ITcpAccept(){}
		virtual ~ITcpAccept(){}
		virtual bool Initialize(IIOServer * ios, ITcpAcceptCallback * cb) = 0;
		virtual bool OpenAccept(const char * ip, unsigned short port) = 0;
	};

	class IUdpSocketCallback
	{
	public:
		IUdpSocketCallback(){}
		virtual ~IUdpSocketCallback(){}
		virtual bool OnRecvFrom(unsigned int nRecvlen, const char *ip, unsigned short port) = 0;
//			virtual bool OnSendTo(unsigned int nSendlen) = 0;
	};

	class IUdpSocket
	{
	public:
		IUdpSocket(){}
		virtual ~IUdpSocket(){}
		virtual bool Initialize(IIOServer * ios, IUdpSocketCallback * cb, const char *ip, unsigned short port = 0) = 0;
		virtual bool DoRecv(char * buf, unsigned int len) = 0;
		virtual bool DoSend(char * buf, unsigned int len, const char *dstip, unsigned short dstport) = 0;
	};

	IIOServer * CreateIOServer();
	void DestroyIOServer(IIOServer * ios);

	ITcpSocket * CreateTcpSocket();
	void DestroyTcpSocket(ITcpSocket * s);

	ITcpAccept * CreateTcpAccept();
	void DestroyTcpAccept(ITcpAccept *s);

	IUdpSocket * CreateUdpSocket();
	void DestroyUdpSocket(IUdpSocket * s);

};


#endif