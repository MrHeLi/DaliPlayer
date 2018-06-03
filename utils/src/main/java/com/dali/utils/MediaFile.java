package com.dali.utils;

import android.support.annotation.NonNull;
import android.text.TextUtils;

public class MediaFile implements Comparable{
    private MediaFile() {}
    public MediaFile(String path,String name) {
        if (TextUtils.isEmpty(path) || TextUtils.isEmpty(name)) {
            throw new RuntimeException("xia xia xia");
        }
        this.name = name;
        this.path = path;
    }
    private String name = "";
    private String path = "";

    public String getName() {
        return name;
    }

    public String getPath() {
        return path;
    }

    @Override
    public int compareTo(@NonNull Object o) {
        return path.compareTo(((MediaFile)o).path);
    }
}
