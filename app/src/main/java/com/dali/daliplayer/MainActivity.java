package com.dali.daliplayer;

import android.app.Activity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends Activity {

    private RecyclerView recycler;
    private List<String> mediaLists;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initMediaLists();
        initRecyclerView();


    }

    private void initMediaLists() {
        mediaLists = new ArrayList<>();
        for (int i = 0; i < 10; i++) {
            mediaLists.add("Media " + i);
        }
    }

    private void initRecyclerView() {
        recycler = findViewById(R.id.recycler);
        recycler.setLayoutManager(new LinearLayoutManager(this));
        recycler.setAdapter(new MeidaAdapter(this, mediaLists));
    }

}
