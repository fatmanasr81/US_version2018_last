//#include "stdafx.h"
#include "image_reconstract_GPU.h"
//#include "afxdialogex.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <omp.h>


#include "cuda_runtime.h"
#include "device_launch_parameters.h"

extern "C" void
bw_filteringTest(float *data, float *filth_out, float *filtl_out);
extern "C" void
ellaptic_filteringTest(float *data, float *filth_out, float *filtl_out);
extern "C" void
chebychev_filteringTest(float *data, float *filth_out, float *filtl_out);
extern "C" void
least_filteringTest(float *data, float *filth_out, float *filtl_out);
extern "C" void
constra_filteringTest(float *data, float *filth_out, float *filtl_out);
extern "C" void
detectionTest(float *filtl_out, float *det_out, float * ph, float *z, float *lpf);
extern "C" void
peaksamplingTest(float *lpf, float *sampling_out);
extern "C" void
avsamplingTest(float *lpf, float *sampling_out);
extern "C" void
samplingTest(float *lpf, float *sampling_out);
extern "C" void
dynamicTest(float *new_scale, float *sampling_out, int *dB_v, float *dynamic, float *max);
extern "C" void
interpolationTest(PixelDefine *pixel, float *new_scale, PixelDefine *s, BYTE *FI);

extern "C" float
filtering_performance(float *data, float *filth_out, float *filtl_out);
extern "C" float
detection_performance(float *filtl_out, float *det_out, float * ph, float *z, float *lpf);
extern "C" float
undersampling_performance(float *lpf, float *sampling_out);
extern "C" float
interpolation_performance(PixelDefine *pixel, float *new_scale, PixelDefine *s, BYTE *FI);
extern "C" float
dynamic_performance(float *new_scale, float *sampling_out, int *dB_v, float *dynamic, float *max);

//extern "C" void
//interpolationTest(PixelDefine *ptrpixel, BYTE *FINAL);

image_reconstract_GPU::image_reconstract_GPU()
{
	//dim3 dimBlock(1, no_of_threads);
	//dim3 dimGrid(1, 1);

	cudaMalloc(&data, TOTAL_SIZE);
	cudaMalloc(&filth_out, TOTAL_SIZE);
	cudaMalloc(&filtl_out, TOTAL_SIZE);
	
	cudaMalloc(&ph,  TOTAL_SIZE);
	cudaMalloc(&z, TOTAL_SIZE);
	cudaMalloc(&det_out, TOTAL_SIZE);
	cudaMalloc(&lpf, TOTAL_SIZE);
	cudaMalloc(&sampling_out, IMSIZE * sizeof(float));
	cudaMalloc(&max, N_LINE * sizeof(float));
	cudaMalloc(&dB_v, sizeof(int));
	cudaMalloc(&Dynamic, IMSIZE * sizeof(float));
	cudaMalloc(&new_scale, IMSIZE * sizeof(float));
	cudaMalloc(&pixel, TOTAL_PIXEL * sizeof(PixelDefine));
	cudaMalloc(&FI, TOTAL_PIXEL * CHANNELCOUNT * sizeof(BYTE));
	cudaMalloc(&ptr, IMSIZE * sizeof(float));
	cudaMalloc(&s, TOTAL_PIXEL * sizeof(PixelDefine));

	//int *dB_p = new(int[1]);
	//dB_p[0] = 60;
	//cudaMemcpy(dB_v, dB_p, 2, cudaMemcpyHostToDevice);


	RF_Data = new float[FILESIZE];
	ptrpixel = new PixelDefine[ROW_PIXEL * COL_PIXEL];
	final_image = new BYTE[IMAGEHEIGHT*IMAGEWIDTH*CHANNELCOUNT];
	
	pixels = new BYTE[IMAGEWIDTH*IMAGEHEIGHT*CHANNELCOUNT];
	newscale = new float[IMSIZE];
	under_sampling_out = new float[IMSIZE];
	detout = new float[FILESIZE];
	final_output = new float[FILESIZE];


	counter1 = 0;
	

}
image_reconstract_GPU::~image_reconstract_GPU(void)
{
	cudaFree(data);
	cudaFree(filth_out);
	cudaFree(filtl_out);


	cudaFree(ph);
	cudaFree(z);
	cudaFree(det_out);
	cudaFree(lpf);
	cudaFree(sampling_out);

	cudaFree(max);
	cudaFree(dB_v);
	cudaFree(Dynamic);
	cudaFree(new_scale);

	cudaFree(pixel);
	cudaFree(FI);
	cudaFree(ptr);
	cudaFree(s);

	delete[]RF_Data; // delete the array we allocated earlier
	//delete[]output;
	delete[]final_output;
	delete[]detout;
	//delete[]fir_out;
	//delete[]x_out;
	delete[]under_sampling_out;
	//delete[]dynamic_out;
	delete[]ptrpixel;
	delete[]newscale;
	delete[]final_image;
	delete[]pixels;

}

void image_reconstract_GPU::read_bin_file_GPU()
{
	FILE *file = NULL;      // File pointer
	const char *filePath = "E:\\project\\RF_4.5MHzCystAllFiltersOff";
	file = fopen(filePath, "rb");
	fread(RF_Data, 4, FILESIZE, file);
	fclose(file);

	//memset(RF_Data,255, TOTAL_SIZE);

	cudaMemcpy(data, RF_Data, TOTAL_SIZE, cudaMemcpyHostToDevice);
	//return *RF_Data;
}

void  image_reconstract_GPU::filtering_GPU(int type)//40
{
	if (type == 0)   //BW
	{
		
		bw_filteringTest(data,filth_out, filtl_out);
	}

	if (type == 1)   //ellaptic
	{
		
			ellaptic_filteringTest(data, filth_out, filtl_out);
	}
	if (type == 2)   //chebychev
	{
	
			chebychev_filteringTest(data, filth_out,filtl_out);
	}
	if (type == 3)   //least
	{

		least_filteringTest(data, filth_out, filtl_out);
	}
	//cudaMemcpy(final_output, filtl_out, TOTAL_SIZE, cudaMemcpyDeviceToHost);
	if (type == 4)   //constra
	{

		constra_filteringTest(data, filth_out, filtl_out);
	}
	//return *final_output;
}

void image_reconstract_GPU::Detection_GPU()
{
		detectionTest(filtl_out,det_out,  ph, z, lpf);
		//detectionTest(data, det_out, ph, z, lpf);


		//return *detout;
		
}

void  image_reconstract_GPU::Under_sampling_GPU(int undertype)
{
	if (undertype == 0)		//average DETECT
	{
		
		peaksamplingTest(lpf, sampling_out);

	}
	if (undertype == 1)		//PEAK DETECT
	{
		
			samplingTest(lpf, sampling_out);
	}
	if (undertype == 2)		//sample DETECT
	{
	
			avsamplingTest(lpf, sampling_out);

	}
	//cudaMemcpy(under_sampling_out , sampling_out, IMSIZE * sizeof(float), cudaMemcpyDeviceToHost);

	//return *under_sampling_out;
}

void image_reconstract_GPU::dynamic_GPU(int dBrange)
{
	int *dB_p = &dBrange;
	cudaMemcpy(dB_v, dB_p, 2, cudaMemcpyHostToDevice);
	//memset(newscale, 255, IMSIZE * sizeof(float));

	//cudaMemcpy(new_scale, newscale,  IMSIZE * sizeof(float), cudaMemcpyHostToDevice);

		dynamicTest(new_scale, sampling_out, dB_v, Dynamic, max);

		//return *newscale;
}


PixelDefine* image_reconstract_GPU::scan_converter_GPU(short int R, float  pitch, float depth, float lens, int nSamples, int nLines)
	{

		float dAngle = pitch / R;

		float halfAngle = ((nLines - 1)*dAngle) / 2;
		float d1 = R*cos(halfAngle);
		float d2 = R - d1;
		float dr = depth / nSamples;
		float* midSamples;
		float* angleVector;
		float* Lin_n_x;
		float* Lin_n_y;
		float *STheta;
		float *SRadius;
		float *y_pixel;
		float *x_pixel;
		float *ImRadius;
		float *ImTheta;
		float *ypixel_step;
		float *xpixel_step;
		midSamples = new float[nSamples];
		angleVector = new float[nLines];
		Lin_n_x = new float[nSamples*nLines];
		Lin_n_y = new float[nSamples*nLines];
		STheta = new float[nSamples*nLines];
		SRadius = new float[nSamples*nLines];
		y_pixel = new float[nSamples*nSamples];
		x_pixel = new float[nSamples*nSamples];
		ImRadius = new float[nSamples*nSamples];
		ImTheta = new float[nSamples*nSamples];
		ypixel_step = new float[nSamples];
		xpixel_step = new float[nSamples];
		for (int i = 0; i<nSamples; i++)
			midSamples[i] = R + lens + dr + i*dr;
		for (int i = 0; i<nLines; i++)
			angleVector[i] = -halfAngle + i*dAngle;
		int x = 0;
		for (int i = 0; i <nLines; i++)
		{
			for (int j = 0; j <nSamples; j++)
			{
				Lin_n_x[x] = midSamples[j] * sin(angleVector[i]);
				Lin_n_y[x] = midSamples[j] * cos(angleVector[i]);
				x++;
			}
		}
		int y = 0;
		for (int i = 0; i <nLines; i++)
			for (int j = 0; j <nSamples; j++)
			{
				STheta[y] = -halfAngle - dAngle + (1 + i) * dAngle;
				SRadius[y] = R + lens + (1 + j) * dr;
				y++;
			}
		float npixel = 512;
		float pixel_mm = (depth + d2) / npixel;

		for (int i = 0; i<nSamples; i++)
		{
			ypixel_step[i] = d1 + lens + pixel_mm + i*pixel_mm;
			xpixel_step[i] = (-(nSamples / 2)*pixel_mm) + i*pixel_mm;
		}
		int z = 0;
		for (int i = 0; i<nSamples; i++)
			for (int j = 0; j<nSamples; j++)
			{
				y_pixel[z] = ypixel_step[j];
				x_pixel[z] = xpixel_step[i];
				ImRadius[z] = sqrt((x_pixel[z] * x_pixel[z]) + (y_pixel[z] * y_pixel[z]));
				ImTheta[z] = atan(x_pixel[z] / y_pixel[z]);
				z++;
			}
		counter1 = 0;
	    int cc = 0;
		for (int i = 0; i<nSamples*nSamples; i++)
		{
			if (ImTheta[i] < -0.418032786885246 || ImTheta[i] > 0.418032786885246 || ImRadius[i] < 62.051562500000000 || ImRadius[i] > 2.417000000000000e+02)
			{
				ptrpixel[i].line_index = 0;
				ptrpixel[i].sample_index = 0;
				ptrpixel[i].c1 = 0;
				ptrpixel[i].c2 = 0;
				ptrpixel[i].c3 = 0;
				ptrpixel[i].c4 = 0;
			}
			else if (ptrpixel[i].line_index == 255 || ptrpixel[i].sample_index == 511)
			{
				ptrpixel[i].c1 = 0;
				ptrpixel[i].c2 = 0;
				ptrpixel[i].c3 = 0;
				ptrpixel[i].c4 = 0;
			}
			else
			{
				ptrpixel[i].line_index = (ImTheta[i] + halfAngle + dAngle) / dAngle;
				ptrpixel[i].sample_index = (ImRadius[i] - lens - R) / dr;
				float x1 = 0, x2 = 0, x = 0, y1 = 0, y2 = 0, y = 0, dist1 = 0, dist2 = 0, dist3 = 0, dist4 = 0, dist_tot = 0;
				x1 = Lin_n_x[ptrpixel[i].sample_index + (ptrpixel[i].line_index * 512)];
				x2 = Lin_n_x[ptrpixel[i].sample_index + ((ptrpixel[i].line_index + 1) * 512)];
				x = x_pixel[i];
				y1 = Lin_n_y[ptrpixel[i].sample_index + (ptrpixel[i].line_index * 512)];
				y2 = Lin_n_y[(ptrpixel[i].sample_index + 1) + (ptrpixel[i].line_index * 512)];
				y = y_pixel[i];
				dist1 = sqrt(((x - x1) * (x - x1)) + ((y - y1)*(y - y1)));
				dist2 = sqrt(((x - x2) * (x - x2)) + ((y - y1)*(y - y1)));
				dist3 = sqrt(((x - x1) * (x - x1)) + ((y - y2)*(y - y2)));
				dist4 = sqrt(((x - x2) * (x - x2)) + ((y - y2)*(y - y2)));
				dist_tot = dist1 + dist2 + dist3 + dist4;
				ptrpixel[i].c1 = (1 - (dist1 / dist_tot)) / 3;
				ptrpixel[i].c2 = (1 - (dist2 / dist_tot)) / 3;
				ptrpixel[i].c3 = (1 - (dist3 / dist_tot)) / 3;
				ptrpixel[i].c4 = (1 - (dist4 / dist_tot)) / 3;
				counter1++;
				ptrpixel[cc].offset = i;
				cc++;
			}
		}
	//	memset(ptrpixel, 255, 512 * 512 * sizeof(PixelDefine));

		cudaMemcpy(pixel, ptrpixel, 512 * 512 * sizeof(PixelDefine), cudaMemcpyHostToDevice);


		return  ptrpixel;

		delete[]midSamples;
		delete[]angleVector;
		delete[]Lin_n_x;
		delete[]Lin_n_y;
		delete[]STheta;
		delete[]SRadius;
		delete[]y_pixel;
		delete[]x_pixel;
		delete[]ImRadius;
		delete[]ImTheta;
		delete[]xpixel_step;
		delete[]ypixel_step;
		
	}
	void image_reconstract_GPU::interpolation_GPU()
	{
		//interpolation_kernal << < dimGrid, dimBlock >> > (pixel, new_scale, s, ptr, FI);
		
		//interpolationTest(ptrpixel, final_image);
		interpolationTest(pixel, new_scale, s, FI);
		//interpolationTest(ptrpixel, new_scale, s, ptr, FI);
		//static bool temp = 0;
		//if (temp == 0){
			cudaMemcpy(final_image, FI, 512 * 512 * 3 * sizeof(BYTE), cudaMemcpyDeviceToHost);
			//temp = 1;
		//}
			//memset(final_image, 255, 512 * 512 * 3 * sizeof(BYTE));

	
		//return *final_image;
		
	}

	void image_reconstract_GPU::performence_GPU(float time[])
	{
		time[0]=filtering_performance(data, filth_out, filtl_out);
		//time[1]=detection_performance(filtl_out, det_out,  ph, z, lpf);
		//time[2]=undersampling_performance(lpf, sampling_out);
		//time[3]=dynamic_performance(new_scale, sampling_out, dB_v, Dynamic, max);
    //time[4]=interpolation_performance(pixel, new_scale, s, FI);
	
	}

