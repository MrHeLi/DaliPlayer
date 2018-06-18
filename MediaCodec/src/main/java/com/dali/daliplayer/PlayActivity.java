package com.dali.daliplayer;

import android.app.Activity;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.annotation.Nullable;
import android.util.Log;

public class PlayActivity extends Activity{
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(this.getClass().getSimpleName(), "oncreate time = "
                + SystemClock.currentThreadTimeMillis());
        setContentView(R.layout.activity_play);
        XPlay play = findViewById(R.id.xplay);
        String url = getIntent().getStringExtra("url");
        play.setURL(url);
//        FrameLayout container = findViewById(R.id.container);
//        XPlay play = new XPlay(this, null);
//        play.setURL(url);
//        container.addView(play);

    }

//    @Override
//    protected void onResume() {
//        super.onResume();
//        String url = getIntent().getStringExtra("url");
//        FrameLayout container = findViewById(R.id.container);
//        XPlay play = new XPlay(this, null);
//        play.setURL(url);
//        container.addView(play);
//    }
}
