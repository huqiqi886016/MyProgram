#include "QtWidgetsApplication1.h"
#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
// ȫ�ּ��̹��ӻص�����
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		if (wParam == WM_KEYUP) {
			KBDLLHOOKSTRUCT* kbdData = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
			int keyCode = kbdData->vkCode;

			// �ڴ˴��������¼�������������µļ���
			std::cout << "����: " << keyCode << std::endl;
			/*QtWidgetsApplication1 q;
			q.on_PushButton_clicked();*/
			// ���˲������´��ݵİ�����ֱ��������return����
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

	// ������һ�����ӣ�������ڵĻ�
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

DWORD WINAPI KeyboardHook(LPVOID lpParam);

QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	HANDLE hThread;
	DWORD dwThreadId;

	hThread = CreateThread(NULL	// Ĭ�ϰ�ȫ����
		, NULL		// Ĭ�϶�ջ��С
		, KeyboardHook // �߳���ڵ�ַ
		, NULL	//���ݸ��̺߳����Ĳ���
		, 0		// ָ���߳���������
		, &dwThreadId	//�߳�ID��
	);

	//CloseHandle(hThread);	//�ر��߳̾�����ں����ü�����һ
}

DWORD WINAPI KeyboardHook(LPVOID lpParam)
{
	// ע��ȫ�ּ��̹���
	HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

	if (hook == NULL) {
		std::cerr << "�޷�ע����̹���" << std::endl;
		return 1;
	}

	// ��Ϣѭ��
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// ж�ع���
	UnhookWindowsHookEx(hook);
	return 0;
}

void QtWidgetsApplication1::on_PushButton_clicked()
{
	ui.textEdit->setText("sgdgdr");
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}
