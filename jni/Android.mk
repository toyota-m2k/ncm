LOCAL_PATH := .

include $(CLEAR_VARS)

LOCAL_MODULE    := NativeCM
LOCAL_SRC_FILES := \
	NativeCM.cpp \
	common/collection/TJsvArray.cpp \
	common/file/CmFile.cpp \
	common/file/JflTmpFile.cpp \
	common/stream/CstFileStream.cpp \
	common/stream/CstMemStream.cpp \
	common/stream/CstOLS.cpp \
	common/stream/CstStreamException.cpp \
	common/stream/CstStreamHelper.cpp \
	common/stream/CstStreamReader.cpp \
	common/stream/CstStreamWriter.cpp \
	common/stream/CstStringStream.cpp \
	common/string/JsvAString.cpp \
	common/string/JsvWString.cpp \
	common/string/TJsvAStringMap.cpp \
	common/string/TJsvString.cpp \
	common/string/TJsvStringBuffer.cpp \
	common/string/TJsvWStringMap.cpp \
	jniutil/MjuClass.cpp
LOCAL_CPPFLAGS := -fexceptions
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)
