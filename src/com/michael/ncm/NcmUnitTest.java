package com.michael.ncm;

import android.util.Log;
import android.content.Context;

import java.io.InputStream;
import java.io.OutputStream;

public class NcmUnitTest {

	boolean init = false;
	public void init(Context ctx) {
		if( !init ) {
			try {
				System.loadLibrary("NcmUnitTest");
			} catch( Throwable e) {
				Log.e("NCM", "loadLibrary error.", e);
			}
			unitTestInit(ctx.getCacheDir().getAbsolutePath());
			init = true;
		}
	}
	public native boolean unitTestRun(byte[] testByteArray, InputStream istream, OutputStream ostream);
	public native void unitTestInit(String path);
}
