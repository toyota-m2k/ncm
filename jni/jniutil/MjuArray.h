#pragma once

#include "MjuObject.h"

/**
 * MjuArray class
 *
 * jxxxxArray のラッパークラスのテンプレート
 */
template <class TJ, class TC> class MjuArray : public MjuObject<TJ> {

protected:
	TC *	mNativeArray;

protected:
	/**
	 * 空のオブジェクトを作成
	 */
	MjuArray()
		: MjuObject<TJ>() {
		mNativeArray = NULL;
	}

	MjuArray(JNIEnv* env)
		: MjuObject<TJ>(env) {
		mNativeArray = NULL;
	}


	/**
	* jobjectArrayを引数に渡してオブジェクトを作成
	*/
	MjuArray(TJ jary)
		: MjuObject<TJ>(jary) {
		mNativeArray = NULL;

	}

	MjuArray(JNIEnv* env, TJ jary)
		: MjuObject<TJ>(env, jary) {
		mNativeArray = NULL;

	}
#if 0
	/**
	 * C配列をjobjectArrayにセットしてオブジェクトを作成
	 */
	MjuArray(const TC *ary, jsize len)
		: MjuObject<TJ>() {
		mNativeArray = NULL;
		if (create(len)) {
			setRange(0, len, ary);
		}
	}

	MjuArray(JNIEnv* env, const TC *ary, jsize len)
		: MjuObject<TJ>(env) {
		mNativeArray = NULL;
		if (create(len)) {
			setRange(0, len, ary);
		}
	}
#endif

private:

	// オブジェクトのコピーは禁止
	MjuArray(MjuObject<TJ>& s);
	MjuArray(JNIEnv* env, MjuObject<TJ>& s);

protected:
	virtual TJ _create(jsize length) = 0;

	virtual TC* _getArray(TJ jary) = 0;

	virtual void _releaseArray(TJ jary, TC* ary) = 0;

	virtual bool _getRange(TJ jary, jsize start, jsize len, TC* buff) const = 0;

	virtual bool _setRange(TJ jary, jsize start, jsize len, const TC* buff) = 0;

public:
	/**
	 * 配列の要素数を取得
	 */
	jsize getLength() const {
		if (MjuObject<TJ>::isAvailable()) {
			return MjuObject<TJ>::mEnv->GetArrayLength(MjuObject<TJ>::mObj);
		}
		return -1;
	}

	/**
	 * リソースを解放
	 */
	virtual void dispose() {
		releaseArray();
		MjuObject<TJ>::dispose();
	}

	/**
	 * 要素数を指定して、空の配列オブジェクトを作成
	 */
	bool create(jsize length) {
		dispose();
		MjuObject<TJ>::mObj = _create(length);
		return MjuObject<TJ>::mObj != NULL;
	}

	/**
	* 要素数を指定して、空の配列オブジェクトを作成
	*/
	bool create(TC* ary, jsize length) {
		dispose();
		MjuObject<TJ>::mObj = _create(length);
		if (!MjuObject<TJ>::mObj) {
			return false;
		}
		return setRange(0, length, ary);
	}

	/**
	 * Cの配列としてデータを取得
	 * 不要になれば、releaseArray()を呼ぶこと
	 */
	TC* getArray() {
		if (!MjuObject<TJ>::isAvailable()) {
			return NULL;
		}
		if (!mNativeArray) {
			mNativeArray = _getArray(MjuObject<TJ>::mObj);
		}
		return mNativeArray;
	}

	/**
	 * getArray()で取得した配列を解放する。
	 */
	void releaseArray() {
		if (mNativeArray) {
			_releaseArray(MjuObject<TJ>::mObj, mNativeArray);
			mNativeArray = NULL;
		}
	}

	/**
	 * 配列の範囲をバッファに読みだす。
	 */
	bool getRange(jsize start, jsize len, TC* buff) const {
		if (MjuObject<TJ>::isAvailable()) {
			return _getRange(MjuObject<TJ>::mObj, start, len, buff);
		}
		return false;
	}

	/**
	 * 配列の範囲を指定して、バッファの内容を書き込む
	 */
	bool setRange(jsize start, jsize len, const TC* buff) {
		if (MjuObject<TJ>::isAvailable()) {
			return _setRange(MjuObject<TJ>::mObj, start, len, buff);
		}
		return false;
	}

};

#define MJU_ARRAYTYPE_DEF(ArrayName, ArrayType, ElementType) \
	class Mju##ArrayName : public MjuArray<ArrayType, ElementType> {	\
	public: \
		Mju##ArrayName() : MjuArray<ArrayType, ElementType>() {}	\
		Mju##ArrayName(ArrayType value): MjuArray<ArrayType, ElementType>(value) {}	\
		Mju##ArrayName(JNIEnv* env) : MjuArray<ArrayType, ElementType>(env) {}	\
		Mju##ArrayName(JNIEnv* env, ArrayType value): MjuArray<ArrayType, ElementType>(env, value) {}	\
	protected:	\
		virtual ArrayType _create(jsize length) {return mEnv->New##ArrayName(length);}	\
		virtual ElementType* _getArray(ArrayType jary) {return mEnv->Get##ArrayName##Elements(jary, NULL);}	\
		virtual void _releaseArray(ArrayType jary, ElementType* ary) {return mEnv->Release##ArrayName##Elements(jary, ary, 0);}	\
		virtual bool _getRange(ArrayType jary, jsize start, jsize len, ElementType* buff) const {mEnv->Get##ArrayName##Region(jary, start, len, buff); return true;}	\
		virtual bool _setRange(ArrayType jary, jsize start, jsize len, const ElementType* buff) {mEnv->Set##ArrayName##Region(jary, start, len, buff);return true;}	\
	}

/**
 * MjuByteArray class
 */
MJU_ARRAYTYPE_DEF(ByteArray, jbyteArray, jbyte);

/**
 * MjuBooleanArray class
 */
MJU_ARRAYTYPE_DEF(BooleanArray, jbooleanArray, jboolean);

/**
 * MjuCharArray class
 */
MJU_ARRAYTYPE_DEF(CharArray, jcharArray, jchar);

/**
 * MjuShortArray class
 */
MJU_ARRAYTYPE_DEF(ShortArray, jshortArray, jshort);

/**
 * MjuIntArray class
 */
MJU_ARRAYTYPE_DEF(IntArray, jintArray, jint);

/**
 * MjuLongArray class
 */
MJU_ARRAYTYPE_DEF(LongArray, jlongArray, jlong);

/**
 * MjuFloatArray class
 */
MJU_ARRAYTYPE_DEF(FloatArray, jfloatArray, jfloat);

/**
 * MjuDoubleArray class
 */
MJU_ARRAYTYPE_DEF(DoubleArray, jdoubleArray, jdouble);
