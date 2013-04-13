/*
code to scramble given image to number of tiles given by user
To run the code : g++ -o hello hello.cpp `pkg-config opencv --cflags --libs`
		  ./hello <imagefile> <level(1 or 2 or 3 ....)>
*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>						// opencv libraries
#include <highgui.h>

// function to scale image  
IplImage* img_resize(IplImage* src_img, int new_width,int new_height)
{
	IplImage* des_img;
	des_img=cvCreateImage(cvSize(new_width,new_height),src_img->depth,src_img->nChannels);
	cvResize(src_img,des_img,CV_INTER_LINEAR);
	return des_img;
} 

int main(int argc, char *argv[])
{
	IplImage* img = 0; 
	IplImage* jumbled=0;
	IplImage *white=cvLoadImage("white.jpg");	//loading image 	
	int height,width,step,channels;
	uchar *data;
	int i,j,k,done=0,space=2;
	srand(time(NULL));

	if(argc<2){
		printf("Usage: main <image-file-name>\n\7");
		exit(0);
	}

	// load an image  
	img=cvLoadImage(argv[1]);
	img=img_resize(img,500,500);
	if(!img){
		printf("Could not load image file: %s\n",argv[1]);
		exit(0);
	}

	int level= atoi(argv[2]);	
	level+=1;

	// get the image data
	height    = img->height;
	width     = img->width;
	step      = img->widthStep;
	channels  = img->nChannels;
	data      = (uchar *)img->imageData;
	printf("Processing a %dx%d image with %d channels\n",height,width,channels); 

	//IplImage **quarter = (IplImage *)malloc(sizeof(IplImage *)*(level*level));

	CvMat temp;
	IplImage *quarter[100];
	int check[100]={0};
	int x=0,y=0;
	int wl=width/level;
	int hl=height/level;
	int neww=600/(level+1);
	int newh=600/(level+1);

	white = img_resize(white,neww,newh);

	// loop to breakdown image into tiles according to user input and store each part of image in array quarter
	for(int i=0;i<level*level;i++)
	{
		quarter[i] =  cvCreateImage(cvSize(wl, hl),img->depth,img->nChannels);

		cvGetSubRect(img, &temp, cvRect(x,y,wl, hl));
		cvCopy(&temp, quarter[i]);
		quarter[i]=img_resize(quarter[i],neww,newh);

		x=(x+wl)%(level*wl);

		if((i+1)%level == 0)
			y=y+hl; 

	}

	// create a window
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
	cvMoveWindow("mainWin", 100, 100);


	jumbled = cvCreateImage(cvSize(1200, height+100),img->depth,img->nChannels);		//defining
	int w=level,h=level;
	int wid=quarter[0]->width;
	int hei=quarter[0]->height;
	int size = quarter[0]->width;
	int m,n,q;

        // scrambling image tiles on left side of window 	
	for(i=0,m=space,n=space;i<level*level;i++,m+=(space+size))
	{
		if (i%w==0 && m!=space)
		{
			m=space;
			n+=(space+size);
		}

		cvSetImageROI(jumbled, cvRect(m, n, wid, hei));
		done=0;
		while(!done)
		{
			q=rand()%(level*level);
			if(check[q]==0)
			{
				done=1;
				check[q]=1;
			}
		}
		cvResize(quarter[q], jumbled);

		cvResetImageROI(jumbled);
	}

	// inserting white tiles on right side of window
	for(i=0,m=space,n=space;i<level*level;i++,m+=(space+size))
	{
		if (i%w==0 && m!=space)
		{
			m=space;
			n+=(space+size);
		}

		cvSetImageROI(jumbled, cvRect(600+m, n, wid, hei));

		cvResize(white, jumbled);

		cvResetImageROI(jumbled);
	}

	cvShowImage("mainWin", jumbled);		// displaying image
	cvWaitKey(0);
	cvReleaseImage(&jumbled);


	return 0;
}


