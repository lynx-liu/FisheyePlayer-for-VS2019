#ifndef SHADER_H
#define SHADER_H

#define _CRT_SECURE_NO_WARNINGS

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h> 

#include "../gl/glew.h"
#include <gl/gl.h>
#include <gl/glu.h>
#ifdef _WIN64
#pragma comment(lib,"../gl/x64/glew32.lib")
#else
#pragma comment(lib,"../gl/x86/glew32.lib")
#endif

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

#define CV_VERSION_ID CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) \
	CVAUX_STR(CV_SUBMINOR_VERSION)

#ifdef _DEBUG
#pragma comment(lib,"../opencv/x86/opencv_world" CV_VERSION_ID "d.lib")
#else
#pragma comment(lib,"../opencv/x86/opencv_world" CV_VERSION_ID ".lib")
#endif

extern "C"
{
#pragma warning (push)
#pragma warning(disable: 4819)
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
#include "libavutil/imgutils.h"
#include "libavutil/mathematics.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include "libavutil/log.h"
#pragma warning (pop)
#ifdef _WIN64
#pragma comment(lib,"../ffmpeg/x64/avcodec.lib")
#pragma comment(lib,"../ffmpeg/x64/avdevice.lib")
#pragma comment(lib,"../ffmpeg/x64/avfilter.lib")
#pragma comment(lib,"../ffmpeg/x64/avformat.lib")
#pragma comment(lib,"../ffmpeg/x64/avutil.lib")
#pragma comment(lib,"../ffmpeg/x64/postproc.lib")
#pragma comment(lib,"../ffmpeg/x64/swresample.lib")
#pragma comment(lib,"../ffmpeg/x64/swscale.lib")
#pragma comment(lib,"../ffmpeg/x64/swresample.lib")
#else
#pragma comment(lib,"../ffmpeg/x86/avcodec.lib")
#pragma comment(lib,"../ffmpeg/x86/avdevice.lib")
#pragma comment(lib,"../ffmpeg/x86/avfilter.lib")
#pragma comment(lib,"../ffmpeg/x86/avformat.lib")
#pragma comment(lib,"../ffmpeg/x86/avutil.lib")
#pragma comment(lib,"../ffmpeg/x86/postproc.lib")
#pragma comment(lib,"../ffmpeg/x86/swresample.lib")
#pragma comment(lib,"../ffmpeg/x86/swscale.lib")
#pragma comment(lib,"../ffmpeg/x86/swresample.lib")
#endif
};

#define min(a,b)    (((a) < (b)) ? (a) : (b))

using namespace std;
using namespace cv;

enum FishEye{ Left, Right };
enum DispMode{ Normal, Sphere, VR };

typedef struct tagFISHEYE_PARAM
{
	Rect	rect;
	float	rotate;
} FISHEYE_PARAM;

class Shader
{
public:
	Shader(void);
	~Shader(void);
	bool canDraw();
	void Release();
	void loadTexture(GLuint texture, char *filename, float rotate = 0);
	void loadTexture(GLuint texture, Mat mat, float rotate = 0, Rect rect = Rect( 0, 0, 0, 0 ));
	void setMode(DispMode mode, FishEye fishEye);
	void draw(GLfloat width, GLfloat height, GLfloat fov);
	void draw(GLfloat width, GLfloat height);
	void showLine(bool show);
	bool showLine();
	FISHEYE_PARAM getFishEyeParam();

private:
	Rect getMaxRect();
	Mat getFishEye();
	void genTexture(Mat mat);
	void setShader(const char *fragmentShader);
	void setUniform(const GLchar *uniformName, GLint uniformVal);
	void setUniform(const GLchar *uniformName, GLfloat uniformVal);
	void setUniform(const GLchar *uniformName, GLint uniformVal, GLfloat *x);
	void setAttribute(const GLchar *uniformName, GLfloat x, GLfloat y, GLfloat z);

private:
	GLuint vertShader;
	GLuint fragShader;
	GLuint shaderProgram;
	GLuint textureID;
	Mat fisheyeMat;
	FISHEYE_PARAM fishEyeParam;
	bool showline;
	FishEye fisheye;
};

#endif
