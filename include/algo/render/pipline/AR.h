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
       if (!frames.empty())//����������֡
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

       // ��������ͷ��֡��
       double frameRate = 60.0;  // 30֡ÿ��
       cap.set(cv::CAP_PROP_FPS, frameRate);

       //��������ͷ�߳�
       std::thread t1(&AR::GetFrame, this);
       t1.detach();
   }
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::mutex g_mutex;
    std::queue<cv::Mat> frames;//�Ƚ��ȳ�����
    Shader* shader;
    GLuint textureID;
    cv::VideoCapture cap;
    void cvmatToTexture(GLuint& textureId, const cv::Mat& mat)
    {
        // ��textureID��ʶ������֮������в�����������ڸ������
        glBindTexture(GL_TEXTURE_2D, textureId);
        // ע��OpenCV��ͼ��ͨ����˳����BGR
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        // ���
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
        std::cout << "���ڽ������ͷռ��" << std::endl;
        cap.release();
        //��ն���
        std::queue<cv::Mat> empty;
        std::swap(frames, empty);
        std::cout << "�ɹ��������ͷռ��" << std::endl;
        IsCamInit = false;
    }
};



#endif

