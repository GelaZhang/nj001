
#include "StringUtil.h"
#include "AppHelper.h"

#include "Surface.h"

#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "png.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "tiff.lib")

CSurface::CSurface( ) :
m_rcScreen( 0, 0, 800, 600 ),
m_rcTempImage(0,0,0,0),
m_hDC(NULL),
m_pOldBmpSurface( NULL ),
m_singleton(false)
{

}

void CSurface::Init( HDC hDC, const CRect &rc )
{
	if ( !m_singleton )
		m_singleton = true;
	else
		return ;
	m_hDC = hDC;
	m_rcScreen = rc;

#if CXIMAGE_SUPPORT_BMP
	m_mapSuffix2ImageType[_T("bmp")] = CXIMAGE_FORMAT_BMP;
#endif
#if CXIMAGE_SUPPORT_JPG
	m_mapSuffix2ImageType[_T("jpg")] = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_PNG
	m_mapSuffix2ImageType[_T("png")] = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_GIF
	m_mapSuffix2ImageType[_T("gif")] = CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_ICO
	m_mapSuffix2ImageType[_T("ico")] = CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
	m_mapSuffix2ImageType[_T("tif")] = CXIMAGE_FORMAT_TIF;
#endif

	m_dcSurface.CreateCompatibleDC( CDC::FromHandle(hDC) );
	m_bmpSurface.CreateCompatibleBitmap( CDC::FromHandle(hDC), rc.Width(), rc.Height() );
	m_pOldBmpSurface = m_dcSurface.SelectObject( &m_bmpSurface );

}

CSurface::~CSurface()
{
	MapPath2Image::iterator itor = m_mapPath2Image.begin();
	while ( itor != m_mapPath2Image.end() )
	{
		delete itor->second;
		++ itor;
	}
	if ( m_pOldBmpSurface )
		m_dcSurface.SelectObject( m_pOldBmpSurface );

	


}

BOOL CSurface::LoadFileImage( const TCHAR *pszPath, CxImage* pImage )
{
	if ( NULL == pszPath || NULL == pImage )
	{
		return NULL;
	}

	const TCHAR* pszDot = _tcsrchr( pszPath, _T('.') );
	if ( NULL == pszDot || NULL == pszDot + 1 )
	{
		return NULL;
	}

	std::wstring cstrSuffix = pszDot + 1 ;
	cstrSuffix = MBCS2Unicode(Utility::toLowerA(Unicode2MBCS(cstrSuffix.c_str())).c_str());
	DWORD dwImgType = 0;
	MapSuffix2ImageType::iterator itor = m_mapSuffix2ImageType.find( cstrSuffix );
	if ( itor == m_mapSuffix2ImageType.end() ) //扩展名不对
	{
		dwImgType = CXIMAGE_FORMAT_UNKNOWN;
	}
	else
	{
		dwImgType = itor->second;
	}


	if ( !pImage->Load( pszPath, dwImgType ) )
		return FALSE;


	if ( pImage->IsValid() )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CSurface::ShowImage( 
							const std::vector<std::wstring> &arrImgPath, 
							const std::vector<CRect> &arrRect, 
							const std::vector<BOOL> &arrHold )
{
	if ( arrImgPath.empty() )
		return FALSE;

	if ( (arrImgPath.size() != arrHold.size() ) || ( arrHold.size() != arrRect.size() ) )
		return FALSE;

	CRect rcImg = arrRect[0];
	for ( int i = 1; i < arrRect.size(); i ++ )
	{
		if ( FALSE == rcImg.UnionRect( rcImg, arrRect[i] ) )
			return FALSE;
	}

	CRect rcTemp;
	rcTemp.IntersectRect( m_rcScreen, rcImg );
	if ( rcTemp != rcImg )
		return FALSE;
	
	for ( int i = 0; i < arrImgPath.size(); i++ )
	{
		
		if ( FALSE == AddImage2Mem( arrImgPath[i], arrRect[i], arrHold[i] ) )
			return FALSE;

	}

	RedrawRect(rcImg);

	return TRUE;

}

BOOL CSurface::AddImage2Mem( const std::wstring &imagPath, int x, int y, CRect &rc, BOOL bHold )
{
	ASSERT(m_hDC);
	//指定区域不在画布之内
	if ( !( ( m_rcScreen.top <= rc.top ) && ( m_rcScreen.left <= rc.left ) &&
		( m_rcScreen.bottom >= rc.bottom ) && ( m_rcScreen.right >= rc.right ) )
		)
	{
		return FALSE;
	}

	MapPath2Image::iterator itor = m_mapPath2Image.find( imagPath );
	if ( itor == m_mapPath2Image.end() )
	{
		CxImage *pImage = new CxImage();
		if ( FALSE == LoadFileImage( imagPath.c_str(), pImage ) )
		{
			delete pImage;
			return FALSE;
		}
		long cx = pImage->GetWidth() + x > m_rcScreen.right ? 
			m_rcScreen.right - x : pImage->GetWidth();
		long cy = pImage->GetHeight() + y > m_rcScreen.bottom ? 
			m_rcScreen.bottom - y : pImage->GetHeight();

		pImage->Draw(m_dcSurface, x, y, cx, cy );
		rc = CRect(x, y, x + cx, y + cy );

		if ( TRUE == bHold )
		{
			m_mapPath2Image[imagPath] = pImage;		//保存图像在内存
		}
		else 
		{
			delete pImage;
		}
	}
	else
	{
		long cx = itor->second->GetWidth() + x > m_rcScreen.right ? 
			m_rcScreen.right - x : itor->second->GetWidth();
		long cy = itor->second->GetHeight() + y > m_rcScreen.bottom ? 
			m_rcScreen.bottom - y : itor->second->GetHeight();

		itor->second->Draw(m_dcSurface, x, y, cx, cy );
		rc = CRect(x, y, x + cx, y + cy );
	}
	return TRUE;
}

BOOL CSurface::AddImage2Mem( const std::wstring &imagPath, const CRect &rc, BOOL bHold )
{
	ASSERT(m_hDC);
	//指定区域不在画布之内
	if ( !( ( m_rcScreen.top <= rc.top ) && ( m_rcScreen.left <= rc.left ) &&
		( m_rcScreen.bottom >= rc.bottom ) && ( m_rcScreen.right >= rc.right ) )
		)
	{
		return FALSE;
	}

	MapPath2Image::iterator itor = m_mapPath2Image.find( imagPath );
	if ( itor == m_mapPath2Image.end() )
	{
		CxImage *pImage = new CxImage();
		if ( FALSE == LoadFileImage( imagPath.c_str(), pImage ) )
		{
			delete pImage;
			return FALSE;
		}
		pImage->Stretch( m_dcSurface, rc );
		if ( TRUE == bHold )
		{
			m_mapPath2Image[imagPath] = pImage;		//保存图像在内存
		}
		else 
		{
			delete pImage;
		}
	}
	else
	{
		itor->second->Stretch( m_dcSurface, rc );
	}
	return TRUE;
}


BOOL CSurface::ShowImage( const std::wstring &imagPath, const CRect &rc, BOOL bHold )
{
	
	if (TRUE == AddImage2Mem( imagPath, rc, bHold) )
	{
		RedrawRect(rc);
		return TRUE;
	}
	return FALSE;
}

void CSurface::RedrawRect( const CRect &rc )
{
	ASSERT(m_hDC);

	if ( ( m_rcScreen.top <= rc.top ) && ( m_rcScreen.left <= rc.left ) &&
		( m_rcScreen.bottom >= rc.bottom ) && ( m_rcScreen.right >= rc.right ) )
	{
		::BitBlt( m_hDC, rc.left, rc.top, rc.Width(), 
			rc.Height(), m_dcSurface.GetSafeHdc(), rc.left, rc.top, SRCCOPY );
	}

	m_rcTempImage = CRect(0,0,0,0);
}

BOOL CSurface::AddImage(const std::wstring &imagPath, const CRect &rc, BOOL bHold /* = FALSE */)
{
	ASSERT(m_hDC);
	if (TRUE == AddImage2Mem( imagPath, rc, bHold) )
	{
		m_rcTempImage.UnionRect( m_rcTempImage, rc );

		return TRUE;
	}
	
	return FALSE;
}

BOOL CSurface::AddImage( 
			  const std::wstring &imagPath, 
			  int x, int y, int cx, int cy, 
			  BOOL bHold )
{
	CRect rc( x, y, cx + x, cy + y );
	return AddImage(imagPath, rc, bHold );
}

BOOL CSurface::AddImage( const std::wstring &imagPath, int x, int y, BOOL bHold )
{
	ASSERT(m_hDC);
	CRect rc;
	if (TRUE == AddImage2Mem( imagPath, x, y , rc, bHold) )
	{
		m_rcTempImage.UnionRect( m_rcTempImage, rc );

		return TRUE;
	}

	return FALSE;
}
BOOL CSurface::SetRGBInRect( COLORREF color, const CRect &rc )
{
	ASSERT(m_hDC);
	//指定区域不在画布之内
	if ( !( ( m_rcScreen.top <= rc.top ) && ( m_rcScreen.left <= rc.left ) &&
		( m_rcScreen.bottom >= rc.bottom ) && ( m_rcScreen.right >= rc.right ) )
		)
	{
		return FALSE;
	}

	CBrush brush(color);
	m_dcSurface.FillRect( rc, &brush );

	return m_rcTempImage.UnionRect( m_rcTempImage, rc );
}

BOOL CSurface::AddImage( HDC hDcMemImage, const CRect &rc )
{

	ASSERT(m_hDC);
	//指定区域不在画布之内
	if ( !( ( m_rcScreen.top <= rc.top ) && ( m_rcScreen.left <= rc.left ) &&
		( m_rcScreen.bottom >= rc.bottom ) && ( m_rcScreen.right >= rc.right ) )
		)
	{
		return FALSE;
	}
	::BitBlt( m_dcSurface, rc.left, rc.top, rc.Width(), rc.Height(),
		hDcMemImage, 0, 0, SRCCOPY );

	m_rcTempImage.UnionRect( m_rcTempImage, rc );

	return TRUE;

}

BOOL CSurface::AddImage( HDC hDcMemImage, int x, int y, int cx, int cy )
{
	CRect rc( x, y, cx + x, cy + y );
	return AddImage(hDcMemImage, rc );
}

BOOL CSurface::AddCharacter( const TCHAR *pszCharacter, const CRect &rc, COLORREF textColor, const CFont &font, UINT uiAlignment )
{
	ASSERT(m_hDC);

	//指定区域不在画布之内
	if ( !( ( m_rcScreen.top <= rc.top ) && ( m_rcScreen.left <= rc.left ) &&
		( m_rcScreen.bottom >= rc.bottom ) && ( m_rcScreen.right >= rc.right ) )
		)
	{
		return FALSE;
	}

	CFont *pOldFont = (CFont*)m_dcSurface.SelectObject( (CFont*)&font );
	m_dcSurface.SetTextColor( textColor );
	m_dcSurface.SetBkMode( TRANSPARENT );
	m_dcSurface.DrawText( pszCharacter, _tcsclen(pszCharacter), (CRect*)&rc, uiAlignment );
	m_dcSurface.SelectObject( pOldFont );

	return m_rcTempImage.UnionRect( m_rcTempImage, rc );
	
}
void CSurface::ShowImage( )
{
	RedrawRect( m_rcTempImage );
}


BOOL CSurface::CreateDC( CDC &srcDC , CDC &dc, CBitmap &bitmap, const CRect &rc)
{
    dc.CreateCompatibleDC(&srcDC);
    bitmap.CreateCompatibleBitmap(&srcDC, rc.Width(), rc.Height());
    dc.SelectObject(bitmap);	
    ::BitBlt(dc.GetSafeHdc(), 0, 0, 
        rc.Width(), rc.Height(), srcDC.GetSafeHdc(), rc.TopLeft().x, rc.TopLeft().y, SRCCOPY);
    return TRUE;

}

BOOL CSurface::CreateDC( CDC &dc, CBitmap &bitmap, const CRect &rc)
{
    dc.CreateCompatibleDC(&m_dcSurface);
    bitmap.CreateCompatibleBitmap(&m_dcSurface, rc.Width(), rc.Height());
    dc.SelectObject(bitmap);	
    ::BitBlt(dc.GetSafeHdc(), 0, 0, 
        rc.Width(), rc.Height(), m_dcSurface.GetSafeHdc(), rc.TopLeft().x, rc.TopLeft().y, SRCCOPY);
    return TRUE;

}

BOOL CSurface::AddImage(const std::wstring &imagPath, int x, int y, COLORREF crTransparent ,BOOL bHold /* = FALSE */)
{
	
	return FALSE;
}