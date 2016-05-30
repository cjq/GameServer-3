
#ifndef EPOLL_H
#define EPOLL_H

/*
* epoll的 post使用socketpair做为挂靠在epoll_wait上的通知触发机制 采用EPOLLLT模式
*/


namespace net
{
	// 定义消息容器
	typedef std::vector<std::pair<int, void*>> MsgVct;

	// 定义server
	

}

#endif