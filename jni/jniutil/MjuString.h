#pragma once

#include "MjuTypeBase.h"

typedef char utf8char;

class MjuString : public MjuTypeBase<jstring> {
public:
	static const char* CLASS_NAME;
private:
	const utf8char*	mUtf8String;

private:
	void releaseUtfString() {
		if (isAvailable() && mUtf8String) {
			mEnv->ReleaseStringUTFChars(mObj, mUtf8String);
			mUtf8String = NULL;
		}
	}
public:
	/**
	* 空のオブジェクトを生成（あとから jstringをセットするか、create()する）
	*/
	MjuString()
		: MjuTypeBase<jstring>(CLASS_NAME) {
		mUtf8String = NULL;
	}

	/**
	* jstringを与えて初期化する
	*/
	MjuString(jstring value)
		: MjuTypeBase<jstring>(value, CLASS_NAME) {
		mUtf8String = NULL;
	}

	/**
	* UTF8文字列を与えて初期化する
	*/
	MjuString(const utf8char* utf8String)
		: MjuTypeBase<jstring>(CLASS_NAME) {
		mUtf8String = NULL;
		mObj = mEnv->NewStringUTF(utf8String);
	}


	/**
	 * 空のオブジェクトを生成（あとから jstringをセットするか、create()する）
	 */
	MjuString(JNIEnv* env)
		: MjuTypeBase<jstring>(env, CLASS_NAME) {
		mUtf8String = NULL;
	}

	/**
	 * jstringを与えて初期化する
	 */
	MjuString(JNIEnv* env, jstring value) 
		: MjuTypeBase<jstring>(env, value, CLASS_NAME) {
		mUtf8String = NULL;
	}

	/**
	 * UTF8文字列を与えて初期化する
	 */
	MjuString(JNIEnv* env, const utf8char* utf8String)
		: MjuTypeBase<jstring>(env, env->NewStringUTF(utf8String), CLASS_NAME) {
		mUtf8String = NULL;
	}
private:
	// オブジェクトのコピーは禁止
	MjuString(MjuObject<jstring>& value);
	MjuString(JNIEnv* env, MjuObject<jstring>& value);


public:
	/**
	 * jstringオブジェクトを取得する
	 */
	jstring getJstring() {
		return mObj;
	}

	/**
	 * オブジェクトを解放する
	 */
	virtual void dispose() {
		releaseUtfString();
		MjuTypeBase<jstring>::dispose();
	}

	/**
	 * オブジェクトを切り離す
	 */
	virtual jstring detach() {
		releaseUtfString();
		return MjuTypeBase<jstring>::detach();
	}

	/**
	 * UTF16 文字列の長さ
	 */
	jsize length() {
		if (!isAvailable()) {
			return -1;
		}
		return mEnv->GetStringLength(mObj);
	}



	/**
	 * UTF8 文字列（バイト列）として取得
	 */
	const utf8char* getUtf8Chars() {
		if (!mUtf8String){
			if (isAvailable()) {
				mUtf8String = mEnv->GetStringUTFChars(mObj, NULL);
			}
		}
		return mUtf8String;
	}


	/**
	 * デフォルトのエンコーディングで byte配列を取得
	 */
	jbyteArray getBytes() {
#if 1
		return getBytes("UTF-8");
#else
		if (!isAvailable()) {
			return NULL;
		}
		jmethodID id = getMethodId("getBytes", "()[B");
		if (id) {
			return (jbyteArray)callObject(id);
		}
		return NULL;
#endif
	}

	/**
	 * エンコーディングを指定して、byte配列を取得
	 */
	jbyteArray getBytes(const utf8char* charset) {
		MjuString cs(mEnv, charset);
		return getBytes(cs);
	}

	jbyteArray getBytes(jstring charset) {
		if (!isAvailable()) {
			return NULL;
		}
		jmethodID id = getMethodId("getBytes", "(Ljava/lang/String;)[B");
		if (id) {
			return (jbyteArray)callObject(id, charset);
		}
		return NULL;
	}

	bool create(const utf8char* utf8String) {
		dispose();
		mObj = mEnv->NewStringUTF(utf8String);
		return isAvailable();
	}

	/**
	 * デフォルトのエンコーディングのバイト配列からStringを構築
	 */
	bool create(jbyteArray bytes) {
#if 1
		return create(bytes, "UTF-8");
#else
		jmethodID id = getMethodId("<init>", "([B)V");
		if (NULL != id) {
			return MjuTypeBase<jstring>::create(id, bytes);
		}
		return false;
#endif
	}

	/**
	 * エンコーディングを指定してバイト配列からStringを構築
	 */
	bool create(jbyteArray bytes, const utf8char* charset) {
		MjuString cs(mEnv, charset);
		return create(bytes, cs);
	}

	/**
	 * エンコーディングを指定してバイト配列からStringを構築
	 */
	bool create(jbyteArray bytes, jstring charset) {
		dispose();
		jmethodID id = getMethodId("<init>", "([BLjava/lang/String;)V");
		if (NULL != id) {
			return MjuTypeBase<jstring>::create(id, bytes, charset);
		}
		return false;
	}

	bool equals(jstring dst) {
		jmethodID id = getMethodId("equals", "(Ljava/lang/Object;)Z");
		if (NULL != id) {
			return callBoolean(id, dst);
		}
		return false;
	}

	bool equalsIgnoreCase(jstring dst) {
		jmethodID id = getMethodId("equalsIgnoreCase", "(Ljava/lang/String;)Z");
		if (NULL != id) {
			return callBoolean(id, dst);
		}
		return false;
	}
};
