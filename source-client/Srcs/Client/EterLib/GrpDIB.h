#pragma once

class CGraphicDib
{
	public:
		CGraphicDib();
		virtual ~CGraphicDib();

		void Destroy();
		bool Create(HDC hDC, int width, int height);

		void SetBkMode(int iBkMode);
		void TextOut(int ix, int iy, const char * c_szText);
		void Put(HDC hDC, int x, int y);

		int GetWidth();
		int GetHeight();

		void* GetPointer();

		HDC GetDCHandle();

	protected:
		void Initialize();

	protected:
		HDC			m_hDC;
		HBITMAP		m_hBmp;
		BITMAPINFO	m_bmi;

		int			m_width;
		int			m_height;

		void *		m_pvBuf;
};
//martysama0134's 2e58d0b8baeb072acdf3afc4a5d1999f
