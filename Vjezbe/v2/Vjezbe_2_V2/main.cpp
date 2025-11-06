#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <cmath>
#include<iostream>

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

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
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    if (!RegisterClassEx(&wincl))
        return 0;

    hwnd = CreateWindowEx(
        0,
        szClassName,
        _T("Digitalna Galerija - Dinamičko Skaliranje"),
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


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);
        int w = rect.right;
        int h = rect.bottom;

        //  Pozadina
        HBRUSH bgBrush = CreateSolidBrush(RGB(230, 240, 255));
        FillRect(hdc, &rect, bgBrush);
        DeleteObject(bgBrush);

        //  Omjeri za dimenzije (relativno u odnosu na veličinu prozora)
        int margin = w / 20; // mali razmak od ivice

        //  Elipsa (krug u gornjem lijevom uglu)
        HPEN penBlue = CreatePen(PS_SOLID, std::max(1, w / 200), RGB(0, 0, 200));
        HBRUSH brushBlue = CreateSolidBrush(RGB(180, 200, 255));
        SelectObject(hdc, penBlue);
        SelectObject(hdc, brushBlue);
        Ellipse(hdc, margin, margin, w / 5, h / 4);
        DeleteObject(penBlue);
        DeleteObject(brushBlue);

        //  Pravougaonik
        HPEN penGreen = CreatePen(PS_SOLID, std::max(1, w / 300), RGB(0, 180, 0));
        HBRUSH brushGreen = CreateSolidBrush(RGB(150, 255, 150));
        SelectObject(hdc, penGreen);
        SelectObject(hdc, brushGreen);
        Rectangle(hdc, w / 3, h / 12, w / 2, h / 3.5);
        DeleteObject(penGreen);
        DeleteObject(brushGreen);

        //  Zaobljeni pravougaonik
        HPEN penRed = CreatePen(PS_SOLID, std::max(1, w / 300), RGB(200, 0, 0));
        HBRUSH brushRed = CreateSolidBrush(RGB(255, 180, 180));
        SelectObject(hdc, penRed);
        SelectObject(hdc, brushRed);
        RoundRect(hdc, w * 0.65, h / 12, w * 0.9, h / 3.5, w / 30, h / 30);
        DeleteObject(penRed);
        DeleteObject(brushRed);

        //  Dijagonalne linije (ukrštene)
        HPEN penGray = CreatePen(PS_DOT, 1, RGB(100, 100, 100));
        SelectObject(hdc, penGray);
        MoveToEx(hdc, margin, h / 2, NULL);
        LineTo(hdc, w / 3, h - margin);
        MoveToEx(hdc, w / 3, h / 2, NULL);
        LineTo(hdc, margin, h - margin);
        DeleteObject(penGray);

        //  Bezierova kriva
        HPEN penCurve = CreatePen(PS_SOLID, std::max(1, w / 250), RGB(255, 100, 0));
        SelectObject(hdc, penCurve);
        POINT points[4] = {
            { w / 2, h / 2 },
            { w / 1.8, h / 3 },
            { w / 1.3, h * 0.9 },
            { w * 0.95, h / 2 }
        };
        PolyBezier(hdc, points, 4);
        DeleteObject(penCurve);

        //  Tekst
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(20, 20, 80));

        TextOut(hdc, w / 2, h - h / 8, _T("GDI Demo V2"), 11);


        EndPaint(hwnd, &ps);
        break;
    }

    case WM_SIZE:
        InvalidateRect(hwnd, NULL, TRUE); // ponovo iscrtaj kad se promijeni veličina
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
