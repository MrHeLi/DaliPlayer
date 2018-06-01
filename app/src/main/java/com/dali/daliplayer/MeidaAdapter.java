package com.dali.daliplayer;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import java.util.List;

class MeidaAdapter extends RecyclerView.Adapter<MeidaAdapter.MediaHolder> {
    private LayoutInflater inflater;
    private List<String> mediaLists;

    public MeidaAdapter(Context context, List<String> mediaLists) {
        inflater = LayoutInflater.from(context);
        this.mediaLists = mediaLists;
    }

    @NonNull
    @Override
    public MediaHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        MediaHolder holder = new MediaHolder(inflater.inflate(R.layout.item_media, parent, false));
        return holder;
    }

    @Override
    public void onBindViewHolder(@NonNull MediaHolder holder, int position) {
        holder.btn_media.setText(mediaLists.get(position));
    }

    @Override
    public int getItemCount() {
        return mediaLists.size();
    }

    class MediaHolder extends RecyclerView.ViewHolder{
        public Button btn_media;
        public MediaHolder(View itemView) {
            super(itemView);
            btn_media = itemView.findViewById(R.id.btn_item_media);
        }
    }
}


