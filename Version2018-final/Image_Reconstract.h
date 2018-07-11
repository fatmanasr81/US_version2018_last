#pragma once
#define FILTER_LENGTH 3
#define N_SMPL 11688
#define N_LINE 256
#define N_SAMPLES 512
#define ROW_PIXEL 512
#define COL_PIXEL 512
#define TOTAL_PIXEL 512*512
#define IMSIZE 131072
#define FIR_FILTER_LENGTH 9
#define FIR_FILTER_LENGTH_D2 (FIR_FILTER_LENGTH/2)
#define FILESIZE 2992128
#define IMAGEHEIGHT 512
#define IMAGEWIDTH 512
#define CHANNELCOUNT 3
typedef unsigned char BYTE ;
struct PixelDef
{
	long offset;
	unsigned short line_index;
	unsigned short sample_index;
	float c1,c2,c3,c4;
};	
class Image_Reconstract
{
	public:
	float *RF_Data;
	int cc;
	int counter;
	float *output;
	float *final_output;
	float *fir_out;
	float *det_out;
	float *x_out;
	float *under_sampling_out;
	float *dynamic_out;
	float *newscale;
	PixelDef *ptrpixel;
	BYTE *final_image;
	BYTE *pixels;
	float averae;

	// Pointer to our buffered data
		Image_Reconstract(void);
		~Image_Reconstract(void);
	float read_bin_file();
	void SetFilterType(int type=0);
	float filtering();
	float Detection();
	float Under_sampling(int undertype=0);
	float dynamic(int dBrange);
	PixelDef*scan_converter(short int R,float  pitch,float depth,float lens,int nSamples, int nLines);
	BYTE interpolation();
	float b[FILTER_LENGTH];
	float a[FILTER_LENGTH];
	float g; 
	float b1[FILTER_LENGTH];
	float a1[FILTER_LENGTH];
	float g1;
};