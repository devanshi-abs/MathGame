/****************************************************************************
 Copyright (c) 2015-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
package com.mygame.mathgame;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.AlarmManager;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.MediaScannerConnection;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.Settings;
import android.speech.tts.TextToSpeech;
import android.util.Log;
import android.widget.Toast;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

public class AppActivity extends Cocos2dxActivity {

    //region DECLARATION

    public static final String MY_PREFS_NAME = "Cocos2dxPrefsFile";

    //TEXT TO SPEECH
    static TextToSpeech tts;
    public static TextToSpeech textToSpeech;

    //GET WRITE PERMISSION VARIABLES FOR PHOTOSAVE
    private static final int EXTERNAL_STORAGE_PERMISSION_CONSTANT = 100;
    private static final int REQUEST_PERMISSION_SETTING = 101;
    private static boolean isPermissionGranted = false;
    private static final int SAVE_IMAGE_REQUEST_CODE = 100;
    private boolean sentToSettings = false;
    private SharedPreferences permissionStatus;

    private static final int REQUEST_EXTERNAL_STORAGE = 1;
    private static String[] PERMISSIONS_STORAGE = {
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE
    };

    //THIS ACTIVIEY REFRENCE
    static AppActivity me;


    //region ADS TARGET INFORMATION
    private static int isFamilyTag;
    private static int isAgeOfConsent;
    private static String ageTag;
    private static boolean iskids;
    //endregion

    //ADAPTIVE BANNER FLAG CHECK
    //region ADS PLACEMENT AND TYPES
    private static boolean isRewardedAds;
    private static boolean BannerPosition;
    private static int AppOrientation;
    private boolean isAdaptiveBanner;
    //endregion

    //APP PACKAGE NAME
    public static String AppPackageName;


    public static String NotificationTitle;

    //region TEST MODE
    private static boolean isTestMode;
    //endregion

    //SHARED PREFERENCE
    public static SharedPreferences sharedPrefs;
    public static SharedPreferences.Editor prefsEditor;

    private static final int REQ_CODE_VERSION_UPDATE = 530;

    public static boolean isAppinforeground;

    public static boolean isFirstNativeAds;
    PendingIntent pendingIntent;

    //public static ProgressDialog loading;

    //Notification
    private static final long FIRST_TRIGGER_DELAY = 8 * 60 * 60 * 1000; // 8 hours
    private static final long REPEAT_INTERVAL = 2L * 24 * 60 * 60 * 1000; // 2 days
    //    private static final long FIRST_TRIGGER_DELAY = 30 * 1000; // 8 hours
//    private static final long REPEAT_INTERVAL = 30 * 1000; // 2 days
    private static final String NOTI_TAG = "LocalNotifications";
    //endregion

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);
        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }
        // Make sure we're running on Pie or higher to change cutout mode
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
//            // Enable rendering into the cutout area
//            WindowManager.LayoutParams lp = getWindow().getAttributes();
//            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
//            getWindow().setAttributes(lp);
//        }


    }


    @Override
    public void onStart() {

        isAppinforeground = true;
        super.onStart();

    }

    @Override
    public void onResume() {

        isAppinforeground = true;
        super.onResume();
    }


    public void onPause() {

        isAppinforeground = false;

        super.onPause();
    }

    @Override
    public void onStop() {
        super.onStop();
    }

    @Override
    public void onDestroy() {

        Log.d("DestroyClick", "onDestroy: ");
        super.onDestroy();
    }

    @Override
    public void onBackPressed() {
        // If an interstitial is on screen, close it.
        super.onBackPressed();
    }


        static {
        System.loadLibrary("MyGame");
    }
    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
      //  MultiDex.install(this);
    }

}
