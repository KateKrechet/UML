#include<Windows.h>
#include<stdio.h>
#include "resource.h"

const CHAR g_szCLASS_NAME[] = "MyWindowClass";
CONST CHAR g_szTitle[] = "My first MainWindow";

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1)Регистрация класса окна
	WNDCLASSEX wc;//wc - Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;//Указатель на процедуру окна.lp-long pointer,fn-function
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	//wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_FLOWER));
	//wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_SUN));
	wc.hIcon = (HICON)LoadImage(hInstance, "umbrella.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInstance, "moon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_SC));
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = g_szCLASS_NAME;
	wc.lpszMenuName = NULL;
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration faled!", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) Создание окна
	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);
	int window_width = screen_width - screen_width / 4;
	int window_height = screen_height - screen_height / 4;
	int window_start_x = screen_width / 8;
	int window_start_y = screen_height / 8;

	HWND hwnd = CreateWindowEx
	(
		WS_EX_CLIENTEDGE,//exStyle
		g_szCLASS_NAME,//имя класса окна
		g_szTitle,//Заголовок окна
		WS_OVERLAPPEDWINDOW,//Стиль окна OVERLAPPEDWINDOW всегда задается для главного окна программы
		window_start_x, window_start_y,//Позиция окна на экране
		window_width, window_height,//Размер окна
		NULL,//Род окно
		NULL,//Меню отсутствует
		hInstance,//*.exe - файл окна
		NULL


	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);//Задаем режим отображения окна
	UpdateWindow(hwnd);//Прортсовка окна(отправляем сообщение WM_PAINT)
	//3)Запускаем цикл сообщений
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}



	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE://Здесь создаются элементы окна
		break;
	case WM_COMMAND://Обработка команд нажатия кнопок и др эл окна
		break;
	case WM_SIZE:
	case WM_MOVE:
	{
		RECT rect;
		GetWindowRect(hwnd,&rect);
		CONST INT SIZE = 256;
		CHAR buffer[SIZE]{};
		SendMessage(hwnd, WM_GETTEXT, SIZE, (LPARAM)buffer);
		sprintf(buffer, "%s Position: %dx%d,Size: %dx%d",
			g_szTitle,
			rect.left, rect.top,//позиция окна
			rect.right - rect.left, rect.bottom - rect.top
		);
			//%s - строка
			//%d - decimal - десятичное число
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)buffer);
	}
	break;
	case WM_CLOSE:
		switch (MessageBox(hwnd, "Вы действительно хотите закрыть окно?",
			"Действительно?", MB_YESNO | MB_ICONQUESTION))
		{
		case IDYES:
			//MessageBox(hwnd, "Лучше б двери закрыли:-)", "Info", MB_OK | MB_ICONERROR);
			DestroyWindow(hwnd); break;
			//case IDNO:MessageBox(hwnd, "Это хорошо", "Info", MB_OK | MB_ICONINFORMATION); break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}