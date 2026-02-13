// coordinate in S COOR_N.COOR_M
#define DEWARP_COORD_MBITS 13
#define DEWARP_COORD_NBITS 18
#define NUMBER_Y_LINE_A_SUBTILE 4
#define DIV_0_BIT_LSHIFT 8		// default 16
#define DIV_1_BIT_LSHIFT 18
#define INTERPOLATION_PRECISION_BITS 5
#define COEFFICIENT_PHASE_NUM (1 << INTERPOLATION_PRECISION_BITS)
#define INTERPOLATION_COEF_BITS 8		// SN
#define INTERPOLATION_COEF_FBITS 6

#define MESH_ID_FST	0xfffa	// frame start
#define MESH_ID_FSS 0xfffb	// slice start
#define MESH_ID_FSP	0xfffc	// tile start
#define MESH_ID_FTE 0xfffd	// tile end
#define MESH_ID_FSE 0xfffe	// slice end
#define MESH_ID_FED 0xffff	// frame end

//#define FLOATING_C
//#define FLOATING_COEFF

#define CACHE_LINE_SIZE 16	// 16 bytes
#define CACHE_DATA_SIZE 128 // 128 bytes i.e., 16 bytes with 8 continue bursts,
#define CACHE_SET_NUM	16	// 16 sets
#define CACHE_WAY_NUM	4	// 4 ways
#define CACHE_X_ADDR	1	// 4 3 2 1
#define CACHE_Y_ADDR	3	// 0 1 2 3
#define CACHE_REPLACE_FIFO
//#define CACHE_REPLACE_LRU

#define YUV400		2
#define YUV420p		0
#define RGB888p		1

#define B_IDX 0
#define G_IDX 1
#define R_IDX 2

#define MESH_TBL_SRC_X_OFFSET 0
#define MESH_TBL_SRC_Y_OFFSET 1
#define MESH_TBL_DST_X_OFFSET 2
#define MESH_TBL_DST_Y_OFFSET 3

#define HTILE_MODE
