package com.dali.utils;

public class FileType {
    private static String Tag = FileType.class.getSimpleName();
    private static String[] media_type = {"mp4"};

    public static boolean isMeidaFile(String path) {
        int index = path.lastIndexOf(".");
        if (index != -1) {
            String type = path.substring(index + 1, path.length());
            for (String mType : media_type) {
                if (mType.equals(type)) {
                    return true;
                }
            }
        }

        return false;
    }
}
