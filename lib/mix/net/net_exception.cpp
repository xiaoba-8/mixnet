/*
 * net_exception.cpp
 *
 *  Created on: 2018-4-2
 *      Author: xiaoba-8
 */

#include <mix/net/net_exception.h>

#include <stdio.h>
#include <stdexcept>
#include <stdarg.h>

namespace mix
{

net_exception::net_exception() : m_errno(0)
{

}

net_exception::net_exception(int errorno, const char *format, ...) :
				m_errno(errorno)
{
	va_list list;
	va_start(list, format);
	char msg[512];
	vsprintf(msg, format, list);
	va_end(list);

	m_what.assign(msg);
}

net_exception::~net_exception() throw ()
{
	// TODO Auto-generated destructor stub
}

const char *net_exception::what() const throw ()
{
	return m_what.c_str();
}

int net_exception::GetErrNo()
{
	return m_errno;
}

}
