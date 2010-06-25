//
// MetaProgramming.h
//
// $Id: //poco/1.3/Foundation/include/Poco/MetaProgramming.h#2 $
//
// Library: Foundation
// Package: Core
// Module:  MetaProgramming
//
// Common definitions useful for Meta Template Programming
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


#ifndef Foundation_MetaProgramming_INCLUDED
#define Foundation_MetaProgramming_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


template <typename T>
struct IsReference
	/// Use this struct to determine if a template type is a reference.
{
	enum 
	{
		VALUE = 0
	};
};


template <typename T>
struct IsReference<T&>
{
	enum 
	{
		VALUE = 1
	};
};


template <typename T>
struct IsReference<const T&>
{
	enum 
	{
		VALUE = 1
	};
};


template <typename T>
struct IsConst
	/// Use this struct to determine if a template type is a const type.
{
	enum 
	{
		VALUE = 0
	};
};


template <typename T>
struct IsConst<const T&>
{
	enum 
	{
		VALUE = 1
	};
};


template <typename T>
struct IsConst<const T>
{
	enum 
	{
		VALUE = 1
	};
};


template <typename T>
struct TypeWrapper
	/// Use the type wrapper if you want to decouple constness and references from template types.
{
	typedef T TYPE;
	typedef const T CONSTTYPE;
	typedef T& REFTYPE;
	typedef const T& CONSTREFTYPE;
};


template <typename T>
struct TypeWrapper<const T>
{
	typedef T TYPE;
	typedef const T CONSTTYPE;
	typedef T& REFTYPE;
	typedef const T& CONSTREFTYPE;
};


template <typename T>
struct TypeWrapper<const T&>
{
	typedef T TYPE;
	typedef const T CONSTTYPE;
	typedef T& REFTYPE;
	typedef const T& CONSTREFTYPE;
};


template <typename T>
struct TypeWrapper<T&>
{
	typedef T TYPE;
	typedef const T CONSTTYPE;
	typedef T& REFTYPE;
	typedef const T& CONSTREFTYPE;
};


} // namespace Poco


#endif // Foundation_MetaProgramming_INCLUDED
