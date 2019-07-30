/*
 * UdpServerSocket.h
 *
 *  Created on: 2018-9-1
 *      Author: xiaoba-8
 */

#ifndef UDPSERVERSOCKET_H_
#define UDPSERVERSOCKET_H_

#include "Socket.h"

namespace mix
{

class UdpServerSocket: public Socket
{
public:
	UdpServerSocket();
	UdpServerSocket(int port);
	UdpServerSocket(const char *host, int port);

	virtual ~UdpServerSocket();

	virtual void Bind(const char *host, int port, int backlog = 50);

	/**
	 * Enable/disable the SO_REUSEADDR socket option.
	 * <p>
	 * When a TCP connection is closed the connection may remain
	 * in a timeout state for a period of time after the connection
	 * is closed (typically known as the <tt>TIME_WAIT</tt> state
	 * or <tt>2MSL</tt> wait state).
	 * For applications using a well known socket address or port
	 * it may not be possible to bind a socket to the required
	 * <tt>SocketAddress</tt> if there is a connection in the
	 * timeout state involving the socket address or port.
	 * <p>
	 * Enabling <tt>SO_REUSEADDR</tt> prior to binding the socket
	 * using {@link #bind(SocketAddress)} allows the socket to be
	 * bound even though a previous connection is in a timeout
	 * state.
	 * <p>
	 * When a <tt>ServerSocket</tt> is created the initial setting
	 * of <tt>SO_REUSEADDR</tt> is not defined. Applications can
	 * use {@link #getReuseAddress()} to determine the initial
	 * setting of <tt>SO_REUSEADDR</tt>.
	 * <p>
	 * The behaviour when <tt>SO_REUSEADDR</tt> is enabled or
	 * disabled after a socket is bound (See {@link #isBound()})
	 * is not defined.
	 *
	 */
	virtual void SetReuseAddress(bool on);

	/**
	 * Tests if SO_REUSEADDR is enabled.
	 *
	 * @return a <code>boolean</code> indicating whether or not SO_REUSEADDR is enabled.
	 */
	virtual bool GetReuseAddress();

protected:
	bool m_reuseOn;
};

}

#endif /* UDPSERVERSOCKET_H_ */
