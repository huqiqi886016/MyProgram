#include "QtWidgetsApplication1.h"
#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
// 全局键盘钩子回调函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		if (wParam == WM_KEYUP) {
			KBDLLHOOKSTRUCT* kbdData = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
			int keyCode = kbdData->vkCode;

			// 在此处处理按键事件，例如输出按下的键码
			std::cout << "键码: " << keyCode << std::endl;
			/*QtWidgetsApplication1 q;
			q.on_PushButton_clicked();*/
			// 过滤不想向下传递的按键，直接在这里return即可
			// return 1;
		}
		if (wParam == WM_KEYDOWN && KeyPress.GetCount())
		{
			BYTE keyState[256] = { 0 };
			BYTE buffer[2] = { 0 };
			GetKeyboardState(keyState);
			int conversion = ToAscii(hookStruct->vkCode, hookStruct->scanCode, keyState, (LPWORD)buffer, hookStruct->flags);

			if (conversion == 1 || conversion == 2)
			{
				BOOL shift = (GetKeyState(VK_SHIFT) & 0x80) == 0x80;
				BOOL capital = GetKeyState(VK_CAPITAL) != 0;
				CHAR c = (CHAR)buffer[0];
				if ((shift ^ capital) && isalpha(c))
				{
					c = tolower(c);
				}
				CKeyEventArgs e(c);
				This->KeyPress.Fire(This, &e);
				bHandled |= e.bHandled;
			}
		}
	}

	// 调用下一个钩子，如果存在的话
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

DWORD WINAPI KeyboardHook(LPVOID lpParam);

QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	HANDLE hThread;
	DWORD dwThreadId;

	hThread = CreateThread(NULL	// 默认安全属性
		, NULL		// 默认堆栈大小
		, KeyboardHook // 线程入口地址
		, NULL	//传递给线程函数的参数
		, 0		// 指定线程立即运行
		, &dwThreadId	//线程ID号
	);

	//CloseHandle(hThread);	//关闭线程句柄，内核引用计数减一
}

DWORD WINAPI KeyboardHook(LPVOID lpParam)
{
	// 注册全局键盘钩子
	HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

	if (hook == NULL) {
		std::cerr << "无法注册键盘钩子" << std::endl;
		return 1;
	}

	// 消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// 卸载钩子
	UnhookWindowsHookEx(hook);
	return 0;
}

void QtWidgetsApplication1::on_PushButton_clicked()
{
	ui.textEdit->setText("sgdgdr");
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}
