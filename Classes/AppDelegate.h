/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
//#include "Header.h"

extern int TotalCoin;
extern int whichLevelSelected;
extern int cntExitPopYesTap,cntExitIcon,cntExitLayout,cntMoreIconAds;
extern int WhichScene,whichSubLvl,totalSubLvl[10];
extern bool isExitShow,isLoadingScreen,isSkipLvl,isLvlCmp[10],isBackBtn,isLevelCompete,isFirstTimeAds,isCommonPopupOpen,isLevelScreen;
extern float viewScaleY;
extern cocos2d::Size ResolutionSize;
extern float Device_Resosultion_Height;
extern std::string strVocalLang;
extern std::string AndroidJniPackage;
extern int LanguageID;

extern int whichMinigame;
extern bool isMinigameLS,isColorsLS;
extern bool isRewardBalloon,isPlayArea;
extern int WhichFillClrPg,WhichClrPg;

extern float XSpaceToPadding;
extern float YSpaceToPadding;
extern bool isAppreciationPopup,isNativeAds,isCollapsiveAds;
extern int whichPopToySelected;
extern bool isCmpPopToy[30];
extern bool isUnLockPopToy[30];
extern bool isRRVehicleUnLock[20];
extern int TTSImageNo;
extern bool isNormalColor;
//=====Screen Time=====//
extern time_t _time1,_time2;
extern struct std::tm _startTime;
extern struct std::tm _endTime;
extern std::string PhotoSaveName;

extern std::map<int,std::string> str_Level_AssetPack;
extern std::vector<std::string> str_DownloadedAssetPacksList;

extern bool isLevelAssestDownloaded[50];
extern bool isCoinPanelShown;
extern int DressUpUnLock[20][50];

//extern int CDToyNo;

extern int ambuAirToolCoin[20],ambuCleanToolCoin[20],bulldozerSerToolCoin[20],busAirToolCoin[20],craneDecorToolCoin[10][20],craneCleanToolCoin[20],jcbAirToolCoin[20],levelLockCoin[100],policeDecorToolCoin[10][20],policeSerToolCoin[20],tractorAirToolCoin[20],tractorCleanToolCoin[20],tractorDecorToolCoin[10][20];

extern int cntStickers;

extern bool isLevelLock[20],isMinigameLock[20];


/**
 @brief    The cocos2d Application.
 
 Private inheritance here hides part of interface from Director.
 */
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();
    
    virtual void initGLContextAttrs();
    
    /**
     @brief    Implement Director and Scene init code here.
     @return true    Initialize success, app continue.
     @return false   Initialize failed, app terminate.
     */
    virtual bool applicationDidFinishLaunching();
    
    /**
     @brief  Called when the application moves to the background
     @param  the pointer of the application
     */
    virtual void applicationDidEnterBackground();
    
    /**
     @brief  Called when the application reenters the foreground
     @param  the pointer of the application
     */
    virtual void applicationWillEnterForeground();
    
    static void GetBannerHeight(cocos2d::Layer *scene_name);
    
    //    static void ShowMore();
    //    static void ShowInterstitial();
    //    static void ShowRewardVideo();
    //
    //    static void ShowNativeAds();
    //    static void RemoveNativeAds();
    
    static bool getBackTouchValue();
    
    static std::string getTranslation(const std::string& key, int languageIndex);
    static std::string getTranslation(const std::string& key);
    
    
#pragma mark Screen Timing
    static void storeUserPlayTime();
    static void sendScreenName(std::string screenName);
    static void sendLevelCompleteStatus(std::string LevelStatus);//LevelStatus -> Complete or ExitHalfWay
};

#endif // _APP_DELEGATE_H_

