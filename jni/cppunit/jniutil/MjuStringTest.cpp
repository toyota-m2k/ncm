#include "MjuStringTest.h"

#include "../../jniutil/MjuString.h"
#include "../../jniutil/MjuArray.h"
#include "../../jniutil/MjuLocal.h"

void MjuStringTest::Test()
{
	// Constructor
	bool result;

	const char test_string1[] = "test string 1";
	const char test_string1_1[] = "TEST STRING 1";
	const char test_string2[] = "test string 2";

	MjuString jstr1(MJU_LOCAL_ENV, test_string1);
	CPPUNIT_ASSERT(jstr1.length() == strlen(test_string1));

	MjuByteArray bytes(MJU_LOCAL_ENV, jstr1.getBytes());

	int count = bytes.getLength();
	jbyte* ary = bytes.getArray();

	CPPUNIT_ASSERT(ary != NULL);
	CPPUNIT_ASSERT(count == strlen(test_string1));
	CPPUNIT_ASSERT(memcmp(test_string1, ary, count) == 0);

	bytes.releaseArray();

	MjuString jstr1d(MJU_LOCAL_ENV);
	result = jstr1d.create(bytes);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(jstr1d.length() == strlen(test_string1));
//	result = jstr1d.equals(jstr1);
	CPPUNIT_ASSERT(jstr1d.equals(jstr1));
//	result = jstr1d.equalsIgnoreCase(jstr1);
	CPPUNIT_ASSERT(jstr1d.equalsIgnoreCase(jstr1));

	MjuString jstr1_1(MJU_LOCAL_ENV, test_string1_1);
	CPPUNIT_ASSERT(!jstr1_1.equals(jstr1));
	CPPUNIT_ASSERT(jstr1_1.equalsIgnoreCase(jstr1));

	MjuString jstr2(MJU_LOCAL_ENV, test_string2);
	CPPUNIT_ASSERT(!jstr2.equals(jstr1));
	CPPUNIT_ASSERT(!jstr2.equalsIgnoreCase(jstr1));

	jmethodID idToLower = jstr1_1.getMethodId("toLowerCase", "()Ljava/lang/String;");
	jobject obj = jstr1_1.callObject(idToLower);
	CPPUNIT_ASSERT(obj != NULL);
	
	MjuClass cls1(MJU_LOCAL_ENV, "java/lang/String");
	MjuClass cls2(MJU_LOCAL_ENV);
	cls2.initByObject(jstr1);
	CPPUNIT_ASSERT(cls2.isAssignableFrom(cls1));

	MjuTypeBase<jobject> jobj(MJU_LOCAL_ENV, obj, "java/lang/Object");
	CPPUNIT_ASSERT(cls1.isInstanceOf(jobj));
	CPPUNIT_ASSERT(cls2.isInstanceOf(jobj));

	

	MjuString jstrLower(MJU_LOCAL_ENV, (jstring)jobj.detach());
	CPPUNIT_ASSERT(jstrLower.equals(jstr1));
	CPPUNIT_ASSERT(jstrLower.equalsIgnoreCase(jstr1));
	CPPUNIT_ASSERT(!jstrLower.equals(jstr1_1));
	CPPUNIT_ASSERT(jstrLower.equalsIgnoreCase(jstr1_1));

}
