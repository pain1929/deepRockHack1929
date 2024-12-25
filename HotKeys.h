#pragma once


namespace HotKeys {

	namespace Other {
		inline bool aimbotActive = false; //瞄准辅助
		inline float  aimRange = 200.f;  // 瞄准范围
		inline bool drawBonesActive = false; // 绘制骨骼
		inline bool draw2DBoundingBoxActive = false; // 2d
		inline bool draw3DBoundingBoxActive = false; // 3d
		inline bool drawNamesActive = false;         // 绘制名字
		inline bool drawPlayerNamesActive = false;   // 绘制玩家名称
		inline bool infiniteAmmoActive = false;      // 无线子弹无效
		inline bool removeRecoilActive = false;      // 移除后坐力无效
	}
	//枪手
	namespace Shooter {

		inline bool qianBaoCdActive = false; //铅爆卡红温
		inline bool qianBaoinfiniteAmmoActive = false; //铅爆无限子弹
		inline bool qianBaoNoPre = false; //!<铅爆无预热
	}
}
