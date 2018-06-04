package com.dali.daliplayer;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import com.dali.utils.MediaFile;

import java.util.List;

class MediaAdapter extends RecyclerView.Adapter<MediaAdapter.MediaHolder> implements View.OnClickListener {
    private String Tag = MediaAdapter.class.getName();
    private LayoutInflater inflater;
    private List<MediaFile> mediaLists;

    @Override
    public void onClick(View v) {
        if (mOnItemClickListener != null) {
            mOnItemClickListener.onItemClick(v, ((int)v.getTag()));
        }
    }

    public interface OnItemClickListener {
        void onItemClick(View view, int position);
    }
    public OnItemClickListener mOnItemClickListener = null;

    public void setOnItemClickListener(OnItemClickListener listener) {
        mOnItemClickListener = listener;
    }

    public MediaAdapter(Context context, List<MediaFile> mediaLists) {
        inflater = LayoutInflater.from(context);
        this.mediaLists = mediaLists;
    }

    @NonNull
    @Override
    public MediaHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = inflater.inflate(R.layout.item_media, parent, false);
        MediaHolder holder = new MediaHolder(view);
        Button btn = view.findViewById(R.id.btn_item_media);
        btn.setOnClickListener(this);
        return holder;
    }

    @Override
    public void onBindViewHolder(@NonNull MediaHolder holder, int position) {
        holder.btn_media.setText(mediaLists.get(position).getName());
        holder.btn_media.setTag(position);
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


