
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include<iostream>
//#include "image_reconstract_GPU.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>


using namespace std;
typedef unsigned char BYTE;
//BW
#define FILTER_LENGTH 3
#define no_of_threads 256
#define no_of_blocks 1
#define FILESIZE 2992128
#define N_SMPL 11688
#define FIR_FILTER_LENGTH 9
#define FIR_FILTER_LENGTH_D2 (FIR_FILTER_LENGTH/2)
#define COL_PIXEL 512
/*#define  R  61
#define   pitch  0.2
#define  depth  180
#define  lens  0.7
#define  nLines  256
#define  nSamples  512*/

#define counter 173621
#define no_of_blocks2 170

#define CHANNELCOUNT 3
#define TOTAL_SIZE 11968512
#define TOTAL_PIXEL 512*512
#define IMSIZE 131072

struct PixelDefine
{
	long offset;
	unsigned short line_index;
	unsigned short sample_index;
	float c1, c2, c3, c4;
};

//low
__global__ void bw_lowpass_filterKernel(float *in, float *output)
{
	float b1[FILTER_LENGTH];
	float a1[FILTER_LENGTH];
	b1[0] = 1.0000;
	b1[1] = 2.0000;
	b1[2] = 1.0000;
	a1[0] = 1.0000;
	a1[1] = -1.2246515810130951;
	a1[2] = 0.45044543005604082;
	float g1 = 0.056448462260736451;
	float d1[3] = { 0,0,0 };

	int j = threadIdx.y;

	
	for (int i = 0; i <N_SMPL; i++)

	{
		d1[2] = d1[1];
		d1[1] = d1[0];
		d1[0] = in[i + j*N_SMPL] - a1[1] * d1[1] - a1[2] * d1[2];
		output[i + j*N_SMPL] = g1*b1[0] * d1[0] + g1*b1[1] * d1[1] + g1*b1[2] * d1[2];

	
	}
	

}

__global__ void bw_highpass_filterKernel(float *in, float *output)
{
	float b[FILTER_LENGTH];
	float a[FILTER_LENGTH];
	b[0] = 1.0000;
	b[1] = -2.0000;
	b[2] = 1.0000;
	a[0] = 1.0000;
	a[1] = -1.5610180758007182;
	a[2] = 0.64135153805756306;
	float g = 0.75502196198037386;
	float d[3] = { 0,0,0 };
	int j = threadIdx.y;

	//for ( j = 0; j <  no_of_threads; j++)
	//{
	for (int i = 0; i <N_SMPL; i++)
		//d1[3] = d1[2];
	{
		d[2] = d[1];
		d[1] = d[0];
		d[0] = in[i + j*N_SMPL] - a[1] * d[1] - a[2] * d[2];
		output[i + j*N_SMPL] = g*b[0] * d[0] + g*b[1] * d[1] + g*b[2] * d[2];
	}
	//}


}

__global__ void ellaptic_lowpass_filterKernel(float *in, float *output)
{
	float b1[FILTER_LENGTH];
	float a1[FILTER_LENGTH];
	b1[0] = 1.0000;
	b1[1] = 1.9951831916798526;
	b1[2] = 1.0000;
	a1[0] = 1.0000;
	a1[1] = -1.2846437324109372;
	a1[2] = 0.54828472668060735;
float	g1 = 0.058813394079824187;
	float d1[3] = { 0,0,0 };

	int j = threadIdx.y;


	for (int i = 0; i <N_SMPL; i++)

	{
		d1[2] = d1[1];
		d1[1] = d1[0];
		d1[0] = in[i + j*N_SMPL] - a1[1] * d1[1] - a1[2] * d1[2];
		output[i + j*N_SMPL] = g1*b1[0] * d1[0] + g1*b1[1] * d1[1] + g1*b1[2] * d1[2];


	}


}

__global__ void ellaptic_highpass_filterKernel(float *in, float *output)
{
	float b[FILTER_LENGTH];
	float a[FILTER_LENGTH];
	b[0] = 1.0000;
	b[1] = -1.9999897887443692;
	b[2] = 1.0000;
	a[0] = 1.0000;
	a[1] = -1.2846437324109372;
	a[2] = 0.73284143556340198;
float	g = 0.80059240346457028;
	
	float d[3] = { 0,0,0 };
	int j = threadIdx.y;

	//for ( j = 0; j <  no_of_threads; j++)
	//{
	for (int i = 0; i <N_SMPL; i++)
		//d1[3] = d1[2];
	{
		d[2] = d[1];
		d[1] = d[0];
		d[0] = in[i + j*N_SMPL] - a[1] * d[1] - a[2] * d[2];
		output[i + j*N_SMPL] = g*b[0] * d[0] + g*b[1] * d[1] + g*b[2] * d[2];
	}
	//}


}
__global__ void chebychev_lowpass_filterKernel(float *in, float *output)
{
	float b1[FILTER_LENGTH];
	float a1[FILTER_LENGTH];
	b1[0] = 1.0000;
	b1[1] = 2;
	b1[2] = 1.0000;
	a1[0] = 1.0000;
	a1[1] = -1.2846384667454742;
	a1[2] = 0.5482635908391793;
	float g1 = 0.065906281023426286;

	float d1[3] = { 0,0,0 };

	int j = threadIdx.y;


	for (int i = 0; i <N_SMPL; i++)

	{
		d1[2] = d1[1];
		d1[1] = d1[0];
		d1[0] = in[i + j*N_SMPL] - a1[1] * d1[1] - a1[2] * d1[2];
		output[i + j*N_SMPL] = g1*b1[0] * d1[0] + g1*b1[1] * d1[1] + g1*b1[2] * d1[2];


	}


}

__global__ void chebychev_highpass_filterKernel(float *in, float *output)
{
	float b[FILTER_LENGTH];
	float a[FILTER_LENGTH];
	b[0] = 1.0000;
	b[1] = -2;
	b[2] = 1.0000;
	a[0] = 1.0000;
	a[1] = -1.6557169643711138;
	a[2] = 0.73281693212820698;
	float g = 0.8471334741248302;

	float d[3] = { 0,0,0 };
	int j = threadIdx.y;

	//for ( j = 0; j <  no_of_threads; j++)
	//{
	for (int i = 0; i <N_SMPL; i++)
		//d1[3] = d1[2];
	{
		d[2] = d[1];
		d[1] = d[0];
		d[0] = in[i + j*N_SMPL] - a[1] * d[1] - a[2] * d[2];
		output[i + j*N_SMPL] = g*b[0] * d[0] + g*b[1] * d[1] + g*b[2] * d[2];
	}
	//}


}
__global__ void detection_Kernel(float *final_output,float *x_out, float* z, float *final_output_PhasShift)
{
	
	static float b1[FIR_FILTER_LENGTH] = { 0 , -0.3706 , 0 , -0.6386 , 0 , 0.6386 , 0 , 0.3706 , 0 };
	*final_output_PhasShift = *final_output + FIR_FILTER_LENGTH_D2;
	int j = threadIdx.y;
	

	for (int i = 0; i < N_SMPL; i++)
		if (i < N_SMPL - 9)
		{
			{
			

				z = final_output + i + j* N_SMPL;
				float acc = 0;
				{
					acc = acc + b1[7] * z[0];
				}
				{
					acc = acc + b1[5] * z[2];
				}
				{
					acc = acc + b1[3] * z[4];
				}
				{
					acc = acc + b1[1] * z[6];
				}
				 float q = final_output_PhasShift[i + j* N_SMPL];
				x_out[i + j* N_SMPL] = sqrt((acc*acc) + q*q);

			}
		}
/*float g2 = 0.0036216815149286421;
 float b0 = 1, b3 = 2, b2 = 1;
	 float a0 = 1, a1 = -1.8226949251963083, a2 = 0.83718165125602262;
	 float a = 0, b = 0, c = 0, d;


	for (int i = 0; i <N_SMPL; i++)
		if (i < N_SMPL - 9)
		{
			{
				d = c;
				c = b;
				b = a;
				a = x_out[i + j*N_SMPL] - a1*b - a2*c;

				det_out[i + j*N_SMPL] = g2*b0*a + g2*b3*b + g2*b2*c;

			}
		}*/

}

__global__ void lpf_Kernel( float *x_out,float *det_out)
{
	int j = threadIdx.y;
	float g2 = 0.0036216815149286421;
	register float b0 = 1, b3 = 2, b2 = 1;
	register float a0 = 1, a1 = -1.8226949251963083, a2 = 0.83718165125602262;
	register float a = 0, b = 0, c = 0, d;


	for (int i = 0; i <N_SMPL; i++)
		//if (i < N_SMPL - 9)
		//{
	{
		d = c;
		c = b;
		b = a;
		a = x_out[i + j*N_SMPL] - a1*b - a2*c;

		det_out[i + j*N_SMPL] = g2*b0*a + g2*b3*b + g2*b2*c;

		//}
	}
}
__global__ void av_sampling_Kernel( float *det_out, float *samp_out)
{
	//int x = threadIdx.y;
	int i = threadIdx.y;
	int w = (i)*COL_PIXEL;
	float L = 0;
	float sum = 0;
	int j = 1;
	float 	average = 0;
	float window_size = (float)N_SMPL / (float)COL_PIXEL;
	for (int k = 0; k < N_SMPL; k++)
	{
		 float NewL = floor((k / window_size) + 1);
		 float dto = det_out[i*N_SMPL + k];
		sum = sum + dto;
		if (L == NewL)
		{
			j = j + 1;
			continue;
		}
		else
		{
			average = sum / j;
			samp_out[w] = average;
		//	samp_out[w + i*N_SMPL]= 255;
			w += 1;
			j = 0;
			sum = 0;
		}
		L = NewL;
	}
}
__global__ void peak_sampling_Kernel( float *det_out, float *psamp_out)
{
	/*int nt = omp_get_max_threads();
	//omp_set_num_threads(nt - 0);
	int undertype;
	/*if (undertype == 0)		                          //PEAK DETECT
	{*/
	float window_size = (float)N_SMPL / (float)COL_PIXEL;

	int i = threadIdx.y;
	float max = 0;
	float L = 0;
	int w = (i)*COL_PIXEL;
	for (int k = 0; k < N_SMPL; k++)
	{
		float NewL = floor((k / window_size) + 1);
		float dto = det_out[i*N_SMPL + k];
		if (max < dto)	max = dto;
		if (L != NewL)
		{
			psamp_out[w] = max;
			w += 1;
			max = 0;
		}
		L = NewL;
	}
}
__global__ void sampling_Kernel( float *det_out, float *ssamp_out)    //Sample
{

	float window_size = (float)N_SMPL / (float)COL_PIXEL;

	int i = threadIdx.y;
	float L = 0;
	int w = i*COL_PIXEL;
	for (int k = 0; k < N_SMPL; k++)
	{
		float NewL = floor((k / window_size) + 1);
		float dto = det_out[i*N_SMPL + k];
		if (L != NewL)
		{
			ssamp_out[w] = dto;
			w += 1;
		}
		L = NewL;
	}
}


__global__ void dynamic_Kernel(float *newscale, float *under_sampling_out, int *dB,float *dynamic_out,float *maxthreads)
{
	int dBrange[1] = {0};
	*dBrange = *dB;
	
	int dBmax = 150;
	float thresh = pow((float)10, (float)(dBmax - dBrange[0]) / 20);
	float ymax = 0;
	float maximum_y = 0;
	float max = 0;

	int j = threadIdx.y;
/*	if (dBrange >= 150 || dBrange <= 0)
	{
		//this command could works in consol app only cout<<"ERROR dBrange must be between 0 and 150"<<endl;
		AfxMessageBox(_T("ERROR dBrange must be between 0 and 150"));
		dBrange = 60;
	}*/
	for (int i = 0; i < 512; i++)
	{
		dynamic_out[i+j*512] = abs(under_sampling_out[i+j*512]);

		/*if (maximum_y < dynamic_out[i])
		{
			maximum_y = dynamic_out[i];
		}*/

		if (maxthreads[j] < dynamic_out[i + j * 512])
		{
			maxthreads[j] = dynamic_out[i + j * 512];
		}
	}

	for(int i=0;i<256;i++)
	{
		if (maximum_y < maxthreads[i])
		{
			maximum_y = maxthreads[i];
		}
	}

	float x = pow((float)10, (float) 7.5);
	ymax = maximum_y / x;

	int min1 = dBmax - dBrange[0];

	int newmax = 255;
	int newmin = 0;
	int oldrange = dBmax - min1;
	int newrange = newmax - newmin;

for (int i = 0; i < 512; i++)
	{

		dynamic_out[i + j * 512] = dynamic_out[i+j*512] / ymax;
		if (dynamic_out[i+j*512] > thresh)
			dynamic_out[i+j*512] = dynamic_out[i+j * 512];
		else
			dynamic_out[i+ j * 512] = thresh;

	//	newscale[i + j * 512] = dynamic_out[i + j * 512];
		dynamic_out[i+ j * 512] = 20 * log10(dynamic_out[i + j * 512]);
		newscale[i+ j * 512] = (((dynamic_out[i + j * 512] - min1) * newrange) / oldrange) + newmin;
	}
}




__global__ void interpolation_kernal(PixelDefine *ptrpixel,float *newscale, PixelDefine *s,float *ptr, BYTE *final_image)
{
	//int	i = threadIdx.y + blockIdx.y *512+ blockIdx.x*512;
	int	i = threadIdx.y;
	for (int j = 0; j <679; j++)
//for (int j = 0; j <1024; j++)
	{
		if (i+j*256< counter)
		{
			s = ptrpixel + ptrpixel[i + j * 256].offset;
			unsigned short Sindx = s->sample_index;
			unsigned short Lindx = s->line_index;
			//memset(newscale, 255, 131072 * sizeof(float));

			ptr = newscale;

			register float Q11 = *(ptr + (Sindx + (Lindx * 512)));
			register float Q12 = *(ptr + (Sindx + 1 + (Lindx * 512)));
			register float Q21 =  *(ptr + (Sindx + ((Lindx + 1) * 512)));
			register float Q22 =  *(ptr + (Sindx + 1 + ((Lindx + 1) * 512)));
			final_image[ptrpixel[i + j *256].offset] = s->c1 * Q11 + s->c2 *Q21 + s->c3 *Q12 + s->c4 *Q22;
			//final_image[ 0 ] =255;
			
		}
	
	}

}


extern "C" void
bw_filteringTest(float *data, float *filth_out ,float *filtl_out)
{
	dim3 dimBlock(1, no_of_threads);
	dim3 dimGrid(1, no_of_blocks);


	bw_highpass_filterKernel << < dimGrid, dimBlock >> >(data, filth_out);

	bw_lowpass_filterKernel << < dimGrid, dimBlock >> >(filth_out, filtl_out);
	
}


extern "C" void
ellaptic_filteringTest(float *data, float *filth_out, float *filtl_out)
{
	dim3 dimBlock(1, no_of_threads);
	dim3 dimGrid(1, no_of_blocks);



	ellaptic_highpass_filterKernel << < dimGrid, dimBlock >> >(data, filth_out);

	ellaptic_lowpass_filterKernel << < dimGrid, dimBlock >> >(filth_out, filtl_out);
	
}

extern "C" void
chebychev_filteringTest(float *data, float *filth_out, float *filtl_out)
{
	dim3 dimBlock(1, no_of_threads);
	dim3 dimGrid(1, no_of_blocks);


	chebychev_highpass_filterKernel << < dimGrid, dimBlock >> >(data, filth_out);

	chebychev_lowpass_filterKernel << < dimGrid, dimBlock >> >(filth_out, filtl_out);
	

}

extern "C" void
detectionTest(float *filtl_out, float *det_out, float * ph, float *z, float *lpf)
{
	dim3 dimBlock(1, no_of_threads);
	dim3 dimGrid(1, no_of_blocks);

	detection_Kernel << < dimGrid, dimBlock >> >(filtl_out, det_out, ph, z);
	lpf_Kernel << < dimGrid, dimBlock >> >(det_out, lpf);

}

extern "C" void
avsamplingTest( float *lpf, float *sampling_out)
{
	dim3 dimBlock(1, no_of_threads);
    dim3 dimGrid(1, no_of_blocks);


	av_sampling_Kernel << < dimGrid, dimBlock >> > (lpf, sampling_out);


}

extern "C" void
peaksamplingTest(float *lpf, float *sampling_out)
{
	dim3 dimBlock(1, no_of_threads);
	dim3 dimGrid(1, no_of_blocks);

	peak_sampling_Kernel << < dimGrid, dimBlock >> > (lpf, sampling_out);


}


extern "C" void
samplingTest(float *lpf, float *sampling_out)
{
	dim3 dimBlock(1, no_of_threads);
	dim3 dimGrid(1, no_of_blocks);


	sampling_Kernel << < dimGrid, dimBlock >> > (lpf, sampling_out);

}



extern "C" void
dynamicTest(float *new_scale,float *sampling_out,int *dB_v, float *dynamic,float *max)
{
	dim3 dimBlock(1, no_of_threads);
	dim3 dimGrid(1, no_of_blocks);

	dynamic_Kernel << < dimGrid, dimBlock >> > (new_scale, sampling_out, dB_v, dynamic, max);
	
}


extern "C" void
interpolationTest(PixelDefine *pixel,float *new_scale, PixelDefine *s,float *ptr, BYTE *FI)
{
	dim3 dimBlock(1, no_of_threads);
	dim3 dimGrid(1, no_of_blocks);

	
	interpolation_kernal << < dimGrid, dimBlock >> > (pixel, new_scale, s, ptr, FI);
	


}

/*extern "C" void
interpolationTest(PixelDefine *ptrpixel, BYTE *FINAL)
{
	dim3 dimBlock(1, 1);
	dim3 dimGrid(1, no_of_blocks);

	float*new_scale = 0;
	PixelDefine *s = 0;
	float *ptr = 0;
	//float * NS = 0;
	BYTE *FI = 0;
	PixelDefine *pixel = 0;
	
	cudaMalloc(&pixel, TOTAL_PIXEL * sizeof(PixelDefine));
	cudaMalloc(&FI, TOTAL_PIXEL * CHANNELCOUNT * sizeof(BYTE));
	cudaMalloc(&ptr, IMSIZE * sizeof(float));
	cudaMalloc(&s, TOTAL_PIXEL * sizeof(PixelDefine));
	cudaMemcpy(pixel, ptrpixel, 512 * 512 * sizeof(PixelDefine), cudaMemcpyHostToDevice);
	cudaMalloc(&new_scale, IMSIZE * sizeof(float));

	


	interpolation_kernal <<< dimGrid, dimBlock >>> (pixel, new_scale, s, ptr, FI);
	//float *newscale = 0;
	//cudaMalloc(&newscale, 131072 * sizeof(float));
	//memset(new_scale, 255, 131072 * sizeof(float));
	cudaMemcpy(FINAL , FI, 512 * 512 * 3 * sizeof(BYTE), cudaMemcpyDeviceToHost);
	
	int k = 0;


}*/
