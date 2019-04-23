#pragma once
#include <Windows.h>
#include"resource.h"
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

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR szDrawWindowClass[MAX_LOADSTRING];
HWND hWndMDIClient = NULL;
HWND hFrameWnd;
HWND hText;
HWND  hToolBarWnd;
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
void imputText(HWND hWnd, int left, int top);
void doCreate_ToolBar(HWND hWnd);
void doToolBar_AddUserButton();
void doToolBar_NotifyHandle(LPARAM	lParam);

