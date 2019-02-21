// SpectrumGraph.cpp : implementation file
//

#include "stdafx.h"
#include "waveInFFT.h"
#include "SpectrumGraph.h"
#include <math.h>
#include "memoryDc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraph

CFrequencyGraph::CFrequencyGraph()
{
	m_nLength = 0;
	m_dArray = NULL;
	m_nMinValue = 0;
	m_nMaxValue = 10;
	m_nHighLevel = 80;
	m_nMediumLevel = 60;
	m_wSteps = 16;
	m_nStepFactor = 1;
	m_clrLow = RGB(0,0,255);
	m_clrMedium = RGB(0,255,0);
	m_clrHigh = RGB(255,0,0);
	m_bGrid = TRUE;
	m_bLogScale = FALSE;
	m_graphType = FG_OSCILLOSCOPE;
}


CFrequencyGraph::~CFrequencyGraph()
{
	if (m_dArray)
		delete[]m_dArray;
}



BEGIN_MESSAGE_MAP(CFrequencyGraph, CWnd)
	//{{AFX_MSG_MAP(CFrequencyGraph)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CFrequencyGraph::OnPaint()
{
	CRect rct;
	GetClientRect(&rct);
	//Needs to use CMemoryDC or else CMemDC...
	//using either class will get ride of the 
	//flicker problem that otherwise presents itself...
	CPaintDC paintdc(this);
	CMemDC dc(&paintdc,&rct);

// graph
	if (m_nLength == 0)
		return;

	CBrush* pOldBrush = NULL;
	CBrush* pbbr = NULL;
	CBrush* pabr = NULL;
	CBrush br(RGB(0,0,0));
	

	
	CPen pen(PS_SOLID,1,RGB(255,255,255));
	CPen bpen(PS_SOLID,1,RGB(0,0,255));
	CPen* pOldPen = NULL;
	CPen* pbOldPen = NULL;

	
	CRect rc;

	int i, n, x,ctr,py,half,y=0;
	int pos=0;
	int leftzerolevel = (rct.Height()/2-10)/2;
	int rightzerolevel = rct.Height()/2+10+leftzerolevel;

	double dRangePerStep = (m_nMaxValue-m_nMinValue);
	double timescale = leftzerolevel/dRangePerStep;
	

	double* data = m_dArray;

	
	switch (m_graphType)
	{
	
	case FG_OSCILLOSCOPE:
		pOldBrush = dc.SelectObject(&br);
		pOldPen = dc.SelectObject(&pen);
		dc.Rectangle( 0, 0, rct.Width(), rct.Height() );
		pbOldPen = dc.SelectObject(&bpen);
		
		for(i=0; i<m_nLength-1; i+=2)
		{
			x=(int)((double)i);
			int y =  static_cast<double>(m_dArray[i])*timescale;
			dc.LineTo( x ,y );
		}
#if 0
		dc.SelectObject(pbOldPen);
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
#endif // 0

	break;
	
	}
	
}

void CFrequencyGraph::Update(int nNumber, double* dArray)
{
	if (m_nLength != nNumber)
	{
		m_nLength = nNumber;
		if (m_dArray)
			delete []m_dArray;
		m_dArray = new double[m_nLength];
	}
	//
	m_nStepFactor = (int)((m_nLength/m_wSteps)+0.5);
	memcpy(m_dArray,dArray,nNumber*sizeof(double));
	Invalidate(FALSE);
}
