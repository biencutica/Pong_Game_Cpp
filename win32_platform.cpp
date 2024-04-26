#include "utils.cpp"
#include <windows.h>

global_variable bool running = true;

struct Render_State {
	int height, width;
	void* memory;

	BITMAPINFO bitmap_info;
};

global_variable Render_State render_state;

#include "renderer.cpp"

LRESULT CALLBACK window_callback(HWND hWnd, UINT Msg, WPARAM  wParam, LPARAM  lParam) {
	LRESULT result = 0;

	//to check if the user wants to exit
	switch (Msg) {
		case WM_CLOSE:
		case WM_DESTROY: {
			running = false;
		} break;

		case WM_SIZE: {
			RECT rect;
			GetClientRect(hWnd, &rect);
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int size = render_state.width * render_state.height * sizeof(unsigned int);

			//check if pointer already has valid memory, so we can free it if user changed size
			if (render_state.memory)
				VirtualFree(render_state.memory, 0, MEM_RELEASE);

			//malloc
			render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); //void pointer, where to store the frames
			
			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1;
			render_state.bitmap_info.bmiHeader.biBitCount = 32;
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;

		
		} break;

		default: {
			result = DefWindowProc(hWnd, Msg, wParam, lParam);
		}
	}
	return result;
}

//entry point for graphical windows-based applications
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	//draw a window
	//create a window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Game Window Class";
	window_class.lpfnWndProc = window_callback; //windows passes messages to us

	//register class - pointer to the windows class
	RegisterClass(&window_class);

	//create window
	HWND window = CreateWindow(window_class.lpszClassName, "Pong", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 720, 0, 0, hInstance, 0);
	
	//handle device context
	HDC hdc = GetDC(window);

	//game loop
	while (running) {
		//input 
		MSG message;
		//read all messages from window (so we can make full screen, move window around etc)
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			//dispatch message to our callback
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		//simulate
		clear_screen(0xff5500);
		draw_rectangle(0, 0, 1, 1, 0xff00ff22);
		//render_background();

		//render
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

	}
}