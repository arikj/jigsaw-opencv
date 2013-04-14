/*
code to scramble given image to number of tiles given by user
To run the code : g++ -o hello hello.cpp `pkg-config opencv --cflags --libs`
		  ./hello <imagefile> <level(1 or 2 or 3 ....)>
*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <sstream>
#include <cv.h>						// opencv libraries
#include <highgui.h>


int **initial,**final;
bool dragging = false;
int level=0;
int current_drag[4] = {-1};	//l->r:0 r->l:1 l->l:2 r->r:3 else -1
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
                        initial_drag(x,y);
                        break;

                case CV_EVENT_LBUTTONUP:
			
                        changedisplay(x,y);

                        break;
        }
}


void initial_drag(int x,int y)
{


current_drag[0] = -1;
current_drag[1] = -1;
current_drag[2] = -1;
current_drag[3] = -1;

for(int i=0;i<level*level;i++)
	{
	        //printf("0:%d 1:%d 2:%d 3:%d x=%d y=%d\n",initial[i][1],initial[i][2],initial[i][3],initial[i][4],x,y);

   	if(initial[i][1]<=x && initial[i][3]>=x && initial[i][2]<=y && initial[i][4]>=y  && initial[i][0]!=-1)
		{ current_drag[0] = i ;
	          current_drag[2] = i ; 
				  	
		break; }

	if(final[i][1]<=x && final[i][3]>=x && final[i][2]<=y && final[i][4]>=y && final[i][0]!=-1)
	{
		 current_drag[1] = i ;
                  current_drag[3] = i ;

                break;
	}	
}
}



void changedisplay(int x, int y)
{

int flag=1;

if(current_drag[0] == -1 && current_drag[1] == -1)
 	return;

else
{
	for(int i=0;i<level*level;i++)
        {

        if(final[i][1]<=x && final[i][3]>=x && final[i][2]<=y && final[i][4]>=y  )
                {
		
		if(final[i][0]==-1 && current_drag[1]==-1){
		flag=1;
		final[i][0]=initial[current_drag[0]][0];
		initial[current_drag[0]][0]=-1;
		 cvSetImageROI(jumbled, cvRect(final[i][1], final[i][2], final[i][3]-final[i][1],final[i][4]-final[i][2]));

                cvResize(quarter[final[i][0]], jumbled);
		     cvResetImageROI(jumbled);

		
		for(int j=0;j<level*level;j++)
			{
			if(final[j][0]!=j)
				{flag=0;break;}
			
			}

		if(flag == 0){
		cvSetImageROI(jumbled, cvRect(initial[current_drag[0]][1],initial[current_drag[0]][2],initial[current_drag[0]][3]-initial[current_drag[0]][1],initial[current_drag[0]][4]-initial[current_drag[0]][2]));

                cvResize(white, jumbled);

		     cvResetImageROI(jumbled);
		
 		  cvShowImage("mainWin", jumbled);
		}

		else if(flag==1)
		{	over = 1;


			cvSetImageROI(jumbled, cvRect(initial[0][1],initial[0][2],initial[level*level-1][3]-initial[0][1],initial[level*level-1][4]-initial[0][2]));

                cvResize(win, jumbled);

		     cvResetImageROI(jumbled);
		
 		  cvShowImage("mainWin", jumbled);	
/*			cv::addText( jumbled,"You Win", cv::Point(500,550), font);
			
			cvResetImageROI(jumbled);
			
 		  cvShowImage("mainWin", jumbled);	
*/			//cvReleaseImage(&jumbled);
			
			

		}
		steps++;
              }

		
		 if(final[i][0]==-1 && current_drag[0]==-1){
                
                final[i][0]=final[current_drag[3]][0];
                final[current_drag[3]][0]=-1;
                 cvSetImageROI(jumbled, cvRect(final[i][1], final[i][2], final[i][3]-final[i][1],final[i][4]-final[i][2]));

                cvResize(quarter[final[i][0]], jumbled);
                     cvResetImageROI(jumbled);

                cvSetImageROI(jumbled, cvRect(final[current_drag[3]][1],final[current_drag[3]][2],final[current_drag[3]][3]-final[current_drag[3]][1],final[current_drag[3]][4]-final[current_drag[3]][2]));

                cvResize(white, jumbled);
		
                     cvResetImageROI(jumbled);
		
                  cvShowImage("mainWin", jumbled);
		steps++;
		}

	     }




		  if(initial[i][1]<=x && initial[i][3]>=x && initial[i][2]<=y && initial[i][4]>=y  )
                {

                if(initial[i][0]==-1 && current_drag[1]==-1){
       		
		initial[i][0]=initial[current_drag[2]][0];

                 initial[current_drag[2]][0]=-1;

                 cvSetImageROI(jumbled, cvRect(initial[i][1], initial[i][2], initial[i][3]-initial[i][1],initial[i][4]-initial[i][2]));

                cvResize(quarter[initial[i][0]], jumbled);
                     cvResetImageROI(jumbled);

                cvSetImageROI(jumbled, cvRect(initial[current_drag[2]][1],initial[current_drag[2]][2],initial[current_drag[2]][3]-initial[current_drag[2]][1],initial[current_drag[2]][4]-initial[current_drag[2]][2]));

                cvResize(white, jumbled);
		

                     cvResetImageROI(jumbled);
		
                  cvShowImage("mainWin", jumbled);
		steps++;
		
              }


                if(initial[i][0]==-1 && current_drag[0]==-1){
		
                initial[i][0]=final[current_drag[1]][0];

                 final[current_drag[1]][0]=-1;

                 cvSetImageROI(jumbled, cvRect(initial[i][1], initial[i][2], initial[i][3]-initial[i][1],initial[i][4]-initial[i][2]));

                cvResize(quarter[initial[i][0]], jumbled);
                     cvResetImageROI(jumbled);

                cvSetImageROI(jumbled, cvRect(final[current_drag[1]][1],final[current_drag[1]][2],final[current_drag[1]][3]-final[current_drag[1]][1],final[current_drag[1]][4]-final[current_drag[1]][2]));

                cvResize(white, jumbled);
		

		
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

int main(int argc, char *argv[])
{
	IplImage* img = 0; 
	//IplImage* jumbled=0;
	white=cvLoadImage("white.jpg");	//loading image 	
	win=cvLoadImage("win.jpg");
	int height,width,step,channels;
	uchar *data;
	int i,j,k,done=0,space=2;
	int start =0;
	int totalimg = 6;

	char images[6][20] = {"fruits.jpg","cartoon1.jpg","cartoon2.jpg","cartoon3.jpg","house.jpg","nature.jpg"};


	srand(time(NULL));

	if(argc<1){
		printf("Please enter the level\n\7");
		exit(0);
	}

	int randimage = rand()%totalimg;	
	// load an image  
	img=cvLoadImage(images[randimage]);
	img=img_resize(img,500,500);
	

	level= atoi(argv[1]);	
	level+=1;

	initial = (int **) malloc(sizeof(int *)*(level*level));
	final = (int **) malloc(sizeof(int *)*(level*level));

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
	printf("Processing a %dx%d image with %d channels\n",height,width,channels); 

	//IplImage **quarter = (IplImage *)malloc(sizeof(IplImage *)*(level*level));

	CvMat temp;
	//IplImage *quarter[100];
	int check[100]={0};
	int x=0,y=0;
	int wl=width/level;
	int hl=height/level;
	int neww=600/(level+1);
	int newh=600/(level+1);

   	//int initial[25][5],final[25][5];

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
//                printf("%d %d %d %d\n",initial[i][1],initial[i][2],initial[i][3],initial[i][4]);

	}

	


	jumbled = cvCreateImage(cvSize(1200, height+100),img->depth,img->nChannels);		//defining
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
		cvResize(quarter[q], jumbled);
//		printf("i:%d\n",jumblearray[i]);
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

		cvResize(white, jumbled);

		cvResetImageROI(jumbled);

	}

	
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
	cvMoveWindow("mainWin", 100, 100);
	
	cvSetMouseCallback("mainWin", my_mouse_callback, (void*) jumbled);

	font = cv::fontQt("Times",15,255,255,255,255);
	cv::addText( img,"Your image", cv::Point(200,50), font);
	cv::addText( img,"Press any key to continue", cv::Point(100,450), font);
	cv::displayOverlay("mainWin", "JIG-SAW PUZZLE", 0);
	cvShowImage("mainWin", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	
	// Set up the callback

	// create a window
	
	while(1)
	{
//     cvResetImageROI(jumbled);

	cv::displayOverlay("mainWin", "JIG-SAW PUZZLE", 0);

	
	CvFont font = cv::fontQt("Times",15,CV_RGB(255,255,255));

	cvShowImage("mainWin", jumbled);
	
	if(over == 1)
	{
	printf("Number of steps taken: %d\n",steps);
	exit(0);
	}
	cvWaitKey(0);
	
	cvReleaseImage(&jumbled);
	
	}
	return 0;
}


