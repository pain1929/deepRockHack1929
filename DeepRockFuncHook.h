#pragma once
#include <Windows.h>
#include "MinHook.h"
#include "engine.h"
#include <mutex>
#include "HotKeys.h"
#include "pattern_scan.hpp"

struct FMinimalViewInfo
{
	FVector Location;
	FRotator Rotation;
	float FOV;
	// etc
};

enum EStereoscopicPass
{
	eSSP_FULL,
	eSSP_LEFT_EYE,
	eSSP_RIGHT_EYE,
	eSSP_LEFT_EYE_SIDE,
	eSSP_RIGHT_EYE_SIDE,
};



// Class Engine.LocalPlayer
struct ULocalPlayer : UPlayer {

};
class DeepRockHook {
	uint64_t base{};

	const uint64_t callQianBaoFire_offset{ 0x1667360 };
	const char* callQianBaoFire_pattern = "48 8B C4 56 48 81 EC ? ? ? ? 48 8B F1 0F 29 78 C8";
	const uint64_t qianBaoFire_offset{ 0x166BC20 };
	const char* qianBaoFire_pattern = "40 55 53 56 57 48 8D 6C 24 D8 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 F0";
	const uint64_t qianBaoAddCd_offset{ 0x16C8D40 };
	const char* qianBaoAddCd_pattern = "48 89 5C 24 08 57 48 83 EC ? 0F B6 B9 50 03 00 00";

	static void* callQianBaoFire_origin;
	static void* qianBaoFire_origin;
	static void* qianBaoAddCd_origin;
	
	
	
	static void __fastcall myQianBaoAddCd(__int64* a1, __int64 a2) {
		using QianBaoAddCd = void(__fastcall*) (__int64* a1, __int64 a2);
		if (HotKeys::Shooter::qianBaoCdActive) { //红温状态
			*((float*)a1 + 191) = 8.8f;
		}
		
		((QianBaoAddCd)qianBaoAddCd_origin)(a1, a2);
	}


	static __int64 __fastcall myCallQianBaoFire(__int64 a1, float a2) {
		using CallQianBaoFire = __int64(__fastcall*) (__int64 a1, float a2);
		if(HotKeys::Shooter::qianBaoNoPre){
			*(float*)(a1 + 1960) = 1000.f; //枪管转速
		}
		
		//*(bool*)(a1 + 893) = true; //开火状态
		return ((CallQianBaoFire)callQianBaoFire_origin)(a1, a2);
	}

	static char __fastcall myQianBaoFire(__int64* a1) {
		if(HotKeys::Shooter::qianBaoinfiniteAmmoActive){
			*((DWORD*)a1 + 424) = 9999; //子弹数量
		}
		
		using QianBaoFireFunc = char(__fastcall*)(__int64* a1);
		auto ret = ((QianBaoFireFunc)qianBaoFire_origin)(a1);
		return ret;
	}
public:


	DeepRockHook() {
		base = (uint64_t)GetModuleHandle("FSD-Win64-Shipping.exe");
		MH_Initialize();
	}
	~DeepRockHook() {
		MH_DisableHook((void*)(base + callQianBaoFire_offset));
		MH_DisableHook((void*)(base + qianBaoFire_offset));
		MH_DisableHook((void*)(base + qianBaoAddCd_offset));
		MH_Uninitialize();
	}


	bool init() {

		auto ret_5 = MH_CreateHook(/*(void*)(base + callQianBaoFire_offset)*/
			Scanner::PatternScan("FSD-Win64-Shipping.exe" , callQianBaoFire_pattern),
			myCallQianBaoFire,
			&callQianBaoFire_origin);
		auto ret_6 = MH_EnableHook((void*)(base + callQianBaoFire_offset));

		auto ret_7 = MH_CreateHook(/*(void*)(base + qianBaoFire_offset)*/
			Scanner::PatternScan("FSD-Win64-Shipping.exe", qianBaoFire_pattern),
			myQianBaoFire, 
			&qianBaoFire_origin);
		auto ret_8 = MH_EnableHook((void*)(base + qianBaoFire_offset));
		
		auto ret_9 = MH_CreateHook(/*(void*)(base + qianBaoAddCd_offset)*/
			Scanner::PatternScan("FSD-Win64-Shipping.exe", qianBaoAddCd_pattern),
			myQianBaoAddCd,
			&qianBaoAddCd_origin);
		auto ret_10 = MH_EnableHook((void*)(base + qianBaoAddCd_offset));
		return true;
	}

};

