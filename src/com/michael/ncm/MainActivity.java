package com.michael.ncm;

import android.content.Context;
import android.util.Log;
import com.example.ncm.R;

import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.os.Build;

import java.io.*;
import java.util.Arrays;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (savedInstanceState == null) {
            getFragmentManager().beginTransaction()
                    .add(R.id.container, new PlaceholderFragment())
                    .commit();
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {
        private NcmUnitTest mTest = new NcmUnitTest();
		private final String mTestString = "Android's application framework lets you create rich and innovative apps using a set of reusable components. This section explains how you can build the components that define the building blocks of your app and how to connect them together using intents. As a user navigates through, out of, and back to your app, the Activity instances in your app transition between different states in their lifecycle. For instance, when your activity starts for the first time, it comes to the foreground of the system and receives user focus. During this process, the Android system calls a series of lifecycle methods on the activity in which you set up the user interface and other components. If the user performs an action that starts another activity or switches to another app, the system calls another set of lifecycle methods on your activity as it moves into the background (where the activity is no longer visible, but the instance and its state remains intact). Within the lifecycle callback methods, you can declare how your activity behaves when the user leaves and re-enters the activity. For example, if you're building a streaming video player, you might pause the video and terminate the network connection when the user switches to another app. When the user returns, you can reconnect to the network and allow the user to resume the video from the same spot.This class explains important lifecycle callback methods that each Activity instance receives and how you can use them so your activity does what the user expects and does not consume system resources when your activity doesn't need them.";

        public PlaceholderFragment() {
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                Bundle savedInstanceState) {
            View rootView = inflater.inflate(R.layout.fragment_main, container, false);
            View v = rootView.findViewById(R.id.doTest);
            v.setOnClickListener(new View.OnClickListener(){
                void compareFile(File f1, File f2) {
                    FileInputStream s = null, d = null;
                    try {
                        s = new FileInputStream(f1);
                        d = new FileInputStream(f2);
                        byte[] bs = new byte[256];
                        byte[] bd = new byte[256];

                        int is, id;
                        do {
                            is = s.read(bs);
                            id = d.read(bd);
                            if( is != id  || !Arrays.equals(bs, bd)) {
                                Log.e("UnitTest", "files are different.");
                                return;
                            }
                        } while(is>0);
                        Log.i("UnitTest", "files are same.");

                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        e.printStackTrace();
                    } finally {
                        try {
                            if( null != d) {
                                d.close();
                            }
                            if(null!=s) {
                                s.close();
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }


                }


                @Override
				public void onClick(View v) {

                    try {
						Context ctx = v.getContext();
						File tmpDir = ctx.getCacheDir();
                        File f = File.createTempFile("xxx", "dat",tmpDir );
                        FileOutputStream fo = new FileOutputStream(f);
                        try {
                            fo.write(mTestString.getBytes());
                        } finally {
                            fo.flush();
                            fo.close();
                        }
                        File f2 = File.createTempFile("yyy", "dat", tmpDir);
                        fo = new FileOutputStream(f2);
                        FileInputStream fi = new FileInputStream(f);
                        try {
                            mTest.unitTestRun(mTestString.getBytes(), fi, fo);
                        } finally {
                            fi.close();
                            fo.close();
                            compareFile(f, f2);

                            f.delete();
                            f2.delete();
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }



				}
			});

            v = rootView.findViewById(R.id.initLib);
            v.setOnClickListener(new View.OnClickListener(){
				
				@Override
				public void onClick(View v) {
					mTest.init(v.getContext());
				}
			});
            
            return rootView;
        }
    }

}
