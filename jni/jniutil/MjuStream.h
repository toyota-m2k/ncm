#pragma once

#include	"MjuTypeBase.h"
#include	"MjuArray.h"
#include	"MjuException.h"

#define INPUT_STREAM_CLASS_NAME "java/io/InputStream"
#define OUTPUT_STREAM_CLASS_NAME "java/io/OutputStream"



class MjuStream : public MjuTypeBase < jobject > {
private:	// private fields
	bool mAutoClose;

public:
	MjuStream(jobject obj, const char* clsname, bool autoClose = false)
		: MjuTypeBase<jobject>(obj, clsname)
		, mAutoClose(autoClose) {}

	MjuStream(JNIEnv* env, jobject obj, const char* clsname, bool autoClose = false)
		: MjuTypeBase<jobject>(env, obj, clsname)
		, mAutoClose(autoClose) {}

	/**
	* InputStream::close()
	*/
	bool close() {
		if (!mObj){
			return true;
		}

		MjuException jex(mEnv, true);
		jex.reset();
		jmethodID id = getMethodId("close", "()V");
		if (!id||jex.check()) {
			return false;
		}
		callVoid(id);
		if (jex.check()) {
			return false;
		}
		return true;
	}

	virtual void dispose() {
		if (mAutoClose) {
			close();
		}
		MjuTypeBase < jobject >::dispose();
	}
};

/**
 * java OutputStream のラッパー
 */
class MjuOutputStream : public MjuStream {
private:	// private fields
	jmethodID mWriteMethodID;
	jmethodID mFlushMethodID;

private:	// private methods
	/**
	* write(byte[],int,int) メソッドのiDを取得（何度も呼び出されるのでキャッシュする）
	*/
	jmethodID getWriteMethodID() {
		if (!mWriteMethodID) {
			mWriteMethodID = getMethodId("write", "([BII)V");
		}
		return mWriteMethodID;
	}

	/**
	* flush(byte[],int,int) メソッドのiDを取得（何度も呼び出されるのでキャッシュする）
	*/
	jmethodID getFlushMethodID() {
		if (!mFlushMethodID) {
			mFlushMethodID = getMethodId("flush", "()V");
		}
		return mFlushMethodID;
	}

public:
	MjuOutputStream(jobject obj, bool autoClose = false)
		: MjuStream(obj, OUTPUT_STREAM_CLASS_NAME, autoClose)
		, mWriteMethodID(NULL)
		, mFlushMethodID(NULL)
	{}

	MjuOutputStream(JNIEnv* env, jobject obj, bool autoClose = false)
		: MjuStream(env, obj, OUTPUT_STREAM_CLASS_NAME, autoClose)
		, mWriteMethodID(NULL)
		, mFlushMethodID(NULL)
	{}

	/**
	* InputStream::read(byte[])
	*/
	bool write(jbyteArray buffer, int len) {
		if (!mObj){
			return false;
		}

		MjuException jex(mEnv, true);
		jex.reset();
		jmethodID id = getWriteMethodID();
		if (jex.check() || !id) {
			return false;
		}

		callVoid(id, buffer, 0, len);
		if (jex.check()) {
			return false;
		}
		return true;
	}

	bool flush() {
		if (!mObj){
			return false;
		}

		MjuException jex(mEnv, true);
		jex.reset();
		jmethodID id = getFlushMethodID();
		if (jex.check() || !id) {
			return false;
		}

		callVoid(id);
		if (jex.check()) {
			return false;
		}
		return true;
	}

	/**
	以下は、当面不要なので実装しない。
	jint write(jbyteArray buffer);
	*/
};


/**
* java InputStream のラッパー
*/
class MjuInputStream : public MjuStream {
private:	// private fields
	jmethodID mReadMethodID;

private:	// private methods
	/**
	* read(byte[]) メソッドのiDを取得（何度も呼び出されるのでキャッシュする）
	*/
	jmethodID getReadMethodID() {
		if (!mReadMethodID) {
			mReadMethodID = getMethodId("read", "([B)I");
		}
		return mReadMethodID;
	}

public:
	MjuInputStream(jobject obj, bool autoClose = false)
		: MjuStream(obj, INPUT_STREAM_CLASS_NAME, autoClose)
		, mReadMethodID(NULL)
	{}

	MjuInputStream(JNIEnv* env, jobject obj, bool autoClose = false)
		: MjuStream(env, obj, INPUT_STREAM_CLASS_NAME, autoClose)
		, mReadMethodID(NULL)
	{}

	/**
	* InputStream::read(byte[])
	*/
	bool read(jbyteArray buffer, jint& lenR) {
		lenR = 0;
		if (!mObj){
			return false;
		}

		MjuException jex(mEnv, true);
		jex.reset();
		jmethodID id = getReadMethodID();
		if (jex.check() || !id) {
			return false;
		}

		lenR = callInt(id, buffer);
		if (jex.check()) {
			return false;
		}
		return true;
	}

	/**
	以下は、当面不要なので実装しない。
	jint read(jbyteArray buffer, jint byteOffset, jint byteCount);
	void mark(jint readlimit);
	void reset();
	jlong skip(jlong byteCount);
	*/
};
