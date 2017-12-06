#pragma once

#include "MjuObjectArray.h"
#include "MjuClass.h"
#include "MjuString.h"

class MjuStringArray : public MjuObjectArray {
public:
	MjuStringArray()
		: MjuObjectArray() {
	}

	MjuStringArray(jobjectArray  value)
		: MjuObjectArray(value) {
	}

	MjuStringArray(const char** pp, int count)
		: MjuObjectArray() {
		createByStrings(pp, count);
	}

	MjuStringArray(JNIEnv* env)
		: MjuObjectArray(env) {
	}

	MjuStringArray(JNIEnv* env, jobjectArray  value)
		: MjuObjectArray(env, value) {
	}

	MjuStringArray(JNIEnv* env, const char** pp, int count)
		: MjuObjectArray(env) {
		createByStrings(pp, count);
	}

private:
	// オブジェクトのコピーは禁止
	MjuStringArray(MjuObject<jobjectArray>& value);
	MjuStringArray(JNIEnv* env, MjuObject<jobjectArray>& value);

public:
	bool createByStrings(const char** pp, int count) {
		dispose();
		MjuString jstr(mEnv);
		if (!create(count, jstr.getClass())) {
			return false;
		}
		for (int i = 0; i < count; i++) {
			jstr.create(pp[i]);
			setAt(i, jstr.detach());
		}
		return true;
	}
}