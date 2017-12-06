package com.michael.ncm;

import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.util.Arrays;

/**
 * CmCharConverter
 *
 * @author Created by M.TOYOTA on 2014.06.17
 * @author Copyright (C) 2014 M.TOYOTA. All rights reserved.
 */
public class CmCharConverter {
    /**
     * 文字セットはサポートされているか？
     *
     * @param charsetName       文字セット名
     * @return  true / false
     */
    public boolean isSupported(String charsetName) {
        try {
            return Charset.isSupported(charsetName);
        } catch( Throwable e) {
            return false;
        }
    }

    /**
     * charsetNameの標準名を取得する
     *
     * @param charsetName
     * @return  標準名
     */
    public String getCanonicalCharsetName(String charsetName) {
        try {
            if(!Charset.isSupported(charsetName) ) {
                return null;
            }
            return Charset.forName(charsetName).displayName();
        } catch( Throwable e) {
            return null;
        }
    }

    /**
     * String(UTF8文字列)を 指定された文字コードにエンコードして、byte[]に出力して返す。
     *
     * @param str               エンコード対象文字列
     * @param charsetName       文字セット名
     * @return                  エンコードされたバイト列
     */
    public byte[] encode(String str, String charsetName) {
        try {
            if(!Charset.isSupported(charsetName) ) {
                return null;
            }
			ByteBuffer raw = Charset.forName(charsetName).encode(str);
			byte[] r = raw.array();
			if( raw.limit() == r.length ){
				return r;
			} else {
				return Arrays.copyOf(r,raw.limit());
			}
        } catch(Throwable e) {
            Log.e("CM", "charset encoding error.", e);
            return null;
        }
    }

    /**
     * byte[]データをcharsetName文字コードとしてデコードし、UTF-8文字列(String)として返す
     *
     * @param data              デコード対象文字列
     * @param charsetName       文字セット名
     * @return                  デコードされたUTF8文字列
     */
    public String decode(byte[] data, String charsetName) {
        try {
            if(!Charset.isSupported(charsetName) ) {
                return null;
            }
            return Charset.forName(charsetName).decode(ByteBuffer.wrap(data)).toString();
        } catch(Throwable e) {
            Log.e("CM", "charset decoding error.", e);
            return null;
        }
    }
}
