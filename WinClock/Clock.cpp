#include <Windows.h>
#include <string>

int idTimer = -1;

SYSTEMTIME GetTimeStruct();

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// WinMain
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	const wchar_t CLASS_NAME[] = L"WINCLOCK CLASS";

	// Create window class 
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpszClassName = CLASS_NAME;

	//Register the above class
	RegisterClass(&wc);

	// Create window
	HWND hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Windows Clock",
		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// If Creation fails return
	if (hWnd == NULL) {
		return 0;
	}

	// Show Window
	ShowWindow(hWnd, nCmdShow);

	// Message Loop
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

// Get Time
SYSTEMTIME GetTimeStruct()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	return st;
}

void DrawTime(HWND hWnd, HDC hdc, COLORREF bkColor, COLORREF fgColor) {
	HFONT font;
	RECT clientRect;

	font = CreateFont(164, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("The Led Display St")
	);


	SYSTEMTIME st = GetTimeStruct();
	
	WCHAR time_buff[100];
	wsprintfW(time_buff, L"%02d : %02d : %02d\n", st.wHour, st.wMinute, st.wSecond);
	

	GetClientRect(hWnd, &clientRect);

	SelectObject(hdc, font);
	SetTextColor(hdc, fgColor);
	SetBkColor(hdc, bkColor);

	DrawText(hdc, time_buff, -1, &clientRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	// Handle Event Loop

	switch (uMsg) {

	case WM_CREATE:
		SetTimer(hWnd, idTimer = 1, 1000, NULL);
		return 0;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;

	case WM_TIMER:
		RECT r;
		GetClientRect(hWnd, &r);
		InvalidateRect(hWnd, &r, TRUE);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		COLORREF bkColor = RGB(75, 0, 130);
		COLORREF fgColor = RGB(255, 20, 147);
		HBRUSH solidBrish = CreateSolidBrush(bkColor);

		HDC hdc = BeginPaint(hWnd, &ps);

		FillRect(hdc, &ps.rcPaint, solidBrish);

		DrawTime(hWnd, hdc, bkColor, fgColor);
		
		EndPaint(hWnd, &ps);

	}
	return 0;

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
