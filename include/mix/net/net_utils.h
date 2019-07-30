/*
 * net_utils.h
 *
 *  Created on: 2018-5-4
 *      Author: xiaoba-8
 */

#ifndef NET_UTILS_H_
#define NET_UTILS_H_

#include <arpa/inet.h>

namespace mix
{

uint64_t ntohll(uint64_t _netlong);
uint64_t htonll(uint64_t _hostlong);
double ntoh_double(double _netd);
double hton_double(double _hostd);

}

#endif /* NET_UTILS_H_ */
