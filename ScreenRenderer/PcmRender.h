#pragma once

#include <windows.h> 
#include <mmeapi.h>
#pragma comment(lib, "winmm.lib") 

#ifndef PCMRENDER_H
#define PCMRENDER_H

#define PCM_BUF_COUNT	3
#define BUFFER_SIZE		32768

class PcmRender
{
	struct PcmBuffer {
		PcmBuffer() {
			hWaveOut = NULL;
			memset(&header, 0, sizeof(header));
			header.dwFlags |= WHDR_DONE;
		}
		
		DWORD Prepare(HWAVEOUT hOut, unsigned int count)
		{
			Release();
			memset(&header, 0, sizeof(header));
			
			header.lpData = data;
			header.dwBufferLength = count;
			header.dwUser = (DWORD_PTR)this;
			hWaveOut = hOut;

			return waveOutPrepareHeader(hWaveOut, &header, sizeof(header));
		}

		DWORD Release()
		{
			DWORD err = MMSYSERR_NOERROR;
			if (hWaveOut!=NULL) {
				err = waveOutUnprepareHeader(hWaveOut, &header, sizeof(header));
				if (err== WAVERR_STILLPLAYING) {
					return err;
				}
				hWaveOut = NULL;
			}

			header.dwFlags |= WHDR_DONE;
			return err;
		}

		char data[BUFFER_SIZE];
		WAVEHDR header;
		HWAVEOUT hWaveOut;
	};
public:
	PcmRender();
	~PcmRender();

	BOOL init(int channels, int samplePerSec, int bitPerSample);
	int write(unsigned char *pcm, int len);
	BOOL clean();

private:
	int Abort();

private:
	// 公共信息
	WAVEFORMATEX m_waveFormat;
    HWAVEOUT m_hWaveOut; 
	int m_bufferIndex;
    HANDLE m_mutex;
	// 播放缓冲区 
	PcmBuffer m_buffer[PCM_BUF_COUNT];

public:
	HANDLE m_hEventDone;
};

#endif /* PCMRENDER_H */