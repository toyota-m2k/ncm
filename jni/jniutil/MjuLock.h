#pragma once

#include "MjuTypeBase.h"
#include "MjuException.h"

#define BEGIN_LOCK(env, obj)		{ MjuAutoLock __lock##obj(env, obj);
#define BEGIN_LOCK_LOCALENV(obj)	{ MjuAutoLock __lock##obj(obj);
#define END_LOCK					}

class MjuLockException : public CException {
public:
	MjuLockException(const char* msg) : CException("lock error.", msg) {}
};

inline void MjuThrowLockException(const char* msg) {
#ifdef	__EXCEPTIONS
	throw new MjuLockException(msg);
#endif
}
class MjuAutoLock {
private:
	JNIEnv* mEnv;
	jobject mObj;
	bool mEnableException;
public:
	MjuAutoLock(jobject obj) : mEnv(MJU_LOCAL_ENV), mObj(obj) {
		lock();
	}
	MjuAutoLock(JNIEnv* env, jobject obj) : mEnv(env), mObj(obj) {
		lock();
	}
	~MjuAutoLock() {
		unlock();
	}

	void lock() {
		_lock(true);
	}
	void unlock() {
		_lock(false);
	}

private:
	bool _lock(bool sw);

};

class MjuLockObject {
public:
	static const char* CLASS_NAME;
private:
	MjuTypeBase<jobject> mLockObject;
	bool mEnableException;
public:
	MjuLockObject():mLockObject(CLASS_NAME){}

	bool init(JNIEnv* env) {
		if (!mLockObject.isAvailable()){
			mLockObject.replaceEnv(env);
			create();
		}
	}

	jobject detach() {
		return mLockObject.detach();
	}

	/**
	* なまオブジェクトを取得するキャストオペレータ
	*/
	operator jobject() const {
		return mLockObject;
	}

	bool lock(JNIEnv* env) {
		return _lock(true, env);
	}

	bool unlock(JNIEnv* env) {
		return _lock(false, env);
	}
private:
	bool _lock(bool sw, JNIEnv* env);
	bool create();

};

