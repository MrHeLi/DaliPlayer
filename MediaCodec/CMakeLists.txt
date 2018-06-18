# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
set(ffmpeg_so_dir ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
set( lib_src_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})
add_library( # Sets the name of the library.
             myplayer

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/myplayer.cpp )
<<<<<<< HEAD

add_library(avcodec-57
            SHARED
            IMPORTED)
set_target_properties( avcodec-57
                       PROPERTIES IMPORTED_LOCATION
                       ${ffmpeg_so_dir}/libavcodec-57.so)
add_library(avdevice-57
            SHARED
            IMPORTED)
set_target_properties( avdevice-57
                       PROPERTIES IMPORTED_LOCATION
                       ${ffmpeg_so_dir}/libavdevice-57.so)
add_library(avfilter-6
            SHARED
            IMPORTED)
set_target_properties( avfilter-6
                       PROPERTIES IMPORTED_LOCATION
                       ${ffmpeg_so_dir}/libavfilter-6.so)
add_library(avformat-57
            SHARED
            IMPORTED)
set_target_properties( avformat-57
                       PROPERTIES IMPORTED_LOCATION
                       ${ffmpeg_so_dir}/libavformat-57.so)
add_library(avutil-55
            SHARED
            IMPORTED)
set_target_properties( avutil-55
                       PROPERTIES IMPORTED_LOCATION
                       ${ffmpeg_so_dir}/libavutil-55.so)

add_library(swresample-2
            SHARED
            IMPORTED)
set_target_properties( swresample-2
                       PROPERTIES IMPORTED_LOCATION
                       ${ffmpeg_so_dir}/libswresample-2.so)

add_library(swscale-4
            SHARED
            IMPORTED)
set_target_properties( swscale-4
                       PROPERTIES IMPORTED_LOCATION
                       ${ffmpeg_so_dir}/libswscale-4.so)
=======
add_library( avformat-57
             SHARED
             IMPORTED )
set_target_properties( avformat-57
                       PROPERTIES IMPORTED_LOCATION
                       ${lib_src_DIR}/libavformat-57.so )
add_library( avcodec-57
             SHARED
             IMPORTED )
set_target_properties( avcodec-57
                       PROPERTIES IMPORTED_LOCATION
                       ${lib_src_DIR}/libavcodec-57.so )
add_library( avdevice-57
             SHARED
             IMPORTED )
set_target_properties( avdevice-57
                       PROPERTIES IMPORTED_LOCATION
                       ${lib_src_DIR}/libavdevice-57.so )
add_library( avfilter-6
             SHARED
             IMPORTED )
set_target_properties( avfilter-6
                       PROPERTIES IMPORTED_LOCATION
                       ${lib_src_DIR}/libavfilter-6.so )
add_library( avutil-55
             SHARED
             IMPORTED )
set_target_properties( avutil-55
                       PROPERTIES IMPORTED_LOCATION
                       ${lib_src_DIR}/libavutil-55.so )
add_library( swresample-2
             SHARED
             IMPORTED )
set_target_properties( swresample-2
                       PROPERTIES IMPORTED_LOCATION
                       ${lib_src_DIR}/libswresample-2.so )
add_library( swscale-4
             SHARED
             IMPORTED )
set_target_properties( swscale-4
                       PROPERTIES IMPORTED_LOCATION
                       ${lib_src_DIR}/libswscale-4.so )

>>>>>>> 5940151616ee17a59b2a690a443d061717f76f9e


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
include_directories( ${CMAKE_SOURCE_DIR}/src/main/cpp/ffmpeg )
find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
include_directories(src/main/cpp/ffmpeg/)
target_link_libraries( # Specifies the target library.
                       myplayer
<<<<<<< HEAD

                       avcodec-57
                       avdevice-57
                       avfilter-6
                       avformat-57
                       avutil-55
                       swresample-2
                       swscale-4

=======
>>>>>>> 5940151616ee17a59b2a690a443d061717f76f9e

                       avformat-57
                       avcodec-57
                       avdevice-57
                       avfilter-6
                       avutil-55
                       swresample-2
                       swscale-4
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )