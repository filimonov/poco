//
// CoreTest.cpp
//
// $Id: //poco/1.3/Foundation/testsuite/src/CoreTest.cpp#7 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#include "CoreTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Bugcheck.h"
#include "Poco/Exception.h"
#include "Poco/Environment.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Buffer.h"
#include "Poco/AtomicCounter.h"
#include "Poco/Nullable.h"
#include <iostream>
#include <vector>
#include <cstring>


using Poco::Bugcheck;
using Poco::Exception;
using Poco::Environment;
using Poco::Thread;
using Poco::Runnable;
using Poco::Buffer;
using Poco::AtomicCounter;
using Poco::Nullable;


namespace
{
	class ACTRunnable: public Poco::Runnable
	{
	public:
		ACTRunnable(AtomicCounter& counter):
			_counter(counter)
		{
		}
		
		void run()
		{
			for (int i = 0; i < 100000; ++i)
			{
				_counter++;
				_counter--;
				++_counter;
				--_counter;
			}
		}
		
	private:
		AtomicCounter& _counter;
	};
}


//
// The bugcheck test is normally disabled, as it
// causes a break into the debugger.
//
#define ENABLE_BUGCHECK_TEST 0


CoreTest::CoreTest(const std::string& name): CppUnit::TestCase(name)
{
}


CoreTest::~CoreTest()
{
}


void CoreTest::testPlatform()
{
	std::cout << "POCO_OS:   " << POCO_OS << std::endl;
	std::cout << "POCO_ARCH: " << POCO_ARCH << std::endl;
}


void CoreTest::testFixedLength()
{
	assert (sizeof(Poco::Int8) == 1);
	assert (sizeof(Poco::UInt8) == 1);
	assert (sizeof(Poco::Int16) == 2);
	assert (sizeof(Poco::UInt16) == 2);
	assert (sizeof(Poco::Int32) == 4);
	assert (sizeof(Poco::UInt32) == 4);
	#if defined(POCO_HAVE_INT64)
	assert (sizeof(Poco::Int64) == 8);
	assert (sizeof(Poco::UInt64) == 8);
	#endif
	assert (sizeof(Poco::IntPtr) == sizeof(void*));
	assert (sizeof(Poco::UIntPtr) == sizeof(void*));	
}


void CoreTest::testBugcheck()
{
#if ENABLE_BUGCHECK_TEST
	try
	{
		Bugcheck::assertion("test", __FILE__, __LINE__);	
		failmsg("must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		Bugcheck::nullPointer("test", __FILE__, __LINE__);	
		failmsg("must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		Bugcheck::bugcheck("test", __FILE__, __LINE__);	
		failmsg("must throw exception");
	}
	catch (Exception&)
	{
	}
#endif
}


void CoreTest::testEnvironment()
{
	Environment::set("FOO", "BAR");
	assert (Environment::has("FOO"));
	assert (Environment::get("FOO") == "BAR");
	assert (!Environment::has("THISONEDOESNOTEXIST123"));
	try
	{
		std::string v = Environment::get("THISONEDOESNOTEXIST123");
		failmsg("Environment variable does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}
	
	std::cout << "OS Name:         " << Environment::osName() << std::endl;
	std::cout << "OS Version:      " << Environment::osVersion() << std::endl;
	std::cout << "OS Architecture: " << Environment::osArchitecture() << std::endl;
	std::cout << "Node Name:       " << Environment::nodeName() << std::endl;
	std::cout << "Node ID:         " << Environment::nodeId() << std::endl;
	std::cout << "Number of CPUs:  " << Environment::processorCount() << std::endl;
}


void CoreTest::testBuffer()
{
	std::size_t s = 10;
	Buffer<int> b(s);
	std::vector<int> v;
	for (int i = 0; i < s; ++i)
		v.push_back(i);

	std::memcpy(b.begin(), &v[0], sizeof(int) * v.size());

	assert (s == b.size());
	for (int i = 0; i < s; ++i)
		assert (b[i] == i);

#if ENABLE_BUGCHECK_TEST
	try { int i = b[s]; fail ("must fail"); }
	catch (Exception&) { }
#endif
}


void CoreTest::testAtomicCounter()
{
	AtomicCounter ac;
	
	assert (ac.value() == 0);
	assert (ac++ == 0);
	assert (ac-- == 1);
	assert (++ac == 1);
	assert (--ac == 0);
	
	ac = 2;
	assert (ac.value() == 2);
	
	ac = 0;
	assert (ac.value() == 0);
	
	AtomicCounter ac2(2);
	assert (ac2.value() == 2);
	
	ACTRunnable act(ac);
	Thread t1;
	Thread t2;
	Thread t3;
	Thread t4;
	Thread t5;
	
	t1.start(act);
	t2.start(act);
	t3.start(act);
	t4.start(act);
	t5.start(act);
	
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	
	assert (ac.value() == 0);
}


void CoreTest::testNullable()
{
	Nullable<int> n1;
	assert (n1.isNull());
	
	assert (n1.value(42) == 42);
	
	try
	{
		int tmp = n1.value();
		fail("null value, must throw");
	}
	catch (Poco::NullValueException&)
	{
	}
	
	n1 = 1;
	assert (!n1.isNull());
	assert (n1.value() == 1);
	
	Nullable<int> n2(42);
	assert (!n2.isNull());
	assert (n2.value() == 42);
	assert (n2.value(99) == 42);
	
	n1 = n2;
	assert (!n1.isNull());
	assert (n1.value() == 42);
	
	n1.clear();
	assert (n1.isNull());
}


void CoreTest::setUp()
{
}


void CoreTest::tearDown()
{
}


CppUnit::Test* CoreTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CoreTest");

	CppUnit_addTest(pSuite, CoreTest, testPlatform);
	CppUnit_addTest(pSuite, CoreTest, testFixedLength);
	CppUnit_addTest(pSuite, CoreTest, testBugcheck);
	CppUnit_addTest(pSuite, CoreTest, testEnvironment);
	CppUnit_addTest(pSuite, CoreTest, testBuffer);
	CppUnit_addTest(pSuite, CoreTest, testAtomicCounter);
	CppUnit_addTest(pSuite, CoreTest, testNullable);

	return pSuite;
}
