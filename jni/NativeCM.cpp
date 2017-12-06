#include <jni.h>

#include "common/CppUnit.h"
#include "common/file/JflTmpFile.h"
#include "jniutil/MjuString.h"
#include "jniutil/MjuLocal.h"
#include "jniutil/MjuException.h"

extern "C"
JNIEXPORT void JNICALL Java_com_michael_ncm_NcmUnitTest_unitTestInit(JNIEnv* env, jobject thiz, jstring path) {
	
	MjuString jstr(env, path);
	MJU_LOCAL_INIT(env);

	CJflTmpFile::SetWorkFolder(jstr.getUtf8Chars());
	// jstr.detach();
	CppUnit_Init();
}


extern "C"
JNIEXPORT void JNICALL Java_com_michael_ncm_NcmUnitTest_unitTestRun(JNIEnv* env, jobject thiz) {
	MJU_LOCAL_INIT(env);

	MJE_INIT_EXCEPTION(env);

	CppUnit_Run();

	if (MJE_CHECK_EXCEPTION) {
		TRACE0("Exception");
		MJE_RESET_EXCEPTION;
	}
	else {
		TRACE0("OK");
	}


}

