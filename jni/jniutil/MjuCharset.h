#pragma once

#include	"MjuString.h"
#include	"MjuArray.h"
#include	"MjuException.h"
#include	"../common/String.h"

#define CHARSET_CONVERTER_CLASS_NAME "com/michael/ncm/CmCharConverter"



class MjuCharset : public MjuTypeBase < jobject > {
private:
	jmethodID mMidEncode;
	jmethodID mMidDecode;
	jmethodID mMidSupported;
	jmethodID mMidCanonicalName;

	jmethodID getEncodeMethodId() {
		if (!mMidEncode) {
			mMidEncode = getMethodId("encode", "(Ljava/lang/String;Ljava/lang/String;)[B");
		}
		return mMidEncode;
	}

	jmethodID getDecodeMethodId() {
		if (!mMidDecode) {
			mMidDecode = getMethodId("decode", "([BLjava/lang/String;)Ljava/lang/String;");
		}
		return mMidDecode;
	}

	jmethodID getIsSupportedMethodID() {
		if (!mMidSupported) {
			mMidSupported = getMethodId("isSupported", "(Ljava/lang/String;)Z");
		}
		return mMidSupported;
	}

	jmethodID getCanonicalNameMethodID() {
		if (!mMidCanonicalName) {
			mMidCanonicalName = getMethodId("getCanonicalCharsetName", "(Ljava/lang/String;)Ljava/lang/String;");
		}
		return mMidCanonicalName;
	}

public:
	MjuCharset() : MjuTypeBase<jobject>(CHARSET_CONVERTER_CLASS_NAME) {
		mMidDecode = mMidEncode = mMidSupported = mMidCanonicalName = 0;
	}

	MjuCharset(JNIEnv* env) : MjuTypeBase<jobject>(env, CHARSET_CONVERTER_CLASS_NAME) {
		mMidDecode = mMidEncode = mMidSupported = mMidCanonicalName = 0;
	}

	bool create() {
		dispose();
		jmethodID id = getConstructorMethodId("()V");
		if (!id) {
			return false;
		}
		return MjuTypeBase<jobject>::create(id);
	}

	bool isSupported(LPCSTR charsetName) {
		MjuException jex(mEnv, true);
		MjuString jcs(mEnv, charsetName);
		if (jex.check()) {
			return false;
		}
		jmethodID mid = getIsSupportedMethodID();
		if (!mid || jex.check()) {
			return false;
		}

		jboolean r = callBoolean(mid, (jstring)jcs);
		return (r && !jex.check());
	}

	CStringA getCanonicalCharsetName(LPCSTR charsetName) {
		CStringA rstr;
		MjuException jex(mEnv, true);
		MjuString jcs(mEnv, charsetName);
		if (jex.check()) {
			return rstr;
		}
		jmethodID mid = getCanonicalNameMethodID();
		if (!mid || jex.check()) {
			return rstr;
		}

		MjuString sr(mEnv, (jstring)callObject(mid, (jstring)jcs));
		if (jex.check() || !sr.isAvailable() || !sr.verifyObjectType()) {
			return rstr;
		}

		rstr = sr.getUtf8Chars();
		if (jex.check()) {
			rstr.Empty();
		}
		return rstr;
	}

	bool encode(LPCSTR src, CStringA& dst, LPCSTR charsetName) {
		MjuException jex(mEnv, true);
		MjuString jstr(mEnv, src);
		if (jex.check()) {
			return false;
		}

		return encode((jstring)jstr, dst, charsetName);
	}

	/**
	 * jstring(Java.Lang.String)を 指定された文字セットでdstに取得する。
	 * @param src	入力jstring
	 * @param dst	出力バッファ
	 * @param charsetName
	 */
	bool encode(jstring src, CStringA& dst, LPCSTR charsetName) {
		MjuException jex(mEnv, true);

		jmethodID mid = getEncodeMethodId();
		if (!mid || jex.check() ) {
			return false;
		}

		MjuString jcs(mEnv, charsetName);
		if (jex.check()) {
			return false;
		}

		jbyteArray r = (jbyteArray)callObject(mid, src, (jstring)jcs);
		if (!r || jex.check()) {
			return false;
		}

		MjuByteArray ar(mEnv, r);
		jint len = ar.getLength();
		ar.getRange(0, len, (jbyte*)dst.GetBuffer(len));
		if (jex.check()) {
			return false;
		}

		dst.ReleaseBuffer(len);
		return true;
	}

	bool decode(LPCBYTE src, int len, CStringA& dst, LPCSTR charsetName) {
		MjuException jex(mEnv, true);
		MjuByteArray jary(mEnv);
		if (!jary.create((jbyte*)src, len)) {
			return false;
		}
		if (jex.check()) {
			return false;
		}

		return decode(jary, dst, charsetName);
	}

	bool decode(jbyteArray src, CStringA& dst, LPCSTR charsetName) {
		MjuException jex(mEnv, true);

		jmethodID mid = getDecodeMethodId();
		if (!mid || jex.check()) {
			return false;
		}

		MjuString jcs(mEnv, charsetName);
		if (jex.check()) {
			return false;
		}

		MjuString jstr(mEnv, (jstring)callObject(mid, src, (jstring)jcs));
		if (jex.check()) {
			return false;
		}
		dst = jstr.getUtf8Chars();
		if (jex.check()) {
			return false;
		}

		return true;
	}
};