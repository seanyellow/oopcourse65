#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <time.h>
#include "audio.h"
#include "gamelib.h"
#include "CDict.h"
#include "CMe.h"
#include "CEnemy.h"
#include <math.h>
#include <stdlib.h>

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CEnemy: Enemy
/////////////////////////////////////////////////////////////////////////////
CEnemy::CEnemy() {
}
CEnemy::~CEnemy() {
}
CEnemy::CEnemy(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList, int endX, int endY, vector<CMovingBitmap*>* letter) {	//	初始值都在此處設定
    is_alive = is_bombed = false;
    dx = dy = index = delay_counter = 0;
    currWordLeng = 0;
    targetX = -2;
    targetY = -2;
    ////
    SetXY(x, y);
    SetDelay(delay);
    SetIsAlive(alive);
    this->dict = d;
    this->bombList = bombList;
    this->enemyQueue = enemyQueue;
    this->minVocabLeng = minVL;
    this->maxVocabLeng = maxVL;
    this->endX = endX;
    this->endY = endY;
    this->letter = letter;
    SetVocab();
    index = (maxVocabLeng == 1 && minVocabLeng == 1) ? 10 : 0;
}



bool CEnemy::HitRectangle(int tx1, int ty1, int tx2, int ty2) {
    int x1 = x + dx;				// 怪物face的左上角x座標
    int y1 = y + dy;				// 怪物face的左上角y座標
    int x2 = x1 + bmp.Width();		// 怪物face的右下角x座標
    int y2 = y1 + bmp.Height();		// 怪物face的右下角y座標
    //
    // 檢測怪物face的矩形與參數矩形是否有交集
    //
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

bool CEnemy::IsAlive() {
    return is_alive;
}

void CEnemy::LoadBitmap() {
    char str[30];
    const unsigned int bitmapNum = 7;		// 圖檔數量

    if (maxVocabLeng == 1 && minVocabLeng == 1) {
        sprintf(str, "Bitmaps/face/face_min%d.bmp", rand() % (4) + 1);	// 1字小怪的bmp
    }
    else  sprintf(str, "Bitmaps/face/face%d.bmp", rand() % bitmapNum + 1);  // 一般小怪的bmp

    bmp.LoadBitmap(str, RGB(0, 255, 0)); // 載入 怪物SKIN
    textCursor.LoadBitmap("Bitmaps/text_cursor.bmp", RGB(0, 255, 0));  //載入 光標
    /////
    talkBoxL.LoadBitmap("Bitmaps/talk_box_blur/talk_box_left.bmp", RGB(0, 255, 0));
    talkBoxC.LoadBitmap("Bitmaps/talk_box_blur/talk_box_center.bmp", RGB(0, 255, 0));
    talkBoxR.LoadBitmap("Bitmaps/talk_box_blur/talk_box_right.bmp", RGB(0, 255, 0));
    /////

    for (int i = 0; i < 22; i++) {	// 載入動畫
        sprintf(str, "Bitmaps/target/target_s%d.bmp", i + 1);
        target.AddBitmap(str, RGB(0, 255, 0));
    }

    target.SetDelayCount(2);
}

void CEnemy::OnMove() {
    const int STEPS = 180;	// 切成幾分dx
    //if (!is_alive) return;
    delay_counter--;
    target.OnMove();

    if (delay_counter < 0) {
        delay_counter = delay;
        index++;
        // if (index >= STEPS)
        //   index = 0;
        // dx = xMoveDistance / STEPS * index;
        double dxTemp = (double(endX) - x) / STEPS * index;
        double dyTemp = (double(endY) - y) / STEPS * index;
        dx  = int(dxTemp);  // dx為 (Enemy<->Me之x總距離) / STEPS * index;
        dy  = int(dyTemp);
    }
}

void CEnemy::SetDelay(int d) {
    delay = d;
}
void CEnemy::SetIsAlive(bool alive) {
    is_alive = alive;
}
void CEnemy::SetXY(int nx, int ny) {
    x = nx;
    y = ny;
}
void CEnemy::OnShow() {
    if (1) {
        bmp.SetTopLeft(x + dx, y + dy);
        bmp.ShowBitmap();

        // 改良後顯示talkbox方式, 完全依照單字長度
        if (currWordLeng < length) {
            talkBoxL.SetTopLeft(x + dx + bmp.Width(), y + dy);
            talkBoxL.ShowBitmap();

            for (int i = 0; i < length; i++) {
                talkBoxC.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + i * talkBoxC.Width(), y + dy);
                talkBoxC.ShowBitmap();
            }

            talkBoxR.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + length * talkBoxC.Width(), y + dy);
            talkBoxR.ShowBitmap();
            ////
            //// show target bmp
            target.SetTopLeft(x + dx + targetX, y + dy + targetY);
            textCursor.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + ((currWordLeng ) * 10) - 1, y + dy);	// 顯示光標
            (currWordLeng != 0) ? target.OnShow(), textCursor.ShowBitmap() : 0;

            //
            for (int i = 0; i < length; i++) {
                letter->at(vocab[i] - 97)->SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + letter->at(0)->Width() * i, y + dy + 4);
                letter->at(vocab[i] - 97)->ShowBitmap();
            }

            for (int i = 0; i < currWordLeng; i++) {	// 讓打過的單字蓋掉 消失不見
                talkBoxC.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + i * talkBoxC.Width(), y + dy);
                talkBoxC.ShowBitmap();
            }
        }
    }
}
void CEnemy::kill() {
    is_alive = false;
    bombList->push_back(new CBomb(GetX(), GetY()));
    bombList->back()->LoadBitmap();
    is_bombed = true;
}


////////////
void  CEnemy::SetVocab() {			//隨機從dict中抓取一個單字到vocab裡面
    while (1) {
        if (maxVocabLeng == 1 && minVocabLeng == 1) {	// 若為1字小怪
            vocab = "a";			//要給定值 不然會出錯
            vocab[0] = 97 + rand() % 26;
            length = 1;
        }
        else {
            vocab = dict->GetText();	// 給vocab一個單字
            length = vocab.length();
        }

        if (length >= minVocabLeng && length <= maxVocabLeng) {			// 條件成立,使用break跳出迴圈 確定生成此單字
            bool firstWordBounceFlag = 0;		//	有撞到第一個單字的flag

            for (int i = enemyQueue->size() - 1; i >= 0; i--) {
                if (vocab[0] == enemyQueue->at(i)->GetFirstWord() && enemyQueue->at(i)->IsAlive())
                    firstWordBounceFlag = 1;
            }

            if (firstWordBounceFlag && !(enemyQueue->size() >= 24)) {}
            else break;
        }
    }
}

string CEnemy::GetVocab() {		  // 回傳整組單字(ex: "apple")
    return vocab;
}
char CEnemy::GetFirstWord()		{ // 以char回傳一個字 (ex: 'a')
    return vocab[0];
}
void CEnemy::AddCurrWordLeng() {
    currWordLeng++;
}
int CEnemy::GetCurrWordLeng() {
    return currWordLeng;
}
int CEnemy::GetVocabLeng() {
    return length;
}
int CEnemy::GetX() {
    return x + dx;
}
int CEnemy::GetY() {
    return y + dy;
}
int CEnemy::GetX2() {
    return x + dx + bmp.Width();
}
int CEnemy::GetY2() {
    return y + dy + bmp.Height();
}
void CEnemy::MinusIndex(int num) {
    index = index - num;
}
bool CEnemy::HitMe(CMe* me) {
    return HitRectangle(me->GetX1(), me->GetY1(),
                        me->GetX2(), me->GetY2());
}
bool  CEnemy::IsBombed() {
    return is_bombed;
}
}