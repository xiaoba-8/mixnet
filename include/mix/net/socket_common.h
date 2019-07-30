/*
 * socket_common.h
 *
 *  Created on: 2018-7-11
 *      Author: xiaoba-8
 */

#ifndef SOCKET_COMMON_H_
#define SOCKET_COMMON_H_

// platform specific stuff
#if (defined(__unix__) || defined(unix)) && !defined(USG)
#include <sys/param.h>
#endif
#include <list>

// int64
#ifdef _WIN32
typedef unsigned __int64 uint64_t;
#else
#include <stdlib.h>
#ifdef SOLARIS
# include <sys/types.h>
#else
# include <stdint.h>
#endif
#endif

#ifndef _WIN32
// ----------------------------------------
// common unix includes / defines
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <strings.h>

// all typedefs in this file will be declared outside the sockets namespace,
// because some os's will already have one or more of the type defined.
typedef int SOCKET;

#endif

#include "net_exception.h"

#endif /* SOCKET_COMMON_H_ */
