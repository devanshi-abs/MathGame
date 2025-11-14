//
//  Counting.h
//  MyGame
//
//  Created by Mac on 30/10/25.
//
//

#ifndef Counting_h
#define Counting_h

#define IPAD_WIDTH                      1366
#define IPAD_HEIGHT                     768

#define OUTOFSCREEN  Vec2(12345678798,12345678798)
#define CENTEROFSCREEN Vec2(IPAD_WIDTH/2,IPAD_HEIGHT/2)

#include <stdio.h>
#include "cocos2d.h"
#include <SimpleAudioEngine.h>
#include "ConfigManager.h"
using namespace CocosDenshion;
using namespace cocos2d;
class Counting : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    cocos2d::Point locPoint;
    float POSX,POSY,POSXY;
    
    ConfigManager* cfgManager;
    
    int tmp;
    int countGenNo;
    int totalObj=16;
    int totalCntNo=10;
    int totalAnsOptions=3;
    int whichOptionDrag;
    int loopObjNo;
    int tapObj=0;
    bool isMoved;
    bool phaseCmp;
    
    //=====Background Sprites=====//
    Sprite *spriteBackground;
    Sprite *spriteClouds[20];
    Sprite *spriteBoard;
    
    //=====Character=====//
    Layer *lyrOwl;
    Sprite *spriteCharacter;
    Sprite *spriteCharShadow;
    Sprite *spriteCharStool;
        
    //=====Answer Options=====//
    Sprite *spriteAnsOptionBox[20];
    Label *lbl_AnsOption[20];
    cocos2d::Point PosAnsBox[20],PosTouch;
    
    Label *lblEqual;
    Label *lblFinalAns;
    Sprite *spriteBox;
    Sprite *spriteSlideDoor[10];
    cocos2d::Point PosFinalAns;
    
    //=====Count Objects======//
    Sprite *spriteObjects[20];
    Sprite *spriteObjectsNumber[20];
    Label *lblNumbers[20];
    
    void setObjScale(cocos2d::Size ScaleToSize,Node *sprtObj);
    void genRandomObj();
    void showObjects();
    
    //=====Create Sprite Functions=====//
    void createBackgroundSprites();
    void createOwlSprites();
    void createAnsOptionSprites();
    void createCountingObjects();
    void createFinalAnsSprites();
    
    void createSpriteFromConfig(const rapidjson::Value& spr);
    cocos2d::Sprite* addSprite(
            cocos2d::Node* parent,
             Sprite*& sprite,
            const std::string& imageName,
            const cocos2d::Vec2& position,
            const cocos2d::Vec2& scale = cocos2d::Vec2(1.0f, 1.0f)
        );
    
    //=====Effects Functions=====//
    
    void entryEffect();
    void showOptionBoxes();
    void scaleEffectQuesMark();
    
    void idealEffectObjs();
    void idealEffectAnsOptions();
    
    void eyeBlinkAnim();
    
    void slideOpen();
    void slideClose();
    bool isSlideOpen;
    
    void countingEffect();
    
    void playAppreciationSound();
    void playSounds(std::string soundFile);
    void showParticle(const Point &particlePos , bool isLevelEnd ,float delayDuration, int durationPlay, int zOrderNo);
    void resetTapCount();

    virtual bool init();
    //----------------------------------------------------END-------------------------------------------------------//
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *event);
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *event);
    
    void EntrySound();
    void TouchOn();
    void TouchOff();
    CREATE_FUNC(Counting);
};

#endif /* Counting_h */
