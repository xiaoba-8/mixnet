/*
 * i_tcp_socket_handler.h
 *
 *  Created on: 2018-7-13
 *      Author: xiaoba-8
 */

#ifndef ITCPSOCKETHANDLER_H_
#define ITCPSOCKETHANDLER_H_

#include "Socket.h"

namespace mix
{

class i_tcp_socket_handler
{
public:
	virtual ~i_tcp_socket_handler();
	virtual void Handler(Socket *pSocket) = 0;
};

}

#endif /* ITCPSOCKETHANDLER_H_ */
