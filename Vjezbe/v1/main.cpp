#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include<iostream>
#include <windows.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               720,                 /* The programs width */
               480,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    SIZE sizeOfText;
    static std::string text;
    static int oldWidth, oldHeight;
    static int width=0, height=0;
    RECT rect;
    HBRUSH brush;
    static int counter=0;
    COLORREF bgColor;
    static int xPos, yPos;

    switch (message)                  /* handle the messages */
    {

    case WM_SIZE:
        oldWidth = width;
        oldHeight = height;

        width = LOWORD(lParam);
        height = HIWORD(lParam);

        text = "x="+std::to_string(width)+", y="+std::to_string(height);
        hdc = GetDC(hwnd);
        GetTextExtentPoint32(hdc, text.c_str(), text.length(), &sizeOfText);
        ReleaseDC(hwnd, hdc);

        if(width>oldWidth && height>oldHeight){
            xPos = width/2;
            yPos = height/2;
            InvalidateRect(hwnd, NULL, true);
        }
        else if(width>oldWidth){
            xPos = oldWidth+(width-oldWidth)/2;
            yPos = height/2;
        }
        else if(height>oldHeight){
            xPos = width/2;
            yPos = oldHeight+(height-oldHeight)/2;
        }
        else{
            xPos = width/2;
            yPos = height/2;
            InvalidateRect(hwnd, NULL, true);
        }
        xPos-=sizeOfText.cx/2;
        yPos-=sizeOfText.cy/2;

        ++counter;
        if(counter>5){
            counter =1;
        }
        break;

    case WM_PAINT:{
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);
        bgColor = RGB(rand()%256, rand()%256, rand()%256);
        brush = CreateHatchBrush(counter, RGB(255*(counter%2),255*(counter%2),255*(counter%2)));
        SetBkColor(hdc, bgColor);
        FillRect(hdc, &rect, brush);
        SetBkColor(hdc, RGB(255,255,255));
        TextOut(hdc, xPos, yPos, _T(text.c_str()), text.length());
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
