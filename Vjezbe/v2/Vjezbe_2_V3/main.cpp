#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <windows.h>
#include <tchar.h>
#include <cstdlib>
#include <ctime>


LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
TCHAR szClassName[] = _T("Kucica");
int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);;

    if (!RegisterClassEx(&wincl))
        return 0;

    hwnd = CreateWindowEx(
        0,
        szClassName,
        _T("Kucica"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        600,
        450,
        HWND_DESKTOP,
        NULL,
        hThisInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);

    while (GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

COLORREF skyColor = RGB(135, 206, 235);

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static RECT rect;
    switch (msg)
    {
    case WM_SIZE:
        srand((unsigned)time(0));
        skyColor = RGB(rand() % 156 + 100, rand() % 156 + 100, 200 + rand() % 55);
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);

        int w = rect.right;
        int h = rect.bottom;

        // nebo
        HBRUSH skyBrush = CreateSolidBrush(skyColor);
        FillRect(hdc, &rect, skyBrush);
        DeleteObject(skyBrush);

        // Kuća
        int houseW = w * 0.4;
        int houseH = h * 0.4;
        int houseX = w * 0.3;
        int houseY = h * 0.5;

        HBRUSH houseBrush = CreateSolidBrush(RGB(210, 160, 100));
        RECT houseRect = { houseX, houseY, houseX + houseW, houseY + houseH };
        FillRect(hdc, &houseRect, houseBrush);
        DeleteObject(houseBrush);

        //  Krov (HatchBrush s crvenom pozadinom)
        HPEN roofPen = CreatePen(PS_SOLID, 2, RGB(100, 30, 30));
        SelectObject(hdc, roofPen);

        // pozadina hatch uzorka postaje crvena
        SetBkColor(hdc, RGB(200, 40, 40));
        HBRUSH roofBrush = CreateHatchBrush(HS_BDIAGONAL, RGB(120, 0, 0)); // tamne linije
        SelectObject(hdc, roofBrush);

        POINT roof[3] = {
            { houseX, houseY },
            { houseX + houseW / 2, houseY - houseH / 2 },
            { houseX + houseW, houseY }
        };
        Polygon(hdc, roof, 3);

        DeleteObject(roofPen);
        DeleteObject(roofBrush);

        //  Vrata (Hatch Brush)
        int doorW = houseW * 0.2;
        int doorH = houseH * 0.5;
        int doorX = houseX + houseW * 0.4;
        int doorY = houseY + houseH - doorH;

        SetBkColor(hdc, RGB(100, 60, 30));
        HBRUSH doorBrush = CreateHatchBrush(HS_CROSS, RGB(50, 30, 10));
        RECT doorRect = { doorX, doorY, doorX + doorW, doorY + doorH };
        FillRect(hdc, &doorRect, doorBrush);
        DeleteObject(doorBrush);

        //  Prozor (Solid)
        int winW = houseW * 0.2;
        int winH = houseH * 0.2;
        int winX = houseX + houseW * 0.15;
        int winY = houseY + houseH * 0.2;

        HBRUSH winBrush = CreateSolidBrush(RGB(180, 220, 255));
        Rectangle(hdc, winX, winY, winX + winW, winY + winH);
        DeleteObject(winBrush);

        MoveToEx(hdc, winX, winY + winH / 2, NULL);
        LineTo(hdc, winX + winW, winY + winH / 2);
        MoveToEx(hdc, winX + winW / 2, winY, NULL);
        LineTo(hdc, winX + winW / 2, winY + winH);

        //  Sunce (Solid Brush)
        int sunR = w * 0.07;
        int sunX = w * 0.8;
        int sunY = h * 0.1;
        HBRUSH sunBrush = CreateSolidBrush(RGB(255, 230, 50));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, sunBrush);
        Ellipse(hdc, sunX, sunY, sunX + sunR, sunY + sunR);
        SelectObject(hdc, oldBrush);
        DeleteObject(sunBrush);

        //  Oblak (Hatch Brush + Bezier)
        HPEN cloudPen = CreatePen(PS_SOLID, 2, RGB(220, 220, 220));
        HBRUSH cloudBrush = CreateHatchBrush(HS_HORIZONTAL, RGB(255, 255, 255));
        SelectObject(hdc, cloudPen);
        SelectObject(hdc, cloudBrush);

        POINT cloudShape[4] = {
            { w * 0.1, h * 0.2 },
            { w * 0.15, h * 0.1 },
            { w * 0.25, h * 0.3 },
            { w * 0.3, h * 0.2 }
        };
        PolyBezier(hdc, cloudShape, 4);

        DeleteObject(cloudPen);
        DeleteObject(cloudBrush);

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

