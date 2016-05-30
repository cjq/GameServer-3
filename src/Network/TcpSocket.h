
#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

class CTcpSocket : public ITcpSocket
{
public:
	CTcpSocket();
	virtual ~CTcpSocket();
	virtual bool Initialize(IIOServer * ios, ITcpSocketCallback * cb);
	virtual bool GetPeerInfo(unsigned int * addr, unsigned short *port);


	virtual bool DoConnect(const char *ip, unsigned short port);
	virtual bool DoSend(char * buf, unsigned int len);
	virtual bool DoRecv(char * buf, unsigned int len);
	virtual bool Close();

	void OnPostClose();

	virtual bool OnEPOLLMessage(int type, int flag);


public:
	IIOServer * m_ios;
	ITcpSocketCallback * m_cb;
	sockaddr_in	m_addr;
	tagRegister m_handle;

	bool		m_bNeedDestroy;

	unsigned int m_iRecvLen;
	char	*	 m_pRecvBuf;

	unsigned int m_iSendLen;
	char *		 m_pSendBuf;
};


#endif