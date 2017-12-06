#pragma once
#include <cstddef>
#include <jni.h>
#include "MjuLocal.h"

template <class T>
class MjuObject
{
protected:
	JNIEnv* mEnv;
	T mObj;
	bool mForbidReleaseOnDispose;

public:
	MjuObject() {
		mForbidReleaseOnDispose = false;
		mObj = NULL;
		mEnv = MJU_LOCAL_ENV;
		if (!mEnv) {
			MjuThrowEnvException("MjuObject constructor.");
		}
	}
	MjuObject(T o) {
		mForbidReleaseOnDispose = false;
		mObj = o;
		mEnv = MJU_LOCAL_ENV;
		if (!mEnv) {
			MjuThrowEnvException("MjuObject constructor.");
		}
	}
	MjuObject(JNIEnv* e) {
		mForbidReleaseOnDispose = false;
		mObj = NULL;
		mEnv = e;
		if (!mEnv) {
			mEnv = MJU_LOCAL_ENV;
			if (!mEnv) {
				MjuThrowEnvException("MjuObject constructor.");
			}
		}
	}
	MjuObject(JNIEnv* e, T o) {
		mForbidReleaseOnDispose = false;
		mObj = o;
		mEnv = e;
		if (!mEnv) {
			mEnv = MJU_LOCAL_ENV;
			if (!mEnv) {
				MjuThrowEnvException("MjuObject constructor.");
			}
		}
	}
private:
	// オブジェクトのコピーは禁止
	MjuObject(MjuObject<T>& src);
	MjuObject(JNIEnv*e, MjuObject<T>& src);

public:
	~MjuObject() {
		dispose();
	}

	bool replaceEnv(JNIEnv* env) {
		mEnv = env;
	}

	/**
	 * なまオブジェクトを設定する代入オペレータ
	 */
	T operator=(T o) {
		dispose();
		mObj = o;
		return mObj;
	}

	/**
	 * なまオブジェクトを取得するキャストオペレータ
	 */
	operator T() const {
		return mObj;
	}

	/**
	 * nullチェックオペレータ
	 */
	bool
	operator !() const {
		return ( mObj == NULL );
	}

	/**
	 * オブジェクトを切り離す
	 */
	virtual T detach() {
		T o = mObj;
		mObj = NULL;
		mForbidReleaseOnDispose = false;
		return o;
	}

	/**
	 * オブジェクトを破棄する
	 */
	virtual void dispose() {
		if (mObj && mEnv) {
			if (!mForbidReleaseOnDispose) {
				mEnv->DeleteLocalRef(mObj);
			}
			mForbidReleaseOnDispose = false;
			mObj = NULL;
		}
	}
	/**
	 * dispose()時に DeleteLocalRef()を呼ぶかどうか
	 *
	 * @param	forbid	true: DeleteLocalRef()しない / false:する（初期値）
	 */
	void forbidReleaseOnDispose(bool forbid = true) {
		mForbidReleaseOnDispose = forbid;
	}

	/**
	 * オブジェクトを保持しているか？
	 */
	bool isAvailable() const {
		return mEnv && mObj;
	}

	bool isSameObject(jobject o) const {
		if (isAvailable()) {
			return mEnv->IsSameObject(mObj, o) != 0;
		}
		return false;
	}

	JNIEnv* getEnv() {
		return mEnv;
	}

	void lock() {
		mEnv->MonitorEnter(mObj);
	}

	void unlock() {
		mEnv->MonitorExit(mObj);
	}

private:
	// オブジェクトのコピーは禁止
	T operator=(MjuObject<T> o) {
		return mObj;
	}

};

