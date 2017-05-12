#include "CMe.h"
#include "CDict.h"
#include "CBomb.h"
#include "CEnemy.h"
#include "CBullet.h"
#include "CMap.h"
#include "CBossA.h"
#include "CBossB.h"
#include "CEmp.h"


namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID {				// 定義各種音效的編號
    AUDIO_DING,				// 0
    AUDIO_LAKE,				// 1
    AUDIO_NTUT,				// 2
    //===========================================
    AUDIO_ROCK,             // 3
    AUDIO_SHOT              // 4
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////


class CGameStateInit : public CGameState {
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnMove();
    protected:
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        const int NOTE_TEXT_X, NOTE_TEXT_Y;				// 定義 遊戲說明 擺放的位置
        const int MENU_POS_Y;							// 定義 MENU 的Y軸
        const int MENU_ITEM_NUM;						// 定義 MENU 項目的數量
        const int CHARACTER_POS_Y;
        CMovingBitmap typing_logo;						// typing typing 精美的LOGO
        CMovingBitmap text1;							// 說明文字
        CMap map;										// 背景
        //主選單元素
        CMovingBitmap menuBorder, menuBorder_ckecked;
        vector<CMovingBitmap*> menuText;				// 選單文字VECTOR
        int currSelectItem, displayState, noteDisplayState; // 當前選擇的MENU, 當前顯示的狀態, 當前顯示的說明狀態

        // 遊戲說明 元素
        CAnimation				noteExkey;						// 遊戲說明裡面的 打字動畫
        CMovingBitmap			noteBorder, noteArrow;			// 框線, 箭頭
        CMovingBitmap			noteSelected, noteUnselected;	// 指示燈
        vector<CMovingBitmap*>	note;							// 多頁的說明文字
        // 角色選擇 元素
        CMovingBitmap			characterBorder, characterArrow[2];
        CMovingBitmap			me_ironman;
        // 介紹頁面
        CMovingBitmap			aboutBorder;
        CMovingBitmap			about;


};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        const int		NUMBALLS;	// 球的總數
        CMovingBitmap	background;	// 背景圖
        CMovingBitmap	help;		// 說明圖
        CMovingBitmap	corner;		// 角落圖
        //////////
        bool			showDebug = false;								// 是否顯示debug資訊
        CDict			dictionary;										// 所有怪物共用的字典
        CMap			map;											// 背景圖
        CMe				me;												// 主角
        vector<CEnemy*> enemyQueue;										// 儲存所有敵人的Vector
        vector<CBullet*>bulletList;										// 儲存飛行中的子彈的Vector
        vector<CBomb*>  bombList;

        CEnemy* 			targetEnemy;									// 指標 用於指向瞄準的敵人
        CInteger		score;											// 分數顯示器
        CEmp			emp;
        const int		LEVEL;											// 關卡總數
        int				callEnemyCounter, maxCallEnemyCounter;			// 召喚怪物間隔計數器, 召喚怪物間隔; maxCallEnemyCounter 決定怪物生成速度 越小速度越快
        int				callBossACounter, maxCallBossACounter;
        int				callBossBCounter, maxCallBossBCounter;
        int				currEnemyNum;									// 當前該關卡 已召喚的敵人數量
        int				currBossANum, currBossBNum;						// 當前該關卡 已召喚的BossA & BossB數量
        bool			lock;											// 判斷是否鎖住第一個字母了
        int				currLevel;										// 當前關卡

        int				levelEnemyNum[10] = {  4,  5,  5,  6,  7,  7,  7, 20, 20, 20 };		// 該關卡最大的敵人數
        int				levelBossANum[10] = {  0,  0,  1,  1,  1,  2,  1,  0,  0,  0 };
        int				levelBossBNum[10] = {  0,  0,  0,  0,  0,  0,  1,  0,  0,  0 };

        int				lives;
        int				totalKeyDownCount, totalCorrectKeyCount;		// 總按鍵數, 總正確按鍵數
        double			accuracy;										// 正確率
        char			keyFlag;
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        int counter;	// 倒數之計數器
};

}