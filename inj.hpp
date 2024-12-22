#pragma once
#include <Windows.h>
BOOL InjectDll(HANDLE hProcess, const char* dllPath) {
    LPVOID lpRemoteMem = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (lpRemoteMem == NULL) {
        // �����ڴ�ʧ��
        return FALSE;
    }

    if (!WriteProcessMemory(hProcess, lpRemoteMem, dllPath, strlen(dllPath) + 1, NULL)) {
        // д���ڴ�ʧ��
        VirtualFreeEx(hProcess, lpRemoteMem, 0, MEM_RELEASE);
        return FALSE;
    }

    HMODULE hKernel32 = GetModuleHandle("Kernel32");
    if (hKernel32 == NULL) {
        // ��ȡKernel32ģ����ʧ��
        VirtualFreeEx(hProcess, lpRemoteMem, 0, MEM_RELEASE);
        return FALSE;
    }

    LPVOID lpLoadLibraryA = (LPVOID)GetProcAddress(hKernel32, "LoadLibraryA");
    if (lpLoadLibraryA == NULL) {
        // ��ȡLoadLibraryA������ַʧ��
        VirtualFreeEx(hProcess, lpRemoteMem, 0, MEM_RELEASE);
        return FALSE;
    }
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpLoadLibraryA, lpRemoteMem, 0, NULL);
    if (hThread == NULL) {
        // ����Զ���߳�ʧ��
        VirtualFreeEx(hProcess, lpRemoteMem, 0, MEM_RELEASE);
        return FALSE;
    }
    //�ȴ��߳̽���
    WaitForSingleObject(hThread, INFINITE);

    //�ͷ��ڴ�
    VirtualFreeEx(hProcess, lpRemoteMem, 0, MEM_RELEASE);
    CloseHandle(hThread);
    return TRUE;
}