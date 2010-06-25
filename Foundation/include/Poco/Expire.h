//
// Expire.h
//
// $Id: //poco/1.3/Foundation/include/Poco/Expire.h#3 $
//
// Library: Foundation
// Package: Events
// Module:  Expire
//
// Implementation of the Expire template.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_Expire_INCLUDED
#define Foundation_Expire_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractDelegate.h"
#include "Poco/Timestamp.h"


namespace Poco {


template <class TArgs>
class Expire: public AbstractDelegate<TArgs>
	/// Decorator for AbstractDelegate adding automatic 
	/// expiring of registrations to AbstractDelegates.
{
public:
	Expire(const AbstractDelegate<TArgs>& p, Timestamp::TimeDiff expireMillisecs):
		AbstractDelegate<TArgs>(p),
		_pDelegate(p.clone()), 
		_expire(expireMillisecs*1000)
	{
	}

	Expire(const Expire& expire):
		AbstractDelegate<TArgs>(expire),
		_pDelegate(expire._pDelegate->clone()),
		_expire(expire._expire),
		_creationTime(expire._creationTime)
	{
	}

	~Expire()
	{
		destroy();
	}
	
	Expire& operator = (const Expire& expire)
	{
		if (&expire != this)
		{
			delete this->_pDelegate;
			this->_pDelegate    = expire._pDelegate->clone();
			this->_expire       = expire._expire;
			this->_creationTime = expire._creationTime;
			this->_pTarget = expire._pTarget;
		}
		return *this;
	}

	bool notify(const void* sender, TArgs& arguments)
	{
		if (!expired())
			return this->_pDelegate->notify(sender, arguments);
		else
			return false;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new Expire(*this);
	}

	void destroy()
	{
		delete this->_pDelegate;
		this->_pDelegate = 0;
	}

	const AbstractDelegate<TArgs>& getDelegate() const
	{
		return *this->_pDelegate;
	}

protected:
	bool expired() const
	{
		return _creationTime.isElapsed(_expire);
	}

	AbstractDelegate<TArgs>* _pDelegate;
	Timestamp::TimeDiff _expire;
	Timestamp _creationTime;

private:
	Expire();
};


} // namespace Poco


#endif // Foundation_Expire_INCLUDED
