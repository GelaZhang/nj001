#ifndef __ZY_SURFACE_H_
#define __ZY_SURFACE_H_

/**
*	\file Surface.h
*	\brief  �ڴ桢�ļ�ͼ����ʾ��
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
	*	\brief ʹ��������캯���������Init����
	*/
	CSurface( );
	void Init( HDC hDC, const CRect &rc );

	~CSurface();
	/**
	*	\brief �����ļ�ͼ��һ������ʾ��
	*	\param[in] arrImgPath ͼ���ļ���·��
	*	\param[in] arrRect ͼ�����ʾ����
	*	\param[in] arrHold ����Ƿ��ļ�ͼ�񻺴�
	*/
	BOOL ShowImage( const std::vector<std::wstring> &arrImgPath, 
		const std::vector<CRect> &arrRect, 
		const std::vector<BOOL> &arrHold );
	/**
	*	\brief ��ʾһ���ļ�ͼ��
	*	\param[in] cstrImagPath ͼ���ļ���·��
	*	\param[in] rc ͼ�����ʾ����
	*	\param[in] bHold ����Ƿ��ļ�ͼ�񻺴�
	*/
	BOOL ShowImage( const std::wstring &imagPath, const CRect &rc, BOOL bHold = FALSE );
	/**
	*	\brief ���һ���ļ�ͼ�񵽻�����ע����Ӻ���ʾ��
	*	\	   ��Ӻ�ɵ���ShowImage()��RedrawRect������ʾ��������ʾ�����¼���ShowImage������
	*	\param[in] cstrImagPath ͼ���ļ���·��
	*	\param[in] rc ͼ�����ʾ����
	*	\param[in] bHold ����Ƿ��ļ�ͼ�񻺴�
	*/
	BOOL AddImage( const std::wstring &imagPath, const CRect &rc, BOOL bHold = FALSE);

	/**
	*	\brief ���һ���ļ�ͼ�񵽻�����ע����Ӻ���ʾ��
	*	\	   ��Ӻ�ɵ���ShowImage()��RedrawRect������ʾ��������ʾ�����¼���ShowImage������
	*	\param[in] cstrImagPath ͼ���ļ���·��
	*	\param[in] x ͼ�����ʾ����x�����꣬
	*	\param[in] y ͼ�����ʾ����y�����꣬
	*	\param[in] cx ��ʾ����x���С
	*	\param[in] cy ��ʾ����y���С
	*	\param[in] bHold ����Ƿ��ļ�ͼ�񻺴�
	*/
	BOOL AddImage( const std::wstring &imagPath, int x, int y, int cx, int cy, BOOL bHold = FALSE);

	/**
	*	\brief ���һ���ļ�ͼ�񵽻�����ע����Ӻ���ʾ��ֻ��ʾͼ��ʵ�ʴ�С��
	*	\	   ��Ӻ�ɵ���ShowImage()��RedrawRect������ʾ��
	*	\      ������ʾ�����¼���ShowImage������
	*	\param[in] cstrImagPath ͼ���ļ���·��
	*	\param[in] x ͼ�����ʾ����x�����꣬
	*	\param[in] y ͼ�����ʾ����y�����꣬
	*	\param[in] bHold ����Ƿ��ļ�ͼ�񻺴�
	*/
	BOOL AddImage( const std::wstring &imagPath, int x, int y, BOOL bHold = FALSE);

	/**
	*	\brief ���һ���ļ�ͼ�񵽻�����ע����Ӻ���ʾ��ֻ��ʾͼ��ʵ�ʴ�С��
	*	\	   ��TransparentBlt����һ��.
	*	\	   ��Ӻ�ɵ���ShowImage()��RedrawRect������ʾ��
	*	\      ������ʾ�����¼���ShowImage������
	*	\param[in] cstrImagPath ͼ���ļ���·��
	*	\param[in] x ͼ�����ʾ����x�����꣬
	*	\param[in] y ͼ�����ʾ����y�����꣬
	*	\param[in] bHold ����Ƿ��ļ�ͼ�񻺴�
	*/
	BOOL AddImage( const std::wstring &imagPath, int x, int y, COLORREF crTransparent ,BOOL bHold = FALSE);

	/**
	*	\brief ���һ���ļ�ͼ�񵽻�����ע����Ӻ���ʾ��
	*	\	   ��Ӻ�ɵ���ShowImage()��RedrawRect������ʾ��������ʾ�����¼���ShowImage������
	*	\      ��֧��������ʾ
	*	\param[in] pdcMemImage ͼ���ڴ�DC
	*	\param[in] rc ͼ�����ʾ����
	*/
	BOOL AddImage( HDC hDcMemImage, const CRect &rc );

	/**
	*	\brief ���һ���ļ�ͼ�񵽻�����ע����Ӻ���ʾ��
	*	\	   ��Ӻ�ɵ���ShowImage()��RedrawRect������ʾ��������ʾ�����¼���ShowImage������
	*	\      ��֧��������ʾ
	*	\param[in] pdcMemImage ͼ���ڴ�DC
	*	\param[in] x ͼ�����ʾ����x�����꣬
	*	\param[in] y ͼ�����ʾ����y�����꣬
	*	\param[in] cx ��ʾ����x���С
	*	\param[in] cy ��ʾ����y���С
	*/
	BOOL AddImage( HDC hDcMemImage, int x, int y, int cx, int cy );

	/**
	*	\brief ���һ�������ֵ�������ע����Ӻ�ֻ�ı仭����������ʾ��
	*	\	   ��Ӻ�ɵ���ShowImage()��RedrawRect������ʾ��������ʾ�����¼���ShowImage������
	*	\      ��֧��������ʾ
	*	\param[in] pszCharacter ����
	*	\param[in] rc ���ֵ���ʾ����
	*	\param[in] uiAlignment ���뷽ʽ����CDC��DrawText���һ������һ�¡�
	*/
	BOOL AddCharacter( const TCHAR *pszCharacter, const CRect &rc, 
		COLORREF textColor, const CFont &font, UINT uiAlignment );

	/**
	*	\brief ��ָ�������趨Ϊָ����RGB,ע����Ӻ�ֻ�ı仭����������ʾ��
	*	\		ֻ�ڻ����в�������Ӻ�ɵ���ShowImage()��RedrawRect������ʾ��
	*	\		������ʾ�����¼���ShowImage������
	*	\param[in] color ָ����RGBֵ
	*	\param[in] rc ָ��������Ϳ��ɫ�����򣩣�
	*/
	BOOL SetRGBInRect(COLORREF color, const CRect &rc );
	/**
	*	\brief ��ʾ֮ǰ����AddImage��ӵ�ͼ����ʾ����Ϊ֮ǰAddImage�е�rc�Ĳ�����
	*/
	void ShowImage();
	/**
	*	\brief �����ػ����ػ���ǰ�������е�ָ��λ�á�
	*	\param[in] rc ָ�����ػ�λ�á�
	*/
	void RedrawRect( const CRect &rc );


    /**
    *	\brief ��ָ��CDC��ָ����rc��������ȡ������д��dc��
    *	\param[in] srcDC ԴDC
    *	\return FALSE ��ȡʧ��.
    */
    BOOL CreateDC(CDC &srcDC , CDC &dc, CBitmap &bitmap, const CRect &rc);
    /**
    *	\brief ��������ָ����rc��������ȡ������д��dc��
    *	\return FALSE ��ȡʧ��.
    */
    BOOL CreateDC( CDC &dc, CBitmap &bitmap, const CRect &rc);
protected:
	/**
	*	\brief ��ͼ���ļ����ز������ڴ滭����
	*/
	BOOL AddImage2Mem( const std::wstring &imagPath, const CRect &rc, BOOL bHold = FALSE );

	/**
	*	\brief ��ͼ���ļ����ز������ڴ滭����
	*	\param[out] rc ͼ����ʾ������
	*/
	BOOL AddImage2Mem( const std::wstring &imagPat, int x, int y, CRect &rc, BOOL bHold = FALSE );

	/**
	*	\brief ����ͼ��
	*/
	BOOL LoadFileImage( const TCHAR *pszPath, CxImage* pImage );

protected:
	typedef std::map<std::wstring, CxImage*> MapPath2Image;
	typedef std::map< std::wstring, DWORD > MapSuffix2ImageType;
protected:
	/**
	*	/brief ��ʾDC���
	*/
	HDC m_hDC;
	
	/**
	*	/brief ��ʾ��������򡢼�����
	*/
	CRect m_rcScreen;
	CDC m_dcSurface;
	CBitmap m_bmpSurface;
	CBitmap *m_pOldBmpSurface;
	/**
	*	/brief ·����CImageָ���ӳ�䡣
	*/
	MapPath2Image m_mapPath2Image;
	/**
	*	/brie �ļ���չ����ͼ�����͵�ӳ��
	*/
	MapSuffix2ImageType m_mapSuffix2ImageType;


	/**
	*	/brief ����AddImage��ӵ�ͼ������򡣴������ڽ���ͼ����ʾ�������
	*/
	CRect m_rcTempImage;



	/**
	*	\brief ָ��ֻ��һ�ε���Init
	*/
	bool m_singleton ;

};
#endif
