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
#define TOTAL_SIZE 11968512


typedef unsigned char BYTE;
struct PixelDefine
{
	long offset;
	unsigned short line_index;
	unsigned short sample_index;
	float c1, c2, c3, c4;
};

class image_reconstract_GPU
{
public:
	
	float *RF_Data;
	int cc;
	int counter[1];
	float *output;
	float *final_output;
	float *fir_out;
	float *detout;
	float *x_out;
	float *under_sampling_out;
	float *dynamic_out;
	float *newscale;
	PixelDefine *ptrpixel;
	BYTE *final_image;
	BYTE *pixels;
	float averae;

	PixelDefine *pixel = 0;
	float *data = 0;
	float *filth_out = 0;
	float *filtl_out = 0;
	
	float*ph = 0;
	float*z = 0;
float *det_out = 0;
	float *lpf = 0;
	float *sampling_out = 0;
	float *max = 0;
	//int *dB_p = &dB;
	int *dB_v = 0;

	float *Dynamic = 0;

	float*new_scale = 0;
	PixelDefine *s = 0;
	float *ptr = 0;
	float * NS = 0;
	BYTE *FI = 0;
	int counter1;
	// Pointer to our buffered data
	image_reconstract_GPU(void);
	~image_reconstract_GPU(void);
	void read_bin_file_GPU();
//	void SetFilterType(int type = 0);
	void filtering_GPU(int type = 0);

	void Detection_GPU();
	void Under_sampling_GPU(int undertype = 0);

	void dynamic_GPU(int dBrange);
	PixelDefine* scan_converter_GPU(short int R, float  pitch, float depth, float lens, int nSamples, int nLines);
	void interpolation_GPU();

	//friend  void interpolationTest(PixelDefine *pixel, float *new_scale, PixelDefine *s, BYTE *FI);

	void performence_GPU(float time[]);

	friend void render();
	friend void display();

	float b[FILTER_LENGTH];
	float a[FILTER_LENGTH];
	float g;
	float b1[FILTER_LENGTH];
	float a1[FILTER_LENGTH];
	float g1;
};

