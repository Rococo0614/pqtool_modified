#include "img.h"


ColorImage::ColorImage(){
	pPixel = 0;
}

ColorImage::~ColorImage(){
	if (pPixel)
	{
		pPixel = 0;
		delete[] pPixel;
	}
	pPixel = 0;
}

void ColorImage::init(int x, int y) {
	width = x;
	height = y;
	pPixel = new Pixel[width*height];
	for (int i = 0; i < width*height; i++){
		Pixel p = { 0,0,0 };
		pPixel[i] = p;
	}
}

Pixel ColorImage::readPixel(int x, int y){
	assert(pPixel);
	return pPixel[x + y*width];
}

void ColorImage::writePixel(int x, int y, Pixel p){
	assert(pPixel);
	int xx = (x >= width) ? width-1 : ((x < 0) ? 0 : x);
	int yy = (y >= height) ? height-1 : ((y < 0) ? 0 : y);
	pPixel[xx + yy*width] = p;
}

void ColorImage::yuv2rgb(){
	for (int i=0; i<height; i++){
		for (int j=0; j<width; j++){
			Pixel p = pPixel[i*width+j];
			int r = (int)(p.R_Y + 1.402*(p.B_V-128));
			pPixel[i*width+j].R_Y = CLIP(r, 0, 255);
			int g = (int)(p.R_Y - 0.344*(p.G_U-128) - 0.714*(p.B_V-128));
			pPixel[i*width+j].G_U = CLIP(g, 0, 255);
			int b = (int)(p.R_Y + 1.772*(p.G_U-128));
			pPixel[i*width+j].B_V = CLIP(b, 0, 255);
		}
	}
}

void ColorImage::outputPPM(const char *filename){
	FILE *outFile = fopen(filename, "wb");
	assert(outFile);
	fprintf(outFile, "P6 %d %d 255\n", width, height);
	fwrite(pPixel, 1, 3*width*height, outFile);
	fclose(outFile);
}

void ColorImage::probe(const char *filename, unsigned short *_y, int radix){
	for (int j=0; j<height; j++){
		for (int i=0; i<width; i++){
			unsigned short y = (unsigned short)((float)_y[j*width + i] * 256.f/(float)radix);
			Pixel p = { y, 128, 128};
			writePixel( i, j, p);
		}
	}
	yuv2rgb();
	outputPPM(filename);	
}

void ColorImage::probe(const char *filename, int *_y, int *_u, int *_v, int radix, int pix_fmt){
	if (pix_fmt == YUV400){
		for (int j=0; j<height; j++){
			for (int i=0; i<width; i++){
				unsigned short y = (unsigned short)((float)_y[j*width + i] * 256.f/(float)radix);
				Pixel p = { y, 128, 128};
				writePixel( i, j, p);
			}
		}
	}
	else if (pix_fmt == YUV420p){
		for (int j=0; j<height; j++){
			for (int i=0; i<width; i++){
				unsigned short y = (unsigned short)((float)_y[j*width + i] * 256.f/(float)radix);
				unsigned short u = (unsigned short)((float)_u[(j/2)*(width/2) + (i/2)] * 256.f/(float)radix);
				unsigned short v = (unsigned short)((float)_v[(j/2)*(width/2) + (i/2)] * 256.f/(float)radix);
				Pixel p = { y, u, v};
				writePixel( i, j, p);
			}
		}
	}
	else if (pix_fmt == YUV444p){
		for (int j=0; j<height; j++){
			for (int i=0; i<width; i++){
				unsigned short y = (unsigned short)((float)_y[j*width + i] * 256.f/(float)radix);
				unsigned short u = (unsigned short)((float)_u[j*width + i] * 256.f/(float)radix);
				unsigned short v = (unsigned short)((float)_v[j*width + i] * 256.f/(float)radix);
				Pixel p = { y, u, v};
				writePixel( i, j, p);
			}
		}
	}
	yuv2rgb();
	outputPPM(filename);	
}

void ColorImage::probe (const char *filename, int *_r, int *_g, int *_b, int radix)
{
	for (int j=0; j<height; j++){
		for (int i=0; i<width; i++){
			unsigned short r = MIN((unsigned short)((float)_r[j*width + i] * 256.f/(float)radix), 255);
			unsigned short g = MIN((unsigned short)((float)_g[j*width + i] * 256.f/(float)radix), 255);
			unsigned short b = MIN((unsigned short)((float)_b[j*width + i] * 256.f/(float)radix), 255);
			Pixel p = { r, g, b};
			writePixel( i, j, p);
		}
	}
	outputPPM(filename);
}
void ColorImage::probe (const char *filename, unsigned short *_r, unsigned short *_g, unsigned short *_b, int radix)
{
	for (int j=0; j<height; j++){
		for (int i=0; i<width; i++){
			unsigned short r = MIN((unsigned short)((float)_r[j*width + i] * 256.f/(float)radix), 255);
			unsigned short g = MIN((unsigned short)((float)_g[j*width + i] * 256.f/(float)radix), 255);
			unsigned short b = MIN((unsigned short)((float)_b[j*width + i] * 256.f/(float)radix), 255);
			Pixel p = { r, g, b};
			writePixel( i, j, p);
		}
	}
	outputPPM(filename);
}
void ColorImage::probe (const char *filename, unsigned char *_r, unsigned char *_g, unsigned char *_b, int radix)
{
	for (int j=0; j<height; j++){
		for (int i=0; i<width; i++){
			unsigned short r = MIN((unsigned short)((float)_r[j*width + i] * 256.f/(float)radix), 255);
			unsigned short g = MIN((unsigned short)((float)_g[j*width + i] * 256.f/(float)radix), 255);
			unsigned short b = MIN((unsigned short)((float)_b[j*width + i] * 256.f/(float)radix), 255);
			Pixel p = { r, g, b};
			writePixel( i, j, p);
		}
	}
	outputPPM(filename);
}

void ColorImage::drawRectangular(int src_x, int src_y, int w, int h, int thick, int r_color, int g_color, int b_color)
{
	Pixel p = {r_color, g_color, b_color};
	for (int j = 0; j < h; j ++){
		int x = src_x;
		int xx = src_x + w - 1;
		int y = src_y + j;
		for (int i = 0; i < thick; i ++){
			writePixel(x+i, y, p);
			writePixel(xx+i, y, p);
		}
	}
		
	for (int i = 0; i < w; i ++){
		int x = src_x + i;
		int y = src_y;
		int yy = src_y + h - 1;
		for (int j = 0; j < thick; j ++){
			writePixel(x, y+j, p);
			writePixel(x, yy+j, p);
		}
	}
}

void ColorImage::drawCircle(int src_x, int src_y, int r, int r_color, int g_color, int b_color)
{
	Pixel p = {r_color, g_color, b_color};
	float phi;
	float delta = 1.f/(2.f*r);

	for (float phi = 0; phi < 2*PI; phi += delta){
		float delta_x = r * cos(phi);
		float delta_y = r * sin(phi);
		int x = (int)(src_x + delta_x + 0.5);
		int y = (int)(src_y + delta_y + 0.5);
		writePixel(x, y, p);
	}
}

void ColorImage::drawDashLine(int src_x, int src_y, int dst_x, int dst_y, int r_color, int g_color, int b_color)
{
	Pixel p = { r_color, g_color, b_color };
	int interval = 16;
	float dda_x, dda_y;
	int dx = dst_x - src_x;
	int dy = dst_y - src_y; 
	int num;
	if (abs(dx) > 0 || abs(dy) > 0) {
		if (abs(dx) > abs(dy)) {
			dda_x = (dx >= 0) ? 1 : -1;
			dda_y = (dy >= 0) ? (float)abs(dy) / (float)abs(dx) : -1 * (float)abs(dy) / (float)abs(dx);
			num = abs(dx) + 1;
		}
		else {
			dda_x = (dx >= 0) ? (float)abs(dx) / (float)abs(dy) : -1 * (float)abs(dx) / (float)abs(dy);
			dda_y = (dy >= 0) ? 1 : -1;
			num = abs(dy) + 1;
		}
		int cnt = 0;
		int isDraw = 1;
		for (int i = 0; i < num; i++) {
			if (isDraw) {
				int x = (int)((float)src_x + dda_x * i + 0.5);
				int y = (int)((float)src_y + dda_y * i + 0.5);
				writePixel(x, y, p);
			}
			cnt++;
			if (cnt == interval) {
				cnt = 0;
				isDraw = !isDraw & 0x1;
			}
		}
	}
}

////	analysis metric

double psnr (int *ref_img, int *cmp_img, int width, int height, int roi_src_x, int roi_src_y, int roi_width, int roi_height)
{
	// support for rgb888 or yuv444
	double psnr;
	double sse = 0;
	double mse = 0;
	int ch1_idx = 0;
	int ch2_idx = width * height;
	int ch3_idx = 2 * width * height;
	for (int j = roi_src_y; j < roi_src_y + roi_height; j ++){
		for (int i = roi_src_x; i < roi_src_x + roi_width; i ++){
			int pos = j * width + i;
			int err_ch1 = ref_img[pos + ch1_idx] - cmp_img[pos + ch1_idx];
			int err_ch2 = ref_img[pos + ch2_idx] - cmp_img[pos + ch2_idx];
			int err_ch3 = ref_img[pos + ch3_idx] - cmp_img[pos + ch3_idx];
			sse += err_ch1 * err_ch1 + err_ch2 * err_ch2 + err_ch3 * err_ch3;
		}
	}

	 if( sse < 1) // for small values return zero
        return 0;
    else
    {
        double mse  = sse / (double)((3*roi_width*roi_height));
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }
}

unsigned char *readPPM(const char *filename)
{
	//char buff[100];
	int w, h;
    unsigned char *img;
    FILE *fp;
    int pix_max_val;

    //open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		exit(1);
    }

	char magic_number[20];
	char magic_number3[128];
	fscanf(fp, "%s", &magic_number, 20);
	fscanf(fp, "%s", &magic_number3, 128);
	if (!strcmp(magic_number3, "#")) {
		fscanf( fp, "%s", &magic_number3,128);
		fscanf( fp, "%s", &magic_number3,128);
        fscanf( fp, "%s", &magic_number3,128);
	}
	else
		fseek(fp, 2L, SEEK_SET);

	fscanf( fp, "%d", &w);
	fscanf( fp, "%d", &h);
	fscanf( fp, "%d", &pix_max_val);

	while (fgetc(fp) != '\n') ;

	img = new unsigned char [3*w*h];

	fread(img, 3*w, h, fp);
	fclose(fp);
	return img;
}