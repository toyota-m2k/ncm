#include "MjuLock.h"

const char* MjuLockObject::CLASS_NAME = "java/lang/Object";


bool MjuAutoLock::_lock(bool sw) {
	if (!mObj || !mEnv){
		if (mEnableException) {
			MjuThrowLockException("cannot lock/unlock: no lock object.");
			return false;
		}
	}

	MjuException jex(true);
	if (mEnv) {
		if (sw) {
			mEnv->MonitorEnter(mObj);
		}
		else {
			mEnv->MonitorExit(mObj);
		}
	}

	if (jex.check()) {
		if (mEnableException) {
			MjuThrowLockException("cannot lock/unlock: system error.");
			return false;
		}
	}
	return true;
}


bool MjuLockObject::create() {
	mLockObject.dispose();
	MjuException jex(true);
	jmethodID mid = mLockObject.getMethodId("<init>", "()V");
	if (mid && !jex.check()) {
		if (mLockObject.create(mid) && !jex.check()) {
			return true;
		}
	}
	if (mEnableException) {
		MjuThrowLockException("cannot create a Object for locking.");
	}
	return false;
}

bool MjuLockObject::_lock(bool sw, JNIEnv* env) {
	if (!mLockObject.isAvailable()|| !env){
		if (mEnableException) {
			MjuThrowLockException("cannot lock/unlock: no lock object.");
			return false;
		}
	}

	MjuException jex(true);
	if (env) {
		if (sw) {
			env->MonitorEnter(mLockObject);
		}
		else {
			env->MonitorExit(mLockObject);
		}
	}

	if (jex.check()) {
		if (mEnableException) {
			MjuThrowLockException("cannot lock/unlock: system error.");
			return false;
		}
	}
	return true;
}
