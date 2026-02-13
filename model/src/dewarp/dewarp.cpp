#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "img.h"
#include "dewarp_cfg.h"
#include "dewarp.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define M_PI   3.14159265358979323846264338327950288
#define minmax(a,b,c)  (((a)<(b))? (b):((a)>(c))? (c):(a))

using namespace cv; 
using namespace std;

int RGB888Image2PPM(unsigned char* rgb888, int dimx, int dimy, char* filename);
void Rotate3D(Vector3D* v3d, RotMatrix3D* mat);
void Equidistant(Vector3D* v3d, Vector2D* v2d, double f);
void _RegionView2(FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, MOUNTMODE MOUNT, double x0, double y0, double r);
void _Panorama360View2(FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, MOUNTMODE MOUNT, double x0, double y0, double r);
void _Panorama180View2(FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, MOUNTMODE MOUNT, double x0, double y0, double r);
void _Panorama360View(unsigned char* src_img, unsigned char* rgn_img, unsigned char* src_rois, FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double x0, double y0, double r);
void _Panorama180View(unsigned char* src_img, unsigned char* rgn_img, unsigned char* src_rois, FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double x0, double y0, double r);
void _RegionView(unsigned char* src_img, unsigned char* rgn_img, unsigned char* src_rois, FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double x0, double y0, double r);

void _LOAD_REGION_CONFIG(FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION)
{
	// to make sure parameters aligned to frame ratio
	double width_sec = FISHEYE_CONFIG->OutW_disp / 40;
	double height_sec = FISHEYE_CONFIG->OutH_disp / 40;

	// load default settings
	if (/*FISHEYE_CONFIG->MntMode == CEILING && */FISHEYE_CONFIG->UsageMode == MODE_02_1O4R )
	{
		FISHEYE_REGION[0].RegionValid = 1;
		FISHEYE_REGION[0].MeshVer = 8;
		FISHEYE_REGION[0].MeshHor = 8;
		FISHEYE_REGION[0].ViewMode = PROJECTION_REGION;
		FISHEYE_REGION[0].ThetaX = M_PI / 4;
		FISHEYE_REGION[0].ThetaZ = 0;
		FISHEYE_REGION[0].ThetaY = 0;
		FISHEYE_REGION[0].ZoomH = 2048;							// zooming factor for horizontal:	0 ~ 4095 to control view 	
		FISHEYE_REGION[0].ZoomV = 2048;							// zooming factor for vertical:		0 ~ 4095 to control view
		FISHEYE_REGION[0].Pan  = UI_CTRL_VALUE_CENTER;			// center = 180, value = 0 ~ 360	=> value = +- 180 degrees	
		FISHEYE_REGION[0].Tilt = UI_CTRL_VALUE_CENTER;			// center = 180, value = 0 ~ 360	=> value = +-30 degrees
		FISHEYE_REGION[0].OutW = (int)(width_sec * 15);
		FISHEYE_REGION[0].OutH = (int)(height_sec * 20);
		FISHEYE_REGION[0].OutX = 0;
		FISHEYE_REGION[0].OutY = 0;

		FISHEYE_REGION[1].RegionValid = 1;
		FISHEYE_REGION[1].MeshVer = 8;
		FISHEYE_REGION[1].MeshHor = 8;
		FISHEYE_REGION[1].ViewMode = PROJECTION_REGION;
		FISHEYE_REGION[1].ThetaX = M_PI / 4;
		FISHEYE_REGION[1].ThetaZ = M_PI / 2;
		FISHEYE_REGION[1].ThetaY = 0;
		FISHEYE_REGION[1].ZoomH = 2048;
		FISHEYE_REGION[1].ZoomV = 2048;
		FISHEYE_REGION[1].Pan	= UI_CTRL_VALUE_CENTER;							// theta-X
		FISHEYE_REGION[1].Tilt	= UI_CTRL_VALUE_CENTER;						// theta-Z
		FISHEYE_REGION[1].OutW	= (int)(width_sec * 15);
		FISHEYE_REGION[1].OutH	= (int)(height_sec * 20);
		FISHEYE_REGION[1].OutX	 = (int)(width_sec * 15);
		FISHEYE_REGION[1].OutY	= (int)(height_sec * 0);

		FISHEYE_REGION[2].RegionValid = 1;
		FISHEYE_REGION[2].MeshVer = 8;
		FISHEYE_REGION[2].MeshHor = 8;
		FISHEYE_REGION[2].ViewMode = PROJECTION_REGION;
		FISHEYE_REGION[2].ThetaX = M_PI / 4;
		FISHEYE_REGION[2].ThetaZ = M_PI;
		FISHEYE_REGION[2].ThetaY = 0;
		FISHEYE_REGION[2].ZoomH = 2048;
		FISHEYE_REGION[2].ZoomV = 2048;
		FISHEYE_REGION[2].Pan = UI_CTRL_VALUE_CENTER;						// theta-X
		FISHEYE_REGION[2].Tilt = UI_CTRL_VALUE_CENTER;					// theta-Z
		FISHEYE_REGION[2].OutW = (int)(width_sec * 15);
		FISHEYE_REGION[2].OutH = (int)(height_sec * 20);
		FISHEYE_REGION[2].OutX = (int)(width_sec * 0);
		FISHEYE_REGION[2].OutY = (int)(height_sec * 20);

		FISHEYE_REGION[3].RegionValid = 1;
		FISHEYE_REGION[3].MeshVer = 8;
		FISHEYE_REGION[3].MeshHor = 8;
		FISHEYE_REGION[3].ViewMode = PROJECTION_REGION;
		FISHEYE_REGION[3].ThetaX = M_PI / 4;
		FISHEYE_REGION[3].ThetaZ = 3 * M_PI / 2;
		FISHEYE_REGION[3].ThetaY = 0;
		FISHEYE_REGION[3].ZoomH = 2048;
		FISHEYE_REGION[3].ZoomV = 2048;
		FISHEYE_REGION[3].Pan = UI_CTRL_VALUE_CENTER;						// theta-X
		FISHEYE_REGION[3].Tilt = UI_CTRL_VALUE_CENTER;						// theta-Z
		FISHEYE_REGION[3].OutW = (int)(width_sec * 15);
		FISHEYE_REGION[3].OutH = (int)(height_sec * 20);
		FISHEYE_REGION[3].OutX = (int)(width_sec * 15);
		FISHEYE_REGION[3].OutY = (int)(height_sec * 20);

		FISHEYE_REGION[4].RegionValid = 0;
	}
	else if (/*FISHEYE_CONFIG->MntMode == CEILING &&  */FISHEYE_CONFIG->UsageMode == MODE_03_4R)
	{
		FISHEYE_REGION[0].RegionValid = 1;
		FISHEYE_REGION[0].MeshVer = 16;
		FISHEYE_REGION[0].MeshHor = 16;
		FISHEYE_REGION[0].ViewMode = PROJECTION_REGION;
		FISHEYE_REGION[0].ThetaX = 0.4*M_PI;
		FISHEYE_REGION[0].ThetaZ = 0;
		FISHEYE_REGION[0].ThetaY = 0;
		FISHEYE_REGION[0].ZoomH = 2048;							// zooming factor for horizontal:	0 ~ 4095 to control view 	
		FISHEYE_REGION[0].ZoomV = 2048;							// zooming factor for vertical:		0 ~ 4095 to control view
		FISHEYE_REGION[0].Pan = UI_CTRL_VALUE_CENTER;			// center = 180, value = 0 ~ 360	=> value = +- 180 degrees	
		FISHEYE_REGION[0].Tilt = UI_CTRL_VALUE_CENTER;			// center = 180, value = 0 ~ 360	=> value = +-30 degrees
		FISHEYE_REGION[0].OutW = (int)(width_sec * 20);
		FISHEYE_REGION[0].OutH = (int)(height_sec * 20);
		FISHEYE_REGION[0].OutX = 0;
		FISHEYE_REGION[0].OutY = 0;

		FISHEYE_REGION[1].RegionValid = 1;
		FISHEYE_REGION[1].MeshVer = 16;
		FISHEYE_REGION[1].MeshHor = 16;
		FISHEYE_REGION[1].ViewMode = PROJECTION_REGION;
		FISHEYE_REGION[1].ThetaX = 0.4 * M_PI;
		FISHEYE_REGION[1].ThetaZ = M_PI / 2;
		FISHEYE_REGION[1].ThetaY = 0;
		FISHEYE_REGION[1].ZoomH = 2048;
		FISHEYE_REGION[1].ZoomV = 2048;
		FISHEYE_REGION[1].Pan	= UI_CTRL_VALUE_CENTER;							// theta-X
		FISHEYE_REGION[1].Tilt = UI_CTRL_VALUE_CENTER;						// theta-Z
		FISHEYE_REGION[1].OutW = (int)(width_sec * 20);
		FISHEYE_REGION[1].OutH = (int)(height_sec * 20);
		FISHEYE_REGION[1].OutX = (int)(width_sec * 20);
		FISHEYE_REGION[1].OutY = (int)(height_sec * 0);

		FISHEYE_REGION[2].RegionValid = 1;
		FISHEYE_REGION[2].MeshVer = 16;
		FISHEYE_REGION[2].MeshHor = 16;
		FISHEYE_REGION[2].ViewMode = PROJECTION_REGION;
		FISHEYE_REGION[2].ThetaX = 0.4 * M_PI;
		FISHEYE_REGION[2].ThetaZ = M_PI;
		FISHEYE_REGION[2].ThetaY = 0;
		FISHEYE_REGION[2].ZoomH = 2048;
		FISHEYE_REGION[2].ZoomV = 2048;
		FISHEYE_REGION[2].Pan = UI_CTRL_VALUE_CENTER;						// theta-X
		FISHEYE_REGION[2].Tilt = UI_CTRL_VALUE_CENTER;					// theta-Z
		FISHEYE_REGION[2].OutW = (int)(width_sec * 20);
		FISHEYE_REGION[2].OutH = (int)(height_sec * 20);
		FISHEYE_REGION[2].OutX = (int)(width_sec * 0);
		FISHEYE_REGION[2].OutY = (int)(height_sec * 20);

		FISHEYE_REGION[3].RegionValid = 1;
		FISHEYE_REGION[3].MeshVer = 16;
		FISHEYE_REGION[3].MeshHor = 16;
		FISHEYE_REGION[3].ViewMode = PROJECTION_REGION;
		FISHEYE_REGION[3].ThetaX = 0.4 * M_PI;
		FISHEYE_REGION[3].ThetaZ = 3 * M_PI / 2;
		FISHEYE_REGION[3].ThetaY = 0;
		FISHEYE_REGION[3].ZoomH = 2048;
		FISHEYE_REGION[3].ZoomV = 2048;
		FISHEYE_REGION[3].Pan = UI_CTRL_VALUE_CENTER;						// theta-X
		FISHEYE_REGION[3].Tilt = UI_CTRL_VALUE_CENTER;						// theta-Z
		FISHEYE_REGION[3].OutW = (int)(width_sec * 20);
		FISHEYE_REGION[3].OutH = (int)(height_sec * 20);
		FISHEYE_REGION[3].OutX = (int)(width_sec * 20);
		FISHEYE_REGION[3].OutY = (int)(height_sec * 20);

		FISHEYE_REGION[4].RegionValid = 0;
	}
	else if (/*FISHEYE_CONFIG->MntMode == CEILING && */ FISHEYE_CONFIG->UsageMode == MODE_04_1P2R)
	{
		// Region #1 => Panorama 180 
		FISHEYE_REGION[0].RegionValid = 1;
		FISHEYE_REGION[0].MeshVer = 16;
		FISHEYE_REGION[0].MeshHor = 16;
		FISHEYE_REGION[0].ViewMode = PROJECTION_PANORAMA_180;
		//FISHEYE_REGION[0].ThetaX = 0;
		//FISHEYE_REGION[0].ThetaY = 0;
		//FISHEYE_REGION[0].ThetaZ = 0;
		FISHEYE_REGION[0].ZoomH = 4096;					// value = 0 ~ 4095, symmeterically control horizontal View Range, ex:  value = 4095 => hor view angle = -90 ~ + 90 
		FISHEYE_REGION[0].ZoomV = 1920;					// value = 0 ~ 4095, symmetrically control vertical view range. ex: value = 4096, ver view angle = -90 ~ + 90 
		FISHEYE_REGION[0].Pan   = UI_CTRL_VALUE_CENTER;	// value range = 0 ~ 360, => -180 ~ 0 ~ +180
		FISHEYE_REGION[0].Tilt  = UI_CTRL_VALUE_CENTER;	// value = 0 ~ 360, center = 180 ( original ) => -180 ~ 0 ~ + 180
		FISHEYE_REGION[0].OutW  = (int)(width_sec * 40);
		FISHEYE_REGION[0].OutH  = (int)(height_sec * 22);
		FISHEYE_REGION[0].OutX  = 0;					//(width_sec * 1);
		FISHEYE_REGION[0].OutY  = 0;					//height_sec * 1);
		//FISHEYE_REGION[0].InRadius = 50;				// a ratio to represent OutRadius lenght. 1 => full origina redius.	value/512 is the value. 
		//FISHEYE_REGION[0].OutRadius = 450;			// a ratio to represent OutRadius lenght. 1 => full origina redius.	value/512 is the value. ssss
		//FISHEYE_REGION[0].PanEnd = 180;

		FISHEYE_REGION[1].RegionValid = 1;
		FISHEYE_REGION[1].MeshVer = 8;
		FISHEYE_REGION[1].MeshHor = 8;
		FISHEYE_REGION[1].ViewMode = PROJECTION_REGION;
		FISHEYE_REGION[1].ThetaX = M_PI / 4;
		FISHEYE_REGION[1].ThetaZ = M_PI / 2;
		FISHEYE_REGION[1].ThetaY = 0;
		FISHEYE_REGION[1].ZoomH = 2048;
		FISHEYE_REGION[1].ZoomV = 2048;
		FISHEYE_REGION[1].Pan = UI_CTRL_VALUE_CENTER;						// theta-X
		FISHEYE_REGION[1].Tilt = UI_CTRL_VALUE_CENTER;						// theta-Z
		FISHEYE_REGION[1].OutW = (int)(width_sec * 20);
		FISHEYE_REGION[1].OutH = (int)(height_sec * 18);
		FISHEYE_REGION[1].OutX = (int)(width_sec * 0);
		FISHEYE_REGION[1].OutY = (int)(height_sec * 22);

		FISHEYE_REGION[2].RegionValid = 1;
		FISHEYE_REGION[2].MeshVer = 8;
		FISHEYE_REGION[2].MeshHor = 8;
		FISHEYE_REGION[2].ViewMode = PROJECTION_REGION;
		FISHEYE_REGION[2].ThetaX = M_PI / 4;
		FISHEYE_REGION[2].ThetaZ = M_PI;
		FISHEYE_REGION[2].ThetaY = 0;
		FISHEYE_REGION[2].ZoomH = 2048;
		FISHEYE_REGION[2].ZoomV = 2048;
		FISHEYE_REGION[2].Pan = UI_CTRL_VALUE_CENTER;						// theta-X
		FISHEYE_REGION[2].Tilt = UI_CTRL_VALUE_CENTER;					// theta-Z
		FISHEYE_REGION[2].OutW = (int)(width_sec * 20);
		FISHEYE_REGION[2].OutH = (int)(height_sec * 18);
		FISHEYE_REGION[2].OutX = (int)(width_sec * 20);
		FISHEYE_REGION[2].OutY = (int)(height_sec * 22);

		FISHEYE_REGION[3].RegionValid = 0;
		FISHEYE_REGION[4].RegionValid = 0;
	}
	else if (/*FISHEYE_CONFIG->MntMode == CEILING && */ FISHEYE_CONFIG->UsageMode == MODE_05_1P2R)
	{
		// Region #1 => Panorama 180 
		FISHEYE_REGION[0].RegionValid = 1;
		FISHEYE_REGION[0].MeshVer = 16;
		FISHEYE_REGION[0].MeshHor = 16;
		FISHEYE_REGION[0].ViewMode = PROJECTION_PANORAMA_180;
		//FISHEYE_REGION[0].ThetaX = 0;
		//FISHEYE_REGION[0].ThetaY = 0;
		//FISHEYE_REGION[0].ThetaZ = 0;
		FISHEYE_REGION[0].ZoomH = 3000;					// value = 0 ~ 4095, symmeterically control horizontal View Range, ex:  value = 4095 => hor view angle = -90 ~ + 90 
		FISHEYE_REGION[0].ZoomV = 2048;					// value = 0 ~ 4095, symmetrically control vertical view range. ex: value = 4096, ver view angle = -90 ~ + 90 
		FISHEYE_REGION[0].Pan  = UI_CTRL_VALUE_CENTER;	// value range = 0 ~ 360, => -180 ~ 0 ~ +180
		FISHEYE_REGION[0].Tilt = UI_CTRL_VALUE_CENTER;	// value = 0 ~ 360, center = 180 ( original ) => -180 ~ 0 ~ + 180
		FISHEYE_REGION[0].OutW = (int)(width_sec * 27 );
		FISHEYE_REGION[0].OutH = (int)(height_sec * 40);
		FISHEYE_REGION[0].OutX = 0;					//(width_sec * 1);
		FISHEYE_REGION[0].OutY = 0;					//height_sec * 1);
		//FISHEYE_REGION[0].InRadius = 50;				// a ratio to represent OutRadius lenght. 1 => full origina redius.	value/512 is the value. 
		//FISHEYE_REGION[0].OutRadius = 450;			// a ratio to represent OutRadius lenght. 1 => full origina redius.	value/512 is the value. ssss
		//FISHEYE_REGION[0].PanEnd = 180;

		FISHEYE_REGION[1].RegionValid = 1;
		FISHEYE_REGION[1].MeshVer = 8;
		FISHEYE_REGION[1].MeshHor = 8;
		FISHEYE_REGION[1].ViewMode = PROJECTION_REGION;
		FISHEYE_REGION[1].ThetaX = M_PI / 4;
		FISHEYE_REGION[1].ThetaZ = M_PI / 2;
		FISHEYE_REGION[1].ThetaY = 0;
		FISHEYE_REGION[1].ZoomH = 2048;
		FISHEYE_REGION[1].ZoomV = 2048;
		FISHEYE_REGION[1].Pan = UI_CTRL_VALUE_CENTER;						// theta-X
		FISHEYE_REGION[1].Tilt = UI_CTRL_VALUE_CENTER;						// theta-Z
		FISHEYE_REGION[1].OutW = (int)(width_sec * 13);
		FISHEYE_REGION[1].OutH = (int)(height_sec * 20);
		FISHEYE_REGION[1].OutX = (int)(width_sec * 27);
		FISHEYE_REGION[1].OutY = (int)(height_sec * 0);

		FISHEYE_REGION[2].RegionValid = 1;
		FISHEYE_REGION[2].MeshVer = 8;
		FISHEYE_REGION[2].MeshHor = 8;
		FISHEYE_REGION[2].ViewMode = PROJECTION_REGION;
		FISHEYE_REGION[2].ThetaX = M_PI / 4;
		FISHEYE_REGION[2].ThetaZ = M_PI;
		FISHEYE_REGION[2].ThetaY = 0;
		FISHEYE_REGION[2].ZoomH = 2048;
		FISHEYE_REGION[2].ZoomV = 2048;
		FISHEYE_REGION[2].Pan = UI_CTRL_VALUE_CENTER;						// theta-X
		FISHEYE_REGION[2].Tilt = UI_CTRL_VALUE_CENTER;					// theta-Z
		FISHEYE_REGION[2].OutW = (int)(width_sec * 13);
		FISHEYE_REGION[2].OutH = (int)(height_sec * 20);
		FISHEYE_REGION[2].OutX = (int)(width_sec * 27);
		FISHEYE_REGION[2].OutY = (int)(height_sec * 20);

		FISHEYE_REGION[3].RegionValid = 0;
		FISHEYE_REGION[4].RegionValid = 0;


	}
	else if (/*FISHEYE_CONFIG->MntMode == CEILING && */ FISHEYE_CONFIG->UsageMode == MODE_06_1P)
	{
		// Region #1 => Panorama 180 
		FISHEYE_REGION[0].RegionValid = 1;
		FISHEYE_REGION[0].MeshVer = 30;
		FISHEYE_REGION[0].MeshHor = 30;
		FISHEYE_REGION[0].ViewMode = PROJECTION_PANORAMA_180;
		//FISHEYE_REGION[0].ThetaX = 0;
		//FISHEYE_REGION[0].ThetaY = 0;
		//FISHEYE_REGION[0].ThetaZ = 0;
		FISHEYE_REGION[0].ZoomH = 4096;					// value = 0 ~ 4095, symmeterically control horizontal View Range, ex:  value = 4095 => hor view angle = -90 ~ + 90 
		FISHEYE_REGION[0].ZoomV = 2800;					// value = 0 ~ 4095, symmetrically control vertical view range. ex: value = 4096, ver view angle = -90 ~ + 90 
		FISHEYE_REGION[0].Pan  = UI_CTRL_VALUE_CENTER;	// value range = 0 ~ 360, => -180 ~ 0 ~ +180
		FISHEYE_REGION[0].Tilt = UI_CTRL_VALUE_CENTER;	// value = 0 ~ 360, center = 180 ( original ) => -180 ~ 0 ~ + 180
		FISHEYE_REGION[0].OutW = (int)(width_sec * 40 );
		FISHEYE_REGION[0].OutH = (int)(height_sec * 40);
		FISHEYE_REGION[0].OutX = 0;					//(width_sec * 1);
		FISHEYE_REGION[0].OutY = 0;					//height_sec * 1);
		//FISHEYE_REGION[0].InRadius = 50;				// a ratio to represent OutRadius lenght. 1 => full origina redius.	value/512 is the value. 
		//FISHEYE_REGION[0].OutRadius = 450;			// a ratio to represent OutRadius lenght. 1 => full origina redius.	value/512 is the value. ssss
		//FISHEYE_REGION[0].PanEnd = 180;

		FISHEYE_REGION[1].RegionValid = 0;
		FISHEYE_REGION[2].RegionValid = 0;
		FISHEYE_REGION[3].RegionValid = 0;
		FISHEYE_REGION[4].RegionValid = 0;
	}
	else if (/*FISHEYE_CONFIG->MntMode == CEILING && */ FISHEYE_CONFIG->UsageMode == MODE_07_2P )
	{
		//_Panorama360View2;
		FISHEYE_REGION[0].RegionValid = 1;
		FISHEYE_REGION[0].MeshVer = 16;
		FISHEYE_REGION[0].MeshHor = 16;
		FISHEYE_REGION[0].ViewMode = PROJECTION_PANORAMA_360;
		FISHEYE_REGION[0].ThetaX = M_PI / 4;
		FISHEYE_REGION[0].ThetaZ = 0;
		FISHEYE_REGION[0].ThetaY = 0;
		//FISHEYE_REGION[0].ZoomH = 4095;				// Not Used in Panorama 360 Mode.
		FISHEYE_REGION[0].ZoomV = 4095;					// To ZoomIn OutRadius
		FISHEYE_REGION[0].Pan = 0;						// for panorama 360 => Pan is the label start position angle ( in degrees 
		FISHEYE_REGION[0].Tilt = 300;					// to add shift offset vertical angle.
		FISHEYE_REGION[0].OutW = (int)(width_sec * 40);
		FISHEYE_REGION[0].OutH = (int)(height_sec * 20);
		FISHEYE_REGION[0].OutX = (int)(width_sec * 0);
		FISHEYE_REGION[0].OutY = (int)(height_sec * 0);
		FISHEYE_REGION[0].InRadius = 300;				// a ratio to represent OutRadius lenght. 1 => full origina redius.	value/512 is the value. 
		FISHEYE_REGION[0].OutRadius = 500;				// a ratio to represent OutRadius lenght. 1 => full origina redius.	value/512 is the value. ssss
		FISHEYE_REGION[0].PanEnd = 180;

		FISHEYE_REGION[1].RegionValid = 1;
		FISHEYE_REGION[1].MeshVer = 16;
		FISHEYE_REGION[1].MeshHor = 16;
		FISHEYE_REGION[1].ViewMode = PROJECTION_PANORAMA_360;
		FISHEYE_REGION[1].ThetaX = M_PI / 4;
		FISHEYE_REGION[1].ThetaZ = 0;
		FISHEYE_REGION[1].ThetaY = 0;
		//FISHEYE_REGION[1].ZoomH = 4095;					// Not Used in Panorama 360 Mode.
		FISHEYE_REGION[1].ZoomV = 4095;					// To ZoomIn OutRadius
		FISHEYE_REGION[1].Pan  = 240;					// for panorama 360 => Pan is the label start position angle ( in degrees 	
		FISHEYE_REGION[1].Tilt = 200;					// to add shift offset vertical angle.
		FISHEYE_REGION[1].OutW = (int)(width_sec * 40);
		FISHEYE_REGION[1].OutH = (int)(height_sec * 20);
		FISHEYE_REGION[1].OutX = (int)(width_sec * 0);
		FISHEYE_REGION[1].OutY = (int)(height_sec * 20);
		FISHEYE_REGION[1].InRadius = 0;				// a ratio to represent OutRadius lenght. 1 = full origina redius.  value/512 is the value. 
		FISHEYE_REGION[1].OutRadius = 512;				// a ratio to represent OutRadius lenght. 1 = full origina redius.	value/512 is the value. 
		FISHEYE_REGION[1].PanEnd = 60;					//	

		FISHEYE_REGION[2].RegionValid = 0;
		FISHEYE_REGION[3].RegionValid = 0;
		FISHEYE_REGION[4].RegionValid = 0;
	}
	else if (FISHEYE_CONFIG->UsageMode == MODE_PANORAMA_180)
	{
		FISHEYE_REGION[0].RegionValid = 1;
		FISHEYE_REGION[0].MeshVer = 8;
		FISHEYE_REGION[0].MeshHor = 8;
		FISHEYE_REGION[0].ViewMode = PROJECTION_PANORAMA_180;
		//FISHEYE_REGION[0].ThetaX = M_PI / 4;
		//FISHEYE_REGION[0].ThetaZ = 0;
		//FISHEYE_REGION[0].ThetaY = 0;
		FISHEYE_REGION[0].ZoomH = 4096;					// Not Used in Panorama 360 Mode.
		FISHEYE_REGION[0].ZoomV = 4096;					// To ZoomIn OutRadius
		FISHEYE_REGION[0].Pan = 180;					// for panorama 360 => Pan is the label start position angle ( in degrees 
		FISHEYE_REGION[0].Tilt = 180;					// to add shift offset vertical angle.
		FISHEYE_REGION[0].OutW = (int)(width_sec * 40);
		FISHEYE_REGION[0].OutH = (int)(height_sec * 40);
		FISHEYE_REGION[0].OutX = 0;						//(width_sec * 1);
		FISHEYE_REGION[0].OutY = 0;						//height_sec * 1);
		//FISHEYE_REGION[0].InRadius = 50;				// a ratio to represent OutRadius lenght. 1 => full origina redius.	value/512 is the value. 
		//FISHEYE_REGION[0].OutRadius = 450;				// a ratio to represent OutRadius lenght. 1 => full origina redius.	value/512 is the value. ssss
		//FISHEYE_REGION[0].PanEnd = 180;

		FISHEYE_REGION[1].RegionValid = 0;	
		FISHEYE_REGION[2].RegionValid = 0;
		FISHEYE_REGION[3].RegionValid = 0;
		FISHEYE_REGION[4].RegionValid = 0;
	}
	else if (FISHEYE_CONFIG->UsageMode == MODE_PANORAMA_360)
	{
		//_Panorama360View2;
		FISHEYE_REGION[0].RegionValid = 1;
		FISHEYE_REGION[0].MeshVer = 8;
		FISHEYE_REGION[0].MeshHor = 8;
		FISHEYE_REGION[0].ViewMode = PROJECTION_PANORAMA_360;
		FISHEYE_REGION[0].ThetaX = M_PI / 4;
		FISHEYE_REGION[0].ThetaZ = 0;
		FISHEYE_REGION[0].ThetaY = 0;
		//FISHEYE_REGION[0].ZoomH = 4095;				// Not Used in Panorama 360 Mode.
		FISHEYE_REGION[0].ZoomV = 4095;					// To ZoomIn OutRadius
		FISHEYE_REGION[0].Pan = 0;						// for panorama 360 => Pan is the label start position angle ( in degrees 
		FISHEYE_REGION[0].Tilt = 180;					// to add shift offset vertical angle.
		FISHEYE_REGION[0].OutW = (int)(width_sec * 40);
		FISHEYE_REGION[0].OutH = (int)(height_sec * 40);
		FISHEYE_REGION[0].OutX = 0;						//(width_sec * 1);
		FISHEYE_REGION[0].OutY = 0;						//(height_sec * 1);
		FISHEYE_REGION[0].InRadius = 0;				// a ratio to represent OutRadius lenght. 1 => full origina redius.	value = 0 ~ 4095,  
		FISHEYE_REGION[0].OutRadius = 4095;				// a ratio to represent OutRadius lenght. 1 => full origina redius.	value = 0 ~ 4095
		FISHEYE_REGION[0].PanEnd = 359;

		FISHEYE_REGION[1].RegionValid = 0;
		FISHEYE_REGION[2].RegionValid = 0;
		FISHEYE_REGION[3].RegionValid = 0;
		FISHEYE_REGION[4].RegionValid = 0;
	}
	else
	{
		printf("Not done yet. !!!");
        //system("pause");
	}
}

void _LOAD_FRAME_CONFIG(FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION)
{
	//printf("FISHEYE_CONFIG->UsageMode = %d,",FISHEYE_CONFIG->UsageMode);
	//system("pause");

	switch (FISHEYE_CONFIG->UsageMode)
	{
	case MODE_PANORAMA_360:
		FISHEYE_CONFIG->RgnNum = 1;
		break;
	case MODE_PANORAMA_180:
		FISHEYE_CONFIG->RgnNum = 1;
		break;
	case MODE_01_1O:
		FISHEYE_CONFIG->RgnNum = 1;
		break;
	case MODE_02_1O4R:
		FISHEYE_CONFIG->RgnNum = 4;	//	1O should be handled in scaler block.
		break;
	case MODE_03_4R:
		FISHEYE_CONFIG->RgnNum = 4;
		break;
	case MODE_04_1P2R:
		FISHEYE_CONFIG->RgnNum = 3;
		break;
	case MODE_05_1P2R:
		FISHEYE_CONFIG->RgnNum = 3;
		break;
	case MODE_06_1P:
		FISHEYE_CONFIG->RgnNum = 1;
		break;
	case MODE_07_2P:
		FISHEYE_CONFIG->RgnNum = 2;//
		break;
	default:
		printf("UsageMode Error!!!");
        //system("pause");
		break;
	}
}

void INITIAL_CONFIG(FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION, int dst_height, int dst_width, int src_width, int src_height, int x0, int y0, int r)
{
	// Global Initialization
	FISHEYE_CONFIG->Enable		= true;
	FISHEYE_CONFIG->BgEnable	= true;
    //FISHEYE_CONFIG->MntMode		= CEILING;
    //FISHEYE_CONFIG->UsageMode	= MODE_02_1O4R;
	FISHEYE_CONFIG->OutW_disp	= dst_width;
	FISHEYE_CONFIG->OutH_disp	= dst_height;
	FISHEYE_CONFIG->BgColor.R	= 0;
	FISHEYE_CONFIG->BgColor.G	= 0;
	FISHEYE_CONFIG->BgColor.B	= 0;
	FISHEYE_CONFIG->InCenterX	= (double)x0;	// front-end set.
	FISHEYE_CONFIG->InCenterY	= (double)y0;	// front-end set.
	FISHEYE_CONFIG->InRadius	= (double)r;	// front-end set.
	FISHEYE_CONFIG->InWidth		= src_width;
	FISHEYE_CONFIG->InHeight	= src_height;

	_LOAD_FRAME_CONFIG(FISHEYE_CONFIG, FISHEYE_REGION);
	_LOAD_REGION_CONFIG(FISHEYE_CONFIG, FISHEYE_REGION);
}

void Normalize3D(Vector3D* v)
{
	double x3d, y3d, z3d, d;

	x3d = v->x;
	y3d = v->y;
	z3d = v->z;
	d = sqrt(x3d * x3d + y3d * y3d + z3d * z3d);
	v->x = x3d / d;
	v->y = y3d / d;
	v->z = z3d / d;
}

void _draw_mesh_output_dewarp(Vector2D* dist2d, bool hit_dispMeshKnot, unsigned char* dst_img, unsigned char* src_img, int x0, int y0, int x, int y, int fishImg_width, int fishImg_height, int view_w, int view_h)
{
	int xf0, xf1, yf0, yf1, px, py;
	double xf, yf;
	double xx0[3], xx1[3];

	// interpolate the pixel result
	xf = dist2d->x + x0;
	yf = dist2d->y + y0;
	xf0 = minmax((int)floor(xf), 0, fishImg_width - 1);
	yf0 = minmax((int)floor(yf), 0, fishImg_height - 1);
	xf1 = minmax(xf0 + 1, 0, fishImg_width - 1);
	yf1 = minmax(yf0 + 1, 0, fishImg_height - 1);


	//printf("HELLOHIHIIH!!\n\r");

	if ((int)floor(xf) != xf0 || (int)floor(yf) != yf0)
	{
		px = x + fishImg_width / 2;
		py = y + fishImg_height / 2;

		*(dst_img + (py * fishImg_width + px) * 3 + 0) = 128;
		*(dst_img + (py * fishImg_width + px) * 3 + 1) = 77;
		*(dst_img + (py * fishImg_width + px) * 3 + 2) = 255;
	}
	else
	{
		xx0[0] = *(src_img + (yf0 * fishImg_width + xf0) * 3 + 0) * ((double)xf1 - xf) + *(src_img + (yf0 * fishImg_width + xf1) * 3 + 0) * (xf - (double)xf0);
		xx0[1] = *(src_img + (yf0 * fishImg_width + xf0) * 3 + 1) * ((double)xf1 - xf) + *(src_img + (yf0 * fishImg_width + xf1) * 3 + 1) * (xf - (double)xf0);
		xx0[2] = *(src_img + (yf0 * fishImg_width + xf0) * 3 + 2) * ((double)xf1 - xf) + *(src_img + (yf0 * fishImg_width + xf1) * 3 + 2) * (xf - (double)xf0);
		xx1[0] = *(src_img + (yf1 * fishImg_width + xf0) * 3 + 0) * ((double)xf1 - xf) + *(src_img + (yf1 * fishImg_width + xf1) * 3 + 0) * (xf - (double)xf0);
		xx1[1] = *(src_img + (yf1 * fishImg_width + xf0) * 3 + 1) * ((double)xf1 - xf) + *(src_img + (yf1 * fishImg_width + xf1) * 3 + 1) * (xf - (double)xf0);
		xx1[2] = *(src_img + (yf1 * fishImg_width + xf0) * 3 + 2) * ((double)xf1 - xf) + *(src_img + (yf1 * fishImg_width + xf1) * 3 + 2) * (xf - (double)xf0);
		px = x + view_w / 2;
		py = y + view_h / 2;

		if (hit_dispMeshKnot)
		{
			*(dst_img + (py * view_w + px) * 3 + 0) = 255;
			*(dst_img + (py * view_w + px) * 3 + 1) = 255;
			*(dst_img + (py * view_w + px) * 3 + 2) = 0;
		}
		else
		{
			*(dst_img + (py * view_w + px) * 3 + 0) = (unsigned char)minmax((int)floor(xx0[0] * ((double)yf1 - yf) + xx1[0] * (yf - (double)yf0)), 0, 255);
			*(dst_img + (py * view_w + px) * 3 + 1) = (unsigned char)minmax((int)floor(xx0[1] * ((double)yf1 - yf) + xx1[1] * (yf - (double)yf0)), 0, 255);
			*(dst_img + (py * view_w + px) * 3 + 2) = (unsigned char)minmax((int)floor(xx0[2] * ((double)yf1 - yf) + xx1[2] * (yf - (double)yf0)), 0, 255);
		}
	}
}

void Equidistant_Panorama(Vector3D* v3d, Vector2D* v2d, double f)
{
	double rd, theta, phy;

	// calculate theta from the angle between (0,0,1) and (x3d, y3d, z3d)
	//theta = acos(v3d->z); //bastian

	theta = M_PI / 2 - v3d->z;

	if (theta == 0.0) {
		theta += 1e-64;
	}

#if 0
	// calculate rd = f * theta
	rd = f * (theta / (M_PI / 2));
	phy = atan(rd / f);

	// calculate new distorted vector (x,y,z) = (0,0,1) * (1-rd/ri) + (x3d, y3d, z3d) * (rd/ri)
	Vector3D v3d_p;
	v3d_p.x = v3d->x * phy / theta;
	v3d_p.y = v3d->y * phy / theta;
	v3d_p.z = v3d->z * phy / theta + (1 - phy / theta);

	// calculate the target point (xi, yi) on image at z = f
	v2d->x = (f / v3d_p.z) * v3d_p.x;
	v2d->y = (f / v3d_p.z) * v3d_p.y;

#else
	// calculate rd = f * theta
	// assume: fisheye image 2D' radius rd
	// if it's equidistant: r = focal*incident_angle
	// max incident angle = PI/2
	rd = f * (theta / (M_PI / 2));
	phy = atan(rd / f);

	// ( 0,0 ) & (v3d->x, v3d->y): find equation y = ax + b
	// a = v3d.y/v3d.x
	// if incident angle from space ( x, y ), 
	double L_a = v3d->y / v3d->x;

	v2d->x = (v3d->x >= 0) ? (-sqrt((rd * rd) / (L_a * L_a + 1))) : sqrt((rd * rd) / (L_a * L_a + 1));
	v2d->y = L_a * v2d->x;

#endif
}

void Equidistant(Vector3D* v3d, Vector2D* v2d, double f)
{
	double rd, ri, theta, phy;

	Normalize3D(v3d);

	// calculate theta from the angle between (0,0,1) and (x3d, y3d, z3d)
	theta = acos(v3d->z);

	// calculate ri = f tan(theta)
	ri = f * tan(theta);
	if (theta == 0.0) {
		ri += 1e-64;
		theta += 1e-64;
	}

	// calculate rd = f * theta
	rd = f * (theta / (M_PI / 2));
	phy = atan(rd / f);

	// calculate new distorted vector (x,y,z) = (0,0,1) * (1-rd/ri) + (x3d, y3d, z3d) * (rd/ri)
	Vector3D v3d_p;
	v3d_p.x = v3d->x * phy / theta;
	v3d_p.y = v3d->y * phy / theta;
	v3d_p.z = v3d->z * phy / theta + (1 - phy / theta);

	// calculate the target point (xi, yi) on image at z = f
	v2d->x = (f / v3d_p.z) * v3d_p.x;
	v2d->y = (f / v3d_p.z) * v3d_p.y;
}
void Rotate3D(Vector3D* v3d, RotMatrix3D* mat)
{
	//int i, j;
	Vector3D v1;

	v1.x = mat->coef[0][0] * v3d->x + mat->coef[0][1] * v3d->y +
		mat->coef[0][2] * v3d->z + mat->coef[0][3];
	v1.y = mat->coef[1][0] * v3d->x + mat->coef[1][1] * v3d->y +
		mat->coef[1][2] * v3d->z + mat->coef[1][3];
	v1.z = mat->coef[2][0] * v3d->x + mat->coef[2][1] * v3d->y +
		mat->coef[2][2] * v3d->z + mat->coef[2][3];



	v3d->x = v1.x;
	v3d->y = v1.y;
	v3d->z = v1.z;
}
bool dgb_draw_mesh_knot(int x, int y, int view_w, int view_h, int mesh_horcnt, int mesh_vercnt)
{
	bool xknot_hit = ((x + view_w / 2) % (view_w / mesh_horcnt)) == 0;
	bool yknot_hit = ((y + view_h / 2) % (view_h / mesh_vercnt)) == 0;
	bool hitknot
		= ((xknot_hit && yknot_hit)
			|| (((x + 1) == (view_w / 2)) && yknot_hit)
			|| (xknot_hit && ((y + 1) == (view_h / 2)))
			|| (((x + 1) == (view_w / 2)) && ((y + 1) == (view_h / 2))));

	// LastMeshFix is to fix unequal mesh block counts.
	bool LastMeshFixX = ((x + view_w / 2) == ((view_w / mesh_horcnt) * mesh_horcnt)) && (view_w != (view_w / mesh_horcnt) * mesh_horcnt);
	bool LastMeshFixY = ((y + view_h / 2) == ((view_h / mesh_vercnt) * mesh_vercnt)) && (view_h != (view_h / mesh_vercnt) * mesh_vercnt);

	hitknot = hitknot && !(LastMeshFixX || LastMeshFixY);

	return hitknot;
}
void _get_region_src_mesh_list(MOUNTMODE MOUNT, FISHEYE_REGION_ATTR* FISHEYE_REGION, int mesh_horcnt, int mesh_vercnt, int rgn_idx, double x0, double y0, double r)
{
	// ViewModeType to decide mapping & UI contrl parameters.
	PROJECTION_MODE ViewModeType = FISHEYE_REGION[rgn_idx].ViewMode;

	if (ViewModeType == PROJECTION_REGION)
	{
		_RegionView2( FISHEYE_REGION, rgn_idx, MOUNT, x0, y0, r);
	}
	else if (ViewModeType == PROJECTION_PANORAMA_360)
	{
		_Panorama360View2( FISHEYE_REGION, rgn_idx, MOUNT, x0, y0, r);
	}
	else if (ViewModeType == PROJECTION_PANORAMA_180)
	{
		_Panorama180View2(FISHEYE_REGION, rgn_idx, MOUNT, x0, y0, r);
	}
	else
	{
		printf("ERROR!!! THIS CASE SHOULD NOT HAPPEN!!!!!!\n\r");
        //system("pause");
	}
}

void _get_region_dst_mesh_list(FISHEYE_REGION_ATTR* FISHEYE_REGION, int view_w, int view_h, int mesh_horcnt, int mesh_vercnt, int rgn_idx)
{
	if (FISHEYE_REGION[rgn_idx].RegionValid != 1)
		return;


	// tmp internal buffer
	// maximum knot number = 1024 (pre-set)
	COORDINATE2D meshknot_hit_buf[MAX_WINDOW_MESH_NUM];

	// 1st loop: to find mesh infos. on source ( backward projection )
	// hit index for buffer 
	int knotcnt = 0;
	for (int y = -view_h / 2; y < view_h / 2; y++)
		for (int x = -view_w / 2; x < view_w / 2; x++)
		{
			bool xknot_hit = ((x + view_w / 2) % (view_w / mesh_horcnt)) == 0;
			bool yknot_hit = ((y + view_h / 2) % (view_h / mesh_vercnt)) == 0;
			bool hitknot
				= ((xknot_hit && yknot_hit)
					|| (((x + 1) == (view_w / 2)) && yknot_hit)
					|| (xknot_hit && ((y + 1) == (view_h / 2)))
					|| (((x + 1) == (view_w / 2)) && ((y + 1) == (view_h / 2))));

			// LastMeshFix is to fix unequal mesh block counts.
			bool LastMeshFixX = ((x + view_w / 2) == ((view_w / mesh_horcnt) * mesh_horcnt)) && (view_w != (view_w / mesh_horcnt) * mesh_horcnt);
			bool LastMeshFixY = ((y + view_h / 2) == ((view_h / mesh_vercnt) * mesh_vercnt)) && (view_h != (view_h / mesh_vercnt) * mesh_vercnt);

			hitknot = hitknot && !(LastMeshFixX || LastMeshFixY);

			if (hitknot)
			{
				meshknot_hit_buf[knotcnt].xcor = FISHEYE_REGION[rgn_idx].OutX + (x + view_w / 2);
				meshknot_hit_buf[knotcnt].ycor = FISHEYE_REGION[rgn_idx].OutY + (y + view_h / 2);
				knotcnt += 1;
			}
		}

	meshknot_hit_buf[knotcnt].xcor = 0xFFFFFFFF;	//End of Knot List.
	meshknot_hit_buf[knotcnt].ycor = 0xFFFFFFFF;	//End of Knot List
	// tmp for debug

	for (int j = 0; j < mesh_vercnt; j++)
	{
		for (int i = 0; i < mesh_horcnt; i++)
		{
			int meshidx = j * mesh_horcnt + i;
			int knotidx = j * (mesh_horcnt + 1) + i;	// knot num = mesh num +1 ( @ horizon )

			FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[meshidx].knot[0].xcor = meshknot_hit_buf[knotidx].xcor;
			FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[meshidx].knot[0].ycor = meshknot_hit_buf[knotidx].ycor;
			FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[meshidx].knot[1].xcor = meshknot_hit_buf[knotidx + 1].xcor;
			FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[meshidx].knot[1].ycor = meshknot_hit_buf[knotidx + 1].ycor;
			FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[meshidx].knot[2].xcor = meshknot_hit_buf[knotidx + (mesh_horcnt + 1)].xcor;
			FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[meshidx].knot[2].ycor = meshknot_hit_buf[knotidx + (mesh_horcnt + 1)].ycor;
			FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[meshidx].knot[3].xcor = meshknot_hit_buf[knotidx + 1 + (mesh_horcnt + 1)].xcor;
			FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[meshidx].knot[3].ycor = meshknot_hit_buf[knotidx + 1 + (mesh_horcnt + 1)].ycor;
		}
	}
}

void _dump_draw_sub_window_img(FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double x0, double y0, double r, unsigned char * src_img, unsigned char* src_rois)
{
	// ViewModeType to decide mapping & UI contrl parameters.
	PROJECTION_MODE ViewModeType = FISHEYE_REGION[rgn_idx].ViewMode;

	int view_w = FISHEYE_REGION[rgn_idx].OutW;
	int view_h = FISHEYE_REGION[rgn_idx].OutH;

	// draw image 
	unsigned char* rgn_img;		
	rgn_img = (unsigned char*)malloc(view_w * view_h * 3 * sizeof(unsigned char));
	

	if (ViewModeType == PROJECTION_REGION)
	{
		_RegionView(src_img, rgn_img, src_rois, FISHEYE_CONFIG, FISHEYE_REGION, rgn_idx, x0, y0, r);
	}
	else if (ViewModeType == PROJECTION_PANORAMA_360)
	{
		_Panorama360View(src_img, rgn_img, src_rois, FISHEYE_CONFIG, FISHEYE_REGION, rgn_idx, x0, y0, r);
	}
	else if (ViewModeType == PROJECTION_PANORAMA_180)
	{
		_Panorama180View(src_img, rgn_img, src_rois, FISHEYE_CONFIG, FISHEYE_REGION, rgn_idx, x0, y0, r);
	}
	else
	{
		printf("ERROR!!! THIS CASE SHOULD NOT HAPPEN!!!!!!\n\r");
        //system("pause");
	}

	char filename[256];
	sprintf(filename, "rgn_window_%02d.ppm", rgn_idx);
	RGB888Image2PPM(rgn_img, view_w, view_h, filename);
}

void GenRotMatrix3D(Vector3D* v3d, RotMatrix3D* mat)
{
	// generate the rotation matrix to
	// rotate vector [0,0,1] to v3d


	double phy1, phy2;
	Vector3D v1, v2;

	// 1. rotate v3d along X axis to have the vector lies in X-Z plane, i.e. y=0 and x is postive
	// cos(phy1) * v3d->y + sin(phy1) * v3d->z = 0;
	// solve phy1
	// v3d->y * cos(phy1) = -v3d->z * sin(phy1)
	// -v3d->y / v3d->z = tan(phy1)
	// phy1 = atan(-v3d->y/v3d->z)
	// if(v3d->x <0)  phy1 += MI_PI;
	// update new v3d by Rx(phy1)


	//眖程竒Ч猭臂锣痻皚ㄓ狦璶だ―眔 y, x, z ㄤ璸衡よΑ
	//y = arctan(r13 / r33) 
	//x = arcsin(-r23)			
	//z = arctan(r21 / r22)

	phy1 = atan(-v3d->y / v3d->z);

	v1.x = v3d->x;
	v1.y = v3d->y * cos(phy1) + v3d->z * sin(phy1);
	v1.z = -v3d->y * sin(phy1) + v3d->z * cos(phy1);

	// 2. rotate v3d along Y axis to have the vector lies in Z axis, i.e. x=0, y=0 and z is positive
	// v3d->x * cos(phy2) - v3d->z * sin(phy2) = 0
	// v3d->x / v3d->z = tan(phy2)
	// phy2 = atan(v3d->x / v3d->z)
	// v3d->z = v3d->x * sin(phy2) + v3d->z*cos(phy2)
	// if (v3d->z < 0)  phy2 += M_PI;
	phy2 = atan(v1.x / v1.z);
	v2.z = v1.x * sin(phy2) + v1.z * cos(phy2);
	if (v2.z < 0)	phy2 += M_PI;

	v2.x = v1.x * cos(phy2) - v1.z * sin(phy2);
	v2.y = v1.y;
	v2.z = v1.x * sin(phy2) + v1.z * cos(phy2);

	// do the reverse rotation
	// Rotation matrix = Rx(-phy1)*Ry(-phy2)
	mat->coef[0][0] = cos(-phy2);
	mat->coef[0][1] = 0;
	mat->coef[0][2] = -sin(-phy2);
	mat->coef[0][3] = 0;
	mat->coef[1][0] = sin(-phy1) * sin(-phy2);
	mat->coef[1][1] = cos(-phy1);
	mat->coef[1][2] = sin(-phy1) * cos(-phy2);
	mat->coef[1][3] = 0;
	mat->coef[2][0] = cos(-phy1) * sin(-phy2);
	mat->coef[2][1] = -sin(-phy1);
	mat->coef[2][2] = cos(-phy1) * cos(-phy2);
	mat->coef[2][3] = 0;
	mat->coef[3][0] = 0;
	mat->coef[3][1] = 0;
	mat->coef[3][2] = 0;
	mat->coef[3][3] = 1;


}


void GenRotMatrix3D_YXZ(RotMatrix3D* mat, FISHEYE_REGION_ATTR* FISHEYE_REGION_IDX, int rgn_idx)
{
	// This Rotation Matrix Order = R = Rz*Rx*Ry
	// rotation order = y -> x -> z
	//_LOAD_REGION_CONFIG;
	// initital position
	double tmp_phy_x = FISHEYE_REGION_IDX[rgn_idx].ThetaX;	//phy_x;
	double tmp_phy_y = FISHEYE_REGION_IDX[rgn_idx].ThetaY;	//phy_y;	// Not Used for Now.
	double tmp_phy_z = FISHEYE_REGION_IDX[rgn_idx].ThetaZ;	//phy_z;

	//_LOAD_REGION_CONFIG(FISHEYE_CONFIG, FISHEYE_REGION);
	// UI Control
	double ctrl_tilt, ctrl_pan;

	if (FISHEYE_REGION_IDX->ViewMode == PROJECTION_REGION)
	{
		ctrl_tilt = minmax(FISHEYE_REGION_IDX[rgn_idx].Tilt - UI_CTRL_VALUE_CENTER, -UI_CTRL_VALUE_CENTER, UI_CTRL_VALUE_CENTER);
		ctrl_pan  = minmax(FISHEYE_REGION_IDX[rgn_idx].Pan  - UI_CTRL_VALUE_CENTER, -UI_CTRL_VALUE_CENTER, UI_CTRL_VALUE_CENTER);
	}
	else
	{
		// not used in panorama case
		ctrl_pan  = 0;
		ctrl_tilt = 0;
	}

	tmp_phy_x += (ctrl_tilt * M_PI / 2)/(2* UI_CTRL_VALUE_CENTER);
	tmp_phy_y += 0;
	tmp_phy_z += (ctrl_pan * M_PI)/(2* UI_CTRL_VALUE_CENTER);



	mat->coef[0][0] = cos(tmp_phy_y) * cos(tmp_phy_z) - sin(tmp_phy_x) * sin(tmp_phy_y) * sin(tmp_phy_z);
	mat->coef[0][1] = -cos(tmp_phy_x) * sin(tmp_phy_z);
	mat->coef[0][2] = sin(tmp_phy_y) * cos(tmp_phy_z) + sin(tmp_phy_x) * cos(tmp_phy_y) * sin(tmp_phy_z);
	mat->coef[0][3] = 0;
	mat->coef[1][0] = cos(tmp_phy_y) * sin(tmp_phy_z) + sin(tmp_phy_x) * sin(tmp_phy_y) * cos(tmp_phy_z);
	mat->coef[1][1] = cos(tmp_phy_x) * cos(tmp_phy_z);
	mat->coef[1][2] = sin(tmp_phy_y) * sin(tmp_phy_z) - sin(tmp_phy_x) * cos(tmp_phy_y) * cos(tmp_phy_z);
	mat->coef[1][3] = 0;
	mat->coef[2][0] = -cos(tmp_phy_x) * sin(tmp_phy_y);
	mat->coef[2][1] = sin(tmp_phy_x);
	mat->coef[2][2] = cos(tmp_phy_x) * cos(tmp_phy_y);
	mat->coef[2][3] = 0;
	mat->coef[3][0] = 0;
	mat->coef[3][1] = 0;
	mat->coef[3][2] = 0;
	mat->coef[3][3] = 1;
}

unsigned char* ReadPPM(const char* filename, int* width, int* height)
{
	int i, j;
	//unsigned char color[3];
	unsigned char* rgb888;
	int w, h;

	FILE* fp = fopen(filename, "rb"); /* b - binary mode */
	if (fp == NULL) {
		printf("Cannot open file: %s\n", filename);
        return NULL;
	}
	if (fscanf(fp, "P6\n %d %d \n 255\n", &w, &h));

	*width = w;
	*height = h;
	rgb888 = (unsigned char*)malloc(w * h * sizeof(unsigned char) * 3);

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
		{
			*(rgb888 + 3 * (j * w + i) + 0) = fgetc(fp);	/* red */
			*(rgb888 + 3 * (j * w + i) + 1) = fgetc(fp);	/* green */
			*(rgb888 + 3 * (j * w + i) + 2) = fgetc(fp);	/* blue */
		}
	fclose(fp);
	return rgb888;
}

int RGB888Image2PPM(unsigned char* rgb888, int dimx, int dimy, char* filename)
{
	int i, j;
	unsigned char color[3];
	FILE* fp = fopen(filename, "wb"); /* b - binary mode */
	(void)fprintf(fp, "P6\n%d %d\n255\n", dimx, dimy);
	for (j = 0; j < dimy; ++j)
	{
		for (i = 0; i < dimx; ++i)
		{
			color[0] = *(rgb888 + 3 * (j * dimx + i) + 0);	/* red */
			color[1] = *(rgb888 + 3 * (j * dimx + i) + 1);	/* green */
			color[2] = *(rgb888 + 3 * (j * dimx + i) + 2);	/* blue */
			(void)fwrite(color, 1, 3, fp);
		}
	}
	(void)fclose(fp);
	return 0;
}
void _get_frame_mesh_list(unsigned char* src_img, unsigned char*dst_img, FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION)
{
	// pack all regions' mesh info, including src & dst.
	int rgnNum = FISHEYE_CONFIG->RgnNum;
	int meshNumRgn[MAX_REGION_NUM];

	static int frameMeshIdx = 0;
	for (int i = 0; i < rgnNum; i++)
	{
		if (FISHEYE_REGION[i].RegionValid == 1)
		{
			meshNumRgn[i] = (FISHEYE_REGION[i].MeshHor * FISHEYE_REGION[i].MeshVer);

			// go through each region loop
			for (int meshidx = 0; meshidx < meshNumRgn[i]; meshidx++)
			{
				// each mesh has 4 knots
				for (int knotidx = 0; knotidx < 4; knotidx++)
				{
					FISHEYE_CONFIG->DstRgnMeshInfo[frameMeshIdx].knot[knotidx].xcor = FISHEYE_REGION[i].DstRgnMeshInfo[meshidx].knot[knotidx].xcor;
					FISHEYE_CONFIG->DstRgnMeshInfo[frameMeshIdx].knot[knotidx].ycor = FISHEYE_REGION[i].DstRgnMeshInfo[meshidx].knot[knotidx].ycor;
					FISHEYE_CONFIG->SrcRgnMeshInfo[frameMeshIdx].knot[knotidx].xcor = FISHEYE_REGION[i].SrcRgnMeshInfo[meshidx].knot[knotidx].xcor;
					FISHEYE_CONFIG->SrcRgnMeshInfo[frameMeshIdx].knot[knotidx].ycor = FISHEYE_REGION[i].SrcRgnMeshInfo[meshidx].knot[knotidx].ycor;
				}
				frameMeshIdx += 1;
			}
		}
	}

	// update mesh index number
	FISHEYE_CONFIG->TotalMeshNum = frameMeshIdx;

}


void _Panorama180View2( FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, MOUNTMODE MOUNT, double x0, double y0, double r)
{
	if (FISHEYE_REGION[rgn_idx].ViewMode != PROJECTION_PANORAMA_180)
		return;

	int view_w = FISHEYE_REGION[rgn_idx].OutW;
	int view_h = FISHEYE_REGION[rgn_idx].OutH;
	int mesh_horcnt = FISHEYE_REGION[rgn_idx].MeshHor;
	int mesh_vercnt = FISHEYE_REGION[rgn_idx].MeshVer;


	// UI PARAMETERS
	int _UI_horViewOffset = (int)FISHEYE_REGION[rgn_idx].Pan;		// value range = 0 ~ 360, => -180 ~ 0 ~ +180
	int _UI_verViewOffset = (int)FISHEYE_REGION[rgn_idx].Tilt;		// value = 0 ~ 360, center = 180 ( original ) => -180 ~ 0 ~ + 180
	int _UI_horViewRange  = (int)FISHEYE_REGION[rgn_idx].ZoomH;		// value = 0 ~ 4095, symmeterically control horizontal View Range, ex:  value = 4095 => hor view angle = -90 ~ + 90 
	int _UI_verViewRange  = (int)FISHEYE_REGION[rgn_idx].ZoomV;		// value = 0 ~ 4095, symmetrically control vertical view range. ex: value = 4096, ver view angle = -90 ~ + 90 

	_UI_verViewRange = (_UI_verViewRange == 4095) ? 4096 : _UI_verViewRange;
	_UI_horViewRange = (_UI_horViewRange == 4095) ? 4096 : _UI_horViewRange;


	// calculate default view range: 
	double view_range_ver_degs = (double)_UI_verViewRange * 90 / 4096;
	double view_range_hor_degs = (double)_UI_horViewRange * 90 / 4096;
	double va_ver_degs = view_range_ver_degs;
	double va_hor_degs = view_range_hor_degs;

	// calculate offsets
	double va_hor_offset = ((double)_UI_horViewOffset - 180) * 90 / 360;
	double va_ver_offset = ((double)_UI_verViewOffset - 180) * 90 / 360;

	// Offset to shift view angle
	double va_ver_start = minmax(90 - va_ver_degs + va_ver_offset,  0,  90);
	double va_ver_end   = minmax(90 + va_ver_degs + va_ver_offset, 90, 180);
	double va_hor_start = minmax(90 - va_hor_degs + va_hor_offset,  0,  90);
	double va_hor_end   = minmax(90 + va_hor_degs + va_hor_offset, 90, 180);

	RotMatrix3D mat0;
	Vector3D pix3d;
	Vector2D dist2d;


	for (int i = 0; i < (mesh_horcnt * mesh_vercnt); i++)
	{
		//printf("i = %d,", i);
		// each mesh has 4 knots
		for (int knotidx = 0; knotidx < 4; knotidx++)
		{
			double x = FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[i].knot[knotidx].xcor - FISHEYE_REGION[rgn_idx].OutX - FISHEYE_REGION[rgn_idx].OutW / 2;
			double y = FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[i].knot[knotidx].ycor - FISHEYE_REGION[rgn_idx].OutY - FISHEYE_REGION[rgn_idx].OutH / 2;
			//double z = r;

			// initial pseudo plane cooridinates as vp3d
			//double theta_hor_cur = va_hor_start + ((2 * va_hor_degs * (view_w / 2 - x)) / (double)view_w);
			double theta_hor_cur = va_hor_start + (((va_hor_end - va_hor_start) * (view_w / 2 - x)) / (double)view_w);
			double theta_hor_cur_rad = M_PI / 2 - (theta_hor_cur * M_PI) / 180;		//x

			double theta_ver_cur = va_ver_start + (((va_ver_end - va_ver_start) * (view_h / 2 - y)) / (double)view_h);
			double theta_ver_cur_rad = M_PI / 2 - (theta_ver_cur * M_PI) / 180;		//y


			double theta_x = theta_hor_cur_rad;
			double theta_y = theta_ver_cur_rad;
			double theta_z = (M_PI / 2);

			FISHEYE_REGION[rgn_idx].ThetaX = theta_x;
			FISHEYE_REGION[rgn_idx].ThetaY = theta_y;
			FISHEYE_REGION[rgn_idx].ThetaZ = theta_z;

			GenRotMatrix3D_YXZ( &mat0, FISHEYE_REGION, rgn_idx);

			pix3d.x = 0;
			pix3d.y = 0;
			pix3d.z = 1;

			Rotate3D(&pix3d, &mat0);

			// generate new 3D vector thru rotated pixel
			Equidistant(&pix3d, &dist2d, r);

			// update source mesh-info here
			FISHEYE_REGION[rgn_idx].SrcRgnMeshInfo[i].knot[knotidx].xcor = dist2d.x + x0;
			FISHEYE_REGION[rgn_idx].SrcRgnMeshInfo[i].knot[knotidx].ycor = dist2d.y + y0;

			//printf("(x2d)(%f,%f),\n\r",dist2d.x + x0, dist2d.y + y0);

		}
	}


	// draw debug frames
	// for loop @ display image
#define FISHEYER 512

	unsigned char* dst_img, * map_img;
	dst_img = (unsigned char*)malloc(view_w * view_h * 3 * sizeof(unsigned char));
	map_img = (unsigned char*)malloc(FISHEYER * FISHEYER * 3 * sizeof(unsigned char));

	memset(map_img, 0, FISHEYER * FISHEYER * 3 * sizeof(unsigned char));
	memset(dst_img, 0, view_w * view_h * 3 * sizeof(unsigned char));
	
	char filename_v[256], filename_m[256];
	for (int i = 0; i < (mesh_horcnt * mesh_vercnt); i++)
	{
		double DebugValue = i*(256 * 3) / (mesh_horcnt * mesh_vercnt);
		double debugR = MIN(255, DebugValue);
		double debugG = MIN(255, MAX(0, DebugValue - 255));
		double debugB = MIN(255, MAX(0, DebugValue - 255 - 255));

		double xsrc = FISHEYE_REGION[rgn_idx].SrcRgnMeshInfo[i].knot[0].xcor;
		double ysrc = FISHEYE_REGION[rgn_idx].SrcRgnMeshInfo[i].knot[0].ycor;

		double xdst = FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[i].knot[0].xcor;
		double ydst = FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[i].knot[0].ycor;
	
		
		for (int fx = -2; fx < 2; fx++)
		{
			for (int fy = -2; fy < 2; fy++)
			{
				int mx = minmax((int)floor(xsrc+fx), 0, FISHEYER - 1);
				int my = minmax((int)floor(ysrc+fy), 0, FISHEYER - 1);

				*(map_img + (my * FISHEYER + mx) * 3 + 0) = (int)debugR;
				*(map_img + (my * FISHEYER + mx) * 3 + 1) = (int)debugG;
				*(map_img + (my * FISHEYER + mx) * 3 + 2) = (int)debugB;
			}
		}

		for (int fx = -2; fx < 2; fx++)
		{
			for (int fy = -2; fy < 2; fy++)
			{
				int px = minmax((int)floor(xdst+fx), 0, view_w - 1);
				int py = minmax((int)floor(ydst+fy), 0, view_h - 1);

				*(dst_img + (py * view_w + px) * 3 + 0) = (int)debugR;
				*(dst_img + (py * view_w + px) * 3 + 1) = (int)debugG;
				*(dst_img + (py * view_w + px) * 3 + 2) = (int)debugB;
			}
		}

		//printf("idx = %d, (xsrc,ysrc)=(%f,%f), px = %d, py = %d, \n\r", i, xsrc, ysrc, mx, my);

	}
	sprintf(filename_v, "pview_mod.ppm");
	sprintf(filename_m, "pmap_mod.ppm");
	RGB888Image2PPM(dst_img, view_w, view_h, filename_v);
	RGB888Image2PPM(map_img, FISHEYER, FISHEYER, filename_m);
	free(dst_img);
	free(map_img);

}
void _Panorama360View2( FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, MOUNTMODE MOUNT, double x0, double y0, double r)
{
	if (FISHEYE_REGION[rgn_idx].ViewMode != PROJECTION_PANORAMA_360)
		return;

	int view_w = FISHEYE_REGION[rgn_idx].OutW;
	int view_h = FISHEYE_REGION[rgn_idx].OutH;
	int mesh_horcnt = FISHEYE_REGION[rgn_idx].MeshHor;
	int mesh_vercnt = FISHEYE_REGION[rgn_idx].MeshVer;

	Vector3D pix3d;
	Vector2D dist2d;

	// UI PARAMETERS
	double _UI_start_angle		= FISHEYE_REGION[rgn_idx].Pan;					// value from 0=360, 0 ~ 356 is the range of adjustment. 
	double _UI_view_offset		= FISHEYE_REGION[rgn_idx].Tilt;					// value = 0 ~ 360, center = 180 ( original ), OR create offset to shift OutRadius & InRadius ( = adjust theta angle in our implementation )
	double _UI_inradius			= FISHEYE_REGION[rgn_idx].InRadius;				// value = 0 ~ 4095	
	double _UI_outradius		= FISHEYE_REGION[rgn_idx].OutRadius;			// value = 0 ~ 4095
	double _UI_zoom_outradius	= FISHEYE_REGION[rgn_idx].ZoomV;				// a ratio to zoom OutRadius length.

	_UI_inradius  = (_UI_inradius == 4095)  ? 4096 : _UI_inradius;
	_UI_outradius = (_UI_outradius == 4095) ? 4096 : _UI_outradius;
	_UI_zoom_outradius = (_UI_zoom_outradius == 4095) ? 4096 : _UI_zoom_outradius;

	int start_angle_degrees  = (int)FISHEYE_REGION[rgn_idx].Pan;
	int end_angle__degrees   = (int)FISHEYE_REGION[rgn_idx].PanEnd;

	// internal calculation
	double raw_outradius_pxl = (_UI_outradius * r )/ 4096;
	double raw_inradius_pxl  = (_UI_inradius * r )/ 4096;
	double radiusOffset	    = (raw_outradius_pxl * (_UI_view_offset - 180)) / 360;

	double inradius_pxl_final  = MIN(r, MAX(0, raw_inradius_pxl + radiusOffset));
	double outradius_pxl_final = MIN(r, MAX(inradius_pxl_final, raw_inradius_pxl + (MAX(0, (raw_outradius_pxl - raw_inradius_pxl)) * _UI_zoom_outradius) / 256 + radiusOffset));

	double va_ver_end_rads   = inradius_pxl_final * M_PI / (2 * r);//_UI_inradius * M_PI / 1024;					// for Equidistant =>rd = f*theta, rd_max = f*PI/2 = r ( in code), f = 2r/PI. => theta = rd/f = rd*PI/2r
	double va_ver_start_rads = outradius_pxl_final * M_PI / (2 * r);//_UI_outradius * M_PI / 1024;
	double va_ver_rads = MIN( M_PI/2, va_ver_start_rads - va_ver_end_rads);

	int total_angle = (360 + (end_angle__degrees - start_angle_degrees)) % 360;

	for (int i = 0; i < (mesh_horcnt * mesh_vercnt); i++)
	{
		// each mesh has 4 knots
		for (int knotidx = 0; knotidx < 4; knotidx++)
		{
			double phi_degrees,	phi_rad, theta_rv;

			double x = FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[i].knot[knotidx].xcor	- FISHEYE_REGION[rgn_idx].OutX - FISHEYE_REGION[rgn_idx].OutW / 2;
			double y = FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[i].knot[knotidx].ycor	- FISHEYE_REGION[rgn_idx].OutY - FISHEYE_REGION[rgn_idx].OutH / 2;
			double z = r;

			if(MOUNT == CEILING) 
			{
				phi_degrees = (total_angle * (x + view_w / 2)) / (double)view_w;
				phi_rad = (((start_angle_degrees + phi_degrees) * M_PI)) / 180;
				//theta_rv = (((va_ver_degs * M_PI) / 180) * (y + (view_h / 2))) / (double)(view_h);
				theta_rv = ( va_ver_rads * (y + (view_h / 2))) / (double)(view_h);
			}
			else if (MOUNT == FLOOR) // FLOORMODE
			{
				phi_degrees = (total_angle * (view_w / 2 - x)) / (double)view_w;
				phi_rad = (((start_angle_degrees + phi_degrees) * M_PI)) / 180;
				//theta_rv = (((va_ver_degs * M_PI) / 180) * ((view_h / 2) - y)) / (double)(view_h);
				theta_rv = ( va_ver_rads * ((view_h / 2) - y)) / (double)(view_h);
			}
			else // WALL MODE
			{
				printf("mount mode ?? in panorama case !!");
                //system("pause");
                return;
			}
			
			// 2D plane cooridnate to cylinder
			// rotation initial @ [x, y, z] = [1, 0, 0];
			double xc =  1 * cos(-phi_rad) + 0 * sin(-phi_rad);
			double yc = -1 * sin(-phi_rad) + 0 * cos(-phi_rad);
			double zc = theta_rv + va_ver_end_rads;

			//Rotate3D(&pix3d, &mat0);
			pix3d.x = xc;
			pix3d.y = yc;
			pix3d.z = zc;
			//Equidistant(&pix3d, &dist2d, r);
			Equidistant_Panorama(&pix3d, &dist2d, r);
			// update source mesh-info here
			FISHEYE_REGION[rgn_idx].SrcRgnMeshInfo[i].knot[knotidx].xcor = dist2d.x + x0;
			FISHEYE_REGION[rgn_idx].SrcRgnMeshInfo[i].knot[knotidx].ycor = dist2d.y + y0;
		}
	}
}
void _RegionView2( FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, MOUNTMODE MOUNT, double x0, double y0, double r)
{
	if (FISHEYE_REGION[rgn_idx].ViewMode != PROJECTION_REGION)
		return;

	int view_w = FISHEYE_REGION[rgn_idx].OutW;
	int view_h = FISHEYE_REGION[rgn_idx].OutH;
	int mesh_horcnt = FISHEYE_REGION[rgn_idx].MeshHor;
	int mesh_vercnt = FISHEYE_REGION[rgn_idx].MeshVer;

	// rotation matrix to point out view center of this region.
	RotMatrix3D mat0;
	GenRotMatrix3D_YXZ(&mat0, FISHEYE_REGION, rgn_idx);

	Vector3D pix3d;
	Vector2D dist2d;

	// go through all meshes in thus regions
	// mat0 is decided by view angle defined in re gion config.
	for (int i = 0; i < (mesh_horcnt * mesh_vercnt); i++)
	{
		// each mesh has 4 knots
		for (int knotidx = 0; knotidx < 4; knotidx++)
		{
			// display window center locate @ (0,0), mesh info shife back to center O.
			// for each region, rollback center is (0,0)
			double x = FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[i].knot[knotidx].xcor
				- FISHEYE_REGION[rgn_idx].OutX - FISHEYE_REGION[rgn_idx].OutW / 2;
			double y = FISHEYE_REGION[rgn_idx].DstRgnMeshInfo[i].knot[knotidx].ycor
				- FISHEYE_REGION[rgn_idx].OutY - FISHEYE_REGION[rgn_idx].OutH / 2;
			double z = r;
	
			if (MOUNT == FLOOR)
			{
				x = -x;
				y = -y;
			}


			// Zooming In/Out Control by ZoomH & ZoomV
			// 1.0 stands for +- 50% of zoomH ratio. => set as 
			x = x *(1 + (1.0 * (FISHEYE_REGION[rgn_idx].ZoomH - 2048) / 4096));
			y = y *(1 + (1.0 * (FISHEYE_REGION[rgn_idx].ZoomV - 2048) / 4096));


			pix3d.x = x; 
			pix3d.y = y;
			pix3d.z = z;

			// Region Porjection Model 
			Rotate3D(&pix3d, &mat0);
			Equidistant(&pix3d, &dist2d, r);

			// update source mesh-info here
			FISHEYE_REGION[rgn_idx].SrcRgnMeshInfo[i].knot[knotidx].xcor = dist2d.x + x0;
			FISHEYE_REGION[rgn_idx].SrcRgnMeshInfo[i].knot[knotidx].ycor = dist2d.y + y0;
		}
	}
}
void _Panorama180View(unsigned char* src_img, unsigned char* rgn_img, unsigned char* src_rois, FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double x0, double y0, double r)
{
	if (FISHEYE_REGION[rgn_idx].ViewMode != PROJECTION_PANORAMA_180)
		return;

	int view_w = FISHEYE_REGION[rgn_idx].OutW;
	int view_h = FISHEYE_REGION[rgn_idx].OutH;
	int src_width = FISHEYE_CONFIG->InWidth;
	int src_height = FISHEYE_CONFIG->InHeight;


	// UI PARAMETERS
	int _UI_horViewOffset = (int)FISHEYE_REGION[rgn_idx].Pan;		// value range = 0 ~ 360, => -180 ~ 0 ~ +180
	int _UI_verViewOffset = (int)FISHEYE_REGION[rgn_idx].Tilt;		// value = 0 ~ 360, center = 180 ( original ) => -180 ~ 0 ~ + 180
	int _UI_horViewRange = (int)FISHEYE_REGION[rgn_idx].ZoomH;		// value = 0 ~ 4095, symmeterically control horizontal View Range, ex:  value = 4095 => hor view angle = -90 ~ + 90 
	int _UI_verViewRange = (int)FISHEYE_REGION[rgn_idx].ZoomV;		// value = 0 ~ 4095, symmetrically control vertical view range. ex: value = 4096, ver view angle = -90 ~ + 90 

	_UI_verViewRange = (_UI_verViewRange == 4095) ? 4096 : _UI_verViewRange;
	_UI_horViewRange = (_UI_horViewRange == 4095) ? 4096 : _UI_horViewRange;


	// calculate default view range: 
	double view_range_ver_degs = (double)_UI_verViewRange * 90 / 4096;
	double view_range_hor_degs = (double)_UI_horViewRange * 90 / 4096;
	double va_ver_degs = view_range_ver_degs;
	double va_hor_degs = view_range_hor_degs;

	// calculate offsets
	double va_hor_offset = ((double)_UI_horViewOffset - 180) * 90 / 360;
	double va_ver_offset = ((double)_UI_verViewOffset - 180) * 90 / 360;

	// Offset to shift view angle
	double va_ver_start = minmax(90 - va_ver_degs + va_ver_offset, 0, 90);
	double va_ver_end = minmax(90 + va_ver_degs + va_ver_offset, 90, 180);
	double va_hor_start = minmax(90 - va_hor_degs + va_hor_offset, 0, 90);
	double va_hor_end = minmax(90 + va_hor_degs + va_hor_offset, 90, 180);

	RotMatrix3D mat0;
	Vector3D pix3d;
	Vector2D dist2d;


	for (int rgny = 0; rgny < view_h; rgny++)
	{
		for (int rgnx = 0; rgnx < view_w; rgnx++)
		{
			double x = rgnx - FISHEYE_REGION[rgn_idx].OutW / 2;
			double y = rgny - FISHEYE_REGION[rgn_idx].OutH / 2;
			//double z = r;

			// initial pseudo plane cooridinates as vp3d
			//double theta_hor_cur = va_hor_start + ((2 * va_hor_degs * (view_w / 2 - x)) / (double)view_w);
			double theta_hor_cur = va_hor_start + (((va_hor_end - va_hor_start) * (view_w / 2 - x)) / (double)view_w);
			double theta_hor_cur_rad = M_PI / 2 - (theta_hor_cur * M_PI) / 180;		//x

			double theta_ver_cur = va_ver_start + (((va_ver_end - va_ver_start) * (view_h / 2 - y)) / (double)view_h);
			double theta_ver_cur_rad = M_PI / 2 - (theta_ver_cur * M_PI) / 180;		//y


			double theta_x = theta_hor_cur_rad;
			double theta_y = theta_ver_cur_rad;
			double theta_z = (M_PI / 2);

			FISHEYE_REGION[rgn_idx].ThetaX = theta_x;
			FISHEYE_REGION[rgn_idx].ThetaY = theta_y;
			FISHEYE_REGION[rgn_idx].ThetaZ = theta_z;

			GenRotMatrix3D_YXZ(&mat0, FISHEYE_REGION, rgn_idx);

			pix3d.x = 0;
			pix3d.y = 0;
			pix3d.z = 1;

			Rotate3D(&pix3d, &mat0);

			// generate new 3D vector thru rotated pixel
			Equidistant(&pix3d, &dist2d, r);

			// update source mesh-info here
			//FISHEYE_REGION[rgn_idx].SrcRgnMeshInfo[i].knot[knotidx].xcor = dist2d.x + x0;
			//FISHEYE_REGION[rgn_idx].SrcRgnMeshInfo[i].knot[knotidx].ycor = dist2d.y + y0;

			// update source mesh-info here
			// sample src pixel here: 
			int px = (int)(dist2d.x + x0);
			int py = (int)(dist2d.y + y0);

			int px_clip = minmax(px, 0, src_width - 1);
			int py_clip = minmax(py, 0, src_height - 1);

			unsigned char pixR, pixG, pixB;
			if (px_clip != px || py_clip != py)
			{
				pixR = FISHEYE_CONFIG->BgColor.R;
				pixG = FISHEYE_CONFIG->BgColor.G;
				pixB = FISHEYE_CONFIG->BgColor.B;
			}
			else
			{
				pixR = *(src_img + (py * src_width + px) * 3 + 0);
				pixG = *(src_img + (py * src_width + px) * 3 + 1);
				pixB = *(src_img + (py * src_width + px) * 3 + 2);
			}

			*(rgn_img + (rgny * view_w + rgnx) * 3 + 0) = pixR;
			*(rgn_img + (rgny * view_w + rgnx) * 3 + 1) = pixG;
			*(rgn_img + (rgny * view_w + rgnx) * 3 + 2) = pixB;

			// draw debug ROIs
			pixR = (rgn_idx % 3 == 0 || rgn_idx >= 3) ? 255 : 0;
			pixG = (rgn_idx % 3 == 1 || rgn_idx >= 3) ? 255 : 0;
			pixB = (rgn_idx % 3 == 2 || rgn_idx >= 3) ? 255 : 0;

			if (rgny == 0 || rgny == view_h - 1 || rgnx == 0 || rgnx == view_w - 1)
			{
				for (int j = -1; j <= 1; j++)
					for (int i = -1; i <= 1; i++)
					{
						py_clip = minmax(py_clip + j, 0, src_height - 1);
						px_clip = minmax(px_clip + i, 0, src_width - 1);

						*(src_rois + (py_clip * src_width + px_clip) * 3 + 0) = pixR; //R
						*(src_rois + (py_clip * src_width + px_clip) * 3 + 1) = pixG; //G
						*(src_rois + (py_clip * src_width + px_clip) * 3 + 2) = pixB; //B
					}
			}
		}
	}
}
void _Panorama360View(unsigned char* src_img, unsigned char* rgn_img, unsigned char* src_rois, FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double x0, double y0, double r)
{
	if (FISHEYE_REGION[rgn_idx].ViewMode != PROJECTION_PANORAMA_360)
		return;

	int view_w = FISHEYE_REGION[rgn_idx].OutW;
	int view_h = FISHEYE_REGION[rgn_idx].OutH;
	int src_width = FISHEYE_CONFIG->InWidth;
	int src_height = FISHEYE_CONFIG->InHeight;

	Vector3D pix3d;
	Vector2D dist2d;

	// UI PARAMETERS
	double _UI_start_angle = FISHEYE_REGION[rgn_idx].Pan;					// value from 0=360, 0 ~ 356 is the range of adjustment. 
	double _UI_view_offset = FISHEYE_REGION[rgn_idx].Tilt;					// value = 0 ~ 360, center = 180 ( original ), OR create offset to shift OutRadius & InRadius ( = adjust theta angle in our implementation )
	double _UI_inradius = FISHEYE_REGION[rgn_idx].InRadius;					// value = 0 ~ 4095	
	double _UI_outradius = FISHEYE_REGION[rgn_idx].OutRadius;				// value = 0 ~ 4095
	double _UI_zoom_outradius = FISHEYE_REGION[rgn_idx].ZoomV;				// a ratio to zoom OutRadius length.

	_UI_inradius = (_UI_inradius == 4095) ? 4096 : _UI_inradius;
	_UI_outradius = (_UI_outradius == 4095) ? 4096 : _UI_outradius;
	_UI_zoom_outradius = (_UI_zoom_outradius == 4095) ? 4096 : _UI_zoom_outradius;

	int start_angle_degrees = (int)FISHEYE_REGION[rgn_idx].Pan;
	int end_angle__degrees = (int)FISHEYE_REGION[rgn_idx].PanEnd;


	// internal calculation
	double raw_outradius_pxl = (_UI_outradius * r) / 4096;
	double raw_inradius_pxl = (_UI_inradius * r) / 4096;
	double radiusOffset = (raw_outradius_pxl * (_UI_view_offset - 180)) / 360;

	double inradius_pxl_final = MIN(r, MAX(0, raw_inradius_pxl + radiusOffset));
	double outradius_pxl_final = MIN(r, MAX(inradius_pxl_final, raw_inradius_pxl + (MAX(0, (raw_outradius_pxl - raw_inradius_pxl)) * _UI_zoom_outradius) / 256 + radiusOffset));

	double va_ver_end_rads = inradius_pxl_final * M_PI / (2 * r);//_UI_inradius * M_PI / 1024;					// for Equidistant =>rd = f*theta, rd_max = f*PI/2 = r ( in code), f = 2r/PI. => theta = rd/f = rd*PI/2r
	double va_ver_start_rads = outradius_pxl_final * M_PI / (2 * r);//_UI_outradius * M_PI / 1024;
	double va_ver_rads = MIN(M_PI / 2, va_ver_start_rads - va_ver_end_rads);

	int total_angle = (360 + (end_angle__degrees - start_angle_degrees)) % 360;

	for (int rgny = 0; rgny < view_h; rgny++)
	{
		for (int rgnx = 0; rgnx < view_w; rgnx++)
		{
			double phi_degrees, phi_rad, theta_rv;

			double x = rgnx - FISHEYE_REGION[rgn_idx].OutW / 2;
			double y = rgny - FISHEYE_REGION[rgn_idx].OutH / 2;
			double z = r;

			if (FISHEYE_CONFIG->MntMode == CEILING)
			{
				phi_degrees = (total_angle * (x + view_w / 2)) / (double)view_w;
				phi_rad = (((start_angle_degrees + phi_degrees) * M_PI)) / 180;
				//theta_rv = (((va_ver_degs * M_PI) / 180) * (y + (view_h / 2))) / (double)(view_h);
				theta_rv = (va_ver_rads * (y + (view_h / 2))) / (double)(view_h);
			}
            else //if (FISHEYE_CONFIG->MntMode == FLOOR) // FLOORMODE
			{
				phi_degrees = (total_angle * (view_w / 2 - x)) / (double)view_w;
				phi_rad = (((start_angle_degrees + phi_degrees) * M_PI)) / 180;
				//theta_rv = (((va_ver_degs * M_PI) / 180) * ((view_h / 2) - y)) / (double)(view_h);
				theta_rv = (va_ver_rads * ((view_h / 2) - y)) / (double)(view_h);
			}
//			else // WALL MODE
//			{
//				printf("mount mode ?? in panorama case !!");
//                //system("pause");
//                break;
//			}

			// 2D plane cooridnate to cylinder
			// rotation initial @ [x, y, z] = [1, 0, 0];
			double xc = 1 * cos(-phi_rad) + 0 * sin(-phi_rad);
			double yc = -1 * sin(-phi_rad) + 0 * cos(-phi_rad);
			double zc = theta_rv + va_ver_end_rads;


			//Rotate3D(&pix3d, &mat0);
			pix3d.x = xc;
			pix3d.y = yc;
			pix3d.z = zc;
			//Equidistant(&pix3d, &dist2d, r);

			Equidistant_Panorama(&pix3d, &dist2d, r);


			// update source mesh-info here
			// sample src pixel here: 
			int px = (int)(dist2d.x + x0);
			int py = (int)(dist2d.y + y0);

			int px_clip = minmax(px, 0, src_width - 1);
			int py_clip = minmax(py, 0, src_height - 1);


			unsigned char pixR, pixG, pixB;

			if (px_clip != px || py_clip != py)
			{
				pixR = FISHEYE_CONFIG->BgColor.R;
				pixG = FISHEYE_CONFIG->BgColor.G;
				pixB = FISHEYE_CONFIG->BgColor.B;
			}
			else
			{
				pixR = *(src_img + (py * src_width + px) * 3 + 0);
				pixG = *(src_img + (py * src_width + px) * 3 + 1);
				pixB = *(src_img + (py * src_width + px) * 3 + 2);
			}

			*(rgn_img + (rgny * view_w + rgnx) * 3 + 0) = pixR;	//R
			*(rgn_img + (rgny * view_w + rgnx) * 3 + 1) = pixG;	//G
			*(rgn_img + (rgny * view_w + rgnx) * 3 + 2) = pixB; //B



            // draw debug ROIs
			pixR = (rgn_idx % 3 == 0 || rgn_idx >= 3) ? 255 : 0;
			pixG = (rgn_idx % 3 == 1 || rgn_idx >= 3) ? 255 : 0;
			pixB = (rgn_idx % 3 == 2 || rgn_idx >= 3) ? 255 : 0;

			if (rgny == 0 || rgny == view_h - 1 || rgnx == 0 || rgnx == view_w - 1)
			{
				for (int j = -1; j <= 1; j++)
					for (int i = -1; i <= 1; i++)
					{
						py_clip = minmax(py_clip + j, 0, src_height - 1);
						px_clip = minmax(px_clip + i, 0, src_width - 1);

						*(src_rois + (py_clip * src_width + px_clip) * 3 + 0) = pixR; //R
						*(src_rois + (py_clip * src_width + px_clip) * 3 + 1) = pixG; //G
						*(src_rois + (py_clip * src_width + px_clip) * 3 + 2) = pixB; //B
					}
			}

		}
	}
}
void _RegionView(unsigned char* src_img, unsigned char* rgn_img, unsigned char* src_rois, FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double x0, double y0, double r)
{
	// DRAW UI FUNCTION
	if (FISHEYE_REGION[rgn_idx].ViewMode != PROJECTION_REGION)
		return;

	int view_w = FISHEYE_REGION[rgn_idx].OutW;
	int view_h = FISHEYE_REGION[rgn_idx].OutH;

	int src_width = FISHEYE_CONFIG->InWidth;
	int src_height = FISHEYE_CONFIG->InHeight;


	// rotation matrix to point out view center of this region.
	RotMatrix3D mat0;
	GenRotMatrix3D_YXZ(&mat0, FISHEYE_REGION, rgn_idx);

	Vector3D pix3d;
	Vector2D dist2d;

	// go through all meshes in thus regions
	// mat0 is decided by view angle defined in re gion config.
	for (int rgny = 0; rgny < view_h; rgny++)
	{
		for (int rgnx = 0; rgnx < view_w; rgnx++)
		{
			// display window center locate @ (0,0), mesh info shife back to center O.
			// for each region, rollback center is (0,0)
			double x = rgnx - FISHEYE_REGION[rgn_idx].OutW / 2;
			double y = rgny - FISHEYE_REGION[rgn_idx].OutH / 2;
			double z = r;

			if (FISHEYE_CONFIG->MntMode == FLOOR)
			{
				x = -x;
				y = -y;
			}

			// Zooming In/Out Control by ZoomH & ZoomV
			// 1.0 stands for +- 50% of zoomH ratio. => set as 
			x = x * (1 + (1.0 * (FISHEYE_REGION[rgn_idx].ZoomH - 2048) / 4096));
			y = y * (1 + (1.0 * (FISHEYE_REGION[rgn_idx].ZoomV - 2048) / 4096));

			pix3d.x = x;
			pix3d.y = y;
			pix3d.z = z;

			// Region Porjection Model 
			Rotate3D(&pix3d, &mat0);
			Equidistant(&pix3d, &dist2d, r);

			// update source mesh-info here
			// sample src pixel here: 
			int px = (int)(dist2d.x + x0);
			int py = (int)(dist2d.y + y0);

			int px_clip = minmax(px, 0, src_width - 1);
			int py_clip = minmax(py, 0, src_height - 1);


			unsigned char pixR, pixG, pixB;

			if (px_clip != px || py_clip != py)
			{
				pixR = FISHEYE_CONFIG->BgColor.R;
				pixG = FISHEYE_CONFIG->BgColor.G;
				pixB = FISHEYE_CONFIG->BgColor.B;
			}
			else
			{
				pixR = *(src_img + (py * src_width + px) * 3 + 0);
				pixG = *(src_img + (py * src_width + px) * 3 + 1);
				pixB = *(src_img + (py * src_width + px) * 3 + 2);
			}

			*(rgn_img + (rgny * view_w + rgnx) * 3 + 0) = pixR;	//R
			*(rgn_img + (rgny * view_w + rgnx) * 3 + 1) = pixG;	//G
			*(rgn_img + (rgny * view_w + rgnx) * 3 + 2) = pixB; //B


			// draw debug ROIs
			pixR = (rgn_idx % 3 == 0 || rgn_idx >= 3) ? 255 : 0;
			pixG = (rgn_idx % 3 == 1 || rgn_idx >= 3) ? 255 : 0;
			pixB = (rgn_idx % 3 == 2 || rgn_idx >= 3) ? 255 : 0;

			if (rgny == 0 || rgny == view_h - 1 || rgnx == 0 || rgnx == view_w - 1)
			{
				for( int j = -1; j <=1; j++)
					for (int i = -1; i <= 1; i++)
					{
						py_clip = minmax(py_clip + j, 0, src_height-1);
						px_clip = minmax(px_clip + i, 0, src_width-1);

						*(src_rois + (py_clip * src_width + px_clip) * 3 + 0) = pixR; //R
						*(src_rois + (py_clip * src_width + px_clip) * 3 + 1) = pixG; //G
						*(src_rois + (py_clip * src_width + px_clip) * 3 + 2) = pixB; //B
					}
			}
		}
	}
}

void _api_set_rgn_proj_region(FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double set_Pan, double set_Tilt, double set_ThetaX, double set_ThetaY, double set_ThetaZ, double set_ZoomH, double set_ZoomV)
{

	if ((FISHEYE_REGION[rgn_idx].ViewMode != PROJECTION_REGION)
		|| (FISHEYE_REGION[rgn_idx].RegionValid == false))
	{
        printf("Set Region Is Not PROJECTION Region or this Region is no activated, Invalid Command\n");
		return;
	}
	else  //update parameters
	{

		FISHEYE_REGION[rgn_idx].Pan = minmax(set_Pan, 0, 359);
		FISHEYE_REGION[rgn_idx].Tilt = minmax(set_Tilt, 0, 359);
		FISHEYE_REGION[rgn_idx].ThetaX = minmax(set_ThetaX, 0, 359);
		FISHEYE_REGION[rgn_idx].ThetaY = minmax(set_ThetaY, 0, 359);
		FISHEYE_REGION[rgn_idx].ThetaZ = minmax(set_ThetaZ, 0, 359);

		FISHEYE_REGION[rgn_idx].ZoomH = minmax(set_ZoomH, 0, 4095);
		FISHEYE_REGION[rgn_idx].ZoomV = minmax(set_ZoomV, 0, 4095);
	}
}


void _api_set_rgn_proj_panorama180(FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double set_Pan, double set_Tilt, double set_ZoomH, double set_ZoomV)
{

	if ((FISHEYE_REGION[rgn_idx].ViewMode != PROJECTION_PANORAMA_180)
		|| (FISHEYE_REGION[rgn_idx].RegionValid == false))
	{
		printf("Set Region Is Not Panorama 180 or this Region is no activated, Invalid Command\n");
		return;
	}
	else  //update parameters
	{

		FISHEYE_REGION[rgn_idx].Pan = minmax(set_Pan, 0, 359);
		FISHEYE_REGION[rgn_idx].Tilt = minmax(set_Tilt, 0, 359);
		FISHEYE_REGION[rgn_idx].ZoomH = minmax(set_ZoomH, 0, 4095);
		FISHEYE_REGION[rgn_idx].ZoomV = minmax(set_ZoomV, 0, 4095);
	}
}

void _api_set_rgn_proj_panorama360( FISHEYE_REGION_ATTR* FISHEYE_REGION,int rgn_idx, double set_InRadius, double set_OutRadius, double set_Pan, double set_Tilt, double set_ZoomV, double set_PanEnd)
{

	if(		(FISHEYE_REGION[rgn_idx].ViewMode != PROJECTION_PANORAMA_360)
		||	(FISHEYE_REGION[rgn_idx].RegionValid == false ))
	{
		printf("Set Region Is Not Panorama 360 or this Region is no activated, Invalid Command\n");
		return;
	}
	else  //update parameters
	{
		FISHEYE_REGION[rgn_idx].InRadius	= minmax(set_InRadius, 0, 4095);
		FISHEYE_REGION[rgn_idx].OutRadius	= minmax(set_OutRadius, 0, 4095);
		FISHEYE_REGION[rgn_idx].Pan			= minmax(set_Pan, 0, 359);
		FISHEYE_REGION[rgn_idx].Tilt		= minmax(set_Tilt, 0, 359);
		FISHEYE_REGION[rgn_idx].ZoomV		= minmax(set_ZoomV,0,4095);
		FISHEYE_REGION[rgn_idx].PanEnd		= minmax(set_PanEnd,0,359);
	}
}

#if 0
int main(int argc, char * argv[])
{
	int rtn = 0;

	// in & out file name
	std::string infile_str;

	int dst_width	= 0;
	int dst_height  = 0;

	int x0 = 0;	// lens center cooridinate of the input image
	int y0 = 0;	// lens center cooridinate of the input image
	int r  = 1; // fisheye radius
	
	for (int i=1; i<argc; i++){
        if (strcmp("-i", argv[i]) == 0){
            if (++i >= argc)
                return -1;
            infile_str = argv[i];	
        }
		else if (strcmp("-ow", argv[i]) == 0) {
			if (++i >= argc)
				return -1;
			dst_width = atoi(argv[i]);
		}
		else if (strcmp("-oh", argv[i]) == 0) {
			if (++i >= argc)
				return -1;
			dst_height = atoi(argv[i]);
		}
		else if (strcmp("-xc", argv[i]) == 0) {
			if (++i >= argc)
				return -1;
			x0 = atoi(argv[i]);
		}
		else if (strcmp("-yc", argv[i]) == 0) {
			if (++i >= argc)
				return -1;
			y0 = atoi(argv[i]);
		}
 		else if (strcmp("-r", argv[i]) == 0) {
			if (++i >= argc)
				return -1;
			r = atoi(argv[i]);				//Y_TILE_NUMBER:create y-dir sliced sections  // bastian
		}
		else if (strcmp("-r0", argv[i]) == 0) {
			if (++i >= argc)
				return -1;
			r = atoi(argv[i]);				//Y_TILE_NUMBER:create y-dir sliced sections  // bastian
		}
    }

	// input info. from input image: 
	int src_width, src_height;
	unsigned char* src_img  = ReadPPM(infile_str.c_str(), &src_width, &src_height);
	unsigned char* src_rois = ReadPPM(infile_str.c_str(), &src_width, &src_height);

	// Check for invalid input
	if(infile_str.empty())
	{
        cout <<  "using pseudo mesh image" << endl ;
		system("pause");
	}

	unsigned char* dst_img;				// image of pixel-based projection of chosen model 

	// update parameters 
	FISHEYE_ATTR FISHEYE_CONFIG;
	FISHEYE_REGION_ATTR FISHEYE_REGION[MAX_REGION_NUM];
	
	INITIAL_CONFIG(&FISHEYE_CONFIG, FISHEYE_REGION, dst_height, dst_width, src_width, src_height, x0, y0, r);

	// initial display iimage
	dst_img = (unsigned char*)malloc(FISHEYE_CONFIG.OutW_disp * FISHEYE_CONFIG.OutH_disp * 3 * sizeof(unsigned char));
	memset(dst_img, 0, FISHEYE_CONFIG.OutW_disp * FISHEYE_CONFIG.OutH_disp * 3 * sizeof(unsigned char));

	// In Each Mode, for Every Region:
	for (int rgn_idx = 0; rgn_idx < FISHEYE_CONFIG.RgnNum; rgn_idx++)
	{
		// check region valid first
		if (!FISHEYE_REGION[rgn_idx].RegionValid)
			return 0;

		// Destination Mesh-Info Allocation
		int view_w = FISHEYE_REGION[rgn_idx].OutW;
		int view_h = FISHEYE_REGION[rgn_idx].OutH;
		int mesh_horcnt = FISHEYE_REGION[rgn_idx].MeshHor;
		int mesh_vercnt = FISHEYE_REGION[rgn_idx].MeshVer;

		// get & store region mesh info.
		//_get_region_dst_mesh_list(FISHEYE_REGION, view_w, view_h, mesh_horcnt, mesh_vercnt, rgn_idx);
		// Get Source Mesh-Info Projected from Destination by Differet ViewModw. 
		//_get_region_src_mesh_list(FISHEYE_CONFIG.MntMode, FISHEYE_REGION, mesh_horcnt, mesh_vercnt, rgn_idx, x0, y0, r);
		// UI need to output region image separately. 

		_dump_draw_sub_window_img( &FISHEYE_CONFIG, FISHEYE_REGION, rgn_idx, x0, y0, r, src_img, src_rois);

		// debug msg
		printf("REGION %d done. \n", rgn_idx);
	}

	char filename[256];
	sprintf(filename, "src_rois.ppm");
	RGB888Image2PPM(src_rois, src_width, src_height, filename);

	return rtn;
}
#endif

