#ifndef _RGBIR_CALI_H_
#define _RGBIR_CALI_H_

#include <sstream>
#include <vector>
using namespace std;

#define RGBIR_CALIBRATION_DLL_EXPORTS

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef	RGBIR_CALIBRATION_DLL_EXPORTS
#define RGBIR_CALIBRATION_API __declspec(dllexport)
#else
#define RGBIR_CALIBRATION_API __declspec(dllimport)
#endif

	// RmIR 
	typedef int(*FNUC_RGBIR_CALC)(vector< vector<int*> > raw_roi_list, int roi_width, int roi_height, int num_of_roi, int num_of_images, int bayer_id,
	 	unsigned int blc_offset_b, unsigned int blc_offset_gb, unsigned int blc_offset_gr, unsigned int blc_offset_r,
		float& CaliSlopeB, float& CaliSlopeG, float& CaliSlopeR
	 	);
	RGBIR_CALIBRATION_API int cali_rgbir_ir_remove(vector< vector<int*> > &raw_roi_list, int roi_width, int roi_height, int num_of_roi, int num_of_images, int bayer_id,
	 	unsigned int blc_offset_b, unsigned int blc_offset_gb, unsigned int blc_offset_gr, unsigned int blc_offset_r,
	 	float& CaliSlopeB, float& CaliSlopeG, float& CaliSlopeR
	 );

	typedef int* (*FNUC_RGBIR2RGGB_CALC)(int** RAW, int IMAGE_W, int IMAGE_H, int bayer_id, float CaliSlopeB, float CaliSlopeG, float CaliSlopeR);
	RGBIR_CALIBRATION_API int* cali_rgbir_rgbir2rggb(int** RAW, int IMAGE_W, int IMAGE_H, int bayer_id, float CaliSlopeB, float CaliSlopeG, float CaliSlopeR);

	

#ifdef __cplusplus
}
#endif

#endif