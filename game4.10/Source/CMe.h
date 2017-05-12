#pragma once
#include "CCharacter.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 控制所有的主角CHARACTER
/////////////////////////////////////////////////////////////////////////////
class CMe {
    public:

        CMe();
        void Initialize();				// 設定初始值
        void LoadBitmap();				// 載入圖形
        void OnMove();					// 移動圖形
        void OnShow();					// 將圖形圖形貼到畫面
        int  GetX1();					// ME左上角 x 座標
        int  GetY1();					// ME左上角 y 座標
        int  GetX2();					// ME右下角 x 座標
        int  GetY2();					// ME右下角 y 座標
        void SetXY(int nx, int ny);		// 設定擦子左上角座標
        void LoadCharacter();			// 載入角色
        void addSelectedChar(int);		// 選擇角色時移動游標
        void setState(int);				// 設定當前顯示的狀態

    protected:
        const int CHARACTER_POS_Y;
        //CAnimation animation;			// 圖形的動畫
        int x, y;						// 圖形左上角座標
        vector<CCharacter*> character;
        int  selectedChar;				// 選中的角色編號
        int  currState;					// 0: 遊戲中, 1: 選擇角色畫面
};
}
