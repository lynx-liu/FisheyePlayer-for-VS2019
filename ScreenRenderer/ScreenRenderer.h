#ifndef SCREENRENDERER_H
#define SCREENRENDERER_H

#pragma warning (push)
#include "Shader.h"
#include "PcmRender.h"
#pragma warning (pop)
#pragma warning(disable: 4251)
#pragma warning(disable: 4996)
#pragma warning(disable: 4244)

#define	COORDS_PER_VERTEX		3
#define COORDS_PER_TEXTURE		2

#define FOV 190
#define FRAMERATE 30

typedef struct tagCABLIRATE_PARAM
{
	FISHEYE_PARAM leftParam;
	FISHEYE_PARAM rightParam;
	float fov;//鱼眼镜头广角
	int width;//标定图的宽度
	int height;//标定图的高度
}CABLIRATE_PARAM;

#define __DLL_EXPORTS__
#ifdef __DLL_EXPORTS__
#define DLLAPI  __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

class ScreenRenderer
{
public:
	DLLAPI ScreenRenderer(void);
	DLLAPI ~ScreenRenderer(void);
	DLLAPI void setShader();
	DLLAPI BOOL ReadFrame(void);
	DLLAPI Mat YV12RGB(BYTE *yuv);
	DLLAPI BOOL setYUV420p(BYTE *yuv);
	DLLAPI BOOL setFrame(Mat frame);
	DLLAPI BOOL setH264(BYTE *h264, int len);
	DLLAPI BOOL setPCM(BYTE *pcm, int len);
	DLLAPI BOOL setAudio(BYTE *audio, int len);
	DLLAPI BYTE* getYUV12(char *filename=NULL);
	DLLAPI void DrawFrame(void);
	DLLAPI void EnableOpenGL(HWND hWnd);
	DLLAPI void DisableOpenGL(HWND hWnd);
	DLLAPI void setMode(DispMode mode);
	DLLAPI DispMode getMode();
	DLLAPI void showLine();
	DLLAPI void setText(TCHAR *text, Point2f point = Point2f(), COLORREF color = RGB(255, 255, 255), int size = 48);

	DLLAPI BOOL SetDevice(TCHAR *szFileName);
	DLLAPI BOOL SetDevice(BYTE *param, int size);
	DLLAPI BOOL OpenFile(TCHAR *szFileName);
	DLLAPI BOOL SaveFile(TCHAR *szFileName);
	DLLAPI void SaveFinish();
	DLLAPI BOOL OpenDevice();
	DLLAPI BOOL OpenYUV420p(int width, int height);
	DLLAPI BOOL OpenRGBA(int width, int height);
	DLLAPI BOOL OpenH264(int width, int height);
	DLLAPI BOOL OpenAudio(int channels, int sample_rate, int sample_fmt, int codec_id);
	DLLAPI BOOL OpenPCM(int channels, int sample_rate, int bitsPerSample);

	DLLAPI void onMouseWheel(int zDelta);
	DLLAPI void onMouseMove(POINT point);
	DLLAPI void onLButtonDown(POINT point);
	DLLAPI void onLButtonUp();
	DLLAPI void onSize(HWND hWnd);

private:
	bool canDraw();
	void Release();
	void generateFBO();
	void deleteFBO();
	void setRatio(float ratio);
	void setParam();
	bool setRendererSize(int width, int height);
	void setupFrameBufferGraphics(bool isFBO);
	void draw(float fov, float nRotateX, float nRotateY);
	BOOL DrawFrame(int width, int height);
	void glDrawText(const TCHAR *str, const Point2f position);
	void glSetFont(int size, int charset, const TCHAR* face);

	int getVRPointMatrix();
	void setUniform(const GLchar *uniformName, GLint uniformVal);
	void setUniform(const GLchar *uniformName, GLfloat uniformVal);
	void setUniform(const GLchar *uniformName, GLint uniformVal, GLfloat *x);
	void setAttribute(const GLchar *uniformName, GLfloat x, GLfloat y, GLfloat z);

	char* wcstombs(char* szStr, WCHAR* wzStr, UINT CodePage);
	BOOL SaveVideoFrame(int width, int height);

	bool ffmpeg_encode_init(const char *filename);
	bool ffmpeg_encode_videoframe(uint8_t *rgb);
	bool ffmpeg_encode_audioframe(uint8_t *pcm);
	void ffmpeg_encode_release();

private:
	GLuint mFboId;
	GLuint mFboTexId;
	GLuint shaderProgram;

	float Ratio;//显示屏宽高比
	float* triangleCoords;
	float* textureCoords;
	int point_count;

	HDC hDC;
	HGLRC hRC;

	Shader leftShader;
	Shader rightShader;
	GLuint texture[2];
	int windowWidth;
	int windowHeight;

	DispMode dispMode;
	VideoCapture capture;
	CABLIRATE_PARAM cablirateParam;

	int frameWidth;
	int frameHeight;

	POINT pointDown;
	float fovEye;
	float nRotateX;//绕X轴旋转
	float nRotateY;//绕Y轴旋转
	float nRotateZ;//绕Z轴旋转

	Point2f textPosition;//显示文字的位置
	Point2f markPosition;//水印的显示位置
	TCHAR szText[MAX_PATH];
	COLORREF cColor;
	int nSize;

	BYTE *yuvBuffer;
	AVFrame *pFrameRGB;
	SwsContext *pYuv2RGB24SwsCtx;

	AVCodec *videoCodec;
	AVCodecContext *pDecoderH264Ctx;
	AVFrame *pFrameYUV;
	AVFrame *pFrameBGR;
	AVPacket *pDecoderH264Packet;
	SwsContext *pDecoderSwsCtx;

	AVCodec *audioCodec;
	AVCodecContext *pDecoderAudioCtx;
	AVFrame *pFramePCM;
	AVPacket *pDecoderAudioPacket;
	PcmRender pcmRender;

	GLuint mPboId[2];
	int mPboIndex;
	int mPboBackIndex;
	VideoWriter videoWriter;

	SwsContext *pEncoderSwsCtx;
	SwrContext *pEncoderSwrCtx;
	AVFormatContext *pEncoderFormatCtx;
	AVCodecContext *pVideoEncoderCtx;
	AVCodecContext *pAudioEncoderCtx;
	uint8_t *video_buffer;
	uint8_t *audio_buffer;
	uint8_t *pcm_buffer;
	int pcmbufferSize;
	int pcm_index;
	AVPacket *pVideoEncoderPacket;
	AVPacket *pAudioEncoderPacket;
	AVFrame *pVideoFrame;
	AVFrame *pAudioFrame;
	int64_t video_pts;
	int64_t audio_pts;
	int video_streamindex;
	int audio_streamindex;
	int streamindex;
	bool encoding;
};

#endif
