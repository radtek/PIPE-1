// TextFilter.h: interface for the CTextFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTFILTER_H__FB5DDE06_2C66_41A3_8E44_5ACC4EA58217__INCLUDED_)
#define AFX_TEXTFILTER_H__FB5DDE06_2C66_41A3_8E44_5ACC4EA58217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LOWCHR  0x1f
#define HIGHCHR 0x7b

class CTextFilter  
{
public:
	int Status;
	int MaxLen;
	void Strip(	CString TextIn);
  void CTextFilter::Strip(char *TxtIn);
	CString TextOut;
	CTextFilter();
	virtual ~CTextFilter();

private:


};

#endif // !defined(AFX_TEXTFILTER_H__FB5DDE06_2C66_41A3_8E44_5ACC4EA58217__INCLUDED_)
