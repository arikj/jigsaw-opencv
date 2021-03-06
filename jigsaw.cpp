/*
	JIGSAW PUZZLE
		
   This code is written in c++ including <cv.h> and <highgui.h> libraries 
   to develop a jigsaw puzzle game.

   An image is loaded randomly from set of images and it is cut into tiles provided 
   by the user and stored in array. The tiles are then scrambled and displayed on right. 
   Player need to drag all tiles and drop to right side in correct positions to complete 
   the image. 	
   
   To compile the code: g++ -o hello hello.cpp `pkg-config opencv --cflags --libs`
   To run the code: ./hello 
 */


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>						// opencv libraries
#include <highgui.h>


int **initial,**final;
int level=0;
int current_drag[4];
IplImage *quarter[100];
IplImage *white;
IplImage *win;
IplImage* jumbled=0;
int over = 0;
char num[100] = "Number of Steps = ";
int steps=0;
std::stringstream ss;
CvFont font; 

void changedisplay(int x, int y);
void initial_drag(int x,int y);


// Implement mouse callback
void my_mouse_callback( int event, int x, int y, int flags, void* param ){

	switch( event ){

		case CV_EVENT_LBUTTONDOWN:
			initial_drag(x,y);			// initial_drag function is called when any tile is dragged
			break;

		case CV_EVENT_LBUTTONUP:

			changedisplay(x,y);			// changedisplay will be called when dragged tile is dropped

			break;
	}
}


// This function will match the coordinates of the mouse click with the tile number 
// and store it in current_drag
void initial_drag(int x,int y)
{


	current_drag[0] = -1;
	current_drag[1] = -1;
	current_drag[2] = -1;
	current_drag[3] = -1;

	for(int i=0;i<level*level;i++)
	{
		if(initial[i][1]<=x && initial[i][3]>=x && initial[i][2]<=y && initial[i][4]>=y  && initial[i][0]!=-1)
		{	 current_drag[0] = i ;			// update current_drag when tiles from left are dragged
			current_drag[2] = i ; 		

			break; }

		if(final[i][1]<=x && final[i][3]>=x && final[i][2]<=y && final[i][4]>=y && final[i][0]!=-1)
		{
			current_drag[1] = i ;			// update current_drag when tiles from right are dragged
			current_drag[3] = i ;

			break;
		}	
	}
}


//This function will change the display when the dragged tiles are dropped at valid positions 
void changedisplay(int x, int y)
{

	int flag=1;

	if(current_drag[0] == -1 && current_drag[1] == -1)
		return;						// return back if no tiles are selected

	else
	{
		for(int i=0;i<level*level;i++)
		{

			if(final[i][1]<=x && final[i][3]>=x && final[i][2]<=y && final[i][4]>=y  )
			{
										// compare if image is dragged from left to right
				if(final[i][0]==-1 && current_drag[1]==-1){
					flag=1;
					final[i][0]=initial[current_drag[0]][0];
					initial[current_drag[0]][0]=-1;
					cvSetImageROI(jumbled, cvRect(final[i][1], final[i][2], final[i][3]-final[i][1],final[i][4]-final[i][2]));
										// copying tile image to new position
					cvResize(quarter[final[i][0]], jumbled,CV_INTER_LINEAR);
					cvResetImageROI(jumbled);


					for(int j=0;j<level*level;j++)		// checking for wining condition
					{
						if(final[j][0]!=j)
						{flag=0;break;}

					}

					if(flag == 0){
						cvSetImageROI(jumbled, cvRect(initial[current_drag[0]][1],initial[current_drag[0]][2],initial[current_drag[0]][3]-initial[current_drag[0]][1],initial[current_drag[0]][4]-initial[current_drag[0]][2]));

						cvResize(white, jumbled,CV_INTER_LINEAR);

						cvResetImageROI(jumbled);	// copying blank image to previous position	

						cvShowImage("mainWin", jumbled);		
					}

					else if(flag==1)
					{	over = 1;


						cvSetImageROI(jumbled, cvRect(initial[0][1],initial[0][2],initial[level*level-1][3]-initial[0][1],initial[level*level-1][4]-initial[0][2]));

						cvResize(win, jumbled,CV_INTER_LINEAR);

						cvResetImageROI(jumbled);

						cvShowImage("mainWin", jumbled);	


					}
					steps++;
				}


				if(final[i][0]==-1 && current_drag[0]==-1){
									// compare if image is dragged from right to right
					final[i][0]=final[current_drag[3]][0];
					final[current_drag[3]][0]=-1;
					cvSetImageROI(jumbled, cvRect(final[i][1], final[i][2], final[i][3]-final[i][1],final[i][4]-final[i][2]));

					cvResize(quarter[final[i][0]], jumbled,CV_INTER_LINEAR);
					cvResetImageROI(jumbled);

					cvSetImageROI(jumbled, cvRect(final[current_drag[3]][1],final[current_drag[3]][2],final[current_drag[3]][3]-final[current_drag[3]][1],final[current_drag[3]][4]-final[current_drag[3]][2]));

					cvResize(white, jumbled,CV_INTER_LINEAR);

					cvResetImageROI(jumbled);

					cvShowImage("mainWin", jumbled);
					steps++;
				}

			}




			if(initial[i][1]<=x && initial[i][3]>=x && initial[i][2]<=y && initial[i][4]>=y  )
			{

				if(initial[i][0]==-1 && current_drag[1]==-1){
										// compare if image is dragged from left to left
					initial[i][0]=initial[current_drag[2]][0];

					initial[current_drag[2]][0]=-1;

					cvSetImageROI(jumbled, cvRect(initial[i][1], initial[i][2], initial[i][3]-initial[i][1],initial[i][4]-initial[i][2]));

					cvResize(quarter[initial[i][0]], jumbled,CV_INTER_LINEAR);
					cvResetImageROI(jumbled);

					cvSetImageROI(jumbled, cvRect(initial[current_drag[2]][1],initial[current_drag[2]][2],initial[current_drag[2]][3]-initial[current_drag[2]][1],initial[current_drag[2]][4]-initial[current_drag[2]][2]));

					cvResize(white, jumbled,CV_INTER_LINEAR);


					cvResetImageROI(jumbled);

					cvShowImage("mainWin", jumbled);
					steps++;

				}


				if(initial[i][0]==-1 && current_drag[0]==-1){
										// compare if image is dragged from right to left
					initial[i][0]=final[current_drag[1]][0];

					final[current_drag[1]][0]=-1;

					cvSetImageROI(jumbled, cvRect(initial[i][1], initial[i][2], initial[i][3]-initial[i][1],initial[i][4]-initial[i][2]));

					cvResize(quarter[initial[i][0]], jumbled,CV_INTER_LINEAR);
					cvResetImageROI(jumbled);

					cvSetImageROI(jumbled, cvRect(final[current_drag[1]][1],final[current_drag[1]][2],final[current_drag[1]][3]-final[current_drag[1]][1],final[current_drag[1]][4]-final[current_drag[1]][2]));

					cvResize(white, jumbled,CV_INTER_LINEAR);



					cvResetImageROI(jumbled);

					cvShowImage("mainWin", jumbled);
					steps++;    
				}



			}

		}
	}

}
// function to scale image  
IplImage* img_resize(IplImage* src_img, int new_width,int new_height)
{
	IplImage* des_img;
	des_img=cvCreateImage(cvSize(new_width,new_height),src_img->depth,src_img->nChannels);
	cvResize(src_img,des_img,CV_INTER_LINEAR);
	return des_img;
} 


//funtion to store tile number, x and y coordinates for each tile
void putvalue(int x, int i,int var,int x1, int y1, int x2, int y2)
{
	if(x==0)
	{ initial[i][0]=var;
		initial[i][1]=x1;
		initial[i][2]=y1;
		initial[i][3]=x2;
		initial[i][4]=y2;
	}

	else
	{
		final[i][0]=var;
		final[i][1]=x1;
		final[i][2]=y1;
		final[i][3]=x2;
		final[i][4]=y2;
	}
}

int main()
{
	IplImage* img = 0; 
	white=cvLoadImage("white.jpg");					//loading image 	
	win=cvLoadImage("win.jpg");
	int height,width,step,channels;
	uchar *data;
	int i,j,k,done=0,space=2;
	int start =0;
	int totalimg = 6;
	char name[100]; 

	printf("Enter the level:\n1 for 2x2 puzzle\n2 for 3x3 puzzle\n    .\n    .\nmaximum level allowed is 9\n");
	scanf("%d",&level);						// input level from user



	if(level>9 || level<0)
	{
		printf("Level not allowed\n");
		exit(0);
	}
	char images[6][20] = {"fruits.jpg","cartoon1.jpg","cartoon2.jpg","cartoon3.jpg","house.jpg","nature.jpg"};


	srand(time(NULL));



	int randimage = rand()%totalimg;				
	img=cvLoadImage(images[randimage]);				// randomly loading an image 
	img=img_resize(img,500,500);

	level+=1;

	initial = (int **) malloc(sizeof(int *)*(level*level));
	final = (int **) malloc(sizeof(int *)*(level*level));		//dynamic array allocation for tiles information

	for(int i=0;i<level*level;i++)
	{
		initial[i] = (int *) malloc(sizeof(int)*5);
		final[i] = (int *) malloc(sizeof(int)*5);

	}
	// get the image data
	height    = img->height;
	width     = img->width;
	step      = img->widthStep;
	channels  = img->nChannels;
	data      = (uchar *)img->imageData;

	CvMat temp;
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
		putvalue(0,i,i,x,y,x+wl,y+hl);
		cvCopy(&temp, quarter[i]);
		quarter[i]=img_resize(quarter[i],neww,newh);

		x=(x+wl)%(level*wl);

		if((i+1)%level == 0)
			y=y+hl; 

	}




	jumbled = cvCreateImage(cvSize(1200, height+100),img->depth,img->nChannels);//defining image size nchannels: number of color grayscale(1) 
	int w=level,h=level;
	int wid=quarter[0]->width;
	int hei=quarter[0]->height;
	int size = quarter[0]->width;
	int m,n,q,gap=50;

	// scrambling image tiles on left side of window 	
	for(i=0,m=space,n=space;i<level*level;i++,m+=(space+size))
	{
		if (i%w==0 && m!=space)
		{
			m=space;
			n+=(space+size);
		}

		cvSetImageROI(jumbled, cvRect(m+gap, n+gap, wid, hei));
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
		putvalue(0,i,q,m+gap,n+gap,m+wid+gap,n+hei+gap);
		cvResize(quarter[q], jumbled,CV_INTER_LINEAR);
		cvResetImageROI(jumbled);
	}

	// inserting white tiles on right side of window
	for(i=0,m=0,n=0;i<level*level;i++,m+=(0+size))
	{
		if (i%w==0 && m!=0)
		{
			m=0;
			n+=size;
		}
		putvalue(1,i,-1,600+m+gap,n+gap,600+m+wid+gap,n+hei+gap);

		cvSetImageROI(jumbled, cvRect(600+m+gap, n+gap, wid, hei));

		cvResize(white, jumbled,CV_INTER_LINEAR);

		cvResetImageROI(jumbled);

	}



	// create a window
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
	cvMoveWindow("mainWin", 100, 100);

	// Set up the callback
	cvSetMouseCallback("mainWin", my_mouse_callback, (void*) jumbled);

	font = cv::fontQt("Times",15,255,255,255,255);
	cv::addText( img,"Your image", cv::Point(200,50), font);
	cv::addText( img,"Press any key to continue", cv::Point(100,450), font);
	cv::displayOverlay("mainWin", "JIG-SAW PUZZLE", 0);				// displaying original image
	cvShowImage("mainWin", img);
	cvWaitKey(0);
	cvReleaseImage(&img);

	CvFont font = cv::fontQt("Times",15,CV_RGB(255,255,255));
	cv::addText(jumbled,"Drag drop to move pictures", cv::Point(700,40), font);

	while(1)
	{

		cv::displayOverlay("mainWin", "JIG-SAW PUZZLE", 0);


		cvShowImage("mainWin", jumbled);

		if(over == 1)
		{
			printf("Number of steps taken: %d\n",steps);
			exit(0);
		}
		cvWaitKey(0);


	}
	return 0;
}


