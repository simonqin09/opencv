#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
 
#include <iostream>
#include "stdio.h"
 
using namespace std;
using namespace cv;
 
CascadeClassifier face_cascade;
string window_name = "Face Detection Demo";
String face_cascade_name = "/home/root/haarcascade_frontalface_alt2.xml";
const int BORDER = 8;  /* order between GUI elements to the edge of the image */
 
template <typename T> string toString(T t)
{
    ostringstream out;
    out << t;
 
    return out.str();
}
 
/*
 * Draw text into an image. Defaults to top-left-justified text,
 * but you can give negative x coords for right-justified text,
 * and/or negative y coords for bottom-justified text
 * Returns the bounding rect around the drawn text
*/
Rect drawString(Mat img, string text, Point coord, Scalar color,
        float fontScale = 0.6f, int thickness = 1, int fontFace = FONT_HERSHEY_COMPLEX)
{
    /* Get the text size & baseline */
    int baseline = 0;
    Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);
    baseline += thickness;
 
    /* Adjust the coords for left/right-justified or top/bottom-justified */
    if (coord.y >= 0) {
        /*
         * Coordinates are for the top-left corner of the text
         * from the top-left of the image, so move down by one row.
         */
        coord.y += textSize.height;
    } else {
        /*
         * Coordinates are for the bottom-left corner of the text
         * from the bottom-left of the image, so come up from the bottom
         */
            coord.y += img.rows - baseline + 1;
    }
 
    /* Become right-justified if desired */
    if (coord.x < 0) {
        coord.x += img.cols - textSize.width + 1;
    }
 
    /* Get the bounding box around the text */
    Rect boundingRect = Rect(coord.x, coord.y - textSize.height, textSize.width, baseline + textSize.height);
 
    /* Draw anti-aliased text */
    putText(img, text, coord, fontFace, fontScale, color, thickness, CV_AA);
 
    /* Let the user know how big their text is, in case they want to arrange things */
    return boundingRect;
}
 
int main(int argc, const char** argv)
{
    VideoCapture capture;
    Mat frame;
    std::vector<Rect> faces;
    Mat frame_gray;
 
 
    if (!face_cascade.load( face_cascade_name ) ) {
        printf("--(!)Error loading training file: haarcascade_frontalface_alt2.xml\n");
        return -1;
    };
 
    try {
        capture.open("v4l2:///dev/video0");
        capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    }
    catch (cv::Exception &e)
    {
        const char *err_msg = e.what();
        cout << "Exception caught: " << err_msg << endl;
    }
 
    if ( !capture.isOpened() ) {
        cout << "ERROR: Could not access the camera!" << endl;
        exit(1);
    }
 
    while(true) {
        capture >> frame;
 
        if (!frame.empty()) {
 
            cvtColor(frame, frame_gray, CV_BGR2GRAY);
            equalizeHist(frame_gray, frame_gray);
 
            face_cascade.detectMultiScale(frame_gray, faces, 1.2, 3, CV_HAAR_DO_CANNY_PRUNING, Size(80, 80));
 
            for (size_t i = 0; i < faces.size(); i++) {
                CvPoint pt1 = { faces[i].x, faces[i].y };
                CvPoint pt2 = { faces[i].x + faces[i].width, faces[i].y + faces[i].height };
                rectangle(frame, pt1, pt2, CV_RGB(0, 255, 0), 3, 4, 0);
 
                Mat faceROI = frame_gray(faces[i]);
            }
            string stringToDisplay = "Number Of Faces: " + toString(faces.size());
 
            drawString(frame, stringToDisplay, Point(BORDER, -BORDER - 2 - 50), CV_RGB(0, 0, 0));
            drawString(frame, stringToDisplay, Point(BORDER + 1, -BORDER - 1 - 50), CV_RGB(0, 255, 0));
 
            imshow(window_name, frame);
        } else {
            printf(" --(!) No captured frame");
        }
 
        int c = waitKey(1);
        if ((char)c == 27) {
            break;
        }
    }
 
    return 0;
}
