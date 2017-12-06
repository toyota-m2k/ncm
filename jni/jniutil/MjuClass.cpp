#include "MjuClass.h"


bool MjuClass::initFromName(const char* className) {
	dispose();
	jclass clazz = mEnv->FindClass(className);
	if( !clazz ) {
		return false;
	}
	mObj = clazz;
	return true;
}

bool MjuClass::initByObject(jobject o) {
	dispose();
	jclass clazz = mEnv->GetObjectClass(o);
	if( !clazz ) {
		return false;
	}
	mObj = clazz;
	return true;
}

jmethodID MjuClass::getMethodId(const char* name, const char* sig, bool ofStatic) const {
	if (isAvailable()) {
		if (!ofStatic) {
			return mEnv->GetMethodID(mObj, name, sig);
		}
		else {
			return mEnv->GetStaticMethodID(mObj, name, sig);
		}
	}
	return NULL;
}

jfieldID MjuClass::getFieldId(const char* name, const char* sig, bool ofStatic) const {
	if (isAvailable()) {
		if (!ofStatic) {
			return mEnv->GetFieldID(mObj, name, sig);
		}
		else {
			return mEnv->GetStaticFieldID(mObj, name, sig);
		}
	}
	return NULL;
}

bool MjuClass::isInstanceOf(jobject obj) const {
	if (obj) {
		if (isAvailable()) {
			return mEnv->IsInstanceOf(obj, mObj)!=0;
		}
	}
	return false;
}

bool MjuClass::isAssignableFrom(jclass clazz) const{
	if (clazz) {
		if (isAvailable()) {
			return mEnv->IsAssignableFrom(clazz, mObj) != 0;
		}
	}
	return false;
}

/**
* 親クラスを取得
*/
jclass MjuClass::getSuperClass() const {
	if (isAvailable()) {
		return mEnv->GetSuperclass(mObj);
	}
	return NULL;
}

