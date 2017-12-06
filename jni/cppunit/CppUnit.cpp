#include "CppUnit.h"


CppUnit::TestManager gTestManager;


#include "collection/TJsvArrayTest.h"
#include "collection/TJsvChain_Test.h"
#include "collection/TJsvMapTest.h"
#include "collection/TJsvObjListTest.h"
#include "collection/TJsvQueTest.h"
#include "collection/TJsvRingBuffer_test.h"

#include "string/JsvAString_Test.h"
#include "string/JsvWString_Test.h"
#include "string/JsvAStringBuffer_Test.h"
#include "stream/CstStreamTest.h"
#include "stream/CstJavaStreamTest.h"

#include "jniutil/MjuStringTest.h"
#include "parser/CpsMimeParserTest.h"
#include "parser/CmcMimeComposerTest.h"


void CppUnit_Init() {
	CPPUNIT_TEST_SUITE_REGISTRATION(CmcMimeComposerTest);
	CPPUNIT_TEST_SUITE_REGISTRATION(CCpsMimeParserTest);

	CPPUNIT_TEST_SUITE_REGISTRATION(TJsvArrayTest);
	CPPUNIT_TEST_SUITE_REGISTRATION(TJsvChain_test);
	CPPUNIT_TEST_SUITE_REGISTRATION(TJsvMapTest);
	CPPUNIT_TEST_SUITE_REGISTRATION(TJsvObjListTest);
	CPPUNIT_TEST_SUITE_REGISTRATION(TJsvQueTest);
	CPPUNIT_TEST_SUITE_REGISTRATION(TJsvRingBuffer_test);

	CPPUNIT_TEST_SUITE_REGISTRATION(CJsvAStringTest);
#ifdef SUPPORT_WSTRING
	CPPUNIT_TEST_SUITE_REGISTRATION(CJsvWStringTest);
#endif

	CPPUNIT_TEST_SUITE_REGISTRATION(CJsvAStringBufferTest);
	CPPUNIT_TEST_SUITE_REGISTRATION(CCstFileStreamTest);
	CPPUNIT_TEST_SUITE_REGISTRATION(MjuStringTest);
	CPPUNIT_TEST_SUITE_REGISTRATION(CCstJavaStreamTest);

}

void CppUnit_Run() {
	gTestManager.doit(NULL, 0);
}

void CppUnit_RunTarget(const char* targets[], int targetsCount) {
	gTestManager.doit(targets, targetsCount);
}
