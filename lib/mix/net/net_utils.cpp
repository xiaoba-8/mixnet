/*
 * net_utils.cpp
 *
 *  Created on: 2018-5-4
 *      Author: xiaoba-8
 */

#include <mix/net/net_utils.h>

namespace mix
{

uint64_t ntohll(uint64_t _netlong)
{
	uint64_t result = ntohl(_netlong);
	return (result << 32 | ntohl(_netlong >> 32));
}

uint64_t htonll(uint64_t _hostlong)
{
	uint64_t result = htonl(_hostlong);
	return (result << 32 | htonl(_hostlong >> 32));
}

double ntoh_double(double _netd)
{
	uint64_t result = ntohll(*((uint64_t *)&_netd));
	return *((double *)&result);
}

double hton_double(double _hostd)
{
	uint64_t result = htonll(*((uint64_t *)&_hostd));
	return *((double *)&result);
}

}
