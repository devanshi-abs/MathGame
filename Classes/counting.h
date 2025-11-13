//
//  counting.h
//  MyGame
//
//  Created by Mac on 30/10/25.
//
//

#ifndef counting_h
#define counting_h

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
class counting : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
#endif
    cocos2d::Point locPoint;
    float POSX,POSY,POSXY;
    
    ConfigManager* cfg;
    
    int tmp;
    int countGenNo;
    int totalObj=16;
    int totalCntNo=10;
    int whichOptionDrag;
    int loopObjNo;
    int tapObj=0;
    bool isMoved;
    bool phaseCmp;
    
    //=====Background Sprites=====//
    Sprite *Background;
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
    cocos2d::Point PosObj[20][30];
    cocos2d::Size objSize[20];
    void genRandomObj();
    
    //=====Create Sprite Functions=====//
    void createBackgroundSprite();
    void createOwlSprite();
    void createAnsOptionSprite();
    void createCountingObjects();
    
    void createSpriteFromConfig(const rapidjson::Value& spr);
    cocos2d::Sprite* addSprite(
            cocos2d::Node* parent,
            const std::string& imageName,
            const cocos2d::Vec2& position,
            const cocos2d::Vec2& scale = cocos2d::Vec2(1.0f, 1.0f)
        );
    
    //=====Effects Functions=====//
    
    void entryEffect();
    void showOptionBox();
    void scaleEffectQuesMark();
    
    void idealEffectObj();
    void idealEffectAnsOption();
    
    void eyeBlinkAnim();
    void showSuccessParticle();
    
    void slideOpen();
    void slideClose();
    bool isSlideOpen;
    
    void countingEffect();
    
    void playAppreciationSound();
    void levelCmpEffect();
    
    void resetTapCount();
    ParticleSystemQuad *prtcl;
    
    virtual bool init();
    virtual void update(float delta);
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    //----------------------------------------------------END-------------------------------------------------------//
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *event);
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *event);
    
    void EntrySound();
    void TouchOn();
    void TouchOff();
    CREATE_FUNC(counting);
};

#endif /* counting_h */
