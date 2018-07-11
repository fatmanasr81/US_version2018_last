#include "stdafx.h"
#include "Image_Reconstract.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <omp.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

extern "C" void
filteringTest(float *RF_Data, float *out);

extern "C" void
detectionTest(float *in, float *detout);

extern "C" void
avsamplingTest(float *samp_out, float *det_out);
extern "C" void

runTest4(float *newscale, float *under_sampling_out,int dB);



Image_Reconstract::Image_Reconstract(void)
{
		ptrpixel = new PixelDef[ROW_PIXEL * COL_PIXEL];
		RF_Data=new float[FILESIZE];
		output = new float[FILESIZE];
		final_output = new float[FILESIZE];
		det_out = new float[FILESIZE];
		fir_out = new float [FILESIZE];
		x_out= new float [FILESIZE];
		under_sampling_out=new float[IMSIZE];
		dynamic_out = new float[IMSIZE];
		newscale = new float[IMSIZE];
		final_image = new BYTE[IMAGEHEIGHT*IMAGEWIDTH*CHANNELCOUNT]();
		pixels = new BYTE [IMAGEWIDTH*IMAGEHEIGHT*CHANNELCOUNT]();
		counter = 0;
		b[0] = 1.0000;
		b[1] = -2.0000;
		b[2] = 1.0000;
		a[0] = 1.0000;
		a[1] = -1.5610180758007182;
		a[2] = 0.64135153805756306;
		g = 0.75502196198037386; 
		b1[0] = 1.0000;
		b1[1] = 2.0000;
		b1[2] = 1.0000;
		a1[0] = 1.0000;
		a1[1] = -1.2246515810130951;
		a1[2] = 0.45044543005604082;
		g1 = 0.056448462260736451;
}


Image_Reconstract::~Image_Reconstract(void)
{
		delete[]RF_Data; // delete the array we allocated earlier
		delete[]output;
		delete[]final_output;
		delete[]det_out;
		delete[]fir_out;
		delete[]x_out;
		delete[]under_sampling_out;
		delete[]dynamic_out;
		delete[]ptrpixel;
		delete[]newscale ;
		delete[]final_image;
		delete[]pixels;
}


	   float Image_Reconstract::read_bin_file()
	{

		FILE *file = NULL;      // File pointer
		const char *filePath ="E:\\project\\RF_4.5MHzCystAllFiltersOff"; 
		file = fopen(filePath, "rb");
		fread(RF_Data,4,FILESIZE, file);
		fclose(file);   
		return *RF_Data;
	}
	void Image_Reconstract::SetFilterType(int type)
	{
		if(type == 0)   //BW
		{
			b[0] = 1.0000;
			b[1] = -2.0000;
			b[2] = 1.0000;
			a[0] = 1.0000;
			a[1] = -1.5610180758007182;
			a[2] = 0.64135153805756306;
			g = 0.75502196198037386; 
			b1[0] = 1.0000;
			b1[1] = 2.0000;
			b1[2] = 1.0000;
			a1[0] = 1.0000;
			a1[1] = -1.2246515810130951;
			a1[2] = 0.45044543005604082;
			g1 = 0.056448462260736451;
		}
		if(type == 1)  //Elliptic
		{
			b[0] = 1.0000;
			b[1] = -1.9999897887443692;
			b[2] = 1.0000;
			a[0] = 1.0000;
			a[1] = -1.2846437324109372;
			a[2] = 0.73284143556340198;
			g = 0.80059240346457028; 
			b1[0] = 1.0000;
			b1[1] = 1.9951831916798526;
			b1[2] = 1.0000;
			a1[0] = 1.0000;
			a1[1] = -1.2846437324109372;
			a1[2] = 0.54828472668060735;
			g1 = 0.058813394079824187;
		}
		if(type == 2) // chebychev type 2 
		{
			b[0] = 1.0000;
			b[1] = -2;
			b[2] = 1.0000;
			a[0] = 1.0000;
			a[1] = -1.6557169643711138       ;
			a[2] =  0.73281693212820698      ;
			g = 0.8471334741248302        ; 
			b1[0] = 1.0000;
			b1[1] = 2;
			b1[2] = 1.0000;
			a1[0] = 1.0000;
			a1[1] = -1.2846384667454742       ;
			a1[2] =  0.5482635908391793       ;
			g1 = 0.065906281023426286      ;
	
		}
	}
	float Image_Reconstract::filtering()//40
	{

	//	filteringTest(RF_Data, final_output);
	int nt = omp_get_max_threads();
	omp_set_num_threads(nt-0);
	
		//low
#pragma omp parallel for schedule(static)
	for(int i = 0 ;i<N_LINE;i++)
	{
		register int start = i*N_SMPL;
		register int end = (i+1)*N_SMPL;
		register float d1[3] = {0,0,0};
		for(int s = start;s<end;s+=1)
		{
		//d1[3]=d1[2];
		d1[2]=d1[1];
		d1[1]=d1[0];
		d1[0]= RF_Data[s] - a1[1]*d1[1] - a1[2]*d1[2];
		output[s]= g1*b1[0]*d1[0] + g1*b1[1]*d1[1] + g1*b1[2]*d1[2];
		}
	}
		//high
#pragma omp parallel for schedule(static)
	for(int i = 0 ;i<N_LINE;i+=1)
	{	
		register int start = i*N_SMPL;
		register int end = (i+1)*N_SMPL;
		register float d[3] = {0,0,0};
		for(int s = start;s<end;s+=1){
			//d[3]=d[2];
			d[2]=d[1];
			d[1]=d[0];
			d[0]= output[s] - a[1]*d[1] - a[2]*d[2];
			final_output[s]= g*b[0]*d[0] + g*b[1]*d[1] + g*b[2]*d[2];
		}
	}

		
	
		return *final_output;
	}

	   float Image_Reconstract::Detection()
	{
		int nt = omp_get_max_threads();
		omp_set_num_threads(nt-0);
	static float b1[FIR_FILTER_LENGTH]={0 , -0.3706 , 0 , -0.6386 , 0 , 0.6386 , 0 , 0.3706 , 0} ;
	int p = 0;// FIR_FILTER_LENGTH ;
	float *final_output_PhasShift = final_output+FIR_FILTER_LENGTH_D2;
#pragma omp parallel for firstprivate(b1) schedule(static)
	for(int i = 0 ;i<(N_SMPL*N_LINE-FIR_FILTER_LENGTH) ;i++)
	{
		register float* z;
		z = final_output+i;
		register float acc = 0;
		{
			acc = acc +b1[7]*z[0];
		}
		{
			acc = acc +b1[5]*z[2];
		}
		{
			acc = acc +b1[3]*z[4];
		}
		{
			acc = acc +b1[1]*z[6];
		}
		register float q = final_output_PhasShift[i];
		x_out[i]=sqrt((acc*acc)+q*q);
	}
	

    //low

	float g2 = 0.0036216815149286421 ;
	register float b0 = 1,b3 = 2,b2 = 1;
	register float a0 = 1,a1 = -1.8226949251963083,a2 = 0.83718165125602262;
	#pragma omp parallel for schedule(static)
	for(int i = 0 ;i<N_LINE;i++)
	{
		register int start = i*N_SMPL;
		register int end = (i+1)*N_SMPL;
		register float a=0,b=0,c=0,d;
		for(int s = start;s<end;s+=1)
		{
			d=c;
			c=b;
			b=a;
			a= x_out[s] - a1*b- a2*c;
			det_out[s]= g2*b0*a + g2*b3*b + g2*b2*c;
		}
	}

		 //  detectionTest(final_output, det_out);
//	det_out = final_output_PhasShift;

	return *det_out;

	}
	   float Image_Reconstract::Under_sampling(int undertype)
	   {
		  int nt = omp_get_max_threads();
		   omp_set_num_threads(nt - 0);
		   if (undertype == 0)		//PEAK DETECT
		   {
			   float window_size = (float)N_SMPL / (float)COL_PIXEL;

#pragma omp parallel for firstprivate(window_size) schedule(static)
			   for (int i = 0; i < N_SMPL*N_LINE; i += N_SMPL)
			   {
				   register float max = 0;
				   register float L = 0;
				   register int w = (i / N_SMPL)*COL_PIXEL;
				   for (int k = 1; k < N_SMPL; k++)
				   {
					   register float NewL = floor((k / window_size) + 1);
					   register float dto = det_out[i + k];
					   if (max < dto)	max = dto;
					   if (L != NewL)
					   {
						   under_sampling_out[w] = max;
						   w += 1;
						   max = 0;
					   }
					   L = NewL;
				   }
			   }
		   }
		   else if (undertype == 1)    //Sample
		   {
			   float window_size = (float)N_SMPL / (float)COL_PIXEL;

#pragma omp parallel for firstprivate(window_size) schedule(static)
			   for (int i = 0; i < N_SMPL*N_LINE; i += N_SMPL)
			   {
				   register float L = 0;
				   register int w = (i / N_SMPL)*COL_PIXEL;
				   for (int k = 1; k < N_SMPL; k++)
				   {
					   register float NewL = floor((k / window_size) + 1);
					   register float dto = det_out[i + k];
					   if (L != NewL)
					   {
						   under_sampling_out[w] = dto;
						   w += 1;
					   }
					   L = NewL;
				   }
			   }
		   }
		   else if (undertype == 2)     //Average
		   {

			/*  float window_size = (float)N_SMPL / (float)COL_PIXEL;


#pragma omp parallel for firstprivate(window_size) schedule(static)
			   for (int i = 0; i < N_SMPL*N_LINE; i += N_SMPL)
			   {
				   register float L = 0;
				   register int w = (i / N_SMPL)*COL_PIXEL;
				   register float sum = 0;
				   register int j = 1;
				   averae = 0;
				   for (int k = 1; k < N_SMPL; k++)
				   {
					   register float NewL = floor((k / window_size) + 1);
					   register float dto = det_out[i + k];
					   sum = sum + dto;
					   if (L == NewL)
					   {
						   j = j + 1;
						   continue;
					   }
					   else
					   {
						   averae = sum / j;
						   under_sampling_out[w] = averae;
						   w += 1;
						   j = 0;
						   sum = 0;
					   }
					   L = NewL;
				   }
			   }*/

			   avsamplingTest(under_sampling_out, det_out);
			   
		   }
		   return *under_sampling_out;
	   }


	   float Image_Reconstract::dynamic(int dBrange)
	{
	int nt = omp_get_max_threads();
			omp_set_num_threads(nt-0);

		int dBmax = 150;
		float thresh = pow((float) 10,(float) (dBmax-dBrange)/20);
		float ymax= 0;
		float maximum_y=0;
		float max = 0;
		if (dBrange >= 150 || dBrange <= 0 )
		{
			//this command could works in consol app only cout<<"ERROR dBrange must be between 0 and 150"<<endl;
			AfxMessageBox(_T("ERROR dBrange must be between 0 and 150"));
			dBrange=60;
		}
	
	#pragma omp parallel for  schedule(static)
	
		for (int i =0 ; i < IMSIZE ; i++)
		{
			dynamic_out[i] = abs(under_sampling_out[i]);
	
			if (maximum_y < dynamic_out[i])
			{
				maximum_y = dynamic_out[i];
			}
		}
		float x = pow((float)10 , (float) 7.5 );
		ymax = maximum_y / x;
			
		int min1 = dBmax-dBrange;
		
		int newmax = 255;
		int newmin = 0;
		int oldrange = dBmax - min1 ;
		int newrange = newmax - newmin ;
	#pragma omp parallel for  schedule(static)
	
		for (int i =0 ; i < IMSIZE ; i++)
		{

			dynamic_out[i] = dynamic_out[i] / ymax;
			if (dynamic_out[i] > thresh)
				dynamic_out[i] = dynamic_out[i];
			else
				dynamic_out[i] = thresh;
			dynamic_out[i] = 20 * log10(dynamic_out[i]);
			newscale[i] = (((dynamic_out[i] - min1 ) * newrange ) / oldrange ) + newmin;
		}
		 
		// runTest4(newscale, under_sampling_out,dBrange);
		  
		return *newscale;
	}
PixelDef* Image_Reconstract::scan_converter(short int R,float  pitch,float depth,float lens,int nSamples, int nLines)
	{
		float dAngle = pitch/R;
		float halfAngle = ((nLines - 1)*dAngle)/2;
        float d1=R*cos(halfAngle);
		float d2=R-d1;
		float dr = depth/nSamples; 
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
		for (int i = 0;i<nSamples;i++)
			  midSamples[i] = R+lens+dr + i*dr;
		for (int i = 0;i<nLines;i++)
			  angleVector[i] = -halfAngle+ i*dAngle;
		int x=0;
		for(int i = 0 ; i <nLines ; i++)
		{
			for(int j = 0 ; j <nSamples ; j++)
			{
				Lin_n_x[x] =midSamples[j] * sin(angleVector[i]);
				Lin_n_y[x] =midSamples[j] * cos(angleVector[i]);
				x++;
			}
		}
		int y =0;
		for(int i = 0 ; i <nLines ; i++)
			for(int j = 0 ; j <nSamples ; j++)
			{
				STheta[y] = -halfAngle - dAngle + (1+i) * dAngle ;
				SRadius[y] = R + lens + (1+j) * dr ;
				y++;
			}
		float npixel=512;
		float pixel_mm= (depth+d2)/npixel;
	
		for (int i = 0 ;i<nSamples;i++)
			{
				ypixel_step[i] = d1+lens+pixel_mm+i*pixel_mm ;
				xpixel_step[i] = (-(nSamples/2)*pixel_mm ) + i*pixel_mm;
			}
		int z=0;
		for (int i = 0;i<nSamples;i++)
			for (int j = 0;j<nSamples;j++)
			{	 
				y_pixel[z] = ypixel_step[j];
				x_pixel[z]= xpixel_step[i];
				ImRadius[z]= sqrt( ( x_pixel[z] * x_pixel[z] ) +(  y_pixel[z] * y_pixel[z] ) );
				ImTheta[z] = atan(x_pixel[z] / y_pixel[z]);
				z++;
			}
		counter = 0;
		cc=0;
		for (int i = 0 ; i<nSamples*nSamples;i++)
		{
				if(ImTheta[i] < -0.418032786885246|| ImTheta[i] > 0.418032786885246 || ImRadius[i] < 62.051562500000000 || ImRadius[i] > 2.417000000000000e+02)
				{
					ptrpixel[i].line_index=0;
					ptrpixel[i].sample_index=0;
					ptrpixel[i].c1=0;
					ptrpixel[i].c2=0;
					ptrpixel[i].c3=0;
					ptrpixel[i].c4=0;
				}
				else if(ptrpixel[i].line_index == 255 || ptrpixel[i].sample_index == 511)
				{
					ptrpixel[i].c1=0;
					ptrpixel[i].c2=0;
					ptrpixel[i].c3=0;
					ptrpixel[i].c4=0;
				}
				else
				{
					ptrpixel[i].line_index= (ImTheta[i]+ halfAngle+dAngle)/dAngle;
					ptrpixel[i].sample_index = (ImRadius[i]-lens-R)/dr;
					float x1 =0 , x2 =0, x=0 , y1=0 , y2=0 , y=0 ,dist1=0,dist2=0,dist3=0,dist4=0,dist_tot=0 ;
					x1 = Lin_n_x[ptrpixel[i].sample_index + (ptrpixel[i].line_index * 512) ];
				    x2 = Lin_n_x[ptrpixel[i].sample_index + ((ptrpixel[i].line_index+1) * 512)];
				    x = x_pixel[i];
				    y1 = Lin_n_y[ptrpixel[i].sample_index + (ptrpixel[i].line_index*512)];
				    y2 =Lin_n_y[(ptrpixel[i].sample_index +1) + (ptrpixel[i].line_index*512)];
				    y = y_pixel[i];
				    dist1 = sqrt(((x-x1) * (x-x1)) + ((y-y1)*(y-y1)) );
				    dist2 = sqrt(((x-x2) * (x-x2)) + ((y-y1)*(y-y1)) );
					dist3 = sqrt(((x-x1) * (x-x1)) + ((y-y2)*(y-y2)) );
					dist4 = sqrt(((x-x2) * (x-x2)) + ((y-y2)*(y-y2)) );
				    dist_tot = dist1 + dist2 + dist3 + dist4;
				    ptrpixel[i].c1 = (1-(dist1/ dist_tot ))/3 ; 
				    ptrpixel[i].c2 = (1-(dist2/ dist_tot ))/3;
				    ptrpixel[i].c3=  (1-(dist3/ dist_tot ))/3;
				    ptrpixel[i].c4=  (1-(dist4/ dist_tot ))/3;
					counter++;
					ptrpixel[cc].offset = i;
					cc++;
				}
			}
		return  ptrpixel;
		delete[]midSamples;
		delete[]angleVector ;
		delete[]Lin_n_x ;
		delete[]Lin_n_y ; 
		delete[]STheta ;
		delete[]SRadius;
		delete[]y_pixel ;
		delete[]x_pixel ;
		delete[]ImRadius ;
		delete[]ImTheta ;
		delete[]xpixel_step;
		delete[]ypixel_step;
	}
	BYTE Image_Reconstract::interpolation()
	{
	int na = omp_get_max_threads();
	omp_set_num_threads(na-0);
	#pragma omp parallel for schedule(static)
	for (int i = 0 ; i < counter; i++)
	{
		register PixelDef* s = ptrpixel + ptrpixel[i].offset ;
		register unsigned short Sindx = s->sample_index;
		register unsigned short Lindx = s->line_index;
		register float *ptr = newscale;

		register float Q11 = *(ptr + (Sindx + (Lindx * 512)));
		register float Q12 = *(ptr +(Sindx + 1  + (Lindx * 512)));
		register float Q21 =	*(ptr + (Sindx + ((Lindx +1) * 512)));
		register float Q22 = *(ptr + (Sindx +1 + ((Lindx +1) * 512)));
		final_image[ptrpixel[i].offset] = s->c1 * Q11 + s->c2 *Q21 + s->c3 *Q12+ s->c4 *Q22;
	//	final_image[ptrpixel[i].offset] =  Q11 ;

	}
		return *final_image;
	}
