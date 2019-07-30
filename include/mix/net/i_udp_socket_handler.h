/*
 * i_udp_socket_handler.h
 *
 *  Created on: 2018-9-1
 *      Author: xiaoba-8
 */

#ifndef IUDPSOCKETHANDLER_H_
#define IUDPSOCKETHANDLER_H_

#include "Socket.h"

namespace mix
{

class i_udp_socket_handler
{
public:
	virtual ~i_udp_socket_handler();
	virtual void Handler(Socket *pSocket, char *pBuffer, int len,
			char *remoteIp, int remotePort) = 0;
	virtual std::string GetTaskId() = 0;
};

}

#endif /* IUDPSOCKETHANDLER_H_ */
