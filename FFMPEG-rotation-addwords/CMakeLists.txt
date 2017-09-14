cmake_minimum_required(VERSION 3.5)
project(ffmpeg-rotation-addwords)

set(CMAKE_CXX_STANDARD 11)
include_directories(${PROJECT_SOURCE_DIR}/lib/include/)
include_directories(${PROJECT_SOURCE_DIR}/Eigen)
include_directories(/usr/local/include/)
link_directories(/${PROJECT_SOURCE_DIR}/lib/lib/)
link_directories(/usr/local/lib/)

set(SOURCE_FILES main.cpp
        lib_json/json_reader.cpp
        lib_json/json_value.cpp
        lib_json/json_writer.cpp
        InputParameter.cpp
        ImagePar.cpp ImagePar.h
        Video2FrameRgb.cpp
        picture.cpp
        #ReadImageData.cpp
        FrameComposed.cpp
        ppm.cpp
        Jud.cpp
        Distance.cpp
        GetMp3data.cpp
        text.cpp text.h
        #CharacterImage.cpp CharacterImage.h
        GetPerspectiveMatrix.cpp
        MediaMuxer.cpp ReadImage.cpp ReadImage.h MatrixCompute.cpp MatrixCompute.h)

add_executable(ffmpeg-rotation-addwords ${SOURCE_FILES})

target_link_libraries(ffmpeg-rotation-addwords libavformat.a libavcodec.a libswscale.a libavcodec.a
        libswresample.a libavutil.a pthread c -lbz2 xvidcore x264 mp3lame
        -ldl -lmp3lame freetype -lm -llzma -lbz2 -lz -pthread fdk-aac png jpeg  )