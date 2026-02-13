#ifndef DEWARP_H
#define DEWARP_H

#define MAX_REGION_NUM 5
#define MESHINFO_DEBUG 1
#define MAX_WINDOW_MESH_NUM	256
#define MAX_FRAME_MESH_NUM	(MAX_WINDOW_MESH_NUM*MAX_REGION_NUM)
#define UI_CTRL_VALUE_CENTER 180	// range from -16 ~ 16,  "16" is the center of value 0 ~32

typedef enum USAGE_MODE {
    MODE_PANORAMA_360 = 1,
    MODE_PANORAMA_180 = 2,
    MODE_01_1O = 3,
    MODE_02_1O4R = 4,
    MODE_03_4R = 5,
    MODE_04_1P2R = 6,
    MODE_05_1P2R = 7,
    MODE_06_1P = 8,
    MODE_07_2P = 9,
}USAGE_MODE;

typedef enum PROJECTION_MODE {
    PROJECTION_PANORAMA_360,
    PROJECTION_PANORAMA_180,
    PROJECTION_REGION
}PROJECTION_MODE;

typedef enum MOUNTMODE {
    CEILING =1,
    FLOOR =2,
    WALL= 3,
    OTHER = 4
}MOUNTMODE;

typedef struct FORMAT_8BITS_RGB {
    unsigned R;
    unsigned G;
    unsigned B;
}FORMAT_8BITS_RGB;


typedef struct COORDINATE2D {
    double xcor;
    double ycor;
}COORDINATE2D;

typedef struct MESH_STRUCT {
    COORDINATE2D knot[4];
    int idx;
}MESH_STRUCT;

typedef struct FISHEYE_ATTR {

    bool Enable;					// dewarp engine on/off
    bool BgEnable;					// given background color enable.

    int RgnNum;						// dewarp Region Number.
    int TotalMeshNum;				// total mesh numbes
    int OutW_disp;					// output display frame width
    int OutH_disp;					// output display frame height
    int PipeID;
    int ChnID;
    double InCenterX;					// input fisheye center position X
    double InCenterY;					// input fisheye center position Y
    double InRadius;					// input fisheye radius in pixels.

    int InWidth;					// input fisheye frame width
    int InHeight;					// input fisheye frame height

    double Hoffset;					// fish-eye image center horizontal offset.
    double VOffset;					// fish-eye image center vertical offset.
    double TCoef;					// KeyStone Corection coefficient.
    double FStrength;				// Fan Correction coefficient.

    USAGE_MODE UsageMode;			// Panorama.360, Panorama.180, Mode1~Mode7. total = 9 for now.
    MOUNTMODE MntMode;				// CEILING/ FLOOR/ WALL
    FORMAT_8BITS_RGB BgColor;		// give background color RGB.

    //MESH_STRUCT DstRgnMeshInfo[16384];	// frame-based dst mesh info.  maximum num=128*28 = 16384 meshes.
    //MESH_STRUCT SrcRgnMeshInfo[16384];	// frame-based src mesh info.

    MESH_STRUCT DstRgnMeshInfo[MAX_FRAME_MESH_NUM];	// frame-based dst mesh info.  maximum num=128*28 = 16384 meshes.
    MESH_STRUCT SrcRgnMeshInfo[MAX_FRAME_MESH_NUM];	// frame-based src mesh info.

}FISHEYE_ATTR;

typedef struct FISHEYE_REGION_ATTR {
    int RgnIndex;		// region index
    float InRadius;		// For Panorama.360 mode, inner radius.
    float OutRadius;	// For Panorama.360 mode, outer radius.
    double Pan;			// Region PTZ-Pan.
    double Tilt;		// Region PTZ-Tilt.
    //int HorZoom;		// adjust horizontal zoom in Region PTZ.
    //int VerZoom;		// adjust vertical zoom in Region PTZ.
    int OutX;			// region initial position x on display frame.
    int OutY;			// region initial position y on display frame.
    int OutW;			// region width.
    int OutH;			// region height.
    int MeshHor;		// mesh counts horizontal
    int MeshVer;		// mesh counts vertical

    // to give region default view center
    double ThetaX;		// User set rotation angle around X-axis, create angle between vector to Z-axis. (start position @ [0,0,1])
    double ThetaY;		// User set rotation angle around Y-axis,
    double ThetaZ;		// User set rotation angle around Z-axis,

    double ZoomH;		// User Set for Region
    double ZoomV;		// User Set for Region
    double PanEnd;		// For Panorama Case Only
    bool RegionValid;	// label valid/ invalid for each region

    MESH_STRUCT DstRgnMeshInfo[MAX_WINDOW_MESH_NUM];	// initial buffer to store destination mesh info. max: 32*32
    MESH_STRUCT SrcRgnMeshInfo[MAX_WINDOW_MESH_NUM];	// initial buffer to store source mesh info
    PROJECTION_MODE ViewMode;		// projection method. ex: Panorama.360, Panorama.180, Region

}FISHEYE_REGION_ATTR;

typedef struct Vector3D {
    double	x;
    double	y;
    double	z;
} Vector3D;

typedef struct Vector2D {
    double	x;
    double	y;
} Vector2D;

typedef struct RotMatrix3D {
    double	coef[4][4];
} RotMatrix3D;


typedef struct REGISTER {
    int out_width;			// mapping rectangular width
    int out_height;			// mapping height
    MOUNTMODE mount_mode;	// refer to ENUM
    USAGE_MODE usage_mode;
    int disp_width;			// display frame widht
    int disp_height;		// display frame height
}REGISTER;

unsigned char* ReadPPM(const char* filename, int* width, int* height);
void INITIAL_CONFIG(FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION, int dst_height, int dst_width, int src_width, int src_height, int x0, int y0, int r);
int RGB888Image2PPM(unsigned char* rgb888, int dimx, int dimy, char* filename);
void Rotate3D(Vector3D* v3d, RotMatrix3D* mat);
void Equidistant(Vector3D* v3d, Vector2D* v2d, double f);
void _RegionView2(FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, MOUNTMODE MOUNT, double x0, double y0, double r);
void _Panorama360View2(FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, MOUNTMODE MOUNT, double x0, double y0, double r);
void _Panorama180View2(FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, MOUNTMODE MOUNT, double x0, double y0, double r);
void _Panorama360View(unsigned char* src_img, unsigned char* rgn_img, unsigned char* src_rois, FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double x0, double y0, double r);
void _Panorama180View(unsigned char* src_img, unsigned char* rgn_img, unsigned char* src_rois, FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double x0, double y0, double r);
void _RegionView(unsigned char* src_img, unsigned char* rgn_img, unsigned char* src_rois, FISHEYE_ATTR* FISHEYE_CONFIG, FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double x0, double y0, double r);
void _api_set_rgn_proj_region(FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double set_Pan, double set_Tilt, double set_ThetaX, double set_ThetaY, double set_ThetaZ, double set_ZoomH, double set_ZoomV);
void _api_set_rgn_proj_panorama180(FISHEYE_REGION_ATTR* FISHEYE_REGION, int rgn_idx, double set_Pan, double set_Tilt, double set_ZoomH, double set_ZoomV);
void _api_set_rgn_proj_panorama360( FISHEYE_REGION_ATTR* FISHEYE_REGION,int rgn_idx, double set_InRadius, double set_OutRadius, double set_Pan, double set_Tilt, double set_ZoomV, double set_PanEnd);

#endif // DEWARP_H
