#include "MjuLocal.h"
#include "MjuCharset.h"
#include <cstddef>

MjuLocal* MjuLocal::sInstance = NULL;

MjuLocal::MjuLocal(JNIEnv* env) {
	mEnv = env;
	mCharset = NULL;
	sInstance = this;
}

MjuLocal::~MjuLocal() {
	if (mCharset) {
		delete mCharset;
	}
	sInstance = NULL;
}

MjuCharset* MjuLocal::getCharset() {
	if (!this) {
		MjuThrowEnvException("MjuLocal is null.");
		return NULL;
	}
	if (!mEnv) {
		MjuThrowEnvException("MjuLocal is not initialized.");
		return NULL;
	}
	if (!mCharset) {
		mCharset = new MjuCharset(mEnv);
	}
	if (!(*mCharset)) {
		if (!mCharset->create()) {
			return NULL;
		}
	}
	return mCharset;

}
