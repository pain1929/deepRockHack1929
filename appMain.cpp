#include "mem.hpp"
#include "inj.hpp"
#include <iostream>
#include <filesystem>
#include "fromGithub/fromGithub.h"
#include "config.h"


GoString toGoStr(const char * s) {
	return { s , (int64_t )strlen(s) };
}
std::string toStdStr(GoString s) {
	return std::string(s.p);
}


void updateFromGithub() {

	 const char*  saveTo = "./newVersion.zip";
     const char * uname = "pain1929";
     const char * rep = "deepRockHack1929";
     const char * fileName = "README.md";

	std::cout<<"当前版本 V" << CURRENT_VERSION <<std::endl;
	std::cout<<"正在读取 README ..."<<std::endl;

	// 读取readme
	auto readMe = GetTextFromGithub(toGoStr(uname), toGoStr(rep), toGoStr(fileName));
	if (readMe) {
		std::cout << readMe << std::endl;
	}
	


	//检查更新
	std::cout << "正在检查更新 ... " << std::endl;
	auto ret = FromGithub(toGoStr(uname), toGoStr(rep), toGoStr(CURRENT_VERSION), toGoStr(saveTo));

	if (ret) {
		std::cout << "发现的新版本. 更新成功. 文件保存到" << saveTo << " 请解压后启动新版本" <<std::endl;
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

	// 设置控制台输出编码为 UTF-8
	SetConsoleOutputCP(CP_UTF8);

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
		std::cout << "请先等待游戏启动" << std::endl;
		system("pause");
		return -1;
	}
	auto handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	

	if (!InjectDll(handle, (basePath + "/dx11_imgui_lib.dll").c_str())) {
		std::cout << "dx11_imgui_lib.dll 注入失败" << std::endl;
		system("pause");
		return -2;
	}
	std::cout << "dx11_imgui_lib.dll 注入成功" << std::endl;
	Sleep(3000);


	if (!InjectDll(handle, (basePath + "/deep_rock_hack.dll").c_str())) {
		std::cout << "deep_rock_hack.dll 注入失败" << std::endl;
		system("pause");
		return -3;
	}

	std::cout << "外挂注入成功!!! 可关闭此程序" << std::endl;
	system("pause");
	return 0;

}