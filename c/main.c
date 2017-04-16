#include <stdio.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include "jda.h"

void* cascador;

void process_image(IplImage* img, int draw)
{
    IplImage* gray = cvCreateImage(cvGetSize(img), img->depth, 1);
    cvCvtColor(img, gray, CV_BGR2GRAY);
    jdaResult result;
    result = jdaDetect(cascador, gray->imageData, gray->width, gray->height, 1.25, 0.1, 70, -1, -0.5);
    for (int i = 0; i < result.n; i++) {
        float* shape = &result.shapes[2 * result.landmark_n*i]; 
        cvRectangle(img, cvPoint(result.bboxes[3*i+0], result.bboxes[3*i+1]), cvPoint(result.bboxes[3*i+0]+result.bboxes[3*i+2], result.bboxes[3*i+1]+result.bboxes[3*i+2]), CV_RGB(0,0,255),2, 8, 0);
        char buff[200];
        sprintf(buff, "%.4lf", result.scores[i]);
    }
    jdaResultRelease(result);

    if(draw)
    {
        cvShowImage("jda", img);
        cvWaitKey(0);
    }
    cvReleaseImage(&gray);
}

void process_webcam_frames()
{
    CvCapture* capture;
    IplImage* frame;
    //IplImage* framecopy;
    int stop =0;
    const char* windowname = "jda";
    capture = cvCaptureFromCAM(0);
    if (!capture)
    {
        printf("# cannot initialize video capture ...\n");
        return;
    }
    cvNamedWindow(windowname, 0);
    while(!stop)
    {
        int key = cvWaitKey(5);
        if(!cvGrabFrame(capture))
        {
            stop = 1;
            frame = 0;
        }
        else
            frame = cvRetrieveFrame(capture,1);
        if(!frame || key=='q')
	{
            stop = 1;
	    printf("stop===%d\n", stop);
	}
        else
        {
            // we mustn't tamper with internal OpenCV buffers
            //if(!framecopy)
            //    framecopy = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, frame->nChannels);
            //cvCopy(frame, framecopy, 1);

            // webcam outputs mirrored frames (at least on my machines)
            // you can safely comment out this line if you find it unnecessary	
            //cvFlip(framecopy, framecopy, 1);
            // ...
            process_image(frame, 0);

            // ...
            cvShowImage(windowname, frame);
        }
    }

    //cvReleaseImage(&framecopy);
    cvReleaseCapture(&capture);
    cvDestroyWindow(windowname);
}

int main(int argc, char* argv[]) {
    cascador = jdaCascadorCreateDouble("jda.model");
    jdaCascadorSerializeTo(cascador, "jda_float32.model");
    jdaCascadorRelease(cascador);
    cascador = jdaCascadorCreateFloat("jda_float32.model");
  

  if (argc==2)
  {
    IplImage* img = cvLoadImage(argv[1], 1);

    process_image(img, 1);
  }
  else
    process_webcam_frames();

  jdaCascadorRelease(cascador);
  return 0;
}
