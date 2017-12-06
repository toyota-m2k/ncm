#pragma once

#include "MjuObject.h"
#include "MjuClass.h"

// Note:
// C++ の例外を使用する場合は、コンパイラオプション -fexception を指定する。
//
// ■C++ の例外を使用しない場合のコーディングパターン
//
//	MjuException ex(env);
//	...(JNIの処理）...
//	if( ex.check() ) {
//		例外発生時の処理
//		if(致命的エラー）{
//			return false; // 処理中止
//		} else { //回復可能なエラー
//			ex.reset();
//		}
//	}
//	...
//	ex.reset();
//	return true;
//
// ■C++ の例外を使用する場合のコーディングパターン
//
//	MjuException ex(env);
//  try {
//		...（JNIの処理）...
//		ex.throwOnError();			// 例外をすべてエラーにするなら、
//		...（JNIの処理）...
//		if( ex.check()) {			// 例外の型によってリカバリーするなら、
//			if(致命的エラー）{
//				ex.throwOnError(); // 処理中止
//			} else { //回復可能なエラー
//				ex.reset();
//			}
//		}
//		return true;
//	} catch(jthrowable e) {
//		return false; // 処理中止
//  }


#define MJE_EXCEPTION					__mjuExecption
#define MJE_INIT_EXCEPTION(env)			MjuException MJE_EXCEPTION(env)
#define MJE_CHECK_EXCEPTION				MJE_EXCEPTION.check()
#define MJE_RESET_EXCEPTION				MJE_EXCEPTION.reset()
#define MJE_THROW_ON_EXCEPTION			MJE_EXCEPTION.checkAndThrow()

/**
 * jthrowableのラッパークラス
 */
class MjuException : public MjuObject<jthrowable>{

private:
	MjuClass mExceptionClass;
	bool mAutoReset;
public:
	/**
	 * 空のオブジェクトを生成
	 */
	MjuException(bool autoReset=false)
		: MjuObject<jthrowable>()
		, mExceptionClass() {
		check();
		mAutoReset = autoReset;
	}

	/**
	* jthrowableを与えてオブジェクトを生成
	*/
	MjuException(jthrowable e)
		: MjuObject<jthrowable>(e)
		, mExceptionClass() {
		mAutoReset = false;
	}

	/**
	 * 空のオブジェクトを生成
	 */
	MjuException(JNIEnv* env, bool autoReset = false)
		: MjuObject<jthrowable>(env)
		, mExceptionClass(env) {
		check();
		mAutoReset = autoReset;
	}

	/**
	 * jthrowableを与えてオブジェクトを生成
	 */
	MjuException(JNIEnv* env, jthrowable e)
		: MjuObject<jthrowable>(env, e)
		, mExceptionClass(env) {
	}

	void SetAutoReset(bool sw = true) {
		mAutoReset = sw;
	}
	
	~MjuException() {
		if (mAutoReset) {
			reset();
		}
	}

private:
	// オブジェクトのコピーは禁止
	MjuException(MjuObject<jthrowable>& e);
	MjuException(JNIEnv* env, MjuObject<jthrowable>& e);

public:
	/**
	 * 例外が発生しているかどうかチェック
	 */
	bool check() {
		if (mObj) {
			return true;
		}
		if (mEnv->ExceptionCheck()) {
			mObj = mEnv->ExceptionOccurred();
			return true;
		}
		return false;
	}

	/**
	 * 例外が発生していればクリア
	 */
	bool reset() {
		bool ex = false;
		if (mEnv->ExceptionCheck()) {
			mEnv->ExceptionClear();
			ex = true;
		}
		dispose();
	}

#ifdef __EXCEPTIONS
	/**
	 * 例外が発生していれば c++ 例外を投げる
	 */
	void throwOnError() {
		if (check()) {
			throw detach();
		}
	}
#endif

	/**
	 * リソース解放
	 */
	virtual void dispose() {
		mExceptionClass.dispose();
		MjuObject<jthrowable>::dispose();
	}

	/**
	 * 例外のクラス情報を返す
	 */
	const MjuClass& getExceptionClass() {
		if (isAvailable()) {
			if (!mExceptionClass.isAvailable()) {
				mExceptionClass.initByObject((jthrowable)mObj);
			}
		}
		return mExceptionClass;
	}

	/**
	 * java例外を発生させる
	 *
	 * @param exceptionClassName 例外クラスの名前（nullならRuntimeExceptionを使用)
	 * @param message 例外にセットするメッセージ
	 * @param andThrow  true:c++例外も投げる
	 */
	void raiseException(char* exceptionClassName, char* message
#ifdef __EXCEPTIONS
		, bool andThrow
#endif
		) {
		reset();
		MjuClass excls(mEnv);
		if (exceptionClassName) {
			excls.initFromName(exceptionClassName);
		}
		if (!excls.isAvailable()) {
			excls.initFromName("java/lang/RuntimeException");
			if (!excls.isAvailable()){
				mEnv->FatalError(message);
			}
		}
		mEnv->ThrowNew(excls, message);
		if (!check()) {
			mEnv->FatalError(message);
		}
#ifdef __EXCEPTIONS
		if (andThrow) {
			throw detach();
		}
#endif
	}
};