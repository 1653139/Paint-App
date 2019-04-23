#include "stdafx.h"
#include "Shape.h"

void LINE::Draw(HDC hdc)
{
	HPEN oPen, pen;
	pen = CreatePen(PS_SOLID, 1, color);
	oPen = (HPEN)SelectObject(hdc, pen);
	MoveToEx(hdc, left, top,NULL);
	LineTo(hdc, right, bottom);
	SelectObject(hdc, oPen);
	DeleteObject(pen);

}

void LINE::Write(fstream & file)
{
	type = 0;
	file.write((char*)&type, sizeof(type));
	file.write((char*)&color, sizeof(color));
	file.write((char*)&left, sizeof(left));
	file.write((char*)&top, sizeof(top));
	file.write((char*)&right, sizeof(right));
	file.write((char*)&bottom, sizeof(bottom));
}

void LINE::Read(fstream & file)
{
	file.read((char*)&type, sizeof(type));
	file.read((char*)&color, sizeof(color));
	file.read((char*)&left, sizeof(left));
	file.read((char*)&top, sizeof(top));
	file.read((char*)&right, sizeof(right));
	file.read((char*)&bottom, sizeof(bottom));
}

void RECTANGLE::Draw(HDC hdc)
{
	HPEN oPen, pen;
	pen = CreatePen(PS_SOLID, 1, color);
	oPen = (HPEN)SelectObject(hdc, pen);
	Rectangle(hdc, left, top, right, bottom);
	SelectObject(hdc, oPen);
	DeleteObject(pen);


}

void RECTANGLE::Write(fstream & file)
{
	type = 1;
	file.write((char*)&type, sizeof(type));
	file.write((char*)&color, sizeof(color));
	file.write((char*)&left, sizeof(left));
	file.write((char*)&top, sizeof(top));
	file.write((char*)&right, sizeof(right));
	file.write((char*)&bottom, sizeof(bottom));
}

void RECTANGLE::Read(fstream & file)
{
	file.read((char*)&type, sizeof(type));
	file.read((char*)&color, sizeof(color));
	file.read((char*)&left, sizeof(left));
	file.read((char*)&top, sizeof(top));
	file.read((char*)&right, sizeof(right));
	file.read((char*)&bottom, sizeof(bottom));
}

void ELLIPSE::Draw(HDC hdc)
{
	HPEN oPen, pen;
	pen = CreatePen(PS_SOLID, 1, color);
	oPen = (HPEN)SelectObject(hdc, pen);
	Ellipse(hdc, left, top, right, bottom);
	SelectObject(hdc, oPen);
	DeleteObject(pen);

}

void ELLIPSE::Write(fstream & file)
{
	type = 2;
	file.write((char*)&type, sizeof(type));
	file.write((char*)&color, sizeof(color));
	file.write((char*)&left, sizeof(left));
	file.write((char*)&top, sizeof(top));
	file.write((char*)&right, sizeof(right));
	file.write((char*)&bottom, sizeof(bottom));
}

void ELLIPSE::Read(fstream & file)
{
	file.read((char*)&type, sizeof(type));
	file.read((char*)&color, sizeof(color));
	file.read((char*)&left, sizeof(left));
	file.read((char*)&top, sizeof(top));
	file.read((char*)&right, sizeof(right));
	file.read((char*)&bottom, sizeof(bottom));
}

void MYTEXT::Draw(HDC hdc)
{
	HFONT hFont = CreateFontIndirect(&font);
	SelectObject(hdc,hFont);
	SetTextColor(hdc, color);
	TextOut(hdc, left, top, str, wcsnlen(str, 100));
	DeleteObject(hFont);

}

void MYTEXT::Write(fstream & file)
{
	type = 3;
	file.write((char*)&type, sizeof(type));
	file.write((char*)&color, sizeof(color));
	file.write((char*)&font, sizeof(font));
	file.write((char*)&left, sizeof(left));
	file.write((char*)&top, sizeof(top));
	file.write((char*)&right, sizeof(right));
	file.write((char*)&bottom, sizeof(bottom));
	file.write((char*)&str, sizeof(str));
}

void MYTEXT::Read(fstream & file)
{
	file.read((char*)&type, sizeof(type));
	file.read((char*)&color, sizeof(color));
	file.read((char*)&font, sizeof(font));
	file.read((char*)&left, sizeof(left));
	file.read((char*)&top, sizeof(top));
	file.read((char*)&right, sizeof(right));
	file.read((char*)&bottom, sizeof(bottom));
	file.read((char*)&str, sizeof(str));
}
