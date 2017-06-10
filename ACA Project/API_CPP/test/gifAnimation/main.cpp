
/*
Example of usage of AnimatedGifSaver class.
Saves a small looped 0-1-2-3 animation in file "0123.gif".
From https://sourceforge.net/p/giflib/patches/6/
*/

#include "AnimatedGifSaver.h"
#include <stdio.h>
#include "kernel_runner.h"


// red and white RGB pixels
#define R 255,0,0
#define W 255,255,255

// Lets define a few frames for this little demo...

// ...red and white RGB pixels
#define R 255,0,0
#define W 255,255,255

// ...frames sizes
const int  SX=5;
const int  SY=7;

// ...and, the frames themselves
// (note: they are defined bottom-to-top (a-la OpenGL) so they appear upside-down).

Byte frame0[SX*SY*3] ={
  W,W,W,W,W,
  W,W,R,W,W,
  W,R,W,R,W,
  W,R,W,R,W,
  W,R,W,R,W,
  W,W,R,W,W,
  W,W,W,W,W,
};

Byte frame1[SX*SY*3] ={
  W,W,W,W,W,
  W,W,R,W,W,
  W,W,R,W,W,
  W,W,R,W,W,
  W,R,R,W,W,
  W,W,R,W,W,
  W,W,W,W,W,
};

Byte frame2[SX*SY*3]={
  W,W,W,W,W,
  W,R,R,R,W,
  W,R,W,W,W,
  W,W,R,W,W,
  W,W,W,R,W,
  W,R,R,W,W,
  W,W,W,W,W,
};

Byte frame3[SX*SY*3]={
  W,W,W,W,W,
  W,R,R,W,W,
  W,W,W,R,W,
  W,W,R,W,W,
  W,W,W,R,W,
  W,R,R,W,W,
  W,W,W,W,W,
};

/* These variables and functions are added to support the scale & smooth 
 * functionalities.
 */
Byte frame[SX*2*SY*2*3];
Byte sframe[SX*2*SY*2*3];

void double_frame(Byte *out, Byte *in, int X, int Y){
	int X2=X*2;
	int Y2=Y*2;
	for(int x=0; x<X2; x++)
		for(int y=0; y<Y2; y++)
			for(int c=0; c<3; c++){
				out[y*X2*3+x*3+c]=in[y/2*X*3+x/2*3+c];
			}
}

void copy_frame(Byte *out, Byte *in, int X, int Y){
	for(int x=0; x<X; x++)
		for(int y=0; y<Y; y++)
			for(int c=0; c<3; c++)
				out[y*X*3+x*3+c] =	in[y*X*3+x*3+c];
}

void smooth_frame(Byte *out, Byte *in, int X, int Y){
	for(int x=1; x<X-1; x++)
		for(int y=1; y<Y-1; y++)
			for(int c=0; c<3; c++){
				out[y*X*3+x*3+c]=
					(in[y*X*3+x*3+c]+
					in[y*X*3+(x-1)*3+c]+
					in[y*X*3+(x+1)*3+c]+
					in[(y-1)*X*3+x*3+c]+
					in[(y+1)*X*3+x*3+c]) / 5;
			}
}

int main()
{
  AnimatedGifSaver saver1(SX*2,SY*2);
  AnimatedGifSaver saver2(SX*2,SY*2);
	Byte *frames[4] = { frame0, frame1, frame2, frame3 };
  
	for(int i=3; i>=0; i--){
		double_frame(frame,frames[i],SX,SY);
		copy_frame(sframe,frame, SX*2,SY*2);
		smooth_frame(sframe,frame, SX*2,SY*2);
  	saver1.AddFrame(sframe,i+1); 
	}
  saver1.Save("0123_base.gif");

	printf("Starting setup\n");
	auto kr=KernelRunner(SX,SY);
	printf("Setup complete\n");
	for(int i=3; i>=0; i--){
		printf("Running frame %d\n", i);
		kr.run_kernel(sframe, frames[i]);
  	saver2.AddFrame(sframe,i+1); 		
	}
	printf("All frames processed\n");
  saver2.Save("0123_kernel.gif");
}
