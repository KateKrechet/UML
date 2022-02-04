#include<Windows.h>
#include"resource.h"

//3 типа окон:главное, диалоговое окно-сообщение(сохранить, напр)
//#define MESSAGE_BOX
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR IpCmdLine, int nCmdShow)
{
#ifdef MESSAGE_BOX
	MessageBox(NULL, "Hello World!", "Info", MB_YESNOCANCEL | MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_HELP | MB_DEFBUTTON2 |
		MB_RIGHT | MB_SETFOREGROUND);
#endif // MESSAGE_BOX

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc, 0);

	return 0;
}
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:MessageBox(NULL, "Была нажата кнопка ОК", "Info", MB_ICONINFORMATION); break;
		case IDCANCEL:EndDialog(hwnd, 0); break;

		}

	}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}