#pragma once

#include "MjuObject.h"
#include "MjuClass.h"

#define DEF_FIELD_METHOD(VALTYPE, TYPELABEL)   \
	VALTYPE get##TYPELABEL##Field() { return MjuObject<T>::mEnv->Get##TYPELABEL##Field(); }	\
	void set##TYPELABEL##Field(VALTYPE value) { MjuObject<T>::mEnv->Set##TYPELABEL##Field(value); } \
	VALTYPE getStatic##TYPELABEL##Field() { return MjuObject<T>::mEnv->GetStatic##TYPELABEL##Field(); }	\
	void setStatic##TYPELABEL##Field(VALTYPE value) { MjuObject<T>::mEnv->SetStatic##TYPELABEL##Field(value); }

#define DEF_CALLFUNC_METHOD(VALTYPE, TYPELABEL)   \
	VALTYPE call##TYPELABEL(jmethodID id, ...) {  \
		va_list argList;	\
		va_start(argList, id);	\
		VALTYPE r = MjuObject<T>::mEnv->Call##TYPELABEL##MethodV(MjuObject<T>::mObj, id, argList);	\
		va_end(argList);	\
		return r;	\
	}	\
	VALTYPE callStatic##TYPELABEL(jmethodID id, ...) {  \
		va_list argList;	\
		va_start(argList, id);	\
		VALTYPE r = MjuObject<T>::mEnv->CallStatic##TYPELABEL##MethodV(MjuObject<T>::mObj, id, argList);	\
		va_end(argList);	\
		return r;	\
	}


template <class T>
class MjuTypeBase : public MjuObject<T> {
protected: 
	const char* mClassName;
	MjuClass mCls;

public:
	/**
	* 空のオブジェクトを生成
	*/
	MjuTypeBase(const char* className)
		: MjuObject<T>()
		, mCls() {
		mClassName = className;
	}

	/**
	* データを指定してオブジェクトを生成
	*/
	MjuTypeBase(T obj, const char* className)
		: MjuObject<T>(obj)
		, mCls() {
		mClassName = className;
	}

	/**
	 * 空のオブジェクトを生成
	 */
	MjuTypeBase(JNIEnv* env, const char* className) 
		: MjuObject<T>(env)
		, mCls(env) {
		mClassName = className;
	}

	/**
	 * データを指定してオブジェクトを生成
	 */
	MjuTypeBase(JNIEnv* env, T obj, const char* className)
		: MjuObject<T>(env, obj)
		, mCls(env) {
		mClassName = className;
	}
private:
	// オブジェクトのコピーは禁止
	MjuTypeBase(MjuObject<T>& src, const char* className);
	MjuTypeBase(JNIEnv*e, MjuObject<T>& src, const char* className);


public:
	/**
	 * クラス情報を初期化する
	 */
	bool initClass() {
		if( !mCls ) {
#if 0
			if (!mClassName) {
				return false;
			}
			return mCls.initFromName(mClassName);
#else
			if( !MjuObject<T>::mObj ) {
				if (!mClassName) {
					return false;
				}
				return mCls.initFromName(mClassName);
			} else {
				return mCls.initByObject(MjuObject<T>::mObj);
			}
#endif
		}
		return true;
	}

	bool verifyObjectType() {
		if (!MjuObject<T>::mObj) {
			return false;
		}
		if (!initClass()) {
			return false;
		}
		return mCls.isInstanceOf(MjuObject<T>::mObj);

	}
public:

	/**
	* クラス情報を取得
	*/
	const MjuClass& getClass() {
		initClass();
		return mCls;
	}

	/**
	 * メソッドIDを取得
	 */
	jmethodID getMethodId(const char* name, const char* sig) {
		if (!MjuObject<T>::mEnv || !initClass()) {
			return NULL;
		}
		return mCls.getMethodId(name, sig);
	}

	/**
	 * コンストラクタのメソッドIDを取得
	 */
	jmethodID getConstructorMethodId(const char* sig) {
		return getMethodId("<init>", sig);
	}


	/**
	 * インスタンスを生成 (STATIC)
	 */
	static jobject createInstance(JNIEnv* env, jclass cls, jmethodID id, va_list argList) {
		if (env && cls && id) {
			return env->NewObjectV(cls, id, argList);
		}
		else {
			return NULL;
		}
	}

	/**
	* インスタンスを生成 (STATIC)
	*/
	static jobject createInstance(JNIEnv* env, jclass cls, jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jobject r = createInstance(env, cls, id, argList);
		va_end(argList);
	}

	/**
	 * インスタンスを破棄する
	 */
	virtual void dispose() {
		MjuObject<T>::dispose();
	}

	/**
	 * インスタンスを生成する
	 */
	bool create(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		
		dispose();
		initClass();
		MjuObject<T>::mObj = (T)createInstance(MjuObject<T>::mEnv, mCls, id, argList);
		va_end(argList);
		return MjuObject<T>::isAvailable();
	}

	//-----------------------------------------------------------------------------------------------
	// メソッド呼び出し系メソッド群
	//-----------------------------------------------------------------------------------------------
	DEF_CALLFUNC_METHOD(jobject, Object);		// callObject	/ callStaticObject
	DEF_CALLFUNC_METHOD(jboolean, Boolean);		// callBoolean	/ callStaticBoolean
	DEF_CALLFUNC_METHOD(jbyte, Byte);			// callByte		/ callStaticByte
	DEF_CALLFUNC_METHOD(jchar, Char);			// callChar		...
	DEF_CALLFUNC_METHOD(jshort, Short);
	DEF_CALLFUNC_METHOD(jint, Int);
	DEF_CALLFUNC_METHOD(jlong, Long);
	DEF_CALLFUNC_METHOD(jfloat, Float);
	DEF_CALLFUNC_METHOD(jdouble, Double);

	void callVoid(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		MjuObject<T>::mEnv->CallVoidMethodV(MjuObject<T>::mObj, id, argList);
		va_end(argList);
	}

	void callStaticVoid(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		MjuObject<T>::mEnv->CallStatocVoidMethodV(MjuObject<T>::mCls, id, argList);
		va_end(argList);
	}

	//-----------------------------------------------------------------------------------------------
	// フィールドアクセス
	//-----------------------------------------------------------------------------------------------
	DEF_FIELD_METHOD(jobject, Object);			// getObjectField() / getStaticObjectField()
	DEF_FIELD_METHOD(jboolean, Boolean);		// getBooleanField() / getStaticBooleanField()
	DEF_FIELD_METHOD(jbyte, Byte);				// getByteField() ...
	DEF_FIELD_METHOD(jchar, Char);
	DEF_FIELD_METHOD(jshort, Short);
	DEF_FIELD_METHOD(jint, Int);
	DEF_FIELD_METHOD(jlong, Long);
	DEF_FIELD_METHOD(jfloat, Float);
	DEF_FIELD_METHOD(jdouble, Double);

#if false
	jobject callObject(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jobject r = MjuObject<T>::mEnv->CallObjectMethodV(MjuObject<T>::mObj, id, argList);
		va_end(argList);
		return r;
	}

	void callVoid(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		MjuObject<T>::mEnv->CallVoidMethodV(MjuObject<T>::mObj, id, argList);
		va_end(argList);
	}

	jboolean callBoolean(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jboolean r = MjuObject<T>::mEnv->CallBooleanMethodV(MjuObject<T>::mObj, id, argList);
		va_end(argList);
		return r;
	}

	jbyte callByte(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jbyte r = MjuObject<T>::mEnv->CallByteMethodV(MjuObject<T>::mObj, id, argList);
		va_end(argList);
		return r;
	}

	jint callInt(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jint r = MjuObject<T>::mEnv->CallIntMethodV(MjuObject<T>::mObj, id, argList);
		va_end(argList);
		return r;
	}

	jlong callLong(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jlong r = MjuObject<T>::mEnv->CallLongMethodV(MjuObject<T>::mObj, id, argList);
		va_end(argList);
		return r;
	}

	jfloat callFloat(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jfloat r = MjuObject<T>::mEnv->CallFloatMethodV(MjuObject<T>::mObj, id, argList);
		va_end(argList);
		return r;
	}

	jdouble callDouble(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jdouble r = MjuObject<T>::mEnv->CallDoubleMethodV(MjuObject<T>::mObj, id, argList);
		va_end(argList);
		return r;
	}





	jobject callStaticObject(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jobject r = MjuObject<T>::mEnv->CallStaticObjectMethodV(MjuObject<T>::mCls, id, argList);
		va_end(argList);
		return r;
	}

	void callStaticVoid(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		MjuObject<T>::mEnv->CallStatocVoidMethodV(MjuObject<T>::mCls, id, argList);
		va_end(argList);
	}

	jboolean callStaticBoolean(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jboolean r = MjuObject<T>::mEnv->CallStaticBooleanMethodV(MjuObject<T>::mCls, id, argList);
		va_end(argList);
		return r;
	}

	jbyte callStaticByte(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jbyte r = MjuObject<T>::mEnv->CallStaticByteMethodV(MjuObject<T>::mCls, id, argList);
		va_end(argList);
		return r;
	}

	jint callStaticInt(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jint r = MjuObject<T>::mEnv->CallStaticIntMethodV(MjuObject<T>::mCls, id, argList);
		va_end(argList);
		return r;
	}

	jlong callStaticLong(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jlong r = MjuObject<T>::mEnv->CallStaticLongMethodV(MjuObject<T>::mCls, id, argList);
		va_end(argList);
		return r;
	}

	jfloat callStaticFloat(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jfloat r = MjuObject<T>::mEnv->CallStaticFloatMethodV(MjuObject<T>::mCls, id, argList);
		va_end(argList);
		return r;
	}

	jdouble callStaticDouble(jmethodID id, ...) {
		va_list argList;
		va_start(argList, id);
		jdouble r = MjuObject<T>::mEnv->CallStaticDoubleMethodV(MjuObject<T>::mCls, id, argList);
		va_end(argList);
		return r;
	}
#endif


};

