// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include "pch.h"
#include "dx11Imgui.h"
#include "HotKeys.h"
void* Dll = nullptr;
bool err = false;


std::shared_ptr<DeepRockHook> g_deepRockHook;


void ClearingThread()
{
	while (1)
	{
		if (GetAsyncKeyState(VK_END) & 1) { FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(Dll), 0); }

		
		//铅爆cd
		if (GetAsyncKeyState(VK_F5) & 1) { HotKeys::Shooter::qianBaoCdActive = !HotKeys::Shooter::qianBaoCdActive; }
		//铅爆无预热
		if (GetAsyncKeyState(VK_F6) & 1) { HotKeys::Shooter::qianBaoNoPre = !HotKeys::Shooter::qianBaoNoPre; }
		//铅爆无限子弹
		if (GetAsyncKeyState(VK_F7) & 1) { HotKeys::Shooter::qianBaoinfiniteAmmoActive = !HotKeys::Shooter::qianBaoinfiniteAmmoActive; }

		//自动瞄准
		if (GetAsyncKeyState(VK_F8) & 1) { HotKeys::Other::aimbotActive = !HotKeys::Other::aimbotActive; }

		//绘制名字
		if (GetAsyncKeyState(VK_F9) & 1) {
			HotKeys::Other::drawPlayerNamesActive = !HotKeys::Other::drawPlayerNamesActive;
			HotKeys::Other::drawNamesActive = !HotKeys::Other::drawNamesActive;
		}


		//增加范围
		if (GetAsyncKeyState(VK_PRIOR) & 1) {
			HotKeys::Other::aimRange += 30; 
		}

		//减小范围
		if (GetAsyncKeyState(VK_NEXT) & 1) { 
			HotKeys::Other::aimRange = max(50, HotKeys::Other::aimRange - 30);
		}

		Sleep(20);
	}
}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(dll);

		err = EngineInit() && CheatInit();
		g_deepRockHook = std::make_shared<DeepRockHook>();
		g_deepRockHook->init();
		

		// 一定要在线程里调用 dx11imgui 系列函数 否则初始化失败 不知道原因
		CreateThread(NULL, 0, [](LPVOID)->DWORD {
			Dx11Imgui::init([]() {});
			return 0;
			}, NULL, 0, NULL);
		
		if (err)
		{
			Dll = dll;
			auto t = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(ClearingThread), nullptr, 0, nullptr);
			if (t) { CloseHandle(t); }
		}

		return err;
	}
	case DLL_PROCESS_DETACH:
	{
		g_deepRockHook = nullptr;
		if (err) { CheatRemove(); };
		return true;
	}
	}
	return TRUE;
}

