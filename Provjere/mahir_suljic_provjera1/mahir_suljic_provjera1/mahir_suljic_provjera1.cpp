// mahir_suljic_provjera1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "mahir_suljic_provjera1.h"

#define MAX_LOADSTRING 100
#define EL_COUNT(x) (sizeof(x)/sizeof(x[0]))

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

static RECT initArea = { 0, 0, 720, 480 };

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
	LoadStringW(hInstance, IDC_MAHIRSULJICPROVJERA1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAHIRSULJICPROVJERA1));

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

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAHIRSULJICPROVJERA1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;// MAKEINTRESOURCEW(IDC_MAHIRSULJICPROVJERA1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
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
		CW_USEDEFAULT, 0, initArea.right, initArea.bottom, nullptr, nullptr, hInstance, nullptr);

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
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT windowArea = { 0,0,0,0 };
	static RECT mainArea = { 0,0,0,0 };
	static double scale = 1.0;

	static bool flag = true; // false - left, true - right

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_SIZE:
	{
		GetClientRect(hWnd, &windowArea);

		constexpr int padding = 10;

		mainArea = {
			windowArea.left + padding,
			windowArea.top + padding,
			windowArea.right - padding,
			windowArea.bottom - padding
		};

		const double xScale = (double)(windowArea.right - windowArea.left) / (initArea.right - initArea.left);
		const double yScale = (double)(windowArea.bottom - windowArea.top) / (initArea.bottom - initArea.top);

		scale = min(xScale, yScale);

		flag = !flag;
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		drawFrame(hdc, windowArea, mainArea);

		if (!flag)
			drawFirstFlag(hdc, mainArea);
		else
			drawSecondFlag(hdc, mainArea, scale);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void drawFrame(HDC hdc, RECT windowArea, RECT mainArea)
{
	constexpr COLORREF hatchLineColor = RGB(0, 0, 0);

	const HBRUSH hatchBrushes[] = {
		CreateHatchBrush(HS_FDIAGONAL, hatchLineColor),
		CreateHatchBrush(HS_BDIAGONAL, hatchLineColor),
	};

	const POINT middle = {
		(windowArea.right - windowArea.left) / 2,
		(windowArea.bottom - windowArea.top) / 2
	};

	const RECT topLeft = { 0, 0, middle.x, middle.y };
	const RECT topRight = { middle.x, 0, windowArea.right, middle.y };
	const RECT bottomLeft = { 0, middle.y, middle.x, windowArea.bottom };
	const RECT bottomRight = { middle.x, middle.y, windowArea.right, windowArea.bottom };

	FillRect(hdc, &topLeft, hatchBrushes[0]);
	FillRect(hdc, &topRight, hatchBrushes[1]);
	FillRect(hdc, &bottomLeft, hatchBrushes[1]);
	FillRect(hdc, &bottomRight, hatchBrushes[0]);

	for (int i = 0; i < EL_COUNT(hatchBrushes); ++i)
		DeleteObject(hatchBrushes[i]);

}

void drawFirstFlag(HDC hdc, RECT mainArea)
{
	const double hThird = (mainArea.right - mainArea.left) / 3;
	const double vThird = (mainArea.bottom - mainArea.top) / 3;

	POINT topPoints[] = {
		{0, 0},
		{hThird,0},
		{2 * hThird,0},
		{3 * hThird,0}
	};

	POINT rightPoints[] = {
		{0,0},
		{0,vThird},
		{0,2 * vThird},
		{0,3 * vThird}
	};


	for (int i = 0; i < EL_COUNT(topPoints); ++i)
	{
		topPoints[i].x += mainArea.left;
		topPoints[i].y += mainArea.top;
	}

	for (int i = 0; i < EL_COUNT(rightPoints); ++i)
	{
		rightPoints[i].x += mainArea.right;
		rightPoints[i].y += mainArea.top;
	}

	const POINT topLeft = topPoints[0];
	const POINT bottomLeft = { mainArea.left, mainArea.bottom };
	const POINT topRight = rightPoints[0];
	const POINT bottomRight = { mainArea.right, mainArea.bottom };

	const POINT firstPoly[] = {
		bottomLeft,
		topLeft,
		topPoints[1]
	};

	const POINT secondPoly[] = {
		bottomLeft,
		topPoints[1],
		topPoints[2]
	};

	const POINT thirdPoly[] = {
		bottomLeft,
		topPoints[2],
		topRight,
		rightPoints[1]
	};

	const POINT fourthPoly[] = {
		bottomLeft,
		rightPoints[1],
		rightPoints[2]
	};

	const POINT fifthPoly[] = {
		bottomLeft,
		rightPoints[2],
		rightPoints[3]
	};

	const HBRUSH brushes[] = {
		CreateSolidBrush(RGB(0,0,255)),
		CreateSolidBrush(RGB(255,255,0)),
		CreateSolidBrush(RGB(255,0,0)),
		CreateSolidBrush(RGB(255,255,255)),
		CreateSolidBrush(RGB(0,255,0))
	};

	const HPEN pen = CreatePen(PS_NULL, 0, NULL);
	SelectObject(hdc, pen);

	SelectObject(hdc, brushes[0]);
	Polygon(hdc, firstPoly, 3);
	SelectObject(hdc, brushes[1]);
	Polygon(hdc, secondPoly, 3);
	SelectObject(hdc, brushes[2]);
	Polygon(hdc, thirdPoly, 4);
	SelectObject(hdc, brushes[3]);
	Polygon(hdc, fourthPoly, 3);
	SelectObject(hdc, brushes[4]);
	Polygon(hdc, fifthPoly, 3);

	for (int i = 0; i < EL_COUNT(brushes); ++i)
		DeleteObject(brushes[i]);

	DeleteObject(pen);
}

void drawSecondFlag(HDC hdc, RECT mainArea, double scale)
{
	const int mainHeight = mainArea.bottom - mainArea.top;

	const POINT middle = {
		(mainArea.right - mainArea.left) / 2,
		(mainArea.bottom - mainArea.top) / 2
	};

	const int middleTop = 2 * mainHeight / 5;
	const int middleBottom = 3 * mainHeight / 5;

	const POINT topLeft = { mainArea.left, mainArea.top };
	const POINT topRight = { mainArea.right, mainArea.top };
	const POINT bottomLeft = { mainArea.left, mainArea.bottom };
	const POINT bottomRight = { mainArea.right, mainArea.bottom };

	const POINT middleBottomPoint = { (mainArea.right - mainArea.left) / 2, mainArea.bottom };

	const HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	const HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
	const HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	const HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
	const HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));

	const HBRUSH brushes[] = {
		blackBrush,
		blueBrush,
		whiteBrush,
		yellowBrush,
		redBrush
	};

	const POINT leftTriangle[] = {
		bottomLeft,
		topLeft,
		middleBottomPoint
	};

	const POINT rightTriangle[] = {
		bottomRight,
		topRight,
		middleBottomPoint
	};

	const int sunRadius = 50 * scale;
	constexpr int sunOffset = 0; // reda radi, sad je evo na pola :P

	const POINT sunPosition = { middle.x, middleTop + sunOffset };

	const RECT sun = {
		sunPosition.x - sunRadius,
		sunPosition.y - sunRadius,
		sunPosition.x + sunRadius,
		sunPosition.y + sunRadius
	};

	SelectObject(hdc, blackBrush);
	Rectangle(hdc, mainArea.left, mainArea.top, mainArea.right, middleTop);

	SelectObject(hdc, yellowBrush);
	Ellipse(hdc, sun.left, sun.top, sun.right, sun.bottom);

	SelectObject(hdc, blueBrush);
	Rectangle(hdc, mainArea.left, middleTop, mainArea.right, middleBottom);

	SelectObject(hdc, whiteBrush);
	Rectangle(hdc, mainArea.left, middleBottom, mainArea.right, mainArea.bottom);

	SelectObject(hdc, redBrush);
	Polygon(hdc, leftTriangle, 3);
	Polygon(hdc, rightTriangle, 3);

	for (int i = 0; i < EL_COUNT(brushes); ++i)
		DeleteObject(brushes[i]);
}
