#include "mem.hpp"
#include "inj.hpp"
#include <iostream>
#include <filesystem>
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
		std::cout << "FSD-Win64-Shipping not found" << std::endl;
		system("pause");
		return -1;
	}
	auto handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	

	if (!InjectDll(handle, (basePath + "/dx11_imgui_lib.dll").c_str())) {
		std::cout << "core module inj failed" << std::endl;
		system("pause");
		return -2;
	}

	Sleep(3000);


	if (!InjectDll(handle, (basePath + "/deep_rock_hack.dll").c_str())) {
		std::cout << "core module inj failed" << std::endl;
		system("pause");
		return -3;
	}

	std::cout << "succeed !" << std::endl;
	system("pause");
	return 0;

}