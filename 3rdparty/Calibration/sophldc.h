#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SOPHLDC_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SOPHLDC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SOPHLDC_EXPORTS
#define SOPHLDC_API __declspec(dllexport)
#else
#define SOPHLDC_API __declspec(dllimport)
#endif

#ifndef SOPHLDC_H
#define SOPHLDC_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>


#ifdef __cplusplus
extern "C" {
#endif


	typedef struct _ldcdata
	{
		float *psrcmapx;					// mapx buffer
		float *psrcmapy;					// mapy buffer
		unsigned char *pimg;				// gray buffer
		unsigned char *pmeshbin;			// mesh_.bin buffer
		int size_meshbin;					// number of bytes of mesh.bin 
		int max_nbrmesh;					// max_nbrmesh = (imgw/meshw)*(imgh/meshh)
		int imgw, imgh;						// image width and height
		int realw, realh;					// 64 alignment image width and height
		int meshw, meshh;					// mesh width and height >=16
		int nbr_bytes_work;					// max number of bytes for working system
		float *pdstmapx;					// 64aligned mapx buffer
		float *pdstmapy;					// 64aligned mapy buffer
		int mesh_1st_size, mesh_2nd_size;
		bool binit;
		int alignnbr;
		_ldcdata() {
			binit = false;
		};
	}ldcdata;
	// m1
	SOPHLDC_API void setzero_ldcdata(ldcdata *pldcdata);
	SOPHLDC_API void free_ldcdata(ldcdata *pldcdata);
	SOPHLDC_API int align_copyto_buf(unsigned char *pimg, float *pmapx, float *pmapy, int imgw, int imgh, ldcdata *pldcdata, char *pfolder, int flag, int dx, int dy);
	SOPHLDC_API int do_ldc_training_buf(ldcdata *pldcdata, char *pwork_folder);
	// m2
	SOPHLDC_API int param_to_mapxy(float *pcam, float *pdist, float *pmapLx, float *pmapLy, int imgw, int imgh);
	SOPHLDC_API int do_ldc_training(char *pfilename, char *pwork_folder, int max_nbrmesh);
	SOPHLDC_API char *GetDLLVersionInfo(bool bsel);
#ifdef __cplusplus
}
#endif

#endif
