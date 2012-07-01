#ifndef __ZY_SURFACE_H_
#define __ZY_SURFACE_H_

/**
*	\file Surface.h
*	\brief  内存、文件图像显示。
*/

#include <vector>
#include <map>
#include <string>

#include <Windows.h>
#include <WinDef.h>
#include <afxwin.h>
#include "CXimage/ximage.h"


class CFont;
class CSurface 
{
public:

	/**
	*	\brief 使用这个构造函数必须调用Init函数
	*/
	CSurface( );
	void Init( HDC hDC, const CRect &rc );

	~CSurface();
	/**
	*	\brief 多张文件图像一次性显示。
	*	\param[in] arrImgPath 图像文件的路径
	*	\param[in] arrRect 图像的显示区域，
	*	\param[in] arrHold 标记是否将文件图像缓存
	*/
	BOOL ShowImage( const std::vector<std::wstring> &arrImgPath, 
		const std::vector<CRect> &arrRect, 
		const std::vector<BOOL> &arrHold );
	/**
	*	\brief 显示一张文件图像。
	*	\param[in] cstrImagPath 图像文件的路径
	*	\param[in] rc 图像的显示区域，
	*	\param[in] bHold 标记是否将文件图像缓存
	*/
	BOOL ShowImage( const std::wstring &imagPath, const CRect &rc, BOOL bHold = FALSE );
	/**
	*	\brief 添加一个文件图像到画布，注意添加后不显示。
	*	\	   添加后可调用ShowImage()或RedrawRect进行显示，调用显示后即重新计算ShowImage的区域
	*	\param[in] cstrImagPath 图像文件的路径
	*	\param[in] rc 图像的显示区域，
	*	\param[in] bHold 标记是否将文件图像缓存
	*/
	BOOL AddImage( const std::wstring &imagPath, const CRect &rc, BOOL bHold = FALSE);

	/**
	*	\brief 添加一个文件图像到画布，注意添加后不显示。
	*	\	   添加后可调用ShowImage()或RedrawRect进行显示，调用显示后即重新计算ShowImage的区域
	*	\param[in] cstrImagPath 图像文件的路径
	*	\param[in] x 图像的显示区域x轴坐标，
	*	\param[in] y 图像的显示区域y轴坐标，
	*	\param[in] cx 显示区域x轴大小
	*	\param[in] cy 显示区域y轴大小
	*	\param[in] bHold 标记是否将文件图像缓存
	*/
	BOOL AddImage( const std::wstring &imagPath, int x, int y, int cx, int cy, BOOL bHold = FALSE);

	/**
	*	\brief 添加一个文件图像到画布，注意添加后不显示。只显示图像实际大小。
	*	\	   添加后可调用ShowImage()或RedrawRect进行显示，
	*	\      调用显示后即重新计算ShowImage的区域
	*	\param[in] cstrImagPath 图像文件的路径
	*	\param[in] x 图像的显示区域x轴坐标，
	*	\param[in] y 图像的显示区域y轴坐标，
	*	\param[in] bHold 标记是否将文件图像缓存
	*/
	BOOL AddImage( const std::wstring &imagPath, int x, int y, BOOL bHold = FALSE);

	/**
	*	\brief 添加一个文件图像到画布，注意添加后不显示。只显示图像实际大小。
	*	\	   与TransparentBlt功能一样.
	*	\	   添加后可调用ShowImage()或RedrawRect进行显示，
	*	\      调用显示后即重新计算ShowImage的区域
	*	\param[in] cstrImagPath 图像文件的路径
	*	\param[in] x 图像的显示区域x轴坐标，
	*	\param[in] y 图像的显示区域y轴坐标，
	*	\param[in] bHold 标记是否将文件图像缓存
	*/
	BOOL AddImage( const std::wstring &imagPath, int x, int y, COLORREF crTransparent ,BOOL bHold = FALSE);

	/**
	*	\brief 添加一个文件图像到画布，注意添加后不显示。
	*	\	   添加后可调用ShowImage()或RedrawRect进行显示，调用显示后即重新计算ShowImage的区域
	*	\      不支持缩放显示
	*	\param[in] pdcMemImage 图像内存DC
	*	\param[in] rc 图像的显示区域，
	*/
	BOOL AddImage( HDC hDcMemImage, const CRect &rc );

	/**
	*	\brief 添加一个文件图像到画布，注意添加后不显示。
	*	\	   添加后可调用ShowImage()或RedrawRect进行显示，调用显示后即重新计算ShowImage的区域
	*	\      不支持缩放显示
	*	\param[in] pdcMemImage 图像内存DC
	*	\param[in] x 图像的显示区域x轴坐标，
	*	\param[in] y 图像的显示区域y轴坐标，
	*	\param[in] cx 显示区域x轴大小
	*	\param[in] cy 显示区域y轴大小
	*/
	BOOL AddImage( HDC hDcMemImage, int x, int y, int cx, int cy );

	/**
	*	\brief 添加一个串文字到画布，注意添加后，只改变画布不马上显示。
	*	\	   添加后可调用ShowImage()或RedrawRect进行显示，调用显示后即重新计算ShowImage的区域
	*	\      不支持缩放显示
	*	\param[in] pszCharacter 文字
	*	\param[in] rc 文字的显示区域，
	*	\param[in] uiAlignment 对齐方式，与CDC的DrawText最后一个参数一致。
	*/
	BOOL AddCharacter( const TCHAR *pszCharacter, const CRect &rc, 
		COLORREF textColor, const CFont &font, UINT uiAlignment );

	/**
	*	\brief 将指定区域设定为指定的RGB,注意添加后，只改变画布不马上显示，
	*	\		只在画布中操作，添加后可调用ShowImage()或RedrawRect进行显示，
	*	\		调用显示后即重新计算ShowImage的区域
	*	\param[in] color 指定的RGB值
	*	\param[in] rc 指定的区域（涂颜色的区域），
	*/
	BOOL SetRGBInRect(COLORREF color, const CRect &rc );
	/**
	*	\brief 显示之前调用AddImage添加的图像。显示区域为之前AddImage中的rc的并集。
	*/
	void ShowImage();
	/**
	*	\brief 区域重画。重画当前的区域中的指定位置。
	*	\param[in] rc 指定的重画位置。
	*/
	void RedrawRect( const CRect &rc );


    /**
    *	\brief 将指定CDC的指定（rc）部分提取出来并写到dc中
    *	\param[in] srcDC 源DC
    *	\return FALSE 获取失败.
    */
    BOOL CreateDC(CDC &srcDC , CDC &dc, CBitmap &bitmap, const CRect &rc);
    /**
    *	\brief 将画布的指定（rc）部分提取出来并写到dc中
    *	\return FALSE 获取失败.
    */
    BOOL CreateDC( CDC &dc, CBitmap &bitmap, const CRect &rc);
protected:
	/**
	*	\brief 将图像文件加载并画到内存画布上
	*/
	BOOL AddImage2Mem( const std::wstring &imagPath, const CRect &rc, BOOL bHold = FALSE );

	/**
	*	\brief 将图像文件加载并画到内存画布上
	*	\param[out] rc 图像显示的区域
	*/
	BOOL AddImage2Mem( const std::wstring &imagPat, int x, int y, CRect &rc, BOOL bHold = FALSE );

	/**
	*	\brief 加载图像
	*/
	BOOL LoadFileImage( const TCHAR *pszPath, CxImage* pImage );

protected:
	typedef std::map<std::wstring, CxImage*> MapPath2Image;
	typedef std::map< std::wstring, DWORD > MapSuffix2ImageType;
protected:
	/**
	*	/brief 显示DC句柄
	*/
	HDC m_hDC;
	
	/**
	*	/brief 显示的最大区域、即画布
	*/
	CRect m_rcScreen;
	CDC m_dcSurface;
	CBitmap m_bmpSurface;
	CBitmap *m_pOldBmpSurface;
	/**
	*	/brief 路径与CImage指针的映射。
	*/
	MapPath2Image m_mapPath2Image;
	/**
	*	/brie 文件扩展名到图像类型的映射
	*/
	MapSuffix2ImageType m_mapSuffix2ImageType;


	/**
	*	/brief 保存AddImage添加的图像的区域。此区域在进行图像显示后清除。
	*/
	CRect m_rcTempImage;



	/**
	*	\brief 指明只能一次调用Init
	*/
	bool m_singleton ;

};
#endif
