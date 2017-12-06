#pragma once

// CppUnit 
#include "../common/Compat.h"


#define CPPUNIT_TEST_SUITE(CLSNAME) \
	public: \
	static TestFixture* createInstance() { \
		return new CLSNAME(); \
	} \
	bool test(const char* classname) { \
		setUp(); \
		LOG(ANDROID_LOG_INFO, "Testing class %s ...", classname)

#define CPPUNIT_TEST(METHODNAME) \
		try{ \
			METHODNAME(); \
			LOG(ANDROID_LOG_INFO, "PASS: %s::%s", classname, #METHODNAME); \
				} catch (CppUnit::Result* e) { \
			LOG(ANDROID_LOG_ERROR, "ERR: %s:%d %s::%s", e->file, e->line, classname, #METHODNAME);\
			if( e->message) { \
				LOG(ANDROID_LOG_ERROR, "   %s", e->message); \
															} \
			return false; \
		} catch (CException* e) { \
			LOG(ANDROID_LOG_ERROR, "detect CException %s:%s (%s::%s)", e->name, (e->message)?e->message:"(no message)", classname, #METHODNAME); \
			return false; \
		} catch (...) { \
			LOG(ANDROID_LOG_ERROR, "unknown error: %s::%s", classname, #METHODNAME); \
			return false; \
		}

#define CPPUNIT_TEST_SUITE_END()	\
		tearDown(); \
		LOG(ANDROID_LOG_INFO, "... Done: %s", classname); \
		return true; \
	}

#define CPPUNIT_ASSERT(a) {if(!(a)) { throw new CppUnit::Result(__FILE__, __LINE__);}}
#define CPPUNIT_ASSERT_MESSAGE(msg,a) { if(!(a)) { LOG(ANDROID_LOG_ERROR, msg); throw new CppUnit::Result(__FILE__, __LINE__); }}

namespace CppUnit {
	class Result {
	public:
		const char* message;
		const char* file;
		int line;

		Result(const char* f, int l) {
			message = NULL;
			file = f;
			line = l;
		}

		Result(const char* f, int l, const char* msg) {
			message = msg;
			file = f;
			line = l;
		}

	};

	class TestFixture {
	public:
		virtual bool test(const char* classname) {
			return true;
		}
		virtual void setUp() { /* initialize */ }
		virtual void tearDown() { /* terminate */ }
	};

	typedef TestFixture* (*PFNCreateTestFixture)();

	struct TestInfo{
		const char* className;
		PFNCreateTestFixture fixture;
	};

	class TestManager {
	private:
		TestInfo m_fixtures[128];
		int m_count;
	public:
		TestManager() {
			m_count = 0;
		}
		~TestManager() {
			m_count = 0;
		}

		bool add(const char* name, PFNCreateTestFixture p) {
			m_fixtures[m_count].className = name;
			m_fixtures[m_count].fixture = p;
			m_count++;
			return true;
		}

		static bool contains(const char* ary[], int aryCount, const char* name) {
			for (int i = 0; i < aryCount; i++) {
				if (strcmp(ary[i], name) == 0) {
					return true;
				}
			}
			return false;
		}

		void doit(const char* targets[], int targetsCount ) {
			for (int i = 0; i < m_count; i++) {
				bool result;
				if (NULL == targets || contains(targets, targetsCount, m_fixtures[i].className)) {
					TestFixture* p = m_fixtures[i].fixture();
					result = p->test(m_fixtures[i].className);
					delete p;
					if (!result) {
						LOG(ANDROID_LOG_ERROR, "Terminated with error on class %s", m_fixtures[i].className);
						return;
					}
				}
			}
			LOG(ANDROID_LOG_INFO, "All Test Completed.");
		}
	};
}

extern CppUnit::TestManager gTestManager;

#define	CPPUNIT_TEST_SUITE_REGISTRATION(CLSNAME) \
	gTestManager.add(#CLSNAME, CLSNAME::createInstance)

#define CPPUNIT_TEST_RUN() \
	gTestManager.doit(NULL, 0)

#define CPPUNIT_TEST_TARGET_RUN(target, count) \
	gTestManager.doit(target, count)


extern void CppUnit_Init();
extern void CppUnit_Run();
extern void CppUnit_RunTarget(const char* targets[], int targetsCount);
