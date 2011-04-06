#include "cvaux.h"
#include "highgui.h"

int main(int argc, char** argv)
{
    cv::Mat img;

    if( argc > 1 )
        img = cv::imread(argv[1]);

    if( !img.data )
    {
        fprintf( stderr, argc == 1 ? "ERROR: no image was specified\n" :
            "ERROR: the specified image could not be loaded\n");
        fprintf( stderr, "Usage: peopledetect <inputimage>\n" );
        return -1;
    }

    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
    cv::vector<cv::Rect> found;
    double t = (double)cv::getTickCount();
    // run the detector with default parameters. to get a higher hit-rate
    // (and more false alarms, respectively), decrease the hitThreshold and
    // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
    hog.detectMultiScale(img, found, 0, cv::Size(8,8), cv::Size(24,16), 1.05, 2);
    t = (double)cv::getTickCount() - t;
    printf("Detection time = %gms\n", t*1000./cv::getTickFrequency());
    for( int i = 0; i < (int)found.size(); i++ )
    {
        cv::Rect r = found[i];
        // the HOG detector returns slightly larger rectangles than the real objects.
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.1);
        r.y += cvRound(r.height*0.1);
        r.width = cvRound(r.width*0.8);
        r.height = cvRound(r.height*0.8);
        cv::rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 1);
    }
    cv::namedWindow("people detector", 1);
    cv::imshow("people detector", img);
    cv::waitKey(0);
    return 0;
}
