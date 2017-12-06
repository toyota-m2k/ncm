#include <jni.h>

#include "cppunit/CppUnit.h"
#include "common/file/JflTmpFile.h"
#include "jniutil/MjuString.h"
#include "jniutil/MjuLocal.h"
#include "jniutil/MjuException.h"
#include "cppunit/stream/CstJavaStreamTest.h"
#include "common/parser/mime/CpsMimeParser.h"

extern "C"
JNIEXPORT void JNICALL Java_com_michael_ncm_NcmUnitTest_unitTestInit(JNIEnv* env, jobject thiz, jstring path) {
	MJU_LOCAL_INIT(env);

	MjuString jstr(env, path);
	CJflTmpFile::SetWorkFolder(jstr.getUtf8Chars());

	CCpsMimeParser::InitParser();

	// jstr.detach();
	CppUnit_Init();
}

#include "jniutil/MjuArray.h"
#include "jniutil/MjuStream.h"


extern "C"
JNIEXPORT void JNICALL Java_com_michael_ncm_NcmUnitTest_unitTestRun(JNIEnv* env, jobject thiz, jbyteArray testByteArray, jobject inputStream, jobject outputStream) {
	MJU_LOCAL_INIT(env);

	MJE_INIT_EXCEPTION(env);

	CCstJavaStreamTest::s_jbyteArray = testByteArray;
	CCstJavaStreamTest::s_inputStream = inputStream;
	CCstJavaStreamTest::s_outputStream = outputStream;


//	MjuByteArray ary(env);
//	ary.create(128);
//	MjuInputStream stream(env, inputStream, false);
//	jmethodID mid = stream.getMethodId("read", "([B)I");
//
////	jint len = env->CallIntMethod(inputStream, mid, (jbyteArray)ary);
//
//	jint len = stream.read(ary);
//	return;


	//	//jclass cls = env->FindClass("java/io/InputStream");
	//	jclass cls = env->GetObjectClass(inputStream);
	//	jmethodID mid = env->GetMethodID(cls, "read", "([B)I");
	//	jint len = env->CallIntMethod(inputStream, mid, (jbyteArray)ary);
	//	return;



	CppUnit_Run();

	if (MJE_CHECK_EXCEPTION) {
		TRACE0("Exception");
		MJE_RESET_EXCEPTION;
	}
	else {
		TRACE0("OK");
	}


}

