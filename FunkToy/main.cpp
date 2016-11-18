#include <Windows.h>

HWND hwnd;
HWND hwdesk;

MSG uMsg;

DWORD HotKeyID;
DWORD TimerID;

typedef struct _item{
	HWND hWnd;
	struct _item *next;
}hideHwnd;

hideHwnd *hidelistHead;
hideHwnd *hidelistEnd;

HANDLE processHeap;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	hwdesk = FindWindowA("Progman","Progman Manager");
	
	HotKeyID = GlobalAddAtomA("TEST");
	RegisterHotKey(0, HotKeyID, 0, VK_F4);

	TimerID = GlobalAddAtomA("silence");
	SetTimer(0, TimerID, 10, 0);

	processHeap = GetProcessHeap();
	hidelistHead = 0;
	hidelistEnd = 0;

	while (TRUE)
	{
		GetMessage(&uMsg, 0, 0, 0);
		if (uMsg.message==WM_HOTKEY)
		{
			MessageBoxA(0, "¶ºÄãÍæ", "silence", MB_OK);
			break;
		}
		else if (uMsg.message==WM_TIMER)
		{
			hwnd = GetForegroundWindow();
			if (hwnd!=hwdesk)
			{
				if (IsWindowVisible(hwnd))
				{
					hideHwnd *address = (hideHwnd *)HeapAlloc(processHeap, HEAP_ZERO_MEMORY, sizeof(hideHwnd));
					if (address)
					{
						if (hidelistEnd==0)
						{
							hidelistHead = address;
							hidelistEnd = address;
							address->hWnd = hwnd;
							address->next = 0;
						}
						else
						{
							hidelistEnd->next = address;
							hidelistEnd = address;
							hidelistEnd->hWnd = hwnd;
							hidelistEnd->next = 0;
						}
					}
					ShowWindow(hwnd, SW_HIDE);
				}
			}
		}
	}
	while (hidelistHead)
	{
		ShowWindow(hidelistHead->hWnd, SW_SHOW);
		LPVOID addrtemp = (LPVOID)hidelistHead;
		hidelistHead = hidelistHead->next;
		HeapFree(processHeap, HEAP_NO_SERIALIZE, addrtemp);
	}
	KillTimer(0, TimerID);
	ExitProcess(0);
}