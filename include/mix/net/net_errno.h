/*
 * base_errno.h
 *
 *  Created on: 2013-6-21
 *      Author: stan
 */

#ifndef NET_ERRNO_H_
#define NET_ERRNO_H_

#define NET_PREFIX	0
#define NET_NUMBER	20000

#define NET_GETHOSTBYNAME_NO						1
#define NET_GETHOSTBYNAME_MSG					"Can't get host by name [%s]"

#define NET_REMOTEERROR_NO						2
#define NET_REMOTEERROR_MSG						"Remote Error [%s]"

#define NET_SOCK_LISTEN_NO						3
#define NET_SOCK_LISTEN_MSG						"Socket Listen Error [%s]"

#define NET_SOCK_ACCEPT_NO						4
#define NET_SOCK_ACCEPT_MSG						"Socket Accept Error [%s]"

#define NET_SOCK_SELECT_NO						5
#define NET_SOCK_SELECT_MSG						"Socket Select Error [%s]"

#define NET_SOCK_CREATE_NO						6
#define NET_SOCK_CREATE_MSG						"Create Socket Error [%s]"

#define NET_SOCK_BIND_NO							7
#define NET_SOCK_BIND_MSG							"Socket Bind Error [%s]"

#define NET_SOCK_CONNECT_NO						8
#define NET_SOCK_CONNECT_MSG						"Socket Connect Error [%s]"

#define NET_SOCK_IOCTL_NO							9
#define NET_SOCK_IOCTL_MSG						"ioctlsocket(FIONBIO) Error! [%s]"

#define NET_SOCK_FCNTL_NO							10
#define NET_SOCK_FCNTL_MSG						"fcntl Error [%s]"

#define NET_SOCK_READ_NO							11
#define NET_SOCK_READ_MSG							"Socket Read Error [%s]"

#define NET_SOCK_READ_TIMEOUT_NO				12
#define NET_SOCK_READ_TIMEOUT_MSG				"Socket Read Timeout"

#define NET_SOCK_SEND_NO							13
#define NET_SOCK_SEND_MSG							"Socket Send Error [%s]"

#define NET_SOCK_WRITE_TIMEOUT_NO				14
#define NET_SOCK_WRITE_TIMEOUT_MSG				"Socket Write Timeout"

#define NET_SOCK_SETOPT_NO						15
#define NET_SOCK_SETOPT_MSG						"Socket setsockopt Error [%s]"

#define NET_PTHREAD_SET_SCHED_NO				16
#define NET_PTHREAD_SET_SCHED_MSG				"pthread_setschedparam error [%s]"

#define NET_PTHREAD_GET_SCHED_NO				17
#define NET_PTHREAD_GET_SCHED_MSG				"pthread_getschedparam error [%s]"

#define NET_PTHREAD_SET_PRIO_NO					18
#define NET_PTHREAD_SET_PRIO_MSG				"pthread_setschedprio error [%s]"

#endif /* NET_ERRNO_H_ */
