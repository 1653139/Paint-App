// Paint_P1.cpp : Defines the entry point for the application.
//
#pragma comment(lib, "ComCtl32.Lib")
#include "stdafx.h"
#include "Paint_P1.h"
#include <commdlg.h>
#include<CommCtrl.h>
#include"Shape.h"
#define MAX_LOADSTRING 100
#define ID_TOOLBAR		1000	// ID of the toolbar
#define IMAGE_WIDTH     18
#define IMAGE_HEIGHT    17
#define BUTTON_WIDTH    0
#define BUTTON_HEIGHT   0
#define TOOL_TIP_MAX_LEN   32
WCHAR text[100];

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR szDrawWindowClass[MAX_LOADSTRING];
HWND hWndMDIClient = NULL;
HWND hFrameWnd;
HWND hText;
HWND  hToolBarWnd;
HWND hChild=NULL;
int currentDraw = ID_DRAW_LINE;
int numWindow = 0;
COLORREF rgbCurrentColor = RGB(0, 0, 0);
LOGFONT fontCurrent;
void onNewDrawWnd(HWND hWnd);
void initFrameWindow(HWND hWnd);
void initChildWindow(HWND hWnd);
void OnChooseColor(HWND hWnd);
void OnChooseFont(HWND hWnd);
void unCheck(HWND hWnd, int menuItem1, int menuItem2, int menuItem3, int menuItem4);
void updateCheck(HWND hWnd, int menuItem);
void onChildPaint(HWND hWnd);
void onMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam, int x1, int y1, int &x2, int &y2);
void onButtonUp(HWND hWnd, LPARAM lParam, int x1, int y1, int &x2, int &y2);


void doCreate_ToolBar(HWND hWnd);
void doToolBar_AddUserButton();
void doToolBar_NotifyHandle(LPARAM	lParam);


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    DrawProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	MDICloseProc(HWND, LPARAM);
INT_PTR CALLBACK    TextDlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PAINTP1, szWindowClass, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_DRAW, szDrawWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINTP1));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	//Fame Class Register
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINTP1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PAINTP1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	if (!RegisterClassExW(&wcex)) return FALSE;

	//Draw child class register
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DrawProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 8;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINTP1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szDrawWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassExW(&wcex)) return FALSE;
	return 1;
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
	case WM_CREATE:
		initFrameWindow(hWnd);
		doCreate_ToolBar(hWnd);
		doToolBar_AddUserButton();
		break;
	case WM_COMMAND:
	{
		HMENU hMenu = GetMenu(hWnd);
		CheckMenuItem(hMenu, ID_DRAW_LINE, MF_BYCOMMAND | MF_CHECKED);
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_NEW:
			onNewDrawWnd(hWnd);
			break;
		case ID_FILE_OPEN:
		{
			
			fstream filein;
			OPENFILENAME  ofn;//  CTDL  dùng  cho  dialog  open
			TCHAR  szFile[256];
			TCHAR  szFilter[] = TEXT("Draw file\0*.drw\0");
			szFile[0] = '\0';//  Khởi  tạo  struct
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;  //  handle  của  window  cha
			ofn.lpstrFilter = szFilter;
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = szFile;  //  chuỗi  tên  file  trả  về
			ofn.nMaxFile = sizeof(szFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn))
			{
				numWindow++;
				MDICREATESTRUCT mdiCreate;
				ZeroMemory(&mdiCreate, sizeof(MDICREATESTRUCT));
				mdiCreate.szClass = szDrawWindowClass;
				mdiCreate.szTitle = ofn.lpstrFile;
				mdiCreate.hOwner = hInst;
				mdiCreate.x = CW_USEDEFAULT;
				mdiCreate.y = CW_USEDEFAULT;
				mdiCreate.cx = CW_USEDEFAULT;
				mdiCreate.cy = CW_USEDEFAULT;
				mdiCreate.style = 0;
				mdiCreate.lParam = NULL;
				SendMessage(hWndMDIClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdiCreate);
				hChild = (HWND)SendMessage(hWndMDIClient, WM_MDIGETACTIVE, (DWORD)hWnd, 0L);
				CHILD_WND_DATA* wndData = (CHILD_WND_DATA*)GetWindowLongPtr(hChild, 0);
				filein.open(ofn.lpstrFile, ios::in | ios::binary);
				filein.read((char*)&wndData->n, sizeof(wndData->n));
				filein.read((char*)&wndData->color, sizeof(wndData->color));
				filein.read((char*)&wndData->font, sizeof(wndData->font));
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hChild, &ps);
				SelectObject(hdc, GetStockObject(NULL_BRUSH));
				if (wndData == NULL)
					return 0;
				for (int i = 0; i < wndData->n; i++)
				{
					wndData->Object[i]->Read(filein);
					if (wndData->Object[i]->type == 0) {
						OBJECT*ob=new LINE;
						ob->Draw(hdc);
						delete ob;
					}

				}
				EndPaint(hChild, &ps);
				
				
			}
			filein.close();
		}
			break;
		case ID_FILE_SAVE:
		{
			hChild = (HWND)SendMessage(hWndMDIClient, WM_MDIGETACTIVE, (DWORD)hWnd, 0L);
			CHILD_WND_DATA* wndData = (CHILD_WND_DATA*)GetWindowLongPtr(hChild, 0);
			fstream fileout;
			OPENFILENAME  ofn;//  CTDL  dùng  cho  dialog  open
			TCHAR msg[256];
			wsprintf(msg, L"Noname-%d.drw", numWindow);
			TCHAR  szFile[256] ;
			
			TCHAR  szFilter[] = TEXT("Draw file\0*.drw\0");
			szFile[0] = '\0';//  Khởi  tạo  struct
			wcsncpy_s(szFile, msg, 256);
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;  //  handle  của  window  cha
			ofn.lpstrFilter = szFilter;
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = szFile;  //  chuỗi  tên  file  trả  về
			ofn.nMaxFile = sizeof(szFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetSaveFileName(&ofn))
			{
				
				fileout.open(ofn.lpstrFile, ios::out | ios::binary);
				fileout.write((char*)&wndData->n, sizeof(wndData->n));
				fileout.write((char*)&wndData->color, sizeof(wndData->color));
				fileout.write((char*)&wndData->font, sizeof(wndData->font));
				if (wndData == NULL)
					return 0;
				for (int i = 0; i < wndData->n; i++)
					wndData->Object[i]->Write(fileout);
				
			}
			fileout.close();
		}
			break;
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_DRAW_COLOR:
			OnChooseColor(hWnd);
			break;
		case ID_DRAW_FONT:
			OnChooseFont(hWnd);
			break;
		case ID_DRAW_LINE:
			updateCheck(hWnd, ID_DRAW_LINE);
			unCheck(hWnd, ID_DRAW_RECTANGLE, ID_DRAW_ELLIPSE, ID_DRAW_TEXT, ID_DRAW_SEC);
			currentDraw = ID_DRAW_LINE;
			break;
		case ID_DRAW_RECTANGLE:
			updateCheck(hWnd, ID_DRAW_RECTANGLE);
			unCheck(hWnd, ID_DRAW_LINE, ID_DRAW_ELLIPSE, ID_DRAW_TEXT, ID_DRAW_SEC);
			currentDraw = ID_DRAW_RECTANGLE;
			break;
		case ID_DRAW_ELLIPSE:
			updateCheck(hWnd, ID_DRAW_ELLIPSE);
			unCheck(hWnd, ID_DRAW_LINE, ID_DRAW_RECTANGLE, ID_DRAW_TEXT, ID_DRAW_SEC);
			currentDraw = ID_DRAW_ELLIPSE;
			break;
		case ID_DRAW_TEXT:
			updateCheck(hWnd, ID_DRAW_TEXT);
			unCheck(hWnd, ID_DRAW_LINE, ID_DRAW_ELLIPSE, ID_DRAW_RECTANGLE, ID_DRAW_SEC);
			currentDraw = ID_DRAW_TEXT;
		
			break;
		case ID_DRAW_SEC:
			updateCheck(hWnd, ID_DRAW_SEC);
			unCheck(hWnd, ID_DRAW_LINE, ID_DRAW_ELLIPSE, ID_DRAW_TEXT, ID_DRAW_RECTANGLE);
			break;
		case ID_WINDOW_TIDE:
			SendMessage(hWndMDIClient, WM_MDITILE, MDITILE_HORIZONTAL, 0L);
			break;
		case ID_WINDOW_CASCADE:
			SendMessage(hWndMDIClient, WM_MDICASCADE, 0L, 0L);
			break;
		case ID_WINDOW_CLOSE:
			EnumChildWindows(hWndMDIClient, (WNDENUMPROC)MDICloseProc, 0L);
			break;
		default:
			return DefFrameProc(hWnd, hWndMDIClient, message, wParam, lParam);
		}
	}



	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);

	}
	break;

	case WM_SIZE:
		UINT w, h;

		w = LOWORD(lParam);
		h = HIWORD(lParam);
		MoveWindow(hWndMDIClient, 0, 30, w, h - 30, TRUE);
		break;
	case WM_NOTIFY:
		doToolBar_NotifyHandle(lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefFrameProc(hWnd, hWndMDIClient, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK DrawProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CHILD_WND_DATA* wndData = (CHILD_WND_DATA*)GetWindowLongPtr(hWnd, 0);
	static int x1, y1, x2, y2;
	switch (message)
	{
	case WM_CREATE:
		initChildWindow(hWnd);

		break;
	case WM_MDIACTIVATE:
	{
		HMENU hMenu = GetMenu(hFrameWnd);
		if (lParam == (LPARAM)hWnd)
		{
			EnableMenuItem(hMenu, 1, MF_ENABLED | MF_BYPOSITION);
		}
		DrawMenuBar(hFrameWnd);
	}
	break;
	case WM_PAINT:
		onChildPaint(hWnd);
		return 0;
	case WM_LBUTTONDOWN:
		x1 = x2 = LOWORD(lParam);
		y1 = y2 = HIWORD(lParam);

		if (currentDraw == ID_DRAW_TEXT)
		{
			HDC hdc = GetDC(hWnd);
			HFONT hFont = CreateFontIndirect(&fontCurrent);
			SelectObject(hdc, hFont);
			SetTextColor(hdc, rgbCurrentColor);
			MYTEXT*t = new MYTEXT;
			t->left = x1;
			t->top = y1;
			t->color = rgbCurrentColor;
			t->font = fontCurrent;
			INT_PTR returnPtr;
			returnPtr = DialogBox(hInst, MAKEINTRESOURCE(IDD_TEXT), hWnd, TextDlgProc);

			TextOut(hdc, x1, y1, text, wcsnlen(text,100));
		
			wcsncpy_s(t->str,text,100);
			wndData->Object[wndData->n] = (OBJECT*)t;
			wndData->n++;

		}
		break;


	case WM_MOUSEMOVE:
		onMouseMove(hWnd, wParam, lParam, x1, y1, x2, y2);
		break;
	case WM_LBUTTONUP:
		onButtonUp(hWnd, lParam, x1, y1, x2, y2);
		break;

	case WM_CLOSE:
		SendMessage(hWndMDIClient, WM_MDIDESTROY, (WPARAM)hWnd, 0L);
		break;
	default:
		return DefMDIChildProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
INT_PTR CALLBACK TextDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{ 
	
	switch (message)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hDlg, IDC_TEXT));
		EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
		return (INT_PTR)FALSE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_TEXT:
			if (HIWORD(wParam) == EN_CHANGE) {
				LRESULT nTextLen = SendMessage((HWND)lParam, WM_GETTEXTLENGTH, 0, 0L);
				EnableWindow(GetDlgItem(hDlg, IDOK), (nTextLen != 0) ? TRUE : FALSE);
			
			}
			break;
		case IDOK:
			
			GetDlgItemText(hDlg, IDC_TEXT, text, 100);
			EndDialog(hDlg, (INT_PTR)TRUE);
			break;
		}
		return (INT_PTR)TRUE;

	}
	return (INT_PTR)FALSE;

}
LRESULT CALLBACK MDICloseProc(HWND hChildWnd,
	LPARAM lParam)
{
	SendMessage(hWndMDIClient, WM_MDIDESTROY, (WPARAM)hChildWnd, 0L);
	return 1;
}
void initFrameWindow(HWND hWnd)
{
	hFrameWnd = hWnd;
	//create the MDI Client Window
	CLIENTCREATESTRUCT css;
	css.hWindowMenu = GetSubMenu(GetMenu(hWnd), 2);
	css.idFirstChild = 50000;
	hWndMDIClient = CreateWindowW(L"MDICLIENT", LPCTSTR(NULL), WS_CHILD | WS_CLIPCHILDREN, 0, 0, 0, 0, hWnd, (HMENU)NULL, hInst, (LPVOID)&css);
	ShowWindow(hWndMDIClient, SW_SHOW);
	HMENU hMenu = GetMenu(hWnd);
	EnableMenuItem(hMenu, 1, MF_GRAYED | MF_BYPOSITION);


}
void initChildWindow(HWND hWnd)
{
	CHILD_WND_DATA *wndData;
	wndData = (CHILD_WND_DATA*)VirtualAlloc(NULL, sizeof(CHILD_WND_DATA), MEM_COMMIT, PAGE_READWRITE);
	wndData->color = RGB(0, 0, 0);
	ZeroMemory(&(wndData->font), sizeof(LOGFONT));
	wndData->font.lfHeight = 20;
	wcscpy_s(wndData->font.lfFaceName, LF_FACESIZE, L"Arial");
	wndData->n = 0;
	SetLastError(0);
	if (SetWindowLongPtr(hWnd, 0, (LONG_PTR)wndData))
		if (GetLastError() != 0)
		{
			MessageBox(hWnd, L"Cannot init data pointer of window.", L"NOTICE", MB_OK);
		}

}

void onNewDrawWnd(HWND hWnd)
{
	numWindow++;
	TCHAR msg[256];
	wsprintf(msg, L"Noname-%d.drw", numWindow);
	MDICREATESTRUCT mdiCreate;
	ZeroMemory(&mdiCreate, sizeof(MDICREATESTRUCT));
	mdiCreate.szClass = szDrawWindowClass;
	mdiCreate.szTitle = msg;
	mdiCreate.hOwner = hInst;
	mdiCreate.x = CW_USEDEFAULT;
	mdiCreate.y = CW_USEDEFAULT;
	mdiCreate.cx = CW_USEDEFAULT;
	mdiCreate.cy = CW_USEDEFAULT;
	mdiCreate.style = 0;
	mdiCreate.lParam = NULL;
	SendMessage(hWndMDIClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdiCreate);
}

void OnChooseColor(HWND hWnd)
{
	CHOOSECOLOR  cc;//  CTDL  dùng  cho  dialog  Choose Color
	COLORREF  acrCustClr[16]; //  Các  màu  do  user  định  nghĩa
	DWORD  rgbCurrent = RGB(255, 0, 0);  //  màu  được  chọn  default 
										 //  Khởi  tạo  struct
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;  //  handle  của  window  cha
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;  //  trả  về  màu  được  chọn
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc))
	{  //  xử  lý  màu  được  chọn, vd.  tạo  brush


		rgbCurrentColor = cc.rgbResult;
	}



}

void OnChooseFont(HWND hWnd)
{
	CHOOSEFONT  cf;//  CTDL  dùng  cho  dialog  ChooseFont
	LOGFONT  lf;                //  CTDL  font,  lưu  kết  quả  font  được  chọn
	HFONT  hfNew, hfOld;
	COLORREF color;
	//  Khởi  tạo  struct
	ZeroMemory(&cf, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = hWnd;  //  handle  của  window  cha
	cf.lpLogFont = &lf;
	
	cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
	if (ChooseFont(&cf))
	{
		fontCurrent= lf;
		rgbCurrentColor = cf.rgbColors;
		/*fontCurrent = CreateFontIndirect(cf.lpLogFont);*/
	}
}

void updateCheck(HWND hWnd, int menuItem)
{
	HMENU hMenu = GetMenu(hWnd);
	int nFlag;
	if (GetMenuState(hMenu, menuItem, MF_BYCOMMAND)& MF_CHECKED)
		nFlag = MF_UNCHECKED;
	else nFlag = MF_CHECKED;
	CheckMenuItem(hMenu, menuItem, MF_BYCOMMAND | nFlag);
}
void unCheck(HWND hWnd, int menuItem1, int menuItem2, int menuItem3, int menuItem4)
{
	HMENU hMenu = GetMenu(hWnd);
	CheckMenuItem(hMenu, menuItem1, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, menuItem2, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, menuItem3, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, menuItem4, MF_BYCOMMAND | MF_UNCHECKED);

}
void onChildPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	CHILD_WND_DATA* wndData = (CHILD_WND_DATA*)GetWindowLongPtr(hWnd, 0);
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	if (wndData == NULL)
		return;
	for (int i = 0; i < wndData->n; i++)
		wndData->Object[i]->Draw(hdc);
	EndPaint(hWnd, &ps);

}

void onMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam, int x1, int y1, int &x2, int &y2) {

	if (!(wParam  &  MK_LBUTTON))  return;
	HPEN hPen = CreatePen(PS_SOLID, 1, rgbCurrentColor);
	HDC  dc = GetDC(hWnd);//  xóa  đường  cũ
	SelectObject(dc, hPen);
	switch (currentDraw)
	{
	case ID_DRAW_LINE: {
		SetROP2(dc, R2_NOTXORPEN);
		MoveToEx(dc, x1, y1, NULL);
		LineTo(dc, x2, y2);//  vẽ  đường  mới
		x2 = LOWORD(lParam);
		y2 = HIWORD(lParam);
		MoveToEx(dc, x1, y1, NULL);
		LineTo(dc, x2, y2);
		break;
	}
	case ID_DRAW_RECTANGLE: {
		SetROP2(dc, R2_NOTXORPEN);
		//MoveToEx(dc, x1, y1, NULL);
		Rectangle(dc, x1, y1, x2, y2);//  vẽ  đường  mới
		x2 = LOWORD(lParam);
		y2 = HIWORD(lParam);
		MoveToEx(dc, x1, y1, NULL);
		Rectangle(dc, x1, y1, x2, y2);
		break;
	}
	case ID_DRAW_ELLIPSE: {
		SetROP2(dc, R2_NOTXORPEN);
		//MoveToEx(dc, x1, y1, NULL);
		Ellipse(dc, x1, y1, x2, y2);//  vẽ  đường  mới
		x2 = LOWORD(lParam);
		y2 = HIWORD(lParam);
		MoveToEx(dc, x1, y1, NULL);
		Ellipse(dc, x1, y1, x2, y2);
		break;
	}
	case ID_DRAW_TEXT: {

		break;
	}
	}
	DeleteObject(hPen);
	ReleaseDC(hWnd, dc);
}
void onButtonUp(HWND hWnd, LPARAM lParam, int x1, int y1, int &x2, int &y2) {

	CHILD_WND_DATA* wndData = (CHILD_WND_DATA*)GetWindowLongPtr(hWnd, 0);
	wndData->color = rgbCurrentColor;
	if (wndData == NULL)
		return;
	switch (currentDraw)
	{
	case ID_DRAW_LINE: {
		LINE*l = new LINE;
		l->left = x1;
		l->top = y1;
		l->right = x2;
		l->bottom = y2;
		l->color = wndData->color;
		wndData->Object[wndData->n] = (OBJECT*)l;
		wndData->n++;
		break;
	}
	case ID_DRAW_RECTANGLE: {
		RECTANGLE*r = new RECTANGLE;
		r->left = x1;
		r->top = y1;
		r->right = x2;
		r->bottom = y2;
		r->color = wndData->color;
		wndData->Object[wndData->n] = (OBJECT*)r;
		wndData->n++;
		break;
	}
	case ID_DRAW_ELLIPSE: {
		ELLIPSE*e = new ELLIPSE;
		e->left = x1;
		e->top = y1;
		e->right = x2;
		e->bottom = y2;
		e->color = wndData->color;
		wndData->Object[wndData->n] = (OBJECT*)e;
		wndData->n++;
		break;
	}

	}
}




void doCreate_ToolBar(HWND hWnd)
{
	// loading Common Control DLL
	InitCommonControls();

	TBBUTTON tbButtons[] =
	{
		// Zero-based Bitmap image, ID of command, Button state, Button style, 
		// ...App data, Zero-based string (Button's label)
		{ STD_FILENEW,	ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ STD_FILEOPEN,	ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ STD_FILESAVE,	ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },

	};

	// create a toolbar
	hToolBarWnd = CreateToolbarEx(hWnd,
		WS_CHILD | WS_VISIBLE | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS,
		ID_TOOLBAR,
		sizeof(tbButtons) / sizeof(TBBUTTON),
		HINST_COMMCTRL,
		0,
		tbButtons,
		sizeof(tbButtons) / sizeof(TBBUTTON),
		BUTTON_WIDTH,
		BUTTON_HEIGHT,
		IMAGE_WIDTH,
		IMAGE_HEIGHT,
		sizeof(TBBUTTON));

}



void doToolBar_NotifyHandle(LPARAM	lParam)
{
	LPTOOLTIPTEXT   lpToolTipText;
	WCHAR			szToolTipText[TOOL_TIP_MAX_LEN]; 	// ToolTipText, loaded from Stringtable resource

														// lParam: address of TOOLTIPTEXT struct
	lpToolTipText = (LPTOOLTIPTEXT)lParam;

	if (lpToolTipText->hdr.code == TTN_NEEDTEXT)
	{
		// hdr.iFrom: ID cua ToolBar button -> ID cua ToolTipText string
		LoadString(hInst, lpToolTipText->hdr.idFrom, szToolTipText, TOOL_TIP_MAX_LEN);

		lpToolTipText->lpszText = szToolTipText;
	}
}
void doToolBar_AddUserButton()
{
	// define new buttons
	TBBUTTON tbButtons[] =
	{
		{ 0, 0,	TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 0, ID_DRAW_LINE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 1, ID_DRAW_RECTANGLE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 2, ID_DRAW_ELLIPSE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 3, ID_DRAW_TEXT,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 4, ID_DRAW_SEC,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
	};

	// structure contains the bitmap of user defined buttons. It contains 2 icons
	TBADDBITMAP	tbBitmap = { hInst, IDB_DRAW };

	// Add bitmap to Image-list of ToolBar
	int idx = SendMessage(hToolBarWnd, TB_ADDBITMAP, (WPARAM) sizeof(tbBitmap) / sizeof(TBADDBITMAP),
		(LPARAM)(LPTBADDBITMAP)&tbBitmap);

	// identify the bitmap index of each button
	tbButtons[1].iBitmap += idx;
	tbButtons[2].iBitmap += idx;
	tbButtons[3].iBitmap += idx;
	tbButtons[4].iBitmap += idx;
	tbButtons[5].iBitmap += idx;
	// add buttons to toolbar
	SendMessage(hToolBarWnd, TB_ADDBUTTONS, (WPARAM) sizeof(tbButtons) / sizeof(TBBUTTON),
		(LPARAM)(LPTBBUTTON)&tbButtons);
}
