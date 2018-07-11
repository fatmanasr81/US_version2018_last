
// MFC_projectDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "Image_Reconstract.h"
#include "image_reconstract_GPU.h"


// CMFC_projectDlg dialog
class CMFC_projectDlg : public CDialogEx
{
// Construction
public:
	CMFC_projectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFC_PROJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	public:
	int REDvalue;
	int GREENvalue;
	int BLUEvalue;


private:
	CImage image;
	HBITMAP hBitmap;//Bitmap handler

	CWnd *pWnd; 
	HDC hdc;	//Display device context
	HDC MemDc;	//Memory device context
	
	Image_Reconstract construct;
	image_reconstract_GPU GPU_construct;
	int i ;
	bool LUTFlag;
	bool LUTFlag_GPU;

public:
	CStatic m_BMP_Ctrl;
	afx_msg void OnBnClickedOk();
	CButton Create_vid;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedcreatevid();
	CButton Freeze_vid;
	afx_msg void OnBnClickedFreeze();
	afx_msg void OnBnClickedSaveimg();
	afx_msg void OnBnClickedMeapre();
	afx_msg void OnBnClickedTable();
	afx_msg void OnBnClickedvid();
	afx_msg void OnBnClickedmax();
	afx_msg void OnBnClickednormal();
	afx_msg void OnBnClickedaverage();
	BOOL m_max;
	BOOL m_normall;
	BOOL m_averagee;
	CButton m_maxcontrol;
	CButton m_normalcontrol;
	CButton m_average;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	CButton m_BW;
	BOOL m_BWVal;
	CButton m_ellipticcontrol;
	CButton m_chebycontrol;
	BOOL m_chevyval;
	BOOL m_ellipticval;
	CSliderCtrl m_dr_control;
	int m_dr_val;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCancel();
	CString m_editval;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CButton Create_vidG;
	CButton Freeze_vidG;
	CSliderCtrl m_dr_controlG;
	CButton m_maxcontrolG;
	CButton m_normalcontrolG;
	CButton m_averageG;
	CButton m_BWG;
	CButton m_ellipticcontrolG;
	CButton m_chevycontrolG;
	afx_msg void OnBnClickedTableg();
	afx_msg void OnBnClickedCreatevidg();
	afx_msg void OnBnClickedvidg();
	afx_msg void OnBnClickedperformanceg();
	afx_msg void OnBnClickedFreezeg();
	afx_msg void OnBnClickedSaveimgg();
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedmaxg();
	afx_msg void OnBnClickednormalg();
	afx_msg void OnBnClickedaverageg();
	afx_msg void OnBnClickedBwradiog();
	afx_msg void OnBnClickedEllipticradiog();
	afx_msg void OnBnClickedChevyradiog();
	int m_dr_valG;
	BOOL m_maxG;
	BOOL m_normalG;
	BOOL m_averagevalG;
	BOOL m_BWValG;
	BOOL m_ellipticvalG;
	BOOL m_chevyvalG;
	afx_msg void OnStnClickededit();
	CString m_editvalG;
	afx_msg void OnBnClickedRadio4();
	CButton constr_least;
	BOOL m_constr_leastval;
	CButton m_least_pth_normcontrol;
	BOOL m_least_pth_normval;
	afx_msg void OnBnClickedleastpthnorm();
	afx_msg void OnBnClickedconstrleast();
};
