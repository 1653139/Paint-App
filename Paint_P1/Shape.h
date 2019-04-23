#pragma once
#include<fstream>
#include <Windows.h>
#define MAX_TEXT_LENGTH 100
#define MAX_OBJECT       200

using namespace std;
class OBJECT {
public:
	int left, top, right, bottom;
	int type;
	COLORREF color;
	virtual void Draw(HDC hdc) = 0;
	virtual void Write(fstream &file) = 0;
	virtual void Read(fstream &file) = 0;

};
class LINE :public OBJECT {
	void Draw(HDC hdc);
	void Write(fstream &file);
	void Read(fstream &file);
};
class RECTANGLE :public OBJECT {
	void Draw(HDC hdc);
	void Write(fstream &file);
	void Read(fstream &file);
};
class ELLIPSE : public OBJECT {
	void Draw(HDC hdc);
	void Write(fstream &file);
	void Read(fstream &file);
};
class MYTEXT :public OBJECT {
public:
		LOGFONT font;
		WCHAR str[MAX_TEXT_LENGTH];
private:
	void Draw(HDC hdc);
	void Write(fstream &file);
	void Read(fstream &file);
};
class CHILD_WND_DATA {
public:
	COLORREF color;
	LOGFONT font;
	OBJECT* Object[MAX_OBJECT];
	int n;
	
};
