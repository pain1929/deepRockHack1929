#include "mem.hpp"
#include "inj.hpp"
#include <iostream>
#include <filesystem>
#include "fromGithub/fromGithub.h"

GoString toGoStr(const char * s) {
	return { s , (int64_t )strlen(s) };
}
std::string toStdStr(GoString s) {
	return std::string(s.p);
}


void updateFromGithub() {

	 const char* currentVer = "1.0.0";
	 const char*  saveTo = "./newVersion.zip";


     const char * uname = "pain1929";
     const char * rep = "deepRockHack1929";
     const char * fileName = "README.md";


	// ��� readMe �ļ�
	auto readMe = GetTextFromGithub(toGoStr(uname), toGoStr(rep), toGoStr(fileName));
	if (readMe) {
		std::cout << readMe << std::endl;
	}
	


	// �������³���
	std::cout << "��ʼ������ ... " << std::endl;
	auto ret = FromGithub(toGoStr(uname), toGoStr(rep), toGoStr(currentVer), toGoStr(saveTo));

	if (ret) {
		std::cout << "�°汾���³ɹ� ���浽 " << saveTo << " �������°汾" << std::endl;
		system("pause");
		exit(0);
	}

}



class Utils {
public:
	static std::wstring getExePathW() {
		wchar_t fullName[256]{};
		GetModuleFileNameW(NULL, fullName, 256);

		auto str = std::wstring(fullName);
		return std::filesystem::path(str).parent_path().generic_wstring();
	}
	static std::string getExePath() {
		char fullName[256]{};
		GetModuleFileNameA(NULL, fullName, 256);
		auto str = std::string(fullName);
		return std::filesystem::path(str).parent_path().generic_string();
	}
};


int main(int argc , char* argv[]) {
	
	updateFromGithub();

	const auto basePath = Utils::getExePath();

	int pid{ 0 };
	try {
		pid = mem::Process::get_pid("FSD-Win64-Shipping.exe");
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	if (!pid) {
		std::cout << "δ�ҵ���Ϸ���� ����������Ϸ" << std::endl;
		system("pause");
		return -1;
	}
	auto handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	

	if (!InjectDll(handle, (basePath + "/dx11_imgui_lib.dll").c_str())) {
		std::cout << "dx11_imgui_lib.dll ע��ʧ�ܣ�" << std::endl;
		system("pause");
		return -2;
	}
	std::cout << "dx11_imgui_lib.dll ע��ɹ���׼��ע��ڶ���ģ�� ��ȴ�..." << std::endl;
	Sleep(3000);


	if (!InjectDll(handle, (basePath + "/deep_rock_hack.dll").c_str())) {
		std::cout << "deep_rock_hack.dll inj failed" << std::endl;
		system("pause");
		return -3;
	}

	std::cout << "ע��ɹ���" << std::endl;
	system("pause");
	return 0;

}