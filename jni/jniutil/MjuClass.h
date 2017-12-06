#pragma once

#include "MjuObject.h"

class MjuClass : public MjuObject<jclass> {
public:
	/**
	* 空のオブジェクトを生成
	*	→ 使用前に、initFromName()または、initByObject()で初期化する。
	*/
	MjuClass() : MjuObject<jclass>() {}

	/**
	* jclassオブジェクトを与えてオブジェクトを生成
	*/
	MjuClass(jclass c) : MjuObject<jclass>(c) {}

	/**
	* クラス名を与えてオブジェクトを生成
	*/
	MjuClass(const char* className) : MjuObject<jclass>() { initFromName(className); }

	/**
	 * 空のオブジェクトを生成
	 *	→ 使用前に、initFromName()または、initByObject()で初期化する。
	 */
	MjuClass(JNIEnv* env) : MjuObject<jclass>(env) {}

	/**
	 * jclassオブジェクトを与えてオブジェクトを生成
	 */
	MjuClass(JNIEnv* env, jclass c) : MjuObject<jclass>(env, c) {}

	/**
	* クラス名を与えてオブジェクトを生成
	*/
	MjuClass(JNIEnv* env, const char* className) : MjuObject<jclass>(env) { initFromName(className); }

private:
	// オブジェクトのコピーは禁止
	MjuClass(MjuObject<jclass>& c);
	MjuClass(JNIEnv* env, MjuObject<jclass>& c);


public:
	/**
	 * クラス名を与えて初期化
	 */
	bool initFromName(const char* className);

	/**
	 * オブジェクトからクラスを取得して初期化
	 */
	bool initByObject(jobject o);

	/**
	 * メソッドIDを取得
	 */
	jmethodID getMethodId(const char* name, const char* sig, bool ofStatic = false) const;

	/**
	* フィールドIDを取得
	*/
	jfieldID getFieldId(const char* name, const char* sig, bool ofStatic = false) const;

	/**
	 * オブジェクトはクラスのサブクラスか？
	 */
	bool isInstanceOf(jobject obj) const;

	/**
	 * clazzは、このクラスにキャスト可能か（clazzはこのクラスのサブクラスか？）
	 */
	bool isAssignableFrom(jclass clazz) const;

	/**
	 * 親クラスを取得
	 */
	jclass getSuperClass() const;

};
