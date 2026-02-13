#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <string.h>

#define PI 3.1415926
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLIP(x,min,max) MAX(MIN(x,max),min) 

#define YUV400		2
#define YUV420p		0
#define RGB888p		1
#define YUV444p		3

struct Pixel {
	unsigned char R_Y,G_U,B_V;
};

class ColorImage {
	Pixel *pPixel;
	int width, height;
public:
	ColorImage();
	~ColorImage();
	void init(int width, int height);
	Pixel readPixel(int x, int y);
	void writePixel(int x, int y, Pixel p);
	void outputPPM(const char *filename);
	void yuv2rgb();
	void probe(const char *filename, unsigned short *y, int radix);
	void probe(const char *filename, int *y, int *u, int *v, int radix, int pix_fmt);
	void probe(const char *filename, int *r, int *g, int *b, int radix);
	void probe(const char *filename, unsigned short *r, unsigned short *g, unsigned short *b, int radix);
	void probe(const char *filename, unsigned char *r, unsigned char *g, unsigned char *b, int radix);
	void drawLine(int src_x, int src_y, int dst_x, int dst_y, int thick, int r_color, int g_color, int b_color);
	void drawRectangular(int src_x, int src_y, int w, int h, int thick, int r_color, int g_color, int b_color);
	void drawCircle(int src_x, int src_y, int r, int r_color, int g_color, int b_color);
	void drawDashLine(int src_x, int src_y, int dst_x, int dst_y, int r_color, int g_color, int b_color);
};

double psnr (int *ref_img, int *cmp_img, int width, int height, int roi_src_x, int roi_src_y, int roi_width, int roi_height);