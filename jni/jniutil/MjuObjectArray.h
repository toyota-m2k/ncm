#pragma once

#include "MjuArray.h"


class MjuObjectArray : public MjuArray<jobjectArray , jobject> {
public:
	MjuObjectArray()
		: MjuArray<jobjectArray, jobject>() {
	}

	MjuObjectArray(jobjectArray  value)
		: MjuArray<jobjectArray, jobject>(value) {
	}

	MjuObjectArray(JNIEnv* env)
		: MjuArray<jobjectArray,jobject>(env) {
	}

	MjuObjectArray(JNIEnv* env, jobjectArray  value)
		: MjuArray<jobjectArray,jobject>(env, value) {
	}
private:
	// オブジェクトのコピーは禁止
	MjuObjectArray(MjuObject<jobjectArray>& value);
	MjuObjectArray(JNIEnv* env, MjuObject<jobjectArray>& value);

protected:
	virtual jobjectArray  _create(jsize length) {
		return _create(length, NULL);
	}

	jobjectArray  _create(jsize length, jclass clazz) {
		if (!clazz) {
			clazz = mEnv->FindClass("java/lang/Object");
			if (!clazz) {
				return NULL;
			}
		}
		return mEnv->NewObjectArray(length, clazz, NULL);
	}

	virtual jobject* _getArray(jobjectArray  jary) {
		return NULL;
	}

	virtual void _releaseArray(jobjectArray  jary, jobject* ary) {
		return NULL;
	}

	virtual bool _getRange(jobjectArray  jary, jsize start, jsize len, jobject* buff) {
		return false;
	}

	virtual bool _setRange(jobjectArray  jary, jsize start, jsize len, const jobject* buff) {
		return false;
	}
private:
	jobject* getArray() {
		return NULL;
	}

	void releaseArray() {
	}

	bool getRange(jsize start, jsize len, jobject* buff) {
		return false;
	}

	bool setRange(jsize start, jsize len, const jobject* buff) {
		return false;
	}

public:
	bool create(jsize length, jclass clazz) {
		dispose();
		mObj = _create(length, clazz);
		return mObj != NULL;
	}

	jobject getAt(jsize index) {
		if (!isAvailable()) {
			return NULL;
		}
		return mEnv->GetObjectArrayElement(mObj, index);
	}

	void setAt(jsize index, jobject elem) {
		if (!isAvailable()) {
			return NULL;
		}
		return mEnv->SetObjectArrayElement(mObj, index, elem);
	}
};
