#include "cvaux.h"
#include "highgui.h"
#include <stdio.h>

//this is a sample for foreground detection functions
int main(int argc, char** argv)
{
    IplImage*       tmp_frame = NULL;
    CvCapture*      cap = NULL;

    if( argc < 2 )
    {
        printf("please specify video file name \n");
        exit(0);
    }

    cap = cvCaptureFromFile(argv[1]);
    tmp_frame = cvQueryFrame(cap);
    if(!tmp_frame)
    {
        printf("bad video \n");
        exit(0);
    }

    cvNamedWindow("BG", 1);
    cvNamedWindow("FG", 1);

    //create BG model
    CvBGStatModel* bg_model = cvCreateFGDStatModel( tmp_frame );
    
    for( int fr = 1;tmp_frame; tmp_frame = cvQueryFrame(cap), fr++ )
    {
        double t = (double)cvGetTickCount();
        cvUpdateBGStatModel( tmp_frame, bg_model );
        t = (double)cvGetTickCount() - t;
        printf( "%.1f\n", t/(cvGetTickFrequency()*1000.) );
        cvShowImage("BG", bg_model->background);
        cvShowImage("FG", bg_model->foreground);
        char k = cvWaitKey(5);
        if( k == 27 ) break;
        //printf("frame# %d \r", fr);
    }


    cvReleaseBGStatModel( &bg_model );
    cvReleaseCapture(&cap);

    return 0;
}
