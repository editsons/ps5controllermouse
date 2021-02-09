#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include "DualSenseWindows_V0.1/ds5w.h"


void moveMouse(int x, int y) {
	INPUT Inputs[1] = { 0 };

	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dx = (long)x; // desired X coordinate
	Inputs[0].mi.dy = (long)y; // desired Y coordinate
	Inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE;

	SendInput(1, Inputs, sizeof(INPUT));
}

void sendLeftMouseBtn() {
	INPUT Inputs[2] = { 0 };

	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	Inputs[1].type = INPUT_MOUSE;
	Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	SendInput(2, Inputs, sizeof(INPUT));
}

void sendRightMouseBtn() {
	INPUT Inputs[2] = { 0 };

	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

	Inputs[1].type = INPUT_MOUSE;
	Inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

	SendInput(2, Inputs, sizeof(INPUT));
}

void sendMouseLeftDown() {
	INPUT Inputs[1] = { 0 };

	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	SendInput(1, Inputs, sizeof(INPUT));
}

void sendMouseLeftUp() {
	INPUT Inputs[1] = { 0 };

	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	SendInput(1, Inputs, sizeof(INPUT));
}

void sendKeyUp(int vkCode) {
	INPUT Inputs[1] = { 0 };

	Inputs[0].type = INPUT_KEYBOARD;
	Inputs[0].ki.wVk = vkCode;
	Inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

	if (vkCode == VK_SHIFT || vkCode == VK_CONTROL) {
		Inputs[0].ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
	}
	/* TODO: Check all extended keys:
		VK_UP,
		VK_DOWN,
		VK_LEFT,
		VK_RIGHT,
		VK_HOME,
		VK_END,
		VK_PRIOR, // PgUp
		VK_NEXT,  //  PgDn
		VK_INSERT,
		VK_DELETE	
	*/
	SendInput(1, Inputs, sizeof(INPUT));
}

void sendKeyDown(int vkCode) {
	INPUT Inputs[1] = { 0 };

	Inputs[0].type = INPUT_KEYBOARD;
	Inputs[0].ki.wVk = vkCode;
	Inputs[0].ki.dwFlags = 0; // key down

	if (vkCode == VK_SHIFT || vkCode == VK_CONTROL) {
		Inputs[0].ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
	}
	/* TODO: Check all extended keys:
		VK_UP,
		VK_DOWN,
		VK_LEFT,
		VK_RIGHT,
		VK_HOME,
		VK_END,
		VK_PRIOR, // PgUp
		VK_NEXT,  //  PgDn
		VK_INSERT,
		VK_DELETE
	*/
	SendInput(1, Inputs, sizeof(INPUT));
}


void sendMouseRightDown() {
	INPUT Inputs[1] = { 0 };

	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

	SendInput(1, Inputs, sizeof(INPUT));
}

void sendMouseRightUp() {
	INPUT Inputs[1] = { 0 };

	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

	SendInput(1, Inputs, sizeof(INPUT));
}


UINT sendMouseScroll(int scroll)
{
	INPUT input;
	POINT pos;
	GetCursorPos(&pos);

	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	input.mi.time = NULL; //Windows will do the timestamp
	input.mi.mouseData = (DWORD)scroll; //A positive value indicates that the wheel was rotated forward, away from the user; a negative value indicates that the wheel was rotated backward, toward the user. One wheel click is defined as WHEEL_DELTA, which is 120.
	input.mi.dx = pos.x;
	input.mi.dy = pos.y;
	input.mi.dwExtraInfo = GetMessageExtraInfo();

	return SendInput(1, &input, sizeof(INPUT));
}


int main()
{
	// Array of controller infos
	DS5W::DeviceEnumInfo infos[16];

	// Number of controllers found
	unsigned int controllersCount = 0;

	// Call enumerate function and switch on return value
	switch (DS5W::enumDevices(infos, 16, &controllersCount)) {
	case DS5W_OK:
		// The buffer was not big enough. Ignore for now
	case DS5W_E_INSUFFICIENT_BUFFER:
		break;

		// Any other error will terminate the application
	default:
		// Insert your error handling
		return -1;
	}

	// Check number of controllers
	if (!controllersCount) {
		return -1;
	}

	// Context for controller
	DS5W::DeviceContext con;

	// Init controller and close application is failed
	if (DS5W_FAILED(DS5W::initDeviceContext(&infos[0], &con))) {
		return -1;
	}

	bool enabled = true;
	bool mPressed = false;
	bool xPressed = false;
	bool cPressed = false;
	bool sPressed = false;
	bool tPressed = false;
	int oldLeftX = 0;
	int oldLeftY = 0;

	// Main loop
	while (true) {
		// Input state
		DS5W::DS5InputState inState;

		// Retrieve data
		if (DS5W_SUCCESS(DS5W::getDeviceInputState(&con, &inState))) {

			if (inState.buttonsB & DS5W_ISTATE_BTN_B_PLAYSTATION_LOGO) {
				mPressed = true;
			}
			else if (mPressed) {
				mPressed = false;
				// m released
				enabled = !enabled;
				if (enabled) {
					std::cout << "Enabled" << std::endl;
				}
				else {
					std::cout << "Disabled" << std::endl;
				}
			}

			if(enabled) {
				// check buttons
				if (inState.buttonsAndDpad & DS5W_ISTATE_BTX_CROSS) {
					if (!xPressed) {
						sendMouseLeftDown();
						std::cout << "cross pressed" << std::endl;
					}
					xPressed = true;
				}
				else if(xPressed) {
					xPressed = false;
					sendMouseLeftUp();
					std::cout << "cross released" << std::endl;
				}

				if (inState.buttonsAndDpad & DS5W_ISTATE_BTX_CIRCLE) {
					if (!cPressed) {
						std::cout << "circle pressed" << std::endl;
						sendKeyDown(VK_SHIFT);
					}
					cPressed = true;
				}
				else if (cPressed) {
					cPressed = false;
					std::cout << "circle released" << std::endl;
					sendKeyUp(VK_SHIFT);
				}

				if (inState.buttonsAndDpad & DS5W_ISTATE_BTX_SQUARE) {
					if (!sPressed) {
						std::cout << "square pressed" << std::endl;
						sendMouseRightDown();
					}
					sPressed = true;
				}
				else if (sPressed) {
					sPressed = false;
					std::cout << "square released" << std::endl;
					sendMouseRightUp();
				}

				if (inState.buttonsAndDpad & DS5W_ISTATE_BTX_TRIANGLE) {
					if (!tPressed) {
						std::cout << "triangle pressed" << std::endl;
						sendKeyDown(VK_CONTROL);
					}
					tPressed = true;
				}
				else if (tPressed) {
					tPressed = false;
					std::cout << "triangle released" << std::endl;
					sendKeyUp(VK_CONTROL);
				}

				if ((int)inState.rightTrigger > 0) {
					sendMouseScroll(-(int)inState.rightTrigger / 10);
				}

				if ((int)inState.leftTrigger > 0) {
					sendMouseScroll((int)inState.leftTrigger / 10);
				}


				// check joysticks
				//int deltaLeftX = (int)inState.leftStick.x - oldLeftX;
				//int deltaLeftY = (int)inState.leftStick.y - oldLeftY;

				int minX = 5;
				int minY = 5;

				if (abs((int)inState.leftStick.x) > minX || abs((int)inState.leftStick.y) > minY) {
					//std::cout << "joystick moved. X=" << oldLeftX << " Y=" << oldLeftY  << std::endl;

					int leftStickX = (int)inState.leftStick.x;
					int leftStickY = (int)inState.leftStick.y;
					
					if (leftStickX > 0) leftStickX -= minX;
					if (leftStickX < 0) leftStickX += minX;
					if (leftStickY > 0) leftStickY -= minY;
					if (leftStickY < 0) leftStickY += minY;

					moveMouse(leftStickX / 10, -(leftStickY / 10));
					
				}
			}

			oldLeftX = (int)inState.leftStick.x;
			oldLeftY = (int)inState.leftStick.y;


			/*// Create struct and zero it
			DS5W::DS5OutputState outState;
			ZeroMemory(&outState, sizeof(DS5W::DS5OutputState));

			// Set output data
			outState.leftRumble = inState.leftTrigger;
			outState.rightRumble = inState.rightTrigger;

			// Send output to the controller
			DS5W::setDeviceOutputState(&con, &outState);*/
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	// Shutdown context
	DS5W::freeDeviceContext(&con);

	// Return zero
	return 0;
}

