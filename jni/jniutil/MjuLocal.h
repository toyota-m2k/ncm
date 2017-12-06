#pragma once

#include "jni.h"
#include "../common/JsvException.h"

#define	MJU_LOCAL_INIT(env)		MjuLocal __localInstance(env)
#define MJU_LOCAL_ENV			MjuLocal::getInstance()->getEnv()
#define MJU_LOCAL				MjuLocal::getInstance()

class MjuCharset;
class MjuLocal {
private:
	JNIEnv*	mEnv;
	MjuCharset* mCharset;

	static MjuLocal* sInstance;

public:
	MjuLocal(JNIEnv* env);
	~MjuLocal();

	static MjuLocal* getInstance() { return sInstance; }
	JNIEnv* getEnv() { return (this)?mEnv:NULL; }
	MjuCharset* getCharset();
};

class MjuEnvException : public CException {
public:
	MjuEnvException(const char* msg) : CException("local env has not been initialized.", msg) {}
};

inline void MjuThrowEnvException(const char* msg) {
#ifdef	__EXCEPTIONS
	throw new MjuEnvException(msg);
#endif
}