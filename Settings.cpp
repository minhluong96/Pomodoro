#include "Settings.h"

void LoadRect(HWND hWnd)
{
	//Take window previous rect
	WCHAR *path = new WCHAR[BUFFSIZE];
	WCHAR *buff = new WCHAR[BUFFSIZE];
	long width, height, x, y;

	//Take current path
	GetCurrentDirectory(BUFFSIZE, path);
	wcscat_s(path, BUFFSIZE, L"\\config.ini");
	//Take x
	GetPrivateProfileString(L"app", L"x", L"100", buff, BUFFSIZE, path);
	x = _wtoi(buff);
	//Take y
	GetPrivateProfileString(L"app", L"y", L"100", buff, BUFFSIZE, path);
	y = _wtoi(buff);
	//Take width
	GetPrivateProfileString(L"app", L"width", L"1055", buff, BUFFSIZE, path);
	width = _wtoi(buff);
	//Take height
	GetPrivateProfileString(L"app", L"height", L"855", buff, BUFFSIZE, path);
	height = _wtoi(buff);

	//Set window rect
	SetWindowPos(hWnd, NULL, x, y, width, height, NULL);
	
}

void SaveRect(HWND hWnd)
{
	WCHAR *path = new WCHAR[BUFFSIZE];
	WCHAR *buff = new WCHAR[BUFFSIZE];
	long width, height;

	//Take current path
	GetCurrentDirectory(BUFFSIZE, path);
	wcscat_s(path, BUFFSIZE, L"\\config.ini");

	//Take window rect
	LPRECT temp = new RECT;
	GetWindowRect(hWnd, temp);
	width = temp->right - temp->left;
	height = temp->bottom - temp->top;
	LPPOINT app = new POINT;
	app->x = temp->left;
	app->y = temp->top;

	ClientToScreen(NULL, app);

	swprintf(buff, BUFFSIZE, L"%d", app->x);
	WritePrivateProfileString(L"app", L"x", buff, path);

	swprintf(buff, BUFFSIZE, L"%d", app->y);
	WritePrivateProfileString(L"app", L"y", buff, path);

	swprintf(buff, BUFFSIZE, L"%d", width);
	WritePrivateProfileString(L"app", L"width", buff, path);

	swprintf(buff, BUFFSIZE, L"%d", height);
	WritePrivateProfileString(L"app", L"height", buff, path);
}