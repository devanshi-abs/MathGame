/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, dis   qtribute, sublicense, and/or sell
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

#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "counting.h"

// #define USE_AUDIO_ENGINE 1
#define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

int TotalCoin=200;
int WhichScene,whichSubLvl,totalSubLvl[10];
int LanguageID = 0;//3
int cntExitPopYesTap,cntExitIcon,cntExitLayout,cntMoreIconAds;
bool isExitShow,isLoadingScreen,isSkipLvl,isLvlCmp[10],isBackBtn=true,isLevelCompete,isFirstTimeAds=true,isCommonPopupOpen,isFromLvl,isLevelScreen;
float viewScaleY=1.0;
cocos2d::Size ResolutionSize;
float Device_Resosultion_Height;
std::string strVocalLang="en-us";
int whichPopToySelected = 1;
bool isCmpPopToy[30];
bool isUnLockPopToy[30];
bool isRRVehicleUnLock[20];
int TTSImageNo;
bool isNormalColor=true;
int WhichFillClrPg=1,WhichClrPg=1;
int whichMinigame;
bool isMinigameLS,isColorsLS;
bool isRewardBalloon=false,isPlayArea=false;

int whichLevelSelected = 2;//1
std::string AndroidJniPackage;

float XSpaceToPadding = 0;
float YSpaceToPadding = 0;
bool isAppreciationPopup,isNativeAds,isCollapsiveAds;

//=====Screen Timing====//
time_t _time1,_time2;
struct std::tm _startTime;
struct std::tm _endTime;

std::string PhotoSaveName = "DinoCarBuilding.png";
//int CDToyNo=1;
bool isLevelAssestDownloaded[50];

bool isCoinPanelShown;
std::map<int,std::string> str_Level_AssetPack;
std::vector<std::string> str_DownloadedAssetPacksList;
int DressUpUnLock[20][50];

int ambuAirToolCoin[20],ambuCleanToolCoin[20],bulldozerSerToolCoin[20],busAirToolCoin[20],craneDecorToolCoin[10][20],craneCleanToolCoin[20],jcbAirToolCoin[20],levelLockCoin[100],policeDecorToolCoin[10][20],policeSerToolCoin[20],tractorAirToolCoin[20],tractorCleanToolCoin[20],tractorDecorToolCoin[10][20];

int cntStickers;

bool isLevelLock[20],isMinigameLock[20];


USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("CarBuilding", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("CarBuilding");
#endif
        director->setOpenGLView(glview);
    }
    
    // turn on display FPS
    director->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);
    
    // Set the design resolution
    //    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    //    auto frameSize = glview->getFrameSize();
    //    // if the frame's height is larger than the height of medium size.
    //    if (frameSize.height > mediumResolutionSize.height)
    //    {
    //        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    //    }
    //    // if the frame's height is larger than the height of small size.
    //    else if (frameSize.height > smallResolutionSize.height)
    //    {
    //        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    //    }
    //    // if the frame's height is smaller than the height of medium size.
    //    else
    //    {
    //        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    //    }
    
    auto file = FileUtils::getInstance();
    std::vector<std::string> Paths = file->getSearchPaths();
    Paths.push_back("Counting");
    
//    str_Level_AssetPack.insert({Lvl_Tractor,"Tractor"});
//    str_Level_AssetPack.insert({Lvl_Bus,"Bus"});
//    str_Level_AssetPack.insert({Lvl_BullDozer,"BullDozer"});
//    str_Level_AssetPack.insert({Lvl_PoliceCar,"PoliceCar"});
//    str_Level_AssetPack.insert({Lvl_Minigames,"Minigames"});
//    str_Level_AssetPack.insert({LvlColorLvls,"ColorsLvls"});
//    str_Level_AssetPack.insert({Lvl_DressUp,"DressUp"});
    
    file->setSearchPaths(Paths);
    
    isUnLockPopToy[1]=true;
    
//    isMinigameLock[Lvl_DinoAirplane]=true;
//    isMinigameLock[Lvl_Aplhabet]=true;
//    isLevelLock[Lvl_DressUp]=true;
//    isLevelLock[Lvl_BullDozer]=true;
//    isLevelLock[Lvl_TapToFill]=true;
    
#pragma mark - "Set Coins Value"
    {
        for (int i=1; i<=11; i++)
        {
            ambuAirToolCoin[i]=0;
        }
        
        ambuAirToolCoin[2]=30;
        ambuAirToolCoin[3]=65;
        ambuAirToolCoin[5]=40;
        ambuAirToolCoin[7]=50;
        ambuAirToolCoin[9]=60;
        
        for (int i=1; i<=9; i++)
        {
            ambuCleanToolCoin[i]=0;
        }
        
        ambuCleanToolCoin[3]=30;
        ambuCleanToolCoin[8]=40;
        ambuCleanToolCoin[9]=50;
        
        for (int i=1; i<=2; i++)
        {
            bulldozerSerToolCoin[i]=0;
        }
        
        bulldozerSerToolCoin[2]=30;
        
        
        for (int i=1; i<=20; i++)
        {
            busAirToolCoin[i]=0;
        }
        
        busAirToolCoin[7]=30;
        busAirToolCoin[9]=65;
        busAirToolCoin[11]=40;
        busAirToolCoin[13]=47;
        
        for (int i=1; i<=5; i++)
        {
            for (int j=1;j<=9;j++)
            {
                if(j%2==0)
                {
                    craneDecorToolCoin[i][j]=RandomHelper::random_int(10, 30);
                }
                else
                {
                    craneDecorToolCoin[i][j]=0;
                }
            }
        }
        for (int i=1; i<=10; i++)
        {
            craneCleanToolCoin[i]=0;
        }
        
        craneCleanToolCoin[2]=20;
        craneCleanToolCoin[4]=45;
        craneCleanToolCoin[6]=50;
        
        for (int i=1; i<=8; i++)
        {
            jcbAirToolCoin[i]=0;
        }
        
        jcbAirToolCoin[1]=15;
        jcbAirToolCoin[3]=35;
        
        for (int i=1; i<=10; i++)
        {
            levelLockCoin[i]=0;
        }
//        levelLockCoin[Lvl_Tractor]=150;
//        levelLockCoin[Lvl_JCB]=100;
        
        for (int i=1; i<=10; i++)
        {
            policeSerToolCoin[i]=0;
        }
        
        policeSerToolCoin[1]=30;
        policeSerToolCoin[2]=55;
        policeSerToolCoin[3]=30;
        policeSerToolCoin[5]=57;
        policeSerToolCoin[7]=67;
        
        
        for (int i=1; i<=10; i++)
        {
            tractorAirToolCoin[i]=0;
        }
        
        tractorAirToolCoin[1]=30;
        tractorAirToolCoin[2]=65;
        tractorAirToolCoin[4]=40;
        
        for (int i=1; i<=10; i++)
        {
            tractorCleanToolCoin[i]=0;
        }
        
        tractorCleanToolCoin[2]=50;
        
        for (int i=1; i<=5; i++)
        {
            for (int j=1;j<=9;j++)
            {
                if(j%2==0)
                {
                    tractorDecorToolCoin[i][j]=RandomHelper::random_int(10, 30);
                }
                else
                {
                    tractorDecorToolCoin[i][j]=0;
                }
            }
        }
        
        //Dressup
        for (int i=1; i<=7; i++)
        {
            for (int j=1; j<=25; j++)
            {
                DressUpUnLock[i][j] = 0;
                
                if(j%2==0)
                {
                    DressUpUnLock[i][j]=RandomHelper::random_int(10, 30);
                }
                else
                {
                    DressUpUnLock[i][j]=0;
                }
            }
        }
        
    }
    
    if(UserDefault::getInstance()->getBoolForKey("isFirstTimeHint")==false)
    {
        UserDefault::getInstance()->setBoolForKey("isFirstTimeHint", true);
        for(int i=1;i<=12;i++)
        {
            UserDefault::getInstance()->setIntegerForKey(__String::createWithFormat("StickerNo%d",cntStickers)->getCString(), 0);
        }
    }
    else
    {
        int whichStickerKeyNo;
        for(int i=1;i<=12;i++)
        {
            whichStickerKeyNo=UserDefault::getInstance()->getIntegerForKey(__String::createWithFormat("StickerNo%d",i)->getCString());
            
            if(whichStickerKeyNo!=0)
            {
                cntStickers++;
            }
        }
    }
    
    //Appdelegate cpp
    ResolutionSize=Director::getInstance()->getWinSize();
    Device_Resosultion_Height=ResolutionSize.height;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::getInstance()->playBackgroundMusic("bgMusic.mp3",true);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.05);
#endif
    
    AndroidJniPackage = __String::createWithFormat("%s/AppActivity",UserDefault::getInstance()->getStringForKey("PackageNameCocos").c_str())->getCString();
    
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    
    glview->setDesignResolutionSize(1366, 768, ResolutionPolicy::EXACT_FIT);
    
    register_all_packages();
    
    // create a scene. it's an autorelease object
    auto scene = counting::createScene();//
    
    // run
    director->runWithScene(scene);
    
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    
    //VideoPlayer2dx::playVideo();
    
#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
//void AppDelegate::ShowMore()
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    cocos2d::JniMethodInfo t;
//    if (cocos2d::JniHelper::getStaticMethodInfo(t,AndroidJniPackage.c_str(), "showMore", "()V"))
//    {
//        t.env->CallStaticVoidMethod(t.classID, t.methodID);
//        t.env->DeleteLocalRef(t.classID);
//    }
//#endif
//}
//void AppDelegate::ShowInterstitial()
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    cocos2d::JniMethodInfo t;
//    if (cocos2d::JniHelper::getStaticMethodInfo(t,AndroidJniPackage.c_str(), "showInterstitial", "()V"))
//    {
//        t.env->CallStaticVoidMethod(t.classID, t.methodID);
//        t.env->DeleteLocalRef(t.classID);
//    }
//#endif
//}
//void AppDelegate::ShowRewardVideo()
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    cocos2d::JniMethodInfo t;
//    if (cocos2d::JniHelper::getStaticMethodInfo(t,AndroidJniPackage.c_str(), "ShowRewardedVideo", "()V"))
//    {
//        t.env->CallStaticVoidMethod(t.classID, t.methodID);
//        t.env->DeleteLocalRef(t.classID);
//    }
//#endif
//}
//void AppDelegate::ShowNativeAds()
//{
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    cocos2d::JniMethodInfo t;
//    if (cocos2d::JniHelper::getStaticMethodInfo(t,AndroidJniPackage.c_str(), "ShowNativeAds", "()V"))
//    {
//        t.env->CallStaticVoidMethod(t.classID, t.methodID);
//        t.env->DeleteLocalRef(t.classID);
//    }
//#endif
//}
//void AppDelegate::RemoveNativeAds()
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    cocos2d::JniMethodInfo t;
//    if (cocos2d::JniHelper::getStaticMethodInfo(t,AndroidJniPackage.c_str(), "RemoveNativeAds", "()V"))
//    {
//        t.env->CallStaticVoidMethod(t.classID, t.methodID);
//        t.env->DeleteLocalRef(t.classID);
//    }
//#endif
//}
void AppDelegate::GetBannerHeight(cocos2d::Layer *scene_name)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    viewScaleY = UserDefault::getInstance()->getFloatForKey("scaleRate");
    
    if (viewScaleY>1 || viewScaleY<=0.7) {
        viewScaleY = 1;
    }
    
    if(UserDefault::getInstance()->getBoolForKey("banner_pos_top")==true)
    {
        scene_name->setAnchorPoint(Vec2(0.5,0));//To set ads at top
    }
    else
    {
        scene_name->setAnchorPoint(Vec2(0.5,1));//To set ads at bottom
    }
    scene_name->setScaleY(viewScaleY);
    
    //  CCLOG("Name -> %s Tag-> %i Scale->%f",scene_name->getName().c_str(),scene_name->getTag(),scene_name->getScale());
#endif
    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    scene_name->setScaleY(0.75);
    //#endif
    
}
void AppDelegate::storeUserPlayTime()
{
    _time2 = time(0);
    _endTime = *localtime(&_time2);
    double _diff = difftime(mktime(&_endTime), mktime(&_startTime));
    
    int hour = int(_diff) / 3600;
    int minute = (int(_diff) % 3600) / 60;
    int second = int(_diff) % 60;
    std::string strHr,strMin,strSec,userTiming,strScrnName,screenName;
    
    if(hour<10){strHr="0"+cocos2d::StringUtils::toString(hour);}
    else{strHr=cocos2d::StringUtils::toString(hour);}
    
    if(minute<10){strMin="0"+cocos2d::StringUtils::toString(minute);}
    else{strMin=cocos2d::StringUtils::toString(minute);}
    
    if(second<10){strSec="0"+cocos2d::StringUtils::toString(second);}
    else{strSec=cocos2d::StringUtils::toString(second);}
    
    userTiming=strHr+":"+strMin+":"+strSec;
    CCLOG("Time %s",userTiming.c_str());
    
    screenName=UserDefault::getInstance()->getStringForKey("CLASSNAME");
    
    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //    cocos2d::JniMethodInfo t;
    //
    //    if (cocos2d::JniHelper::getStaticMethodInfo(t,AndroidJniPackage.c_str(), "storeScreenTime", "(Ljava/lang/String;Ljava/lang/String;)V"))
    //    {
    //        const char* cstr1 = screenName.c_str();
    //        const char* cstr2 = userTiming.c_str();
    //
    //        jstring jstr1 = t.env->NewStringUTF(cstr1);
    //        jstring jstr2 = t.env->NewStringUTF(cstr2);
    //
    //        t.env->CallStaticVoidMethod(t.classID, t.methodID,jstr1,jstr2);
    //        t.env->DeleteLocalRef(t.classID);
    //    }
    //#endif
}
void AppDelegate::sendLevelCompleteStatus(std::string LevelStatus)//LevelStatus -> Complete or ExitHalfWay
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    
    if (cocos2d::JniHelper::getStaticMethodInfo(t,AndroidJniPackage.c_str(), "sendLevelCompleteStatus", "(Ljava/lang/String;)V"))
    {
        //            const char* cstr1 = screenName.c_str();
        const char* cstr2 = LevelStatus.c_str();
        
        // jstring jstr1 = t.env->NewStringUTF(cstr1);
        jstring jstr2 = t.env->NewStringUTF(cstr2);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID,jstr2);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void AppDelegate::sendScreenName(std::string screenName)
{
    //========Get starting time for Screen Timing========//
    _time1 = time(0);
    _startTime = *localtime(&_time1);
    
    //========Store Screen name========//
    UserDefault::getInstance()->setStringForKey("CLASSNAME", screenName);
    UserDefault::getInstance()->flush();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    
    if (cocos2d::JniHelper::getStaticMethodInfo(t,AndroidJniPackage.c_str(), "storeScreenName", "(Ljava/lang/String;)V"))
    {
        const char* cstr1 = screenName.c_str();
        
        jstring jstr1 = t.env->NewStringUTF(cstr1);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID,jstr1);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
    
}
bool AppDelegate::getBackTouchValue()
{
    if(isExitShow==true || isAppreciationPopup==true)
    {
        return true;
    }
    else
    {
        return false;
    }
}
// Implement the translation fetching method
std::string AppDelegate::getTranslation(const std::string& key, int languageIndex)
{
    // Load the .plist file into a ValueMap
    auto languages = FileUtils::getInstance()->getValueMapFromFile("MultiLanguage.plist");
    
    // Check if the provided key exists
    if (languages.find(key) != languages.end()) {
        // Extract the dictionary associated with the given key (e.g., "RoomClean")
        auto translationDict = languages[key].asValueMap();
        
        // Construct the key for the language number
        std::string languageKey = "New item - " + std::to_string(languageIndex);
        
        // Check if the language key exists
        if (translationDict.find(languageKey) != translationDict.end()) {
            // Return the translation as a string
            return translationDict[languageKey].asString();
        } else {
            CCLOG("Language key not found for index: %d", languageIndex);
            return "";  // Return empty string if the language key is not found
        }
    } else {
        CCLOG("Key not found: %s", key.c_str());
        return "";  // Return empty string if the key is not found
    }
}
std::string AppDelegate::getTranslation(const std::string& key)
{
    // Load the .plist file into a ValueMap
    auto languages = FileUtils::getInstance()->getValueMapFromFile("MultiLanguage.plist");
    
    // Check if the provided key exists
    if (languages.find(key) != languages.end()) {
        // Extract the dictionary associated with the given key (e.g., "RoomClean")
        auto translationDict = languages[key].asValueMap();
        
        // Construct the key for the language number
        std::string languageKey = "New item - " + std::to_string(LanguageID);
        
        // Check if the language key exists
        if (translationDict.find(languageKey) != translationDict.end()) {
            // Return the translation as a string
            return translationDict[languageKey].asString();
        } else {
            CCLOG("Language key not found for index: %d", LanguageID);
            return "";  // Return empty string if the language key is not found
        }
    } else {
        CCLOG("Key not found: %s", key.c_str());
        return "";  // Return empty string if the key is not found
    }
}
