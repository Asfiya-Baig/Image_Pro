#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <fstream>
#include <highgui.h>



using namespace std;
using namespace cv;

Mat image, image_RGB, output, buffer;
char key;
float newval, area[10];
vector<Point> approx;
RotatedRect temp;

int main()
{
VideoCapture capture("Name_fo_the_video.avi");	//Opens the camera of the device connected
capture>>image_RGB;
while(1)
{
    capture>>image_RGB;
    cvtColor(image_RGB, image, CV_BGR2GRAY); 
    Mat drawing = image.clone(); 
    image = 5*image-50;
    threshold(image,output, 40, 255, THRESH_BINARY_INV); 
    // Create a structuring element (SE)
    int morph_size = 2;
    Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    
    morphologyEx( output, output, MORPH_CLOSE, element, Point(-1,-1), 5); 
    
    vector<vector<Point> > contours;	//Defining Contour variable as a vector-list of list of points
    findContours(output.clone(), contours, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    for (unsigned int i = 0; i < contours.size(); i++)
	{
		
    		if (contourArea(contours[i]) > 4000)
		{
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
		//if(approx.size()>=6 && approx.size()< 15)
		{
		//cout<<approx.size()<<endl;
		temp = fitEllipse(Mat(contours[i]));
		area[i] = contourArea(contours[i]);
   		//drawContours(drawing, contours, i,(0,0,255),3);
		ellipse(drawing, temp, Scalar(255,255,255), 3, 8);
		}
		}
	}
    if(image.rows && image.cols)
    {
        namedWindow("Input",WINDOW_NORMAL);		//Create a image window for displaying the output frames
        imshow("Input",drawing); 		//Live feed
	namedWindow("Output",WINDOW_NORMAL);		//Create a image window for displaying the output frames
        imshow("Output",output);

    }

    else
    {
        cout<<"No more frames "<<endl;
        break;
    }

    key = waitKey(100); 	//Capture Keyboard stroke
    if (char(key) == 27)
    {
	break; 		//If you hit ESC key loop will break and code will terminate
    }
}

capture.release();
return 0;

}
