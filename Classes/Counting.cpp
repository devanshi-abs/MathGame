//
//  counting.cpp
//  MyGame
//
//  Created by Mac on 30/10/25.
//
//

#include "Counting.h"
#include "AppDelegate.h"
#include "json/document.h"
#include "json/rapidjson.h"
#include "json/stringbuffer.h"
#include "json/writer.h"


USING_NS_CC;
Scene*  Counting::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Counting::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}
bool Counting::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    POSX=1;//winsize.width/IPAD_WIDTH;
    POSY=1;//winsize.height/IPAD_HEIGHT;
    POSXY=1;//(POSX+POSY)/2;
    
    tmp=1;
    
    // --- Load JSON Config ---
    cfgManager = ConfigManager::getInstance();
    cfgManager->loadConfig("counting_config.json");
    
#pragma mark - "Background sprites"

    createBackgroundSprites();
    
#pragma mark - "Character sprites"
    
    createOwlSprites();
    
#pragma mark - "Answer Options"
 
    createAnsOptionSprites();
        
#pragma mark - "Final Answer"
    
    createFinalAnsSprites();
   
#pragma mark - "Count Objects"
        
    createCountingObjects();
    
    //====Ideal Effects===//
    
    eyeBlinkAnim();
    scheduleOnce(SEL_SCHEDULE(&Counting::entryEffect), 0.5);
    schedule(SEL_SCHEDULE(&Counting::scaleEffectQuesMark), 5, -1, 3);
    schedule(SEL_SCHEDULE(&Counting::idealEffectObjs), 4, -1, 3);
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(Counting::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(Counting::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(Counting::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(Counting::onTouchesEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    //scheduleUpdate();
    
    TouchOn();
    return true;
}
void Counting::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
    Touch *touch=(Touch*)(touches[0]);
    cocos2d::Point Scenelocation=touch->getLocation();
    cocos2d::Point location=this->convertToNodeSpace(Scenelocation);
    
    if(spriteBackground->getNumberOfRunningActions()!=0)
    {
        return;
    }
    
    for (int i=1;i<=totalAnsOptions;i++)
    {
        if(spriteAnsOptionBox[i]->getBoundingBox().containsPoint(location))
        {
            playSounds("tap.mp3");
            unschedule(SEL_SCHEDULE(&Counting::scaleEffectQuesMark));
            unschedule(SEL_SCHEDULE(&Counting::idealEffectAnsOptions));

            lblFinalAns->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.2, 0.8),EaseBackOut::create(ScaleTo::create(0.4, 1)), NULL)));
            spriteAnsOptionBox[i]->setLocalZOrder(5);
            PosTouch=spriteAnsOptionBox[i]->getPosition();
            isMoved=true;
            whichOptionDrag=i;
            
            break;
        }
    }
    
    for (int i = 1; i <= countGenNo; i++)
    {
        if(spriteObjects[i]->getBoundingBox().containsPoint(location) && phaseCmp==false
           && spriteObjects[i]->getOpacity()==255)
        {
            tapObj++;
            float scaleRatio=spriteObjects[1]->getScaleX();
            
            spriteObjects[i]->setOpacity(180);
            playSounds(__String::createWithFormat("%d.mp3",tapObj)->getCString());

            spriteObjects[i]->runAction(Sequence::create(ScaleTo::create(0.2, scaleRatio+0.2),ScaleTo::create(0.2, scaleRatio),NULL));
            
            scheduleOnce(SEL_SCHEDULE(&Counting::resetTapCount), 5);
        }
    }
}
void Counting::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
    Touch *touch=(Touch*)(touches[0]);
    cocos2d::Point Scenelocation=touch->getLocation();
    cocos2d::Point location=this->convertToNodeSpace(Scenelocation);
        
    if(whichOptionDrag!=0 && isMoved==true)
    {
        spriteAnsOptionBox[whichOptionDrag]->setPosition(Vec2(location.x, location.y+50));
    }
}
void Counting::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *event)
{
    Touch *touch=(Touch*)(touches[0]);
    cocos2d::Point Scenelocation=touch->getLocation();
    cocos2d::Point location=this->convertToNodeSpace(Scenelocation);
    
    if(whichOptionDrag!=0 && isMoved==true)
    {
        schedule(SEL_SCHEDULE(&Counting::scaleEffectQuesMark),5, -1, 0);
        lblFinalAns->stopAllActions();
        lblFinalAns->setPosition(PosFinalAns);
        lblFinalAns->setScale(1,1);
        
        if(spriteAnsOptionBox[whichOptionDrag]->getBoundingBox().intersectsRect(spriteBox->getBoundingBox()) && spriteAnsOptionBox[whichOptionDrag]->getTag()==countGenNo && isSlideOpen==true)
        {
            unschedule(SEL_SCHEDULE(&Counting::slideOpen));
            lblEqual->stopAllActions();
            for (int i=1;i<=2;i++)
            {
                spriteSlideDoor[i]->stopAllActions();
                spriteSlideDoor[i]->setVisible(false);
            }
            
            isMoved=false;
            playSounds("particleClick.mp3");

            showParticle(spriteBox->getPosition(), false, 0.5 ,0.5,20);

            spriteBackground->runAction(DelayTime::create(3));
            
            spriteAnsOptionBox[whichOptionDrag]->runAction(Sequence::create(JumpTo::create(0.5, spriteBox->getPosition(), 100, 1),
                    ScaleTo::create(0.1,1.05),ScaleTo::create(0.1,1.0),DelayTime::create(1.0),
                    CallFunc::create([this]{countingEffect();}),NULL));
        }
        else
        {
            schedule(SEL_SCHEDULE(&Counting::idealEffectAnsOptions), 4.5, -1, 3);

            isMoved=false;
            spriteBackground->runAction(DelayTime::create(0.7));
            spriteAnsOptionBox[whichOptionDrag]->setLocalZOrder(3);

            if(spriteAnsOptionBox[whichOptionDrag]->getTag()==countGenNo)
            {
                playSounds("whoosh.mp3");

                spriteAnsOptionBox[whichOptionDrag]->runAction(Sequence::create(EaseBackIn::create(MoveTo::create(0.2, PosTouch)),
                    CallFunc::create([this]{whichOptionDrag=0;}),NULL));
            }
            else
            {
                playSounds("tapWrong.mp3");

                spriteAnsOptionBox[whichOptionDrag]->runAction(Sequence::create(EaseBackIn::create(MoveTo::create(0.2, PosTouch)),RotateTo::create(0.1,8),
                    RotateTo::create(0.1,-8),RotateTo::create(0.1,8),RotateTo::create(0.1,0),CallFunc::create([this]{whichOptionDrag=0;}),NULL));
            }
        }
    }

    tmp++;
}
#pragma  mark- "Your Methods"
void Counting::genRandomObj()
{
    resetTapCount();
    phaseCmp=false;
    lblEqual->setVisible(false);
    spriteBox->setVisible(false);
    
    countGenNo = RandomHelper::random_int(1,10);
    
    showObjects();
    
    loopObjNo=0;
    for (int i = 1; i <= countGenNo; i++)
    {
        
        spriteObjects[i]->runAction(Sequence::create(DelayTime::create(0.3),DelayTime::create(i * 0.1f),Show::create(),CallFunc::create([this]{
            
            loopObjNo++;
            if(loopObjNo==countGenNo)
            {
                showOptionBoxes();
            }
            
            }),NULL));
    }
    
    // --- Step 1: Generate numbers than Remove the correct numbers to avoid duplication ---
    std::vector<int> numbers;
    for (int i = 1; i <= 10; ++i)
        numbers.push_back(i);
    
        numbers.erase(std::remove(numbers.begin(), numbers.end(), countGenNo), numbers.end());
    
    // --- Step 2: Shuffle the numbers randomly ---
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(numbers.begin(), numbers.end(), g);
    
    std::vector<int> randomNumbers;
    randomNumbers.push_back(countGenNo);  // Always include the correct answer
    randomNumbers.push_back(numbers[0]);
    randomNumbers.push_back(numbers[1]);
    
    // --- Step 3: Shuffle the final 3 so the correct answer isn’t always first ---
    std::shuffle(randomNumbers.begin(), randomNumbers.end(), g);
    
    // --- Step 4: Display them on the labels ---
    CCLOG("Three unique random numbers: %d, %d, %d",
          randomNumbers[0], randomNumbers[1], randomNumbers[2]);
    
    std::vector<Vec2> shuffledPositions = { PosAnsBox[1], PosAnsBox[2], PosAnsBox[3] };
    std::random_shuffle(shuffledPositions.begin(), shuffledPositions.end());
    
    for (int i = 1; i <=totalAnsOptions; i++)
    {
        spriteAnsOptionBox[i]->setTag(0);
        spriteAnsOptionBox[i]->setPosition(shuffledPositions[i - 1]);//PosAnsBox[i]
        spriteAnsOptionBox[i]->setScale(0);
        spriteAnsOptionBox[i]->setLocalZOrder(3);
        
        int index = randomNumbers[i-1];
        lbl_AnsOption[i]->setString(__String::createWithFormat("%d",index)->getCString());
        spriteAnsOptionBox[i]->setTag(index);
    }
}
void Counting::setObjScale(cocos2d::Size ScaleToSize ,Node *sprtObj)
{
    // Get the original size of the image
    Size originalSize = sprtObj->getContentSize();
    
    // Calculate the scale factor for width and height
    float scaleX = ScaleToSize.width / originalSize.width;
    float scaleY = ScaleToSize.height / originalSize.height;
    
    // Choose the smaller scale factor to maintain the aspect ratio
    float scale = std::min(scaleX, scaleY);
    
    // Apply the scale to the sprite
    sprtObj->setScale(scale);
    
    CCLOG("Vec2(%0.2f)",scale);
}
void Counting::showOptionBoxes()
{
    for (int i = 1; i <=totalAnsOptions; i++)
    {
        spriteAnsOptionBox[i]->runAction(Sequence::create(DelayTime::create(0.3),
        DelayTime::create(i * 0.2f),EaseBackOut::create(ScaleTo::create(0.5, 0.9)),NULL));
    }
    
    spriteBackground->runAction(Sequence::create(DelayTime::create(1),CallFunc::create([this]{
        lblEqual->setVisible(true);
        spriteBox->setVisible(true);
        lblEqual->setOpacity(0);
        spriteBox->setOpacity(0);

        lblEqual->runAction(FadeIn::create(0.5));
        spriteBox->runAction(FadeTo::create(0.5,150));
        
        for (int i=1;i<=2;i++)
        {
            spriteSlideDoor[i]->setVisible(true);
        }
        scheduleOnce(SEL_SCHEDULE(&Counting::slideOpen), 0.5);


    }), NULL));
    
    schedule(SEL_SCHEDULE(&Counting::idealEffectAnsOptions), 5.5, -1, 3);

}
void Counting::showObjects()
{
    for (int i=1;i<=totalCntNo;i++)
    {
        spriteObjects[i]->setPosition(OUTOFSCREEN);
        spriteObjects[i]->setScale(0);
        spriteObjects[i]->setRotation(0);
    }
    
    // --- Fetch size for this count number (countGenNo) ---
        std::string sizeKey = StringUtils::format("objects.objectSizes.%d", countGenNo);
        rapidjson::Value* sizeVal = cfgManager->getValueForPath(sizeKey);
        Size curObjSize(100, 100); // default

        if (sizeVal && sizeVal->IsObject())
        {
            float w = (*sizeVal)["width"].GetFloat();
            float h = (*sizeVal)["height"].GetFloat();
            curObjSize = Size(w, h);
        }
    
    
    int rndmToy=0;
    rndmToy=RandomHelper::random_int(1, totalObj);

    // --- Fetch positions for this count number ---
    std::string posKey = StringUtils::format("objects.positions.%d", countGenNo);
    const rapidjson::Value& posArr = cfgManager->getArray(posKey);

    if (posArr.IsArray())
    {
        for (rapidjson::SizeType i = 0; i < posArr.Size() && i < countGenNo; ++i)
        {
            const rapidjson::Value& posVal = posArr[i];
            if (posVal.HasMember("x") && posVal.HasMember("y"))
            {
                Vec2 pos(posVal["x"].GetFloat(), posVal["y"].GetFloat());

                spriteObjects[i + 1]->setTexture(
                    StringUtils::format("ToyShop_Item%d.png", rndmToy)
                );
                spriteObjects[i + 1]->setPosition(pos);
                spriteObjects[i + 1]->setVisible(false);

                setObjScale(curObjSize, spriteObjects[i + 1]);
            }
        }
    }
    else
    {
        CCLOG("Missing positions for countGenNo = %d in config!", countGenNo);
    }
}
#pragma mark - "Create Sprite Functions"
void Counting::createBackgroundSprites()
{
    addSprite(this, spriteBackground,"counting_Bg.png",Vec2(IPAD_WIDTH/2, IPAD_HEIGHT/2+100),Vec2(1.0, 1.0));
    
    // --- Apply clouds dynamically ---
    const rapidjson::Value* clouds = cfgManager->getValueForPath("background.clouds");
    if (clouds && clouds->IsArray()) {
        for (rapidjson::SizeType i = 0; i < clouds->Size(); i++) {
            const auto& cloud = (*clouds)[i];
            std::string cloudFile = cloud["file"].GetString();
            Vec2 start(cloud["start"]["x"].GetFloat(), cloud["start"]["y"].GetFloat());
            Vec2 end(cloud["end"]["x"].GetFloat(), cloud["end"]["y"].GetFloat());
            float duration = cloud["duration"].GetFloat();
            
            Sprite* spriteCloud;
            addSprite(this, spriteCloud,cloudFile,start,Vec2(1.0, 1.0));

            
            spriteCloud->runAction(RepeatForever::create(
            Sequence::create(
                             MoveTo::create(duration, end),
                             Place::create(start),
                             nullptr
                             )
            ));
        }
    }
    
    addSprite(this,spriteBoard,"boardbig.png",Vec2(693.82, -345.60 - 100),Vec2(1.2f, 1.2f));
}
void Counting::createOwlSprites()
{
    lyrOwl = Layer::create();
    this->addChild(lyrOwl);
    lyrOwl->setPositionX(-350);

    addSprite(lyrOwl, spriteCharStool,"Stool.png",Vec2(174.36, 76.80),Vec2(1.0, 1.0));

    addSprite(lyrOwl, spriteCharShadow,"Char_shadow.png",Vec2(191.19, 220.80),Vec2(0.4, 0.4));

    addSprite(lyrOwl, spriteCharacter,"charOwl.png",Vec2(186.38, 343.20),Vec2(1.0, 1.0));

}
Sprite* Counting::addSprite(Node* parent,
                              Sprite*& sprite,
                                     const std::string& imageName,
                                     const Vec2& position,
                                     const Vec2& scale)
{
    sprite = Sprite::create(imageName);
    if (!sprite) {
        CCLOG(" Failed to load image: %s", imageName.c_str());
        return nullptr;
    }

    sprite->setPosition(position);
    sprite->setScale(scale.x, scale.y);

    if (parent)
        parent->addChild(sprite);

    CCLOG(" Created sprite: %s (pos: %.2f, %.2f | scale: %.2f, %.2f)",
          imageName.c_str(), position.x, position.y, scale.x, scale.y);

    return sprite;
}
void Counting::createAnsOptionSprites()
{
    PosAnsBox[1]=(Vec2(482.19,232.80));
    PosAnsBox[2]=(Vec2(905.46,232.80));
    PosAnsBox[3]=(Vec2(693.82,231.60));
    
    for (int i=1;i<=totalAnsOptions;i++)
    {
        std::string imgName = StringUtils::format("box%d.png", i);
        addSprite(this, spriteAnsOptionBox[i], imgName, PosAnsBox[i], Vec2(0.0f, 0.0f)); // scale 0 same as before

        // Ensure it's added on the correct Z-order
        spriteAnsOptionBox[i]->setLocalZOrder(3);

        lbl_AnsOption[i]= Label::createWithTTF("10", "ChalkBoard.ttf", 120);
        lbl_AnsOption[i]->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbl_AnsOption[i]->setColor(Color3B::BLACK);//GRAY
        lbl_AnsOption[i]->setPosition(spriteAnsOptionBox[i]->getContentSize().width/2*POSX, spriteAnsOptionBox[i]->getContentSize().height/2*POSY-20);
        spriteAnsOptionBox[i]->addChild(lbl_AnsOption[i]);
    }
}
void Counting::createCountingObjects()
{
    for (int i = 1; i <= totalCntNo; i++)
    {
        // Generate a random image name for the object
        std::string imgName = StringUtils::format(
            "ToyShop_Item%d.png",
            RandomHelper::random_int(1, totalObj)
        );
        addSprite(this, spriteObjects[i], imgName, OUTOFSCREEN, Vec2(0.0f, 0.0f));
    }
}
void Counting::createFinalAnsSprites()
{
    lblEqual= Label::createWithTTF("=", "ChalkBoard.ttf", 150);
    lblEqual->setPosition(Vec2(826.09,456.00));
    lblEqual->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lblEqual->setColor(Color3B::BLACK);
    this->addChild(lblEqual);
    
    addSprite(this, spriteBox, StringUtils::format("box%d.png", 1),
              Vec2(957.16f, 476.40f),
              Vec2(POSX * 0.9f, POSY * 0.9f));

    spriteBox->setColor(Color3B::BLACK);
    spriteBox->setOpacity(150);
    
    for (int i = 1; i <= 2; i++)
    {
        std::string imgName = StringUtils::format("purpleSlide%d.png", i);

        addSprite(this, spriteSlideDoor[i], imgName,
                  Vec2::ZERO, // position will be set below
                  Vec2(POSX * 0.9f, POSY * 0.9f));

        spriteSlideDoor[i]->setVisible(false);
    }
    spriteSlideDoor[1]->setAnchorPoint(Vec2(0, 0.5));
    spriteSlideDoor[2]->setAnchorPoint(Vec2(1, 0.5));
    spriteSlideDoor[1]->setPosition(Vec2(883.81,476.40));
    spriteSlideDoor[2]->setPosition(Vec2(1031.71,476.40));

    PosFinalAns=Vec2(spriteBox->getContentSize().width/2*POSX, spriteBox->getContentSize().height/2*POSY-20);

    lblFinalAns= Label::createWithTTF("?", "ChalkBoard.ttf", 140);
    lblFinalAns->setPosition(PosFinalAns);
    lblFinalAns->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lblFinalAns->setColor(Color3B::YELLOW);
    spriteBox->addChild(lblFinalAns);
    
    lblEqual->setVisible(false);
    spriteBox->setVisible(false);

}
#pragma mark - "Effects Functions"
void Counting::entryEffect()
{
    spriteBoard->runAction(Sequence::create((MoveTo::create(0.5,Vec2(693.82,345.60))), NULL));
    
    lyrOwl->runAction(Sequence::create((DelayTime::create(2),EaseBackOut::create(MoveTo::create(1,Vec2(-50,0)))), NULL));
    
    scheduleOnce(SEL_SCHEDULE(&Counting::genRandomObj), 0.9);
    scheduleOnce(SEL_SCHEDULE(&Counting::EntrySound), 1.2);

}
void Counting::scaleEffectQuesMark()
{
    auto jump = JumpBy::create(0.8f, Vec2::ZERO, 50, 1);
    auto scaleUp = ScaleTo::create(0.1f, 1.2f);
    auto scaleDown = ScaleTo::create(0.1f, 1.0f);
    auto bounce = Sequence::create(scaleUp, scaleDown, nullptr);
    
    auto fullSeq = Sequence::create(ScaleTo::create(0.0,0.6),jump, bounce,  nullptr);
    lblFinalAns->runAction(fullSeq);
}
void Counting::idealEffectObjs()
{
    for (int i=1;i<=countGenNo;i++)
    {
        spriteObjects[i]->runAction(Sequence::create(RotateTo::create(0.8, 3),RotateTo::create(0.8, -3),NULL));
    }
}
void Counting::idealEffectAnsOptions()
{
    for (int i=1;i<=totalAnsOptions;i++)
    {
        spriteAnsOptionBox[i]->setScale(1);
        spriteAnsOptionBox[i]->setRotation(0);
        spriteAnsOptionBox[i]->stopAllActions();
    }
    spriteAnsOptionBox[RandomHelper::random_int(1, 3)]->runAction(Sequence::create(ScaleTo::create(0.2, 0.9),ScaleTo::create(0.2, 1),ScaleTo::create(0.2, 0.9),ScaleTo::create(0.2, 1), NULL));
}
void Counting::eyeBlinkAnim()
{
    if(!spriteCharacter)
    {
        CCLOG("Null Character");
        return;
    }
    
    //====Eye Blink Animation====//
  
        float blinkDelay = cfgManager->getFloat("character.eyeBlink.blinkDelay", 0.2f);
        float minInterval = cfgManager->getFloat("character.eyeBlink.minInterval", 3.0f);
        float maxInterval = cfgManager->getFloat("character.eyeBlink.maxInterval", 10.0f);
    
        blinkDelay *= cocos2d::RandomHelper::random_real(0.8f, 1.2f);
    
        // --- Run blink animation now ---
        Animation* anim = Animation::create();
        anim->addSpriteFrameWithFile("charOwl.png");     // eyes open
        anim->addSpriteFrameWithFile("charOwlEB.png");   // blink closed
        anim->addSpriteFrameWithFile("charOwl.png");     // open again
        anim->setDelayPerUnit(blinkDelay / 3);
        anim->setLoops(1);
        
        spriteCharacter->runAction(Animate::create(anim));
        
        // --- Schedule the next blink at a random interval ---
        float nextBlink = cocos2d::RandomHelper::random_real(minInterval, maxInterval);
        this->scheduleOnce([this](float dt) {
            this->eyeBlinkAnim();
        }, nextBlink, "blink_schedule");

}
void Counting::slideOpen()
{
    float openDuration = cfgManager->getFloat("slideDoor.openDuration", 0.5f);
    float minDelay = cfgManager->getFloat("slideDoor.minDelayBetween", 1.0f);
    float maxDelay = cfgManager->getFloat("slideDoor.maxDelayBetween", 2.5f);
    float randomDelay = cocos2d::RandomHelper::random_real(minDelay, maxDelay);
    
    isSlideOpen = true;
    
    for (int i = 1; i <= 2; i++)
    {
        spriteSlideDoor[i]->runAction(Sequence::create(
                                                     ScaleTo::create(openDuration, 0, 1),
                                                     nullptr
                                                     ));
    }
    
    lblEqual->runAction(Sequence::create(
                                              DelayTime::create(randomDelay),
                                              CallFunc::create([this] { slideClose(); }),
                                              nullptr
                                              ));
}

void Counting::slideClose()
{
    float closeDuration = cfgManager->getFloat("slideDoor.closeDuration", 0.5f);
    float minDelay = cfgManager->getFloat("slideDoor.minDelayBetween", 1.0f);
    float maxDelay = cfgManager->getFloat("slideDoor.maxDelayBetween", 2.5f);
    float randomDelay = cocos2d::RandomHelper::random_real(minDelay, maxDelay);
    
    isSlideOpen = false;
    
    for (int i = 1; i <= 2; i++)
    {
        spriteSlideDoor[i]->runAction(Sequence::create(
                                                     ScaleTo::create(closeDuration, 0.9f, 0.9f),
                                                     nullptr
                                                     ));
    }
    
    lblEqual->runAction(Sequence::create(
                                              DelayTime::create(randomDelay),
                                              CallFunc::create([this] { slideOpen(); }),
                                              nullptr
                                              ));
}
void Counting::resetTapCount()
{
    tapObj=0;

    for (int i = 1; i <= totalCntNo; i++)
    {
        spriteObjects[i]->setOpacity(255);
    }
}
void Counting::countingEffect()
{
    phaseCmp=true;
    unschedule(SEL_SCHEDULE(&Counting::idealEffectObjs));
    
    loopObjNo=0;
    float scaleRatio=spriteObjects[1]->getScaleX();
    for (int i = 1; i <= countGenNo; i++)
    {
        spriteObjects[i]->runAction(Sequence::create(DelayTime::create(0.3),DelayTime::create(i * 1.0f),ScaleTo::create(0.2, scaleRatio+0.2),CallFunc::create([this]{
            loopObjNo++;
            playSounds(__String::createWithFormat("%d.mp3",loopObjNo)->getCString());
        }),ScaleTo::create(0.2, scaleRatio),CallFunc::create([this]{
            
            if(loopObjNo==countGenNo)
            {
                showParticle(Vec2(691.42,753.60), true, 1.0 ,0.5,20);

                scheduleOnce(SEL_SCHEDULE(&Counting::genRandomObj), 2.2);
            }
            
        }),NULL));
    }
}
void Counting::showParticle(const Point &particlePos , bool isLevelEnd ,float delayDuration, int durationPlay, int zOrderNo)
{

    // Schedule a one-time callback after delay
    this->runAction(Sequence::create(
        DelayTime::create(delayDuration),
        CallFunc::create([=]() {
            // Choose particle file based on type
            std::string plistFile = isLevelEnd ? "fallingConfeti (9).plist" : "successMiniGame (2).plist";
            std::string strSoundAppreciation = __String::createWithFormat("appreciation_%d.mp3",RandomHelper::random_int(2, 6))->getCString();
            std::string soundFile = isLevelEnd ? "yay.mp3" : strSoundAppreciation;
            
            auto particleSystem = ParticleSystemQuad::create(plistFile);
            particleSystem->setPosition(particlePos);
            particleSystem->setDuration(durationPlay);
            particleSystem->setLocalZOrder(zOrderNo);
            this->addChild(particleSystem);

            // Play sound
            playSounds(soundFile);
        }),
        nullptr
    ));
}

void Counting::playAppreciationSound()
{
    std::string strSoundAppreciation = __String::createWithFormat("appreciation_%d.mp3",RandomHelper::random_int(2, 6))->getCString();
    playSounds(strSoundAppreciation);
}
void Counting::playSounds(std::string soundFile)
{
    SimpleAudioEngine::getInstance()->playEffect(soundFile.c_str());
}
void Counting::EntrySound()
{
    playSounds("CountTitleSnd.mp3");
}
void Counting::TouchOn()
{
    this->getEventDispatcher()->setEnabled(true);
}
void Counting::TouchOff()
{
    this->getEventDispatcher()->setEnabled(false);
}
