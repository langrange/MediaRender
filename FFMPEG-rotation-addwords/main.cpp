#include <iostream>
#include <fstream>
#include <sstream>

#include "MatrixCompute.h"
#include "InputParameter.h"
#include "json/reader.h"
#include "FrameStruct.h"
#include "Video2FrameRgb.h"
#include "FrameComposed.h"
#include "GetMp3data.h"
#include "MediaMuxer.h"
#include "text.h"
#include "ppm.h"
#include "picture.h"

int main() {

    clock_t start,finish;
    double totaltime;
    start=clock();

    InputParameter InputDataContent ;
    InputDataContent.input_MP3_file_name = "../Media/music.mp3";    //input_Mp3_file
    InputDataContent.output_MP4_file_name = "../Media/out_Media.mp4" ; //output_MP4_file
    InputDataContent.input_jsonfile_file_name = "../Media/config.json" ;  // configure_file

    GetMp3data MP3ReadContext ;   //读入Mp3文件数据
    MP3ReadContext.input_MP3_file_name = InputDataContent.input_MP3_file_name ;
    int ret_mp3 = MP3ReadContext.getaudiodata() ;
    if(ret_mp3<0)
    {
        std::cout << "get mp3 data error "<< std::endl ;
        return -1 ;
    }

    Json::Reader reader;   //从json文件中读取数据
    Json::Value root;
    std::ifstream in(InputDataContent.input_jsonfile_file_name, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Error opening file\n";
        return -1;
    }
    reader.parse(in, root) ;
    in.close();

    InputDataContent.width = root["width"].asInt() ;
    InputDataContent.height = root["height"].asInt() ;

    FrameAction frameInput ;

    int in_dex = 0;
    for(int i=0;i<root["layers"].size();i++){

       // Bg和Mask视频相关json文件读入
       if(0 == root["layers"][i]["type"].asInt()){
            BackMask bgmk_temp;
            bgmk_temp.video_name = file_Full_Path(root["sources"][root["layers"][i]["source"].asInt()]["name"].asString());
            frameInput.BgMaskContent.push_back(bgmk_temp) ;
        }

        //需处理图片的相关json文件以及图片的读入
        if(3 == root["layers"][i]["type"].asInt()){
            ImagePar imagePar_temp ;
            imagePar_temp.FrameFormatfile_path = file_Full_Path(root["layers"][i]["data"].asString()) ;
            imagePar_temp.imagepath = file_Full_Path(root["sources"][root["layers"][i]["source"].asInt()]["decoration"].asString())  ;
            imagePar_temp.animation = root["layers"][i]["animation"].asInt();
            imagePar_temp.readimagedata();

            frameData frame_temp ;
            frame_temp.width = imagePar_temp.width ;
            frame_temp.height = imagePar_temp.height ;
            frame_temp.imageId = imagePar_temp.imageId ;
            frame_temp.animation = imagePar_temp.animation;
            frameInput.imageinput.push_back(frame_temp) ;
            frameInput.image_action_Tag.push_back(imagePar_temp.root) ;

        }

        // 文字相关json文件读入以及将文字转化为图片
        if(4 == root["layers"][i]["type"].asInt()){
            text Doctemp ;
            Doctemp.FrameFormatfile_path = file_Full_Path(root["layers"][i]["data"].asString());
            Doctemp.AttributesFile_Path = file_Full_Path(root["edit_file"].asString())  ;
            Doctemp.imagepath = file_Full_Path(root["sources"][root["layers"][i]["source"].asInt()]["decoration"].asString());
            Doctemp.editdatapath = root["sources"][root["layers"][i]["source"].asInt()]["edit_data"].asCString();
            Doctemp.CharacterImageGet() ;

            frameData frame_text;
            frame_text.width = Doctemp.width;
            frame_text.height = Doctemp.height;
            frame_text.imageId = Doctemp.imageId;
            frameInput.charinput.push_back(frame_text);
            frameInput.char_action_Tag.push_back(Doctemp.root);
            in_dex++ ;
        }


    }

    clock_t start0,finish0;
    double totaltime0;
    start0=clock();

    Video2FrameRgb MediaContentBg(frameInput.BgMaskContent[0].video_name,
                                  InputDataContent.width,InputDataContent.height);     //读入bg视频
    int retBg = MediaContentBg.readmedia() ;
    if(retBg<0){ std::cout<<"read bg media error!"<< std::endl; return - 1;}

    Video2FrameRgb MediaContentMask(frameInput.BgMaskContent[1].video_name,
                                  InputDataContent.width,InputDataContent.height);    //读入mask视频
    int retMask = MediaContentMask.readmedia() ;
    if(retMask<0){ std::cout<<"read mask media error!"<< std::endl; return - 1;}

    finish0=clock();
    totaltime0=(double)(finish0-start0)/CLOCKS_PER_SEC;
    std::cout<<"\n读取视频花费时间为"<<totaltime0<< "秒" <<std::endl;

    //Synthesis Every frame data
    FrameComposed frameMake(frameInput.charinput,frameInput.imageinput,MediaContentBg.VideoFrameData,MediaContentMask.VideoFrameData,
                            frameInput.image_action_Tag,frameInput.char_action_Tag,InputDataContent.width,InputDataContent.height);     // 每一帧图片的合成

    clock_t start2,finish2;
    double totaltime2;
    start2=clock();

    frameMake.addimage();

    finish2=clock();
    totaltime2=(double)(finish2-start2)/CLOCKS_PER_SEC;
    std::cout<<"\n图片处理花费时间为"<<totaltime2<< "秒" <<std::endl;
/*
    int index =0 ;int con=0;
    int hh = MediaContentBg.height,ww = MediaContentBg.width;
    ppm A(ww,hh);
    for(int i=0;i<hh;i++)
        for(int j=0;j<ww;j++)
        {
            A.r[index] = frameMake.videoframegoal[40].ptr_rgb[con++];
            A.g[index] = frameMake.videoframegoal[40].ptr_rgb[con++];
            A.b[index] = frameMake.videoframegoal[40].ptr_rgb[con++];
            ++index;
        }
    A.write("wodekkkk.ppm") ;
*/
    //Creat Mp4 file
    MediaMuxer creatMp4(InputDataContent.output_MP4_file_name,frameMake.videoframegoal,
                        MP3ReadContext.temp_voice,MediaContentBg.STREAM_DURATION,MediaContentBg.STREAM_FRAME_RATE,
                        MediaContentBg.width,MediaContentBg.height);
    int retMediaMux = creatMp4.mediamux() ;
    if(retMediaMux<0){ std::cout << "mux mp4 error !"<< std::endl ;return -1 ;}

    finish=clock();
    totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
    std::cout<<"\n此程序的运行时间为"<<totaltime<<"秒！"<<std::endl;

    return 0;

}