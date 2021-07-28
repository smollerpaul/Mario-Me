#pragma once
#include <dinput.h>
#define DIRECTINPUT_VERSION 0x0800

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024

class Keyboard
{
private:
	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

public:
	void Init(HWND hWnd);
	void Process();

	void OnKeyUp(int keyCode);
	void OnKeyDown(int keyCode);
	int IsKeyDown(int keyCode);

	void PressKey(int keyCode);
	void ReleaseKey(int keyCode);
};

