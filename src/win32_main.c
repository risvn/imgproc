#include<windows.h>


// Window procedure to handle messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    //size
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {

   // Define window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.style = CS_OWNDC|CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = "Image_search";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    


    if(RegisterClass(&wc))
  { 
       // Create window
    HWND hwnd = CreateWindowEx(0,wc.lpszClassName, "Image_search",
                               WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                               NULL, NULL, hInstance, NULL);

        // ✅ SHOW AND UPDATE THE WINDOW
        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);
  } ;

  


    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;

}
