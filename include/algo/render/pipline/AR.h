#ifndef AR_H
#define  AR_H
#include <mutex>
#include<opencv2/opencv.hpp>
#include <queue>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../shader/shader_m.h"
#include"../../mesh/primitive/TexPlane.h"
class AR
{
public:
    bool ARmode = true;
    bool IsCamInit = false;
    TexPlane texplane;
    AR()
    {
        texplane = TexPlane();
    };
    ~AR()
    {
        delete shader;
    }
   void draw()
    { 
       cv::Mat frame;
       if (!frames.empty())//队列里面有帧
       {
           g_mutex.lock();
           frame = frames.front();
           frames.pop();
           g_mutex.unlock();
       }
       if (!frame.empty())
       {
           cvmatToTexture(texplane.textureID, frame);
       }
       texplane.draw();
    }

   void InitCam()
   {
       cap.open(0);
       if (!cap.isOpened())
       {
           std::cerr << "Error: Could not open camera." << std::endl;
           return;
       }
       else
       {
           IsCamInit = true;
       }
       int width = 1920;
       int height = 1080;
       cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
       cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);

       // 设置摄像头的帧率
       double frameRate = 60.0;  // 30帧每秒
       cap.set(cv::CAP_PROP_FPS, frameRate);

       //开启摄像头线程
       std::thread t1(&AR::GetFrame, this);
       t1.detach();
   }
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::mutex g_mutex;
    std::queue<cv::Mat> frames;//先进先出队列
    Shader* shader;
    GLuint textureID;
    cv::VideoCapture cap;
    void cvmatToTexture(GLuint& textureId, const cv::Mat& mat)
    {
        // 绑定textureID标识的纹理，之后的所有操作都是相对于该纹理的
        glBindTexture(GL_TEXTURE_2D, textureId);
        // 注意OpenCV中图像通道的顺序是BGR
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        // 解绑
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void GetFrame()
    {
        while (ARmode)
        {
            cv::Mat frame;
            if (!cap.read(frame))
            {
                std::cerr << "Error: Could not read frame from camera." << std::endl;
                break;  // Break the loop on error
            }
            if (frames.size() > 5)
            {
                g_mutex.lock();
                frames.pop();
                g_mutex.unlock();
            }
            g_mutex.lock();
            frames.push(frame);
            //cv::imshow(" ",frame);
            //cv::waitKey(1);
            g_mutex.unlock();
        }
        std::cout << "正在解除摄像头占用" << std::endl;
        cap.release();
        //清空队列
        std::queue<cv::Mat> empty;
        std::swap(frames, empty);
        std::cout << "成功解除摄像头占用" << std::endl;
        IsCamInit = false;
    }
};



#endif

