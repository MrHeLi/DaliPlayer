package com.dali.utils;

import android.os.AsyncTask;
import android.text.TextUtils;
import android.util.Log;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

public class FileUtils {
    private static String Tag = FileUtils.class.getSimpleName();
    private static List<MediaFile> medias = new ArrayList<>();
    public interface OnFileLoadListener {
        void onLoad(List<MediaFile> medias);
    }
    public static OnFileLoadListener loadedListener;
    public static void setOnFileLoadedListener(OnFileLoadListener listener) {
        loadedListener = listener;
    }

    public static void loadMedias(String path) {
        MediaLoadable loader = new MediaLoadable();
        loader.execute(path);
    }

    static class MediaLoadable extends AsyncTask<String, Integer, Integer> {
        Timer timer;

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            Log.i(Tag, "start load");
            timer = new Timer();
            timer.schedule(new TimerTask() {
                @Override
                public void run() {
                    if (loadedListener != null) {
                        loadedListener.onLoad(medias);
                    }
                }
            }, 20, 100);
        }

        @Override
        protected Integer doInBackground(String... strings) {
            int result = -1;
            for (String path : strings) {
                limitedRecurFileLoad(path);
            }
            return result;
        }

        @Override
        protected void onPostExecute(Integer result) {
            super.onPostExecute(result);
            if (result != -1) {
                Log.i(Tag, "load success");
            } else {
                //load error
            }
            if (loadedListener != null) {
                loadedListener.onLoad(medias);
                timer.cancel();
                timer = null;
            }
        }
    }
    private static int limitedCount = 0;
    public static void limitedRecurFileLoad(String path) {
        if (TextUtils.isEmpty(path) || limitedCount > 10000) {
            return;
        }
        limitedCount ++;

        File file = new File(path);
        if (file.exists()) {
            if (file.isDirectory()) {
                File[] files = file.listFiles();
                if (files != null) {
                    for (int i = 0; i < files.length; i++) {
                        limitedRecurFileLoad(files[i].getAbsolutePath());
                    }
                }
            } else {
                if (FileType.isMeidaFile(file.getAbsolutePath())) {
                    MediaFile cFile = parsePath2Media(path);
                    if (!medias.contains(cFile)) {
                        medias.add(cFile);
//                        Log.i(Tag, "new path = " + cFile.getPath());
                    }
                }
            }
        }
    }

    public static MediaFile parsePath2Media(String path) {
        if (TextUtils.isEmpty(path) && !FileType.isMeidaFile(path)) {
            return null;
        }
        int index = path.lastIndexOf(File.separator);
        String name = path.substring(index + 1, path.length());
        MediaFile file = new MediaFile(path, name);
        return file;
    }

}
