/*
 * net_exception.h
 *
 *  Created on: 2018-4-2
 *      Author: xiaoba-8
 */

#ifndef NETEXCEPTION_H_
#define NETEXCEPTION_H_

#include <string>

namespace mix
{

class net_exception
{
public:
    explicit net_exception();
    explicit net_exception(int errorno, const char *format, ...);
	virtual ~net_exception() throw ();

	virtual const char * what() const throw();
	virtual int GetErrNo();
protected:
	std::string m_what;
	int m_errno;

};

}

#endif /* NETEXCEPTION_H_ */
