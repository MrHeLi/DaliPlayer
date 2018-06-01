package com.dali.daliplayer;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.widget.Toast;

import com.dali.utils.FileUtils;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends Activity {
    private String Tag = MainActivity.class.getSimpleName();
    private RecyclerView recycler;
    private List<String> mediaLists;
    private MeidaAdapter meidaAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initMediaLists();
        initRecyclerView();


    }

    private void initMediaLists() {//"/storage/emulated/Movies"
        String path = Environment.getExternalStorageDirectory().getAbsolutePath();
        FileUtils.loadMedias(path);
        FileUtils.setOnFileLoadedListener(new FileUtils.OnFileLoadListener() {
            @Override
            public void onLoad(final List<String> medias) {
                for (int i = 0; i < medias.size(); i++) {
                    if (!mediaLists.contains(medias.get(i))) {
                        mediaLists.add(medias.get(i));
                    }
                }
                meidaAdapter.notifyDataSetChanged();
            }
        });
        Toast.makeText(this, path, Toast.LENGTH_SHORT).show();
        mediaLists = new ArrayList<>();
    }

    private void initRecyclerView() {
        recycler = findViewById(R.id.recycler);
        meidaAdapter = new MeidaAdapter(this, mediaLists);
        recycler.setLayoutManager(new LinearLayoutManager(this));
        recycler.setAdapter(meidaAdapter);
    }

}
