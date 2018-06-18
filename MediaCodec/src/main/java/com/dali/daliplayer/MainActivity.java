package com.dali.daliplayer;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;

import com.dali.utils.FileUtils;
import com.dali.utils.MediaFile;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements MediaAdapter.OnItemClickListener{
    private String Tag = MainActivity.class.getSimpleName();
    private RecyclerView recycler;
    private List<MediaFile> mediaLists;
    private MediaAdapter meidaAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        permission();
    }

    private void init() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                initMediaLists();
                initRecyclerView();
            }
        });
    }

    private void permission() {
        if (!(ContextCompat.checkSelfPermission(this,
                Manifest.permission.READ_EXTERNAL_STORAGE) ==
                PackageManager.PERMISSION_GRANTED)) {
            ActivityCompat.requestPermissions(this,
                    new String[] {Manifest.permission.READ_EXTERNAL_STORAGE}, 1);
        } else {
            init();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == 1) {
            int grantResult = grantResults[0];
            boolean granted = grantResult == PackageManager.PERMISSION_GRANTED;
            init();
        }
    }

    private void initMediaLists() {//"/storage/emulated/Movies"
        String path = Environment.getExternalStorageDirectory().getAbsolutePath();
        FileUtils.loadMedias(path + File.separator + "Movies");
        FileUtils.setOnFileLoadedListener(new FileUtils.OnFileLoadListener() {
            @Override
            public void onLoad(final List<MediaFile> medias) {
                for (int i = 0; i < medias.size(); i++) {
                    if (!mediaLists.contains(medias.get(i))) {
                        mediaLists.add(medias.get(i));
                    }
                }
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        meidaAdapter.notifyDataSetChanged();
                    }
                });
            }
        });
        mediaLists = new ArrayList<>();
    }

    private void initRecyclerView() {
        recycler = findViewById(R.id.recycler);
        meidaAdapter = new MediaAdapter(this, mediaLists);
        meidaAdapter.setOnItemClickListener(this);
        recycler.setLayoutManager(new LinearLayoutManager(this));
        recycler.setAdapter(meidaAdapter);
    }

    @Override
    public void onItemClick(View view, int position) {
        FFmpegJni.play(mediaLists.get(position).getPath());
    }
}
