
// MFC_projectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_project.h"
#include "MFC_projectDlg.h"
#include "Image_Reconstract.h"
#include "image_reconstract_GPU.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <Windows.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CMFC_projectDlg dialog




CMFC_projectDlg::CMFC_projectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_projectDlg::IDD, pParent)
	, REDvalue(0)
	, GREENvalue(0)
	, BLUEvalue(0)
	, m_max(FALSE)
	, m_normall(TRUE)
	, m_averagee(TRUE)
	, m_BWVal(FALSE)
	, m_chevyval(TRUE)
	, m_ellipticval(TRUE)
	, m_dr_val(60)
	, m_editval(_T(""))
	, m_dr_valG(60)
	, m_maxG(FALSE)
	, m_normalG(TRUE)
	, m_averagevalG(TRUE)
	, m_BWValG(FALSE)
	, m_ellipticvalG(TRUE)
	, m_chevyvalG(TRUE)
	, m_editvalG(_T(""))
	, m_constr_leastval(TRUE)
	, m_least_pth_normval(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_projectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BMP, m_BMP_Ctrl);
	DDX_Control(pDX, IDC_createVid, Create_vid);
	DDX_Control(pDX, IDC_Freeze, Freeze_vid);
	DDX_Radio(pDX, IDC_max, m_max);
	DDX_Radio(pDX, IDC_normal, m_normall);
	DDX_Radio(pDX, IDC_average, m_averagee);
	DDX_Control(pDX, IDC_max, m_maxcontrol);
	DDX_Control(pDX, IDC_normal, m_normalcontrol);
	DDX_Control(pDX, IDC_average, m_average);
	DDX_Control(pDX, IDC_RADIO1, m_BW);
	DDX_Radio(pDX, IDC_RADIO1, m_BWVal);
	DDX_Control(pDX, IDC_RADIO2, m_ellipticcontrol);
	DDX_Control(pDX, IDC_RADIO3, m_chebycontrol);
	DDX_Radio(pDX, IDC_RADIO3, m_chevyval);
	DDX_Radio(pDX, IDC_RADIO2, m_ellipticval);
	DDX_Control(pDX, IDC_SLIDER1, m_dr_control);
	DDX_Slider(pDX, IDC_SLIDER1, m_dr_val);
	DDX_Text(pDX, IDC_edit, m_editval);
	DDX_Control(pDX, IDC_CreateVidG, Create_vidG);
	DDX_Control(pDX, IDC_FreezeG, Freeze_vidG);
	DDX_Control(pDX, IDC_SLIDER2, m_dr_controlG);
	DDX_Control(pDX, IDC_maxG, m_maxcontrolG);
	DDX_Control(pDX, IDC_normalG, m_normalcontrolG);
	DDX_Control(pDX, IDC_averageG, m_averageG);
	DDX_Control(pDX, IDC_BW_radioG, m_BWG);
	DDX_Control(pDX, IDC_Elliptic_radioG, m_ellipticcontrolG);
	DDX_Control(pDX, IDC_Chevy_radioG, m_chevycontrolG);
	DDX_Slider(pDX, IDC_SLIDER2, m_dr_valG);
	DDX_Radio(pDX, IDC_maxG, m_maxG);
	DDX_Radio(pDX, IDC_normalG, m_normalG);
	DDX_Radio(pDX, IDC_averageG, m_averagevalG);
	DDX_Radio(pDX, IDC_BW_radioG, m_BWValG);
	DDX_Radio(pDX, IDC_Elliptic_radioG, m_ellipticvalG);
	DDX_Radio(pDX, IDC_Chevy_radioG, m_chevyvalG);
	DDX_Text(pDX, IDC_editG, m_editvalG);
	DDX_Control(pDX, IDC_constr_least, constr_least);
	DDX_Radio(pDX, IDC_constr_least, m_constr_leastval);
	DDX_Control(pDX, IDC_least_pth_norm, m_least_pth_normcontrol);
	DDX_Radio(pDX, IDC_least_pth_norm, m_least_pth_normval);
}

BEGIN_MESSAGE_MAP(CMFC_projectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_createVid, &CMFC_projectDlg::OnBnClickedcreatevid)
	ON_BN_CLICKED(IDC_Freeze, &CMFC_projectDlg::OnBnClickedFreeze)
	ON_BN_CLICKED(IDC_Save_img, &CMFC_projectDlg::OnBnClickedSaveimg)
	ON_BN_CLICKED(IDC_Mea_pre, &CMFC_projectDlg::OnBnClickedMeapre)
	ON_BN_CLICKED(IDC_Table, &CMFC_projectDlg::OnBnClickedTable)
	ON_BN_CLICKED(IDC_vid, &CMFC_projectDlg::OnBnClickedvid)
	ON_BN_CLICKED(IDC_max, &CMFC_projectDlg::OnBnClickedmax)
	ON_BN_CLICKED(IDC_normal, &CMFC_projectDlg::OnBnClickednormal)
	ON_BN_CLICKED(IDC_average, &CMFC_projectDlg::OnBnClickedaverage)
	ON_BN_CLICKED(IDC_RADIO1, &CMFC_projectDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMFC_projectDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CMFC_projectDlg::OnBnClickedRadio3)
	ON_WM_HSCROLL()
	
	ON_BN_CLICKED(IDC_BUTTON3, &CMFC_projectDlg::OnBnClickedButton3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMFC_projectDlg::OnNMCustomdrawSlider1)

	ON_BN_CLICKED(IDC_TableG, &CMFC_projectDlg::OnBnClickedTableg)
	ON_BN_CLICKED(IDC_CreateVidG, &CMFC_projectDlg::OnBnClickedCreatevidg)
	ON_BN_CLICKED(IDC_vidG, &CMFC_projectDlg::OnBnClickedvidg)
	ON_BN_CLICKED(IDC_performanceG, &CMFC_projectDlg::OnBnClickedperformanceg)
	ON_BN_CLICKED(IDC_FreezeG, &CMFC_projectDlg::OnBnClickedFreezeg)
	ON_BN_CLICKED(IDC_Save_imgG, &CMFC_projectDlg::OnBnClickedSaveimgg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CMFC_projectDlg::OnNMCustomdrawSlider2)
	ON_BN_CLICKED(IDC_maxG, &CMFC_projectDlg::OnBnClickedmaxg)
	ON_BN_CLICKED(IDC_normalG, &CMFC_projectDlg::OnBnClickednormalg)
	ON_BN_CLICKED(IDC_averageG, &CMFC_projectDlg::OnBnClickedaverageg)
	ON_BN_CLICKED(IDC_BW_radioG, &CMFC_projectDlg::OnBnClickedBwradiog)
	ON_BN_CLICKED(IDC_Elliptic_radioG, &CMFC_projectDlg::OnBnClickedEllipticradiog)
	ON_BN_CLICKED(IDC_Chevy_radioG, &CMFC_projectDlg::OnBnClickedChevyradiog)
	ON_STN_CLICKED(IDC_edit, &CMFC_projectDlg::OnStnClickededit)
	//ON_BN_CLICKED(IDC_RADIO4, &CMFC_projectDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_least_pth_norm, &CMFC_projectDlg::OnBnClickedleastpthnorm)
	ON_BN_CLICKED(IDC_constr_least, &CMFC_projectDlg::OnBnClickedconstrleast)
END_MESSAGE_MAP()


// CMFC_projectDlg message handlers

BOOL CMFC_projectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
	// TODO: Add extra initialization here
	hdc		= 0;
	MemDc	= 0;
	hBitmap = 0;
	REDvalue	= 0;
	GREENvalue	= 0;
	BLUEvalue	= 0;
	i = 0 ;
	LUTFlag=0;
	LUTFlag_GPU = 0;
	m_editval.Format(_T("%d"),60);
	m_dr_control.SetRange(0,140,TRUE);
	m_dr_control.SetPos(60);

	m_editvalG.Format(_T("%d"), 60);
	m_dr_controlG.SetRange(0, 140, TRUE);
	m_dr_controlG.SetPos(60);
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CMFC_projectDlg::OnPaint()
{

		if (!hdc)	//Make this only once
	{
		pWnd = GetDlgItem(IDC_BMP);
		hdc = ::GetDC(pWnd->m_hWnd);
		MemDc = CreateCompatibleDC(hdc);
	}

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
HCURSOR CMFC_projectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CMFC_projectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnTimer(nIDEvent);
	UpdateData(true);
	OnBnClickedcreatevid();

}
void CMFC_projectDlg::OnBnClickedcreatevid()
{
	// TODO: Add your control notification handler code here
	if(LUTFlag == FALSE)
		OnBnClickedTable();
	construct.read_bin_file();
	if(m_BWVal == FALSE)
		construct.SetFilterType(0);
	else if(m_ellipticval == FALSE)
		construct.SetFilterType(1);
	else if(m_chevyval == FALSE)
		construct.SetFilterType(2);
	else
		construct.SetFilterType(0);
	construct.filtering();
	construct.Detection();
	if(m_max == FALSE)
		construct.Under_sampling(0);
	else if(m_normall == FALSE)
		construct.Under_sampling(1);
	else if (m_averagee == FALSE)
		construct.Under_sampling(2);
	else
		construct.Under_sampling(0);
	construct.dynamic(m_dr_val);
	construct.interpolation();
	memset(construct.pixels,0,IMAGEWIDTH*IMAGEHEIGHT*CHANNELCOUNT);
	volatile int cco=0;
	for (int i = 0; i < IMAGEWIDTH*IMAGEHEIGHT; i++)
	{
		int indx = (512-i/512) + ((i*IMAGEHEIGHT) % (IMAGEWIDTH*IMAGEHEIGHT));
		construct.pixels[(i*CHANNELCOUNT) + 0] = construct.final_image[indx];
		construct.pixels[(i*CHANNELCOUNT) + 1] = construct.final_image[indx];
		construct.pixels[(i*CHANNELCOUNT) + 2] = construct.final_image[indx];
	}
	BITMAPINFOHEADER bmih;
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = IMAGEWIDTH;
	bmih.biHeight = IMAGEHEIGHT;
	bmih.biPlanes = 1;//Must be = 1;
	bmih.biBitCount = 8 * CHANNELCOUNT;
	bmih.biCompression = BI_RGB;
	BITMAPINFO dbmi;
	ZeroMemory(&dbmi, sizeof(dbmi));
	dbmi.bmiHeader = bmih;
	hBitmap = CreateDIBitmap(hdc, &bmih, CBM_INIT, construct.pixels, &dbmi, DIB_RGB_COLORS);
	if (hBitmap == NULL) 
	{
		MessageBox(_T("Could not Create image"));
		return;
	}
	SelectObject(MemDc, hBitmap);
	BitBlt(hdc, 0, 0, IMAGEWIDTH, IMAGEHEIGHT, MemDc, 0, 0, SRCCOPY);

	

}
void CMFC_projectDlg::OnBnClickedFreeze()
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
}
void CMFC_projectDlg::OnBnClickedSaveimg()
{
	// TODO: Add your control notification handler code here
	
	CString FileTypeFilters = _T("Image Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");

	CFileDialog SaveFileDlg(FALSE, _T(".bmp"), _T("MyImage"), OFN_OVERWRITEPROMPT, FileTypeFilters);

	if (SaveFileDlg.DoModal() == IDOK)
	{
		CString ImagePath;
		ImagePath = SaveFileDlg.GetPathName();

		image.Attach(hBitmap);
		image.Save(ImagePath, Gdiplus::ImageFormatBMP);
		AfxMessageBox(_T("Image Saved"));
	}
}
void CMFC_projectDlg::OnBnClickedMeapre()
{
	// TODO: Add your control notification handler code here
	int z;
	if(LUTFlag == FALSE)
		OnBnClickedTable();
	construct.read_bin_file();
	construct.SetFilterType(0);



	unsigned long Last = GetTickCount();
	#define N_ITERATIONS 100
	for(z=0 ; z<N_ITERATIONS ;z++)
	{	
		construct.filtering();							//40msec
		construct.Detection();							//49msec
			if(m_max == FALSE)
		construct.Under_sampling(0);
	else if(m_normall == FALSE)
		construct.Under_sampling(1);
	else if (m_averagee == FALSE)
		construct.Under_sampling(2);
	else
		construct.Under_sampling(0);					//peak(21mSec),sample(20mSec),Average(23mSec)
		construct.dynamic(60);							//45msec
		construct.interpolation();



		
	}

	unsigned int   Now = GetTickCount();
	unsigned int ApplicationHours  , ApplicationMinutes , ApplicationSeconds ,ApplicationMicroSeconds;
	unsigned int  Difference = GetTickCount() - Last;
	ApplicationHours = ((Difference)/ (3600 * 999)) % 24;
	ApplicationMinutes =((Difference) / (60 * 999)) % 60;
	ApplicationSeconds = ((Difference) / 999) % 60;
	ApplicationMicroSeconds = Difference * 999;

	Last = GetTickCount();
		for(z=0 ; z<N_ITERATIONS ;z++)
	{	
		construct.filtering();							//40msec
	}
	Now = GetTickCount();
	unsigned int filteringMicroSeconds;
	Difference = GetTickCount() - Last;
	ApplicationHours = ((Difference)/ (3600 * 999)) % 24;
	ApplicationMinutes =((Difference) / (60 * 999)) % 60;
	ApplicationSeconds = ((Difference) / 999) % 60;
	filteringMicroSeconds = Difference * 999;

	Last = GetTickCount();
			for(z=0 ; z<N_ITERATIONS ;z++)
	{	
		construct.Detection();							//49msec
	}
	Now = GetTickCount();
	unsigned int detectionMicroSeconds;
	Difference = GetTickCount() - Last;
	ApplicationHours = ((Difference)/ (3600 * 999)) % 24;
	ApplicationMinutes =((Difference) / (60 * 999)) % 60;
	ApplicationSeconds = ((Difference) / 999) % 60;
	detectionMicroSeconds = Difference * 999;

	Last = GetTickCount();
			for(z=0 ; z<N_ITERATIONS ;z++)
	{	
		construct.Under_sampling(0);							//49msec
	}
	Now = GetTickCount();
	unsigned int underMicroSeconds;
	Difference = GetTickCount() - Last;
	ApplicationHours = ((Difference)/ (3600 * 999)) % 24;
	ApplicationMinutes =((Difference) / (60 * 999)) % 60;
	ApplicationSeconds = ((Difference) / 999) % 60;
	underMicroSeconds = Difference * 999;

 Last = GetTickCount();
			for(z=0 ; z<N_ITERATIONS ;z++)
	{	
		construct.dynamic(60);							//49msec
	}
	Now = GetTickCount();
	unsigned int dynamicMicroSeconds;
	Difference = GetTickCount() - Last;
	ApplicationHours = ((Difference)/ (3600 * 999)) % 24;
	ApplicationMinutes =((Difference) / (60 * 999)) % 60;
	ApplicationSeconds = ((Difference) / 999) % 60;
	dynamicMicroSeconds = Difference * 999;


	Last = GetTickCount();
		for(z=0 ; z<N_ITERATIONS ;z++)
	{	
		construct.interpolation();							//40msec
	}
	   Now = GetTickCount();
	unsigned int interMicroSeconds;
	 Difference = GetTickCount() - Last;
	ApplicationHours = ((Difference)/ (3600 * 999)) % 24;
	ApplicationMinutes =((Difference) / (60 * 999)) % 60;
	ApplicationSeconds = ((Difference) / 999) % 60;
	interMicroSeconds = Difference * 999;

	CString Msg;
	Msg.Format(_T("This application has been running for  frame per %d Microsecond ,\n filtration %d micro , \n detection %d micro , \n under sampling %d micro  ,  \n Low Compresstion %d micro  , \n Scan Converter interpolation %d micro  ") , (ApplicationMicroSeconds/z),filteringMicroSeconds/z ,detectionMicroSeconds/z, underMicroSeconds/z ,dynamicMicroSeconds/z,interMicroSeconds/z);
	MessageBox(Msg);
	UpdateData(FALSE);
}
void CMFC_projectDlg::OnBnClickedTable()
{
	//TODO: Add your control notification handler code here
	short int R = 61;
	float  pitch = 0.2;
	float depth = 180;
	float lens = 0.7;
	int nLines = 256;
	int nSamples =512;
	construct.scan_converter(R,pitch, depth,lens,nSamples,nLines);
	LUTFlag = 1 ;
}

void CMFC_projectDlg::OnBnClickedvid()
{
	// TODO: Add your control notification handler code here
	SetTimer(1,3000,NULL);
}


void CMFC_projectDlg::OnBnClickedmax()
{
	// TODO: Add your control notification handler code here
	UpdateData (TRUE);
	m_normall = 1 ;
	m_averagee = 1;
	UpdateData (FALSE);
	OnBnClickedcreatevid();
}


void CMFC_projectDlg::OnBnClickednormal()
{
	// TODO: Add your control notification handler code here
	UpdateData (TRUE);
	m_max = 1 ;
	m_averagee = 1;
	UpdateData (FALSE);	
	OnBnClickedcreatevid();
}


void CMFC_projectDlg::OnBnClickedaverage()
{
	// TODO: Add your control notification handler code here
	UpdateData (TRUE);
	m_max = 1 ;
	m_normall = 1;
	UpdateData (FALSE);
	OnBnClickedcreatevid();
}


void CMFC_projectDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	UpdateData (TRUE);
	m_chevyval = 1 ;
	m_ellipticval = 1;
	UpdateData (FALSE);
	OnBnClickedcreatevid();
}


void CMFC_projectDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	UpdateData (TRUE);
	m_BWVal= 1 ;
	m_chevyval = 1;
	UpdateData (FALSE);
	OnBnClickedcreatevid();
}


void CMFC_projectDlg::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	UpdateData (TRUE);
	m_BWVal = 1 ;
	m_ellipticval = 1;
	UpdateData (FALSE);
	OnBnClickedcreatevid();
}

void CMFC_projectDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	UpdateData (TRUE);
	if (pScrollBar == (CScrollBar*)& m_dr_control)
	{
		int value = m_dr_control.GetPos();
		m_editval.Format(_T("%d"), value);
		UpdateData(FALSE);

		OnBnClickedcreatevid();
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}

// try ours

	else if (pScrollBar == (CScrollBar*)& m_dr_controlG)
	{
		int value = m_dr_controlG.GetPos();
		m_editvalG.Format(_T("%d"), value);
		UpdateData(FALSE);


		OnBnClickedCreatevidg();
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}

}

void CMFC_projectDlg::OnBnClickedButton3()
{
	

	// TODO: Add your control notification handler code here
}


void CMFC_projectDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}




void CMFC_projectDlg::OnBnClickedTableg()
{
	// TODO: Add your control notification handler code here
	short int R = 61;
	float  pitch = 0.2;
	float depth = 180;
	float lens = 0.7;
	int nLines = 256;
	int nSamples = 512;
	GPU_construct.scan_converter_GPU(R, pitch, depth, lens, nSamples, nLines);
	LUTFlag_GPU = 1;

}


void CMFC_projectDlg::OnBnClickedCreatevidg()
{
	// TODO: Add your control notification handler code here
	if (LUTFlag_GPU == FALSE)
		OnBnClickedTableg();
	GPU_construct.read_bin_file_GPU();
	if (m_BWValG == FALSE)
		GPU_construct.filtering_GPU(0);
	else if (m_ellipticvalG == FALSE)
		GPU_construct.filtering_GPU(1);
	else if (m_chevyvalG == FALSE)
		GPU_construct.filtering_GPU(2);
	else if (m_least_pth_normval == FALSE)
		GPU_construct.filtering_GPU(3);
	else if (m_constr_leastval == FALSE)
		GPU_construct.filtering_GPU(4);
	else
		GPU_construct.filtering_GPU(0);

	GPU_construct.Detection_GPU();
	if (m_maxG == FALSE)
		GPU_construct.Under_sampling_GPU(0);
	else if (m_normalG == FALSE)
		GPU_construct.Under_sampling_GPU(1);
	else if (m_averagevalG == FALSE)
		GPU_construct.Under_sampling_GPU(2);
	else
		GPU_construct.Under_sampling_GPU(0);
	GPU_construct.dynamic_GPU(m_dr_valG);
	GPU_construct.interpolation_GPU();
	memset(GPU_construct.pixels, 0, IMAGEWIDTH*IMAGEHEIGHT*CHANNELCOUNT);
	volatile int cco = 0;
	for (int i = 0; i < IMAGEWIDTH*IMAGEHEIGHT; i++)
	{
		int indx = (512 - i / 512) + ((i*IMAGEHEIGHT) % (IMAGEWIDTH*IMAGEHEIGHT));
		GPU_construct.pixels[(i*CHANNELCOUNT) + 0] =  GPU_construct.final_image[indx];
		GPU_construct.pixels[(i*CHANNELCOUNT) + 1] = GPU_construct.final_image[indx];
		GPU_construct.pixels[(i*CHANNELCOUNT) + 2] =  GPU_construct.final_image[indx];
	}
	BITMAPINFOHEADER bmih;
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = IMAGEWIDTH;
	bmih.biHeight = IMAGEHEIGHT;
	bmih.biPlanes = 1;//Must be = 1;
	bmih.biBitCount = 8 * CHANNELCOUNT;
	bmih.biCompression = BI_RGB;
	BITMAPINFO dbmi;
	ZeroMemory(&dbmi, sizeof(dbmi));
	dbmi.bmiHeader = bmih;
hBitmap = CreateDIBitmap(hdc, &bmih, CBM_INIT, GPU_construct.pixels, &dbmi, DIB_RGB_COLORS);
	if (hBitmap == NULL)
	{
		MessageBox(_T("Could not Create image"));
		return;
	}
	SelectObject(MemDc, hBitmap);
	BitBlt(hdc, 0, 0, IMAGEWIDTH, IMAGEHEIGHT, MemDc, 0, 0, SRCCOPY);

}


void CMFC_projectDlg::OnBnClickedvidg()
{
	// TODO: Add your control notification handler code here
}


void CMFC_projectDlg::OnBnClickedperformanceg()
{
	// TODO: Add your control notification handler code here
	/*	int z;
	if (LUTFlag_GPU == FALSE)
	OnBnClickedTableg();
	GPU_construct.read_bin_file_GPU();
	//GPU_construct.filtering_GPU(0);
	unsigned long Last = GetTickCount();
	#define N_ITERATIONS 250


	for (z = 0; z<N_ITERATIONS; z++)
	{
	GPU_construct.filtering_GPU(0);							//40msec
	GPU_construct.Detection_GPU();							//49msec
	if (m_max == FALSE)
	GPU_construct.Under_sampling_GPU(0);
	else if (m_normall == FALSE)
	GPU_construct.Under_sampling_GPU(1);
	else if (m_averagee == FALSE)
	GPU_construct.Under_sampling_GPU(2);
	else
	GPU_construct.Under_sampling_GPU(0);					//peak(21mSec),sample(20mSec),Average(23mSec)
	GPU_construct.dynamic_GPU(60);							//45msec
	GPU_construct.interpolation_GPU();
	}

	unsigned int   Now = GetTickCount();
	unsigned int ApplicationHours, ApplicationMinutes, ApplicationSeconds, ApplicationMicroSeconds;
	unsigned int  Difference = GetTickCount() - Last;
	ApplicationHours = ((Difference) / (3600 * 999)) % 24;
	ApplicationMinutes = ((Difference) / (60 * 999)) % 60;
	ApplicationSeconds = ((Difference) / 999) % 60;
	ApplicationMicroSeconds = Difference * 999;

	Last = GetTickCount();
	for (z = 0; z<N_ITERATIONS; z++)
	{
	GPU_construct.filtering_GPU(0);							//40msec
	}
	Now = GetTickCount();
	unsigned int filteringMicroSeconds;
	Difference = GetTickCount() - Last;
	ApplicationHours = ((Difference) / (3600 * 999)) % 24;
	ApplicationMinutes = ((Difference) / (60 * 999)) % 60;
	ApplicationSeconds = ((Difference) / 999) % 60;
	filteringMicroSeconds = Difference * 999;

	Last = GetTickCount();
	for (z = 0; z<N_ITERATIONS; z++)
	{
	GPU_construct.Detection_GPU();							//49msec
	}
	Now = GetTickCount();
	unsigned int detectionMicroSeconds;
	Difference = GetTickCount() - Last;
	ApplicationHours = ((Difference) / (3600 * 999)) % 24;
	ApplicationMinutes = ((Difference) / (60 * 999)) % 60;
	ApplicationSeconds = ((Difference) / 999) % 60;
	detectionMicroSeconds = Difference * 999;

	Last = GetTickCount();
	for (z = 0; z<N_ITERATIONS; z++)
	{
	GPU_construct.Under_sampling_GPU(0);							//49msec
	}
	Now = GetTickCount();
	unsigned int underMicroSeconds;
	Difference = GetTickCount() - Last;
	ApplicationHours = ((Difference) / (3600 * 999)) % 24;
	ApplicationMinutes = ((Difference) / (60 * 999)) % 60;
	ApplicationSeconds = ((Difference) / 999) % 60;
	underMicroSeconds = Difference * 999;

	Last = GetTickCount();
	for (z = 0; z<N_ITERATIONS; z++)
	{
	GPU_construct.dynamic_GPU(60);							//49msec
	}
	Now = GetTickCount();
	unsigned int dynamicMicroSeconds;
	Difference = GetTickCount() - Last;
	ApplicationHours = ((Difference) / (3600 * 999)) % 24;
	ApplicationMinutes = ((Difference) / (60 * 999)) % 60;
	ApplicationSeconds = ((Difference) / 999) % 60;
	dynamicMicroSeconds = Difference * 999;


	Last = GetTickCount();
	for (z = 0; z<N_ITERATIONS; z++)
	{
	GPU_construct.interpolation_GPU();							//40msec
	}
	Now = GetTickCount();
	unsigned int interMicroSeconds;
	Difference = GetTickCount() - Last;
	ApplicationHours = ((Difference) / (3600 * 999)) % 24;
	ApplicationMinutes = ((Difference) / (60 * 999)) % 60;
	ApplicationSeconds = ((Difference) / 999) % 60;
	interMicroSeconds = Difference * 999;
	//z = 1;
	CString Msg;
	Msg.Format(_T("This application has been running on GPU for  frame per %d Microsecond ,\n filtration %d micro , \n detection %d micro , \n under sampling %d micro  ,  \n Low Compresstion %d micro  , \n Scan Converter interpolation %d micro  "), ApplicationMicroSeconds / z, filteringMicroSeconds / z, detectionMicroSeconds / z, underMicroSeconds / z, dynamicMicroSeconds / z, interMicroSeconds / z);
	MessageBox(Msg);
	UpdateData(FALSE);*/

	//cuda timing 
if (LUTFlag_GPU == FALSE)
OnBnClickedTableg();
GPU_construct.read_bin_file_GPU();
    float time[5] = { 0 };
	 GPU_construct.performence_GPU(m_dr_valG,time);
	 float total_time = time[0] + time[1] + time[2] + time[3] + time[4];
	CString Msg;
	Msg.Format(_T("This application takes %f   msec for 1 frame  \n filtration %f  ,\n detection %f msec , \n undersampling %f  msec ,\n dynamic %f  msec ,\n  interpolation %f msec  "), total_time,time[0],time[1],time[2],time[3],time[4]);
	MessageBox(Msg);
	UpdateData(FALSE);

	// time using QueryPerformanceCounter

	/*	int z;
	if (LUTFlag_GPU == FALSE)
	OnBnClickedTableg();
	GPU_construct.read_bin_file_GPU();
	#define N_ITERATIONS 200

	LARGE_INTEGER perfCntStart, perfCntStop, proc_freq;
	::memset(&proc_freq, 0x00, sizeof(proc_freq));
	::memset(&perfCntStart, 0x00, sizeof(perfCntStart));
	::memset(&perfCntStop, 0x00, sizeof(perfCntStop));
	::QueryPerformanceCounter(&perfCntStart);
	::QueryPerformanceFrequency(&proc_freq);

	for (z = 0; z<N_ITERATIONS; z++)
	{
	GPU_construct.filtering_GPU(0);							//40msec
	GPU_construct.Detection_GPU();							//49msec
	if (m_max == FALSE)
	GPU_construct.Under_sampling_GPU(0);
	else if (m_normall == FALSE)
	GPU_construct.Under_sampling_GPU(1);
	else if (m_averagee == FALSE)
	GPU_construct.Under_sampling_GPU(2);
	else
	GPU_construct.Under_sampling_GPU(0);					//peak(21mSec),sample(20mSec),Average(23mSec)
	GPU_construct.dynamic_GPU(60);							//45msec
	GPU_construct.interpolation_GPU();
	}

	QueryPerformanceCounter(&perfCntStop);
	float ApplicationMicroSeconds = float(perfCntStop.QuadPart - perfCntStart.QuadPart) / float(proc_freq.QuadPart)  ;

	::memset(&proc_freq, 0x00, sizeof(proc_freq));
	::memset(&perfCntStart, 0x00, sizeof(perfCntStart));
	::memset(&perfCntStop, 0x00, sizeof(perfCntStop));
	::QueryPerformanceCounter(&perfCntStart);
	::QueryPerformanceFrequency(&proc_freq);
	for (z = 0; z<N_ITERATIONS; z++)
	{
	GPU_construct.filtering_GPU(0);							//40msec
	}
	QueryPerformanceCounter(&perfCntStop);
	float filteringMicroSeconds = float(perfCntStop.QuadPart - perfCntStart.QuadPart) / float(proc_freq.QuadPart) ;


	::memset(&proc_freq, 0x00, sizeof(proc_freq));
	::memset(&perfCntStart, 0x00, sizeof(perfCntStart));
	::memset(&perfCntStop, 0x00, sizeof(perfCntStop));
	::QueryPerformanceCounter(&perfCntStart);
	::QueryPerformanceFrequency(&proc_freq);

	for (z = 0; z<N_ITERATIONS; z++)
	{
	GPU_construct.Detection_GPU();							//49msec
	}
	QueryPerformanceCounter(&perfCntStop);
	float detectionMicroSeconds = float(perfCntStop.QuadPart - perfCntStart.QuadPart) / float(proc_freq.QuadPart) ;

	::memset(&proc_freq, 0x00, sizeof(proc_freq));
	::memset(&perfCntStart, 0x00, sizeof(perfCntStart));
	::memset(&perfCntStop, 0x00, sizeof(perfCntStop));
	::QueryPerformanceCounter(&perfCntStart);
	::QueryPerformanceFrequency(&proc_freq);

	for (z = 0; z<N_ITERATIONS; z++)
	{
	GPU_construct.Under_sampling_GPU(0);							//49msec
	}
	QueryPerformanceCounter(&perfCntStop);
	float underMicroSeconds = float(perfCntStop.QuadPart - perfCntStart.QuadPart) / float(proc_freq.QuadPart) ;

	::memset(&proc_freq, 0x00, sizeof(proc_freq));
	::memset(&perfCntStart, 0x00, sizeof(perfCntStart));
	::memset(&perfCntStop, 0x00, sizeof(perfCntStop));
	::QueryPerformanceCounter(&perfCntStart);
	::QueryPerformanceFrequency(&proc_freq);

	for (z = 0; z<N_ITERATIONS; z++)
	{
	GPU_construct.dynamic_GPU(60);							//49msec
	}
	QueryPerformanceCounter(&perfCntStop);
	float dynamicMicroSeconds = float(perfCntStop.QuadPart - perfCntStart.QuadPart) / float(proc_freq.QuadPart) ;


	::memset(&proc_freq, 0x00, sizeof(proc_freq));
	::memset(&perfCntStart, 0x00, sizeof(perfCntStart));
	::memset(&perfCntStop, 0x00, sizeof(perfCntStop));
	::QueryPerformanceCounter(&perfCntStart);
	::QueryPerformanceFrequency(&proc_freq);

	for (z = 0; z<N_ITERATIONS; z++)
	{
	GPU_construct.interpolation_GPU();							//40msec
	}
	QueryPerformanceCounter(&perfCntStop);
	float interMicroSeconds = float(perfCntStop.QuadPart - perfCntStart.QuadPart) / float(proc_freq.QuadPart) ;


	CString Msg;
	Msg.Format(_T("This application has been running on GPU for  frame per %f Microsecond ,\n filtration %f micro , \n detection %f micro , \n under sampling %f micro  ,  \n Low Compresstion %f micro  , \n Scan Converter interpolation %f micro "), ApplicationMicroSeconds / z, filteringMicroSeconds / z, detectionMicroSeconds / z, underMicroSeconds / z, dynamicMicroSeconds / z, interMicroSeconds / z);
	MessageBox(Msg);
	UpdateData(FALSE);*/

}


void CMFC_projectDlg::OnBnClickedFreezeg()
{
	// TODO: Add your control notification handler code here
}


void CMFC_projectDlg::OnBnClickedSaveimgg()
{
	// TODO: Add your control notification handler code here
	CString FileTypeFilters = _T("Image Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");

	CFileDialog SaveFileDlg(FALSE, _T(".bmp"), _T("MyImage"), OFN_OVERWRITEPROMPT, FileTypeFilters);

	if (SaveFileDlg.DoModal() == IDOK)
	{
		CString ImagePath;
		ImagePath = SaveFileDlg.GetPathName();

		image.Attach(hBitmap);
		image.Save(ImagePath, Gdiplus::ImageFormatBMP);
		AfxMessageBox(_T("Image Saved"));
	}
}



void CMFC_projectDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CMFC_projectDlg::OnBnClickedmaxg()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_normalG = 1;
	m_averagevalG = 1;
	UpdateData(FALSE);
	OnBnClickedCreatevidg();
}


void CMFC_projectDlg::OnBnClickednormalg()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_maxG = 1;
	m_averagevalG = 1;
	UpdateData(FALSE);
	OnBnClickedCreatevidg();
}


void CMFC_projectDlg::OnBnClickedaverageg()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_maxG = 1;
	m_normalG = 1;
	UpdateData(FALSE);
	OnBnClickedCreatevidg();
}


void CMFC_projectDlg::OnBnClickedBwradiog()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_chevyvalG = 1;
	m_ellipticvalG = 1;
	m_least_pth_normval = 1;
	m_constr_leastval = 1;
	UpdateData(FALSE);
	OnBnClickedCreatevidg();
}


void CMFC_projectDlg::OnBnClickedEllipticradiog()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_BWValG = 1;
	m_chevyvalG = 1;
	m_least_pth_normval = 1;
	m_constr_leastval = 1;
	UpdateData(FALSE);
	OnBnClickedCreatevidg();
}


void CMFC_projectDlg::OnBnClickedChevyradiog()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_BWValG = 1;
	m_ellipticvalG = 1;
	m_least_pth_normval = 1;
	m_constr_leastval = 1;
	UpdateData(FALSE);
	OnBnClickedCreatevidg();
}


void CMFC_projectDlg::OnStnClickededit()
{
	// TODO: Add your control notification handler code here
}


void CMFC_projectDlg::OnBnClickedRadio4()
{
	// TODO: Add your control notification handler code here
}


void CMFC_projectDlg::OnBnClickedleastpthnorm()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_BWValG = 1;
	m_ellipticvalG = 1;
	m_chevyvalG = 1;
	m_constr_leastval = 1;
	UpdateData(FALSE);
	OnBnClickedCreatevidg();
}


void CMFC_projectDlg::OnBnClickedconstrleast()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_BWValG = 1;
	m_ellipticvalG = 1;
	m_chevyvalG = 1;
	m_least_pth_normval = 1;
	UpdateData(FALSE);
	OnBnClickedCreatevidg();
}
