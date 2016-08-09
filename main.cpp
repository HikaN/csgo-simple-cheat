
//this is - бидлакод
//Coded by baarsik / GUI - HikaN
//translate - google


#include "ProcMem.h"
#include "offsets.h"
#include "functions.h"
#include "base.h"

//WINAPI GUI
#include <Windows.h>
#include <tchar.h>



//antiflash
//on
void antiflash_on()
{
	DWORD localPlayer = csgo.Read<DWORD>(dwClient + m_dwLocalPlayer);
	{
		if (csgo.Read<float>(localPlayer + m_flFlashMaxAlpha) > 75.0f)
		{
			csgo.Write<float>(localPlayer + m_flFlashMaxAlpha, 0.0f);
		}
	}
}

//off
void antiflash_off()
{
	DWORD localPlayer = csgo.Read<DWORD>(dwClient + m_dwLocalPlayer);
	{
		if (csgo.Read<float>(localPlayer + m_flFlashMaxAlpha) <= 75.0f)
		{
			csgo.Write<float>(localPlayer + m_flFlashMaxAlpha, 255.0f);
		}
	}
}



//advertisement functions
void ESP();
void trigg();

void antiflash_on();
void antiflash_off();




void azazatrigger()
{
	while (true)
	{
		Sleep(30);
		trigg();
	}
}

void azazaesp()
{
	while (true)
	{
		Sleep(50);
		ESP();
	}
}




//trigger
void trigg()
{

	// Read the information about the player
	myPlayer.ReadInfo();

	// If our player is dead, do not continue
	if (!myPlayer.isAlive)
		return;

	int id = myPlayer.CrossHairID - 1;

	// If incorrect, the opponent does not continue
	if (id <= 0 || id > 64)
		return;

	// Read information about the purpose
	otherPlayer[id].ReadInfo(id);

	// If the target is a Ghost or dead, do not continue
	if (otherPlayer[id].dormant || !otherPlayer[id].isAlive)
		return;

	//If we do not shoot at allies, do not continue
	if (myPlayer.team == otherPlayer[id].team)
		return;

	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(20);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}


//ESP
void ESP()
{
	DWORD glowPointer = csgo.Read<DWORD>(dwClient + m_dwGlowObject);
	int glowObjects = csgo.Read<DWORD>(dwClient + m_dwGlowObject + 0x4);
	otherPlayer_t* Player = NULL;
	myPlayer.ReadInfo();

	for (int i = 0; i < glowObjects; i++)
	{
		DWORD mObj = glowPointer + i * sizeof(glow_t);
		glow_t glowObject = csgo.Read<glow_t>(mObj);
		Player = getEntityByCEntityList(glowObject.dwBase);

		// If object the glow is incorrect, not continue
		if (glowObject.dwBase == NULL || Player == nullptr)
			continue;

		//if the character lies it - 0
		if (Player->dormant)
		{
			csgo.Write<float>(mObj + 0x10, 0.0f);
			continue;
		}

		//Fix the maximum amount of health
		int maxHealth = Player->health > 100 ? Player->health : 100;

/*			skip allies backlight*/
		if (myPlayer.team == Player->team)
			continue;
		else
			//enemies green
			glow_player(mObj, (maxHealth - Player->health) / 100.0f, Player->health / 100.0f);
	}
}




//--------------------------------------------------------------------winapi gui-----------------------------------------------------------------

//Handles controls

HWND hWndChk1;
#define IDC_CHK1 0
HWND hWndChk2;
#define IDC_CHK2 1
HWND hWndChk3;
#define IDC_CHK3 2

HANDLE triggerd;
HANDLE espa;

//Handles controls

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		//events of the controls
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{


		/*		event on the checkbox - ESP*/
				case IDC_CHK1:
					if (SendMessage(GetDlgItem(hWnd, IDC_CHK1), BM_GETCHECK, 0, 0))
					{
						Sleep(100);
						espa = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)azazaesp, 0, 0, 0);
					}
					else
					{
						Sleep(100);
						TerminateThread(espa, NULL);
						CloseHandle(espa);
					}

					break;



				/*	event on the checkbox - Trigger*/
				case IDC_CHK2:
					if (SendMessage(GetDlgItem(hWnd, IDC_CHK2), BM_GETCHECK, 0, 0))
					{
						Sleep(100);
						triggerd = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)azazatrigger, 0, 0, 0);
					}
					else
					{
						Sleep(100);
						TerminateThread(triggerd, NULL);
						CloseHandle(triggerd);
					}

					break;



		/*			event on the checkbox - Flash*/
				case IDC_CHK3:
					if (SendMessage(GetDlgItem(hWnd, IDC_CHK3), BM_GETCHECK, 0, 0))
					{
						Sleep(100);
						antiflash_on();
					}
					else
					{
						Sleep(100);
						antiflash_off();
					}

					break;





			//default:
			//	break;


			}


			// the end events of the controls

			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
	}

	return 0;
}


HWND CreateWin(HINSTANCE hInst, int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("VAC"); //window class
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);


	//window creation
	HWND hWnd = CreateWindowEx(NULL, _T("VAC"), _T("CS:GO Simple Cheat by HikaN "), WS_SYSMENU, GetSystemMetrics(SM_CXSCREEN) / 2 - 160, GetSystemMetrics(SM_CYSCREEN) / 2 - 120, 330, 105, NULL, NULL, NULL, NULL);



	//creating elements
	if (hWnd != NULL)
	{

		hWndChk1 = CreateWindowEx(0, _T("Button"), _T("Glow ESP"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 25, 25, 100, 30, hWnd, (HMENU)IDC_CHK1, hInst, NULL);

		hWndChk2 = CreateWindowEx(NULL, _T("Button"), _T("Trigger"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 130, 25, 70, 30, hWnd, (HMENU)IDC_CHK2, hInst, NULL);

		hWndChk3 = CreateWindowEx(NULL, _T("Button"), _T("Anti Flash"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 214, 25, 100, 30, hWnd, (HMENU)IDC_CHK3, hInst, NULL);


		//font
		HFONT varCreateFont = CreateFont(-MulDiv(10, GetDeviceCaps(GetDC(0), LOGPIXELSY), 72), 0, 0, 0, FW_NORMAL, false, true, false, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS, _T("Microsoft Sans Serif"));



		SendMessage(hWndChk1, WM_SETFONT, (WPARAM)varCreateFont, 1);
		SendMessage(hWndChk2, WM_SETFONT, (WPARAM)varCreateFont, 1);
		SendMessage(hWndChk3, WM_SETFONT, (WPARAM)varCreateFont, 1);


	}
	return hWnd;
}



//GUI
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateWin(hInst, nCmdShow);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		// Checking the status of the game (player)
		if (!base.checkGameStarted())
			continue;

		// Зачем нам использовать функционал, если мы не в игре?
		if (gGameState != GAME_IN_PROGRESS)
			continue;

	}

	return 0;
}