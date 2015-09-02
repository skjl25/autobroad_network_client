//============================================================================
// Name        : Empty.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "libfreenect.h"
//#include "libfreenect_sync.h"
#include <pthread.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <pthread.h>
#include <math.h>
#include <alsa/asoundlib.h>
#include "portaudio.h"

#include "/root/workspace/include-files/cvideo.h"
#include "/root/workspace/include-files/load_image.h"
#include "/root/workspace/include-files/utility.h"
#include "/root/workspace/include-files/viewer.h"
#include "/root/workspace/include-files/TCPServerSocket.h"
#include "/root/workspace/include-files/TCPClientSocket.h"
#include "/root/workspace/include-files/image_processing.h"
#include "/root/workspace/include-files/camera.h"

#define numOfCamera 1
bool proximityViewEnabled=true;
utility util;
TCPClientSocket client1;
cvideo encoder;
IplImage* showImage=NULL;
using namespace std;

/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (128)
#define NUM_SECONDS     (0.01)
#define NUM_CHANNELS    (2)
/* #define DITHER_FLAG     (paDitherOff) */
#define DITHER_FLAG     (0) /**/
/** Set to 1 if you want to capture the recording to a file. */
#define WRITE_TO_FILE   (0)

/* Select sample format. */
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"

typedef struct
{
	int          frameIndex;  /* Index into sample array. */
	int          maxFrameIndex;
	SAMPLE      *recordedSamples;
}
paTestData;

static int recordCallback( const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData )
{
	paTestData *data = (paTestData*)userData;
	const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
	SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	long framesToCalc;
	long i;
	int finished;
	unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

	(void) outputBuffer; /* Prevent unused variable warnings. */
	(void) timeInfo;
	(void) statusFlags;
	(void) userData;

	if( framesLeft < framesPerBuffer )
	{
		framesToCalc = framesLeft;
		finished = paComplete;
	}
	else
	{
		framesToCalc = framesPerBuffer;
		finished = paContinue;
	}

	if( inputBuffer == NULL )
	{
		for( i=0; i<framesToCalc; i++ )
		{
			*wptr++ = SAMPLE_SILENCE;  /* left */
			if( NUM_CHANNELS == 2 ) {
				*wptr++ = SAMPLE_SILENCE;  /* right */
			}
		}
	}
	else
	{
		for( i=0; i<framesToCalc; i++ )
		{
			*wptr++ = *rptr++;  /* left */
			if( NUM_CHANNELS == 2 ){
				*wptr++ = *rptr++;  /* right */
			}
		}
	}
	data->frameIndex += framesToCalc;
	SAMPLE val = data->recordedSamples[data->frameIndex];
	//    printf("-----%d  %d  %f\n",data->frameIndex, framesToCalc, val);
	return finished;
}

static int playCallback( const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData )
{
	paTestData *data = (paTestData*)userData;
	SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	SAMPLE *wptr = (SAMPLE*)outputBuffer;
	unsigned int i;
	int finished;
	unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

	(void) inputBuffer; /* Prevent unused variable warnings. */
	(void) timeInfo;
	(void) statusFlags;
	(void) userData;

	if( framesLeft < framesPerBuffer )
	{
		/* final buffer... */
		for( i=0; i<framesLeft; i++ )
		{
			*wptr++ = *rptr++;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
		}
		for( ; i<framesPerBuffer; i++ )
		{
			*wptr++ = 0;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = 0;  /* right */
		}
		data->frameIndex += framesLeft;
		finished = paComplete;
	}
	else
	{
		for( i=0; i<framesPerBuffer; i++ )
		{
			*wptr++ = *rptr++;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
		}
		data->frameIndex += framesPerBuffer;
		finished = paContinue;
	}
	return finished;
}


double sound()
{
	PaStreamParameters  inputParameters,outputParameters;
	PaStream*           stream;
	PaError             err = paNoError;
	paTestData          data;
	int                 i;
	int                 totalFrames;
	int                 numSamples;
	int                 numBytes;
	SAMPLE              max, val;
	double              average;

	data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
	data.maxFrameIndex = totalFrames = 1;

	data.frameIndex = 0;
	numSamples = totalFrames * NUM_CHANNELS;
	numBytes = numSamples * sizeof(SAMPLE);
	data.recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
	if( data.recordedSamples == NULL )
	{
		printf("Could not allocate record array.\n");
		//		goto done;
	}
	for( i=0; i<numSamples; i++ ) data.recordedSamples[i] = 0;

	err = Pa_Initialize();
	//	if( err != paNoError ) goto done;

	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	if (inputParameters.device == paNoDevice) {
		fprintf(stderr,"Error: No default input device.\n");
		//		goto done;
	}
	inputParameters.channelCount = 2;                    /* stereo input */
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	/* Record some audio. -------------------------------------------- */
	err = Pa_OpenStream(
			&stream,
			&inputParameters,
			NULL,                  /* &outputParameters, */
			SAMPLE_RATE,
			FRAMES_PER_BUFFER,
			paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			recordCallback,
			&data );
	//	if( err != paNoError ) goto done;

	err = Pa_StartStream( stream );
	//	if( err != paNoError ) goto done;
	//	printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);

	while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
	{
		//		Pa_Sleep(1);
		//		printf("index = %d\n", data.frameIndex );
		//		fflush(stdout);
	}
	//	if( err < 0 ) goto done;
	err = Pa_CloseStream( stream );
	//	if( err != paNoError ) goto done;

	/* Measure maximum peak amplitude. */
	max = 0;
	average = 0.0;
	for( i=0; i<numSamples; i++ )
	{
		val = data.recordedSamples[i];
		//        printf("%f\n",val);
		if( val < 0 ) val = -val; /* ABS */
		if( val > max )
		{
			max = val;
		}
		average += val;
	}
	average = average / (double)numSamples;
	//	printf("%f\n",average);
	return average;

	//	printf("sample max amplitude = "PRINTF_S_FORMAT"\n", max );
	//	printf("sample average = %lf\n", average );

	/* Playback recorded data.  -------------------------------------------- */
	//	done:
	//	Pa_Terminate();
	//	if( data.recordedSamples )       /* Sure it is NULL or valid. */
	//		free( data.recordedSamples );
	//	if( err != paNoError )
	//	{
	//		fprintf( stderr, "An error occured while using the portaudio stream\n" );
	//		fprintf( stderr, "Error number: %d\n", err );
	//		fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
	//		err = 1;          /* Always return 0 or 1, but no other return codes. */
	//	}
	//    return err;
}

double getAverageDepthPixels(short* depthImage1,int height,int width)
{
	double returnVal=0;
	int sum1=0;
	int index=0;
	int index1=0;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			int pixel1=depthImage1[index];
			if(pixel1!=0)
			{
				sum1=sum1+pixel1;
				index1++;
			}
			index++;
		}
	}
	if(index1!=0)
	{
		returnVal=(double)sum1/index1;
	}
	else
	{
		returnVal=0.0;
	}
	return returnVal;
}


void depthFilterMethod(Camera data)
{

	data.grabKinectDepthImage();
	short* backgroundDepth=data.getRawDepthImage();
	short* dataImage=(short*)malloc(sizeof(short)*data.height*data.width);
	int index=0;
	printf("Acquired Background Information\n");
	getchar();

	double prevNumber1=0;
	double prevNumber2=0;
	double currentVal=0;

	while(1)
	{
		data.grabKinectGrayImage();
		data.grabKinectDepthImage();

		for(int k=0;k<data.height*data.width;k++)
		{
			int pixelForegroundDepth=data.rawdepthImage[k];//-data[i].rawdepthImage[k];
			int pixelBackgroundDepth=backgroundDepth[k];
			int pixel=pixelForegroundDepth-pixelBackgroundDepth;
			dataImage[k]=(short)pixel;
			if(dataImage[k]<0)
			{
				dataImage[k]=0;
			}
		}

		double depthVal=getAverageDepthPixels(dataImage,data.height,data.width);

		prevNumber2=prevNumber1;
		prevNumber1=currentVal;
		currentVal=depthVal;
		if(prevNumber1==0.0 && index>2)
		{
			if(prevNumber2 == 0.0)
				prevNumber1 = (double)currentVal;
			else if(currentVal == 0.0)
				prevNumber1 = (double)prevNumber2;
			else
				prevNumber1=(double)(prevNumber2+currentVal)/2;
		}
		data.autoViewVal=(int)prevNumber1;
		showImage=convertShortGrayArrayToIplImage(dataImage,data.height,data.width);

		//			data[i].rgbImage=convertColorArrayToIplImage(data[i].rawrgbImage, data[i].height, data[i].width);
		//			cvSaveImage("/home/sklee25/Desktop/depth_orig.pgm",data[i].depthImage);
		//			save_pgm("/home/sklee25/Desktop/gray_orig.pgm",data[i].rawgrayImage,640,480);

		//			soundVal=soundVal*1000-45;
		//			fprintf(pFile,"%f-%f\n",depthVal,soundVal/firstSound);

		printf("%d\n",data.autoViewVal);
		//			fflush(pFile);

		//			if(index%2==0)
		//			{
		//				difference=abs((soundVal*1000-45)-prevSound);
		//				printf("%f\n",difference);
		//			}
		index++;
		cvShowImage("main", showImage);
		cvWaitKey(2);
		cvReleaseImage(&showImage);
		///////////////////////////////////////////////////////////////////
		int sendArraySize=(data.width*data.height*3+sizeof(int));
		char* sendBuffer=(char*)malloc(sizeof(char)*sendArraySize);
		memcpy(sendBuffer,data.rawrgbImage,sendArraySize);
		memcpy(&sendBuffer[sendArraySize-sizeof(int)],&data.autoViewVal,sizeof(int));
		client1.EstablishConnection("163.152.21.249",6000);

		client1.SendData(client1.sock, sendBuffer, sendArraySize);
		delete[] sendBuffer;
		///////////////////////////////////////////////////////////////////
		data.cleanImageMemory();
	}
}

void visibleFilterMethod(Camera data)
{
	data.grabKinectBackgroundGrayImage();
	data.grayImage=convertCharGrayArrayToIplImage(data.backgroundImage,data.height,data.width);
	cvShowImage("background", data.grayImage);
	printf("Acquired Background Image\n");
	getchar();

	while(1)
	{
		data.grabKinectVisibleImages();
		data.grabKinectDepthImage();

		//Implementation of the background subtraction for visible images
		for(int k=0;k<data.height*data.width;k++)
		{
			int pixelForegroundDepth=(uint8_t)data.rawgrayImage[k];//-data[i].rawdepthImage[k];
			int pixelBackgroundDepth=(uint8_t)data.backgroundImage[k];
			int pixel=pixelForegroundDepth-pixelBackgroundDepth;

			if(pixel<50)
			{
				pixel=0;
				data.dataWithDepth[k]=(short)0;
			}
			if(pixel>255)
			{
				pixel=255;
			}
			if(pixel!=0)
			{
				data.dataWithDepth[k]=data.rawdepthImage[k];
				data.autoViewVal=data.autoViewVal+(int)data.dataWithDepth[k];
			}
		}
		data.depthImage=convertShortGrayArrayToIplImage(data.dataWithDepth,data.height,data.width);
		//		}
		printf("%d\n",data.autoViewVal/10000);
		data.autoViewVal=data.autoViewVal/10000;
		data.rgbImage=convertColorArrayToIplImage(data.rawrgbImage,data.height,data.width);
		cvShowImage("main_display_0", data.depthImage);
		cvWaitKey(2);

		data.cleanImageMemory();

		///////////////////////////////////////////////////////////////////
		int sendArraySize=(data.width*data.height*3+sizeof(int));
		char* sendBuffer=(char*)malloc(sizeof(char)*sendArraySize);
		memcpy(sendBuffer,data.rawrgbImage,sendArraySize);
		memcpy(&sendBuffer[sendArraySize-sizeof(int)],&data.autoViewVal,sizeof(int));
		client1.EstablishConnection("163.152.21.249",6000);

		client1.SendData(client1.sock, sendBuffer, sendArraySize);
		delete[] sendBuffer;
		///////////////////////////////////////////////////////////////////

	}
}

void pureDepthMethod(Camera data)
{
	while(1)
	{
		data.grabKinectGrayImage();
		data.grabKinectDepthImage();
		data.autoViewVal=getAverageDepthPixels(data.rawdepthImage,data.height,data.width);

		data.depthImage=convertShortGrayArrayToIplImage(data.rawdepthImage,data.height,data.width);
		cvShowImage("main", data.depthImage);
		cvWaitKey(2);
		data.cleanImageMemory();

		///////////////////////////////////////////////////////////////////
		int sendArraySize=(data.width*data.height*3+sizeof(int));
		char* sendBuffer=(char*)malloc(sizeof(char)*sendArraySize);
		memcpy(sendBuffer,data.rawrgbImage,sendArraySize);
		memcpy(&sendBuffer[sendArraySize-sizeof(int)],&data.autoViewVal,sizeof(int));
		client1.EstablishConnection("163.152.21.249",6000);
		client1.SendData(client1.sock, sendBuffer, sendArraySize);
		delete[] sendBuffer;
		///////////////////////////////////////////////////////////////////
	}
}

int main(int argc, char **argv) {
	Camera data[numOfCamera];//=new Camera[numOfCamera];
	for(int i=0;i<numOfCamera;i++)
	{
		data[i]=Camera(i);
	}
	double prevSound;
	double currentSound;
	double difference=0.0;
	double firstSound=0.0;

	depthFilterMethod(data[0]);
	//				visibleFilterMethod(data[0]);
	//	pureDepthMethod(data[0]);


	///////////////////////////////////////////////////////
	//		double soundVal=sound();
	//			printf("depthVal is %f soundVal is %f\n",depthVal,soundVal*1000);

	//		prevSound=soundVal*1000-45;
	//			fprintf(pFile,"%f-%f\n",depthVal,soundVal/firstSound);
	//			if(index%2==0)
	//			{
	//				difference=abs((soundVal*1000-45)-prevSound);
	//				printf("%f\n",difference);
	//			}
	printf("Done with sound()\n");

	return 0;
}
