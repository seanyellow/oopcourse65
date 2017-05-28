#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include "CFile.h"


namespace game_framework {


CFile::CFile() {
}
void CFile::WriteHighScore(int score, int level, double accuracy, string meName, int totalCorrectKeyCount) {
    struct tm* T = NULL;
    time_t t;
    time(&t);
    T = localtime(&t);
    fstream	fp, test;
    fp.open("user/bestRecord.txt", ios::out);
    sprintf(ChDate, "%d%02d%02d%02d%02d", int(T->tm_year + 1900), int(T->tm_mon + 1), int(T->tm_mday), int(T->tm_hour), int(T->tm_min));
    this->HighScore_Date.assign(ChDate);
    fp << "character:" << meName
       << ",score:" << score
       << ",level:" << level
       << ",accuracy:" << accuracy
       << ",date:" << HighScore_Date
       << ",totalCorrectKeyCount:" << totalCorrectKeyCount << endl;
    fp.close();
}
void CFile::ReadHighScoreFile() {
    string slideOne, slideTwo[12];
    char temp[100];
    int i = 0;
    fstream	fp;
    fp.open("user/bestRecord.txt", ios::in);

    while (fp.getline(temp, sizeof(temp), ',')) {
        slideOne = temp;
        stringstream ss(slideOne);

        while (getline(ss, slideTwo[i], ':')) {
            if (i == 1) this->HighScore_MeName = slideTwo[1];
            else if (i == 3) this->HighScore_Score = stoi(slideTwo[3], nullptr, 10);
            else if (i == 5) this->HighScore_Level = stoi(slideTwo[5], nullptr, 10);
            else if (i == 7) this->HighScore_Accuracy = stod(slideTwo[7], nullptr);
            else if (i == 9) this->HighScore_Date = slideTwo[9];
            else if (i == 11) this->HighScore_TotalCorrectKeyCount = stoi(slideTwo[11], nullptr, 10);

            i++;
        }
    }

    fp.close();
}

int CFile::ReadHighScore_Score() {
    return this->HighScore_Score;
}
int CFile::ReadHighScore_Level() {
    return this->HighScore_Level;
}
double CFile::ReadHighScore_Accuracy() {
    return this->HighScore_Accuracy;
}
string CFile::ReadHighScore_Character() {
    return this->HighScore_MeName;
}
string CFile::ReadHighScore_Date() {
    return this->HighScore_Date;
}
int	CFile::ReadHighScore_TotalCorrectKeyCount() {
    return this->HighScore_TotalCorrectKeyCount;
}
bool CFile::isHighScoreExist() {
    return 1;
}
//======================================
void CFile::WriteRecord(int score, int level, double accuracy, string meName, int totalCorrectKeyCount) {
    struct tm* T = NULL;
    time_t t;
    time(&t);
    T = localtime(&t);
    fstream	fp;
    fp.open("user/record.txt", ios::out | ios::app);
    sprintf(ChDate, "%d%02d%02d%02d%02d", int(T->tm_year + 1900), int(T->tm_mon + 1), int(T->tm_mday), int(T->tm_hour), int(T->tm_min));
    this->record_Date.assign(ChDate);
    fp << "character:" << meName
       << ",score:" << score
       << ",level:" << level
       << ",accuracy:" << accuracy
       << ",date:" << record_Date
       << ",totalCorrectKeyCount:" << totalCorrectKeyCount << endl;
}
void CFile::ReadRecordFile() {
}
int CFile::ReadRecordScore_Score() {
    return this->record_Score;
}
int CFile::ReadRecordScore_Level() {
    return this->record_Level;
}
double CFile::ReadRecordScore_Accuracy() {
    return this->record_Accuracy;
}
string CFile::ReadRecordScore_Character() {
    return this->record_MeName;
}
string CFile::ReadRecordScore_Date() {
    return this->record_Date;
}
int	CFile::ReadRecordScore_TotalCorrectKeyCount() {
    return this->record_TotalCorrectKeyCount;
}
}

