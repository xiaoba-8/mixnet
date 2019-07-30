/*
 * TcpServerSocket.h
 *
 *  Created on: 2018-7-12
 *      Author: xiaoba-8
 */

#ifndef TCPSERVERSOCKET_H_
#define TCPSERVERSOCKET_H_

#include "Socket.h"

namespace mix
{

class TcpServerSocket: public Socket
{
public:
	TcpServerSocket();
	TcpServerSocket(int port);
	TcpServerSocket(const char *host, int port);

	virtual ~TcpServerSocket();

	/**
	 *
	 * Binds the <code>TcpServerSocket</code> to a specific address
	 * (IP address and port number).
	 * <p>
	 * If the address is <code>null</code>, then the system will pick up
	 * an ephemeral port and a valid local address to bind the socket.
	 * <P>
	 * The <code>backlog</code> argument must be a positive
	 * value greater than 0. If the value passed if equal or less
	 * than 0, then the default value will be assumed.
	 * @param	host, port	The IP address & port number to bind to.
	 * @param	backlog		The listen backlog length.
	 */
	virtual void Bind(const char *host, int port, int backlog = 50);

	/**
	 * Listens for a connection to be made to this socket and accepts
	 * it. The method blocks until a connection is made.
	 *
	 * <p>A new Socket <code>s</code> is created and, if there
	 * is a security manager,
	 * the security manager's <code>checkAccept</code> method is called
	 * with <code>s.getInetAddress().getHostAddress()</code> and
	 * <code>s.getPort()</code>
	 * as its arguments to ensure the operation is allowed.
	 * This could result in a SecurityException.
	 *
	 */
	virtual Socket *Accept(int timeout = 0);

	/**
	 * Closes this socket.
	 *
	 * Any thread currently blocked in {@link #accept()} will throw
	 * a {@link RootException}.
	 *
	 */
	virtual void Close();

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
	 * When a <tt>TcpServerSocket</tt> is created the initial setting
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

#endif /* TCPSERVERSOCKET_H_ */
