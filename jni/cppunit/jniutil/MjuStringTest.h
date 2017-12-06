#pragma once

#include "../CppUnit.h"

class MjuStringTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(MjuStringTest);

	CPPUNIT_TEST(Test);

	CPPUNIT_TEST_SUITE_END();
private:

	// your stuff...

public:

	virtual void setUp() { /* initialize */ }
	virtual void tearDown() { /* terminate */ }

	void Test();
};


//CUPPA:impl=+


