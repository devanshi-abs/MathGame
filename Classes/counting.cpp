//
//  counting.cpp
//  MyGame
//
//  Created by Mac on 30/10/25.
//
//

#include "counting.h"
#include "AppDelegate.h"

USING_NS_CC;
Scene*  counting::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = counting::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}
bool counting::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto pKeybackListener = EventListenerKeyboard::create();
    pKeybackListener->onKeyReleased = CC_CALLBACK_2(counting::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pKeybackListener, this);
#endif
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    cocos2d::Size winsize=Director::getInstance()->getWinSize();
    POSX=1;//winsize.width/IPAD_WIDTH;
    POSY=1;//winsize.height/IPAD_HEIGHT;
    POSXY=1;//(POSX+POSY)/2;
    
    tmp=1;
    
    // --- Load JSON Config ---
    cfg = ConfigManager::getInstance();
    cfg->loadConfig("counting_config.json");
    
#pragma mark - "Background sprites"

    //===== Background sprites =====//
    
    createBackgroundSprite();
    
#pragma mark - "Character sprites"
    
    //===== Character sprites =====//
    createOwlSprite();
    
#pragma mark - "Answer Options"
    {
        //===== Bottom Option sprites =====//
        
        createAnsOptionSprite();
        
        //===== Final Answer sprites =====//
        
        lblEqual= Label::createWithTTF("=", "ChalkBoard.ttf", 150);
        lblEqual->setPosition(Vec2(826.09,456.00));
        lblEqual->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lblEqual->setColor(Color3B::BLACK);
        this->addChild(lblEqual);
        
        Img_Box=Sprite::create(__String::createWithFormat("box%d.png",1)->getCString());
        Img_Box->setScale(POSX*0.9,POSY*0.9);
        this->addChild(Img_Box);
        Img_Box->setPosition(Vec2(957.16,476.40));
        Img_Box->setColor(Color3B::BLACK);
        Img_Box->setOpacity(150);
        
        for(int i=1;i<=2;i++)
        {
            Img_SlideDoor[i]=Sprite::create(__String::createWithFormat("purpleSlide%d.png",i)->getCString());
            Img_SlideDoor[i]->setScale(POSX*0.9,POSY*0.9);
            this->addChild(Img_SlideDoor[i]);
            Img_SlideDoor[i]->setVisible(false);
        }
        Img_SlideDoor[1]->setAnchorPoint(Vec2(0, 0.5));
        Img_SlideDoor[2]->setAnchorPoint(Vec2(1, 0.5));
        Img_SlideDoor[1]->setPosition(Vec2(883.81,476.40));
        Img_SlideDoor[2]->setPosition(Vec2(1031.71,476.40));

        PosFinalAns=Vec2(Img_Box->getContentSize().width/2*POSX, Img_Box->getContentSize().height/2*POSY-20);

        lblFinalAns= Label::createWithTTF("?", "ChalkBoard.ttf", 140);
        lblFinalAns->setPosition(PosFinalAns);
        lblFinalAns->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lblFinalAns->setColor(Color3B::YELLOW);
        Img_Box->addChild(lblFinalAns);
    }
   
#pragma mark - "Count Objects"
        
    createCountingObjects();
    
    lblEqual->setVisible(false);
    Img_Box->setVisible(false);

    //====Ideal Effects===//
    
    eyeBlinkAnim();
    scheduleOnce(SEL_SCHEDULE(&counting::EntryEffect), 0.5);
    schedule(SEL_SCHEDULE(&counting::scaleEffectQuesMark), 5, -1, 3);
    schedule(SEL_SCHEDULE(&counting::idealEffectObj), 4, -1, 3);
    
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(counting::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(counting::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(counting::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(counting::onTouchesEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    //scheduleUpdate();
    
    TouchOn();
    return true;
}
#pragma mark- "UPDATE Method"
void counting::update(float delta)
{
}
void counting::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
    Touch *touch=(Touch*)(touches[0]);
    cocos2d::Point Scenelocation=touch->getLocation();
    cocos2d::Point location=this->convertToNodeSpace(Scenelocation);
    
    if(Background->getNumberOfRunningActions()!=0)
    {
        return;
    }
    
    for (int i=1;i<=3;i++)
    {
        if(Img_AnsOptionBox[i]->getBoundingBox().containsPoint(location))
        {
            SimpleAudioEngine::getInstance()->playEffect("tap.mp3");
            unschedule(SEL_SCHEDULE(&counting::scaleEffectQuesMark));
            unschedule(SEL_SCHEDULE(&counting::idealEffectAnsOption));

            lblFinalAns->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.2, 0.8),EaseBackOut::create(ScaleTo::create(0.4, 1)), NULL)));
            Img_AnsOptionBox[i]->setLocalZOrder(5);
            PosTouch=Img_AnsOptionBox[i]->getPosition();
            isMoved=true;
            whichOptionDrag=i;
            
            break;
        }
    }
    
    for (int i = 1; i <= countGenNo; i++)
    {
        if(Img_Objects[i]->getBoundingBox().containsPoint(location) && phaseCmp==false && Img_Objects[i]->getOpacity()==255)
        {
            tapObj++;
            float scaleRatio=Img_Objects[1]->getScaleX();
            
            Img_Objects[i]->setOpacity(180);
            SimpleAudioEngine::getInstance()->playEffect(__String::createWithFormat("%d.mp3",tapObj)->getCString());

            Img_Objects[i]->runAction(Sequence::create(ScaleTo::create(0.2, scaleRatio+0.2),ScaleTo::create(0.2, scaleRatio),CallFunc::create([this]{
            }),NULL));
            
            scheduleOnce(SEL_SCHEDULE(&counting::resetTapCount), 5);
        }
    }
}
void counting::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
    Touch *touch=(Touch*)(touches[0]);
    cocos2d::Point Scenelocation=touch->getLocation();
    cocos2d::Point location=this->convertToNodeSpace(Scenelocation);
    
//    Img_Objects[tmp]->setPosition(location);
    //prtcl->setPosition(location);
    
   // Img_SlideDoor[tmp]->setPosition(location);
    
    if(whichOptionDrag!=0 && isMoved==true)
    {
        Img_AnsOptionBox[whichOptionDrag]->setPosition(Vec2(location.x, location.y+50));
    }
}
void counting::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *event)
{
    Touch *touch=(Touch*)(touches[0]);
    cocos2d::Point Scenelocation=touch->getLocation();
    cocos2d::Point location=this->convertToNodeSpace(Scenelocation);
    
    if(whichOptionDrag!=0 && isMoved==true)
    {
        schedule(SEL_SCHEDULE(&counting::scaleEffectQuesMark),5, -1, 0);
        lblFinalAns->stopAllActions();
        lblFinalAns->setPosition(PosFinalAns);
        lblFinalAns->setScale(1,1);
        
        
        if(Img_AnsOptionBox[whichOptionDrag]->getBoundingBox().intersectsRect(Img_Box->getBoundingBox()) && Img_AnsOptionBox[whichOptionDrag]->getTag()==countGenNo && isSlideOpen==true)
        {
            unschedule(SEL_SCHEDULE(&counting::slideOpen));
            Img_CharStool->stopAllActions();
            for (int i=1;i<=2;i++)
            {
                Img_SlideDoor[i]->stopAllActions();
                Img_SlideDoor[i]->setVisible(false);
            }
            
            isMoved=false;
            SimpleAudioEngine::getInstance()->playEffect("particleClick.mp3");

            scheduleOnce(SEL_SCHEDULE(&counting::showSuccessParticle), 0.5);
            
            Background->runAction(DelayTime::create(3));
            
            Img_AnsOptionBox[whichOptionDrag]->runAction(Sequence::create(JumpTo::create(0.5, Img_Box->getPosition(), 100, 1),ScaleTo::create(0.1,1.05),ScaleTo::create(0.1,1.0),DelayTime::create(1.0),CallFunc::create([this]{countingEffect();}),NULL));
        }
        else
        {
            schedule(SEL_SCHEDULE(&counting::idealEffectAnsOption), 4.5, -1, 3);

            isMoved=false;
            Background->runAction(DelayTime::create(0.7));
            Img_AnsOptionBox[whichOptionDrag]->setLocalZOrder(3);

            if(Img_AnsOptionBox[whichOptionDrag]->getTag()==countGenNo)
            {
                SimpleAudioEngine::getInstance()->playEffect("whoosh.mp3");

                Img_AnsOptionBox[whichOptionDrag]->runAction(Sequence::create(EaseBackIn::create(MoveTo::create(0.2, PosTouch)),CallFunc::create([this]{whichOptionDrag=0;}),NULL));
            }
            else
            {
                SimpleAudioEngine::getInstance()->playEffect("tapWrong.mp3");

                Img_AnsOptionBox[whichOptionDrag]->runAction(Sequence::create(EaseBackIn::create(MoveTo::create(0.2, PosTouch)),RotateTo::create(0.1,8),RotateTo::create(0.1,-8),RotateTo::create(0.1,8),RotateTo::create(0.1,0),CallFunc::create([this]{whichOptionDrag=0;}),NULL));
            }
        }
    }
    
    //CCLOG("Vec2(%0.2f,%0.2f)",location.x,location.y);
    
    CCLOG("Img_SlideDoor[%d]->setPosition(Vec2(%0.2f,%0.2f));",tmp,location.x,location.y);

//    CCLOG("PosObj[%d][%d]=(Vec2(%0.2f,%0.2f));",1,tmp,location.x,location.y);
    tmp++;
}
#pragma  mark- "Your Methods"
void counting::genRandomObj()
{
    resetTapCount();
    phaseCmp=false;
    lblEqual->setVisible(false);
    Img_Box->setVisible(false);
    
    countGenNo = RandomHelper::random_int(1,10);
    
    for (int i=1;i<=totalCntNo;i++)
    {
        Img_Objects[i]->setPosition(OUTOFSCREEN);
        Img_Objects[i]->setScale(0);
        Img_Objects[i]->setRotation(0);
    }
    
    int rndmToy=0;
    rndmToy=RandomHelper::random_int(1, totalObj);

    for (int i=1;i<=countGenNo;i++)
    {
        Img_Objects[i]->setTexture(__String::createWithFormat("ToyShop_Item%d.png",rndmToy)->getCString());
        Img_Objects[i]->setPosition(PosObj[countGenNo][i]);
        Img_Objects[i]->setVisible(false);
        setObjScale(objSize[countGenNo],Img_Objects[i]);
    }
    loopObjNo=0;
    for (int i = 1; i <= countGenNo; i++)
    {
        
        Img_Objects[i]->runAction(Sequence::create(DelayTime::create(0.3),DelayTime::create(i * 0.1f),Show::create(),CallFunc::create([this]{
            
            loopObjNo++;
            if(loopObjNo==countGenNo)
            {
                showOptionBox();
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
    
    for (int i = 1; i <=3; i++)
    {
        Img_AnsOptionBox[i]->setTag(0);
        Img_AnsOptionBox[i]->setPosition(shuffledPositions[i - 1]);//PosAnsBox[i]
        Img_AnsOptionBox[i]->setScale(0);
        Img_AnsOptionBox[i]->setLocalZOrder(3);
        
        int index = randomNumbers[i-1];
        lbl_AnsOption[i]->setString(__String::createWithFormat("%d",index)->getCString());
        Img_AnsOptionBox[i]->setTag(index);
    }
    

}
void counting::setObjScale(cocos2d::Size ScaleToSize ,Node *sprtObj)
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
void counting::showOptionBox()
{
    for (int i = 1; i <=3; i++)
    {
        Img_AnsOptionBox[i]->runAction(Sequence::create(DelayTime::create(0.3),DelayTime::create(i * 0.2f),EaseBackOut::create(ScaleTo::create(0.5, 0.9)),NULL));
    }
    
    Background->runAction(Sequence::create(DelayTime::create(1),CallFunc::create([this]{
        lblEqual->setVisible(true);
        Img_Box->setVisible(true);
        lblEqual->setOpacity(0);
        Img_Box->setOpacity(0);

        lblEqual->runAction(FadeIn::create(0.5));
        Img_Box->runAction(FadeTo::create(0.5,150));
        
        for (int i=1;i<=2;i++)
        {
            Img_SlideDoor[i]->setVisible(true);
        }
        scheduleOnce(SEL_SCHEDULE(&counting::slideOpen), 0.5);


    }), NULL));
    
    schedule(SEL_SCHEDULE(&counting::idealEffectAnsOption), 5.5, -1, 3);

}
#pragma mark - "Create Sprite Functions"
void counting::createBackgroundSprite()
{
    Background=Sprite::create("counting_Bg.png");
    this->addChild(Background);
    Background->setPosition(Vec2(IPAD_WIDTH/2, IPAD_HEIGHT/2+100));
    
    // --- Apply clouds dynamically ---
    const rapidjson::Value* clouds = cfg->getValueForPath("background.clouds");
    if (clouds && clouds->IsArray()) {
        for (rapidjson::SizeType i = 0; i < clouds->Size(); i++) {
            const auto& cloud = (*clouds)[i];
            std::string cloudFile = cloud["file"].GetString();
            Vec2 start(cloud["start"]["x"].GetFloat(), cloud["start"]["y"].GetFloat());
            Vec2 end(cloud["end"]["x"].GetFloat(), cloud["end"]["y"].GetFloat());
            float duration = cloud["duration"].GetFloat();
            
            auto sp = Sprite::create(cloudFile);
            sp->setPosition(start);
            this->addChild(sp);
            
            sp->runAction(RepeatForever::create(
            Sequence::create(
                             MoveTo::create(duration, end),
                             Place::create(start),
                             nullptr
                             )
            ));
        }
    }
    
    Img_Board=Sprite::create("boardbig.png");
    this->addChild(Img_Board);
    Img_Board->setScale(1.2,1.2);
    Img_Board->setPosition(Vec2(693.82,-345.60-100));
}
void counting::createOwlSprite()
{
        lyrOwl=Layer::create();
        this->addChild(lyrOwl);
        lyrOwl->setPositionX(-350);
        
        Img_CharStool=Sprite::create("Stool.png");
        lyrOwl->addChild(Img_CharStool);
        Img_CharStool->setScale(0.9,0.9);
        Img_CharStool->setPosition(Vec2(174.36,76.80));
        
        Img_CharShadow=Sprite::create("Char_shadow.png");
        lyrOwl->addChild(Img_CharShadow);
        Img_CharShadow->setScale(0.4);
        Img_CharShadow->setPosition(Vec2(191.19,220.80));

        Img_Character=Sprite::create("charOwl.png");
        lyrOwl->addChild(Img_Character);
        Img_Character->setScale(1.0,1.0);
        Img_Character->setPosition(Vec2(186.38,343.20));
    
}
void counting::createAnsOptionSprite()
{
    PosAnsBox[1]=(Vec2(482.19,232.80));
    PosAnsBox[2]=(Vec2(905.46,232.80));
    PosAnsBox[3]=(Vec2(693.82,231.60));
    
    for (int i=1;i<=3;i++)
    {
        Img_AnsOptionBox[i]=Sprite::create(__String::createWithFormat("box%d.png",i)->getCString());
        Img_AnsOptionBox[i]->setScale(0);//0.9
        this->addChild(Img_AnsOptionBox[i],3);
        Img_AnsOptionBox[i]->setPosition(PosAnsBox[i]);
        
        lbl_AnsOption[i]= Label::createWithTTF("10", "ChalkBoard.ttf", 120);
        lbl_AnsOption[i]->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbl_AnsOption[i]->setColor(Color3B::BLACK);//GRAY
        lbl_AnsOption[i]->setPosition(Img_AnsOptionBox[i]->getContentSize().width/2*POSX, Img_AnsOptionBox[i]->getContentSize().height/2*POSY-20);
        //        lblThinkingPopup->setLineHeight(20);
        Img_AnsOptionBox[i]->addChild(lbl_AnsOption[i]);
    }
}
void counting::createCountingObjects()
{
    PosObj[1][1]=(Vec2(594.02,486.00));
    
    PosObj[2][1]=(Vec2(477.38,562.80));
    PosObj[2][2]=(Vec2(663.76,452.40));
    
    PosObj[3][1]=(Vec2(468.96,560.40));
    PosObj[3][2]=(Vec2(681.80,564.00));
    PosObj[3][3]=(Vec2(569.97,418.80));
    
    PosObj[4][1]=(Vec2(461.75,573.60));
    PosObj[4][2]=(Vec2(465.35,426.00));
    PosObj[4][3]=(Vec2(676.99,572.40));
    PosObj[4][4]=(Vec2(683.00,429.60));
    
    PosObj[5][1]=(Vec2(429.28,574.80));
    PosObj[5][2]=(Vec2(436.49,447.60));
    PosObj[5][3]=(Vec2(580.79,513.60));
    PosObj[5][4]=(Vec2(725.09,580.80));
    PosObj[5][5]=(Vec2(720.28,446.40));
    
    PosObj[6][1]=(Vec2(429.28,574.80));
    PosObj[6][2]=(Vec2(436.49,447.60));
    PosObj[6][3]=(Vec2(580.79,513.60));
    PosObj[6][4]=(Vec2(725.09,580.80));
    PosObj[6][5]=(Vec2(720.28,446.40));
    PosObj[6][6]=(Vec2(579.59,368.40));
    
    PosObj[7][1]=(Vec2(431.68,578.40));
    PosObj[7][2]=(Vec2(430.48,478.80));
    PosObj[7][3]=(Vec2(568.77,574.80));
    PosObj[7][4]=(Vec2(562.75,478.80));
    PosObj[7][5]=(Vec2(709.45,573.60));
    PosObj[7][6]=(Vec2(704.64,478.80));
    PosObj[7][7]=(Vec2(562.75,380.40));
    
    PosObj[8][1]=(Vec2(432.89,578.40));
    PosObj[8][2]=(Vec2(434.09,480.00));
    PosObj[8][3]=(Vec2(432.89,385.20));
    PosObj[8][4]=(Vec2(568.77,577.20));
    PosObj[8][5]=(Vec2(565.16,481.20));
    PosObj[8][6]=(Vec2(561.55,384.00));
    PosObj[8][7]=(Vec2(708.25,574.80));
    PosObj[8][8]=(Vec2(705.85,480.00));
    
    PosObj[9][1]=(Vec2(432.89,578.40));
    PosObj[9][2]=(Vec2(434.09,480.00));
    PosObj[9][3]=(Vec2(432.89,385.20));
    PosObj[9][4]=(Vec2(568.77,577.20));
    PosObj[9][5]=(Vec2(565.16,481.20));
    PosObj[9][6]=(Vec2(561.55,384.00));
    PosObj[9][7]=(Vec2(708.25,574.80));
    PosObj[9][8]=(Vec2(705.85,480.00));
    PosObj[9][9]=(Vec2(702.24,386.40));
    
    PosObj[10][1]=(Vec2(432.45,608.40));
    PosObj[10][2]=(Vec2(433.65,526.80));
    PosObj[10][3]=(Vec2(432.45,445.20));
    PosObj[10][4]=(Vec2(550.29,609.60));
    PosObj[10][5]=(Vec2(551.49,529.20));
    PosObj[10][6]=(Vec2(555.10,448.80));
    PosObj[10][7]=(Vec2(672.94,609.60));
    PosObj[10][8]=(Vec2(674.14,531.60));
    PosObj[10][9]=(Vec2(671.74,450.00));
    PosObj[10][10]=(Vec2(430.04,364.80));
    
    objSize[1]=Size(165,165);
    objSize[2]=Size(135,135);
    objSize[3]=Size(125,125);
    objSize[4]=Size(115,115);
    objSize[5]=Size(115,115);
    objSize[6]=Size(115,115);
    objSize[7]=Size(105,105);
    objSize[8]=Size(95,95);
    objSize[9]=Size(95,95);
    objSize[10]=Size(75,75);
    
    for (int i=1;i<=totalCntNo;i++)
    {
        Img_Objects[i]=Sprite::create(__String::createWithFormat("ToyShop_Item%d.png",RandomHelper::random_int(1, totalObj))->getCString());
        this->addChild(Img_Objects[i]);
        Img_Objects[i]->setPosition(PosObj[2][i]);
        Img_Objects[i]->setScale(0);
    }
}
#pragma mark - "Effects Functions"
void counting::EntryEffect()
{
    Img_Board->runAction(Sequence::create((MoveTo::create(0.5,Vec2(693.82,345.60))), NULL));
    
    lyrOwl->runAction(Sequence::create((DelayTime::create(2),EaseBackOut::create(MoveTo::create(1,Vec2(-50,0)))), NULL));
    
    scheduleOnce(SEL_SCHEDULE(&counting::genRandomObj), 0.9);
    scheduleOnce(SEL_SCHEDULE(&counting::EntrySound), 1.2);

}
void counting::scaleEffectQuesMark()
{
    auto jump = JumpBy::create(0.8f, Vec2::ZERO, 50, 1);
    auto scaleUp = ScaleTo::create(0.1f, 1.2f);
    auto scaleDown = ScaleTo::create(0.1f, 1.0f);
    auto bounce = Sequence::create(scaleUp, scaleDown, nullptr);
    
    auto fullSeq = Sequence::create(ScaleTo::create(0.0,0.6),jump, bounce,  nullptr);
    lblFinalAns->runAction(fullSeq);
}
void counting::idealEffectObj()
{
    for (int i=1;i<=countGenNo;i++)
    {
        Img_Objects[i]->runAction(Sequence::create(RotateTo::create(0.8, 3),RotateTo::create(0.8, -3),NULL));
    }
}
void counting::idealEffectAnsOption()
{
    for (int i=1;i<=3;i++)
    {
        Img_AnsOptionBox[i]->setScale(1);
        Img_AnsOptionBox[i]->setRotation(0);
        Img_AnsOptionBox[i]->stopAllActions();
    }
    Img_AnsOptionBox[RandomHelper::random_int(1, 3)]->runAction(Sequence::create(ScaleTo::create(0.2, 0.9),ScaleTo::create(0.2, 1),ScaleTo::create(0.2, 0.9),ScaleTo::create(0.2, 1), NULL));
}
void counting::eyeBlinkAnim()
{
    //====Eye Blink Animation====//
  
        float blinkDelay = cfg->getFloat("character.eyeBlink.blinkDelay", 0.2f);
        float minInterval = cfg->getFloat("character.eyeBlink.minInterval", 3.0f);
        float maxInterval = cfg->getFloat("character.eyeBlink.maxInterval", 10.0f);
    
        blinkDelay *= cocos2d::RandomHelper::random_real(0.8f, 1.2f);
    
        // --- Run blink animation now ---
        Animation* anim = Animation::create();
        anim->addSpriteFrameWithFile("charOwl.png");     // eyes open
        anim->addSpriteFrameWithFile("charOwlEB.png");   // blink closed
        anim->addSpriteFrameWithFile("charOwl.png");     // open again
        anim->setDelayPerUnit(blinkDelay / 3);
        anim->setLoops(1);
        
        Img_Character->runAction(Animate::create(anim));
        
        // --- Schedule the next blink at a random interval ---
        float nextBlink = cocos2d::RandomHelper::random_real(minInterval, maxInterval);
        this->scheduleOnce([this](float dt) {
            this->eyeBlinkAnim();
        }, nextBlink, "blink_schedule");

}
void counting::slideOpen()
{
    float openDuration = cfg->getFloat("slideDoor.openDuration", 0.5f);
    float minDelay = cfg->getFloat("slideDoor.minDelayBetween", 1.0f);
    float maxDelay = cfg->getFloat("slideDoor.maxDelayBetween", 2.5f);
    float randomDelay = cocos2d::RandomHelper::random_real(minDelay, maxDelay);
    
    isSlideOpen = true;
    
    for (int i = 1; i <= 2; i++)
    {
        Img_SlideDoor[i]->runAction(Sequence::create(
                                                     ScaleTo::create(openDuration, 0, 1),
                                                     nullptr
                                                     ));
    }
    
    Img_CharStool->runAction(Sequence::create(
                                              DelayTime::create(randomDelay),
                                              CallFunc::create([this] { slideClose(); }),
                                              nullptr
                                              ));
}

void counting::slideClose()
{
    float closeDuration = cfg->getFloat("slideDoor.closeDuration", 0.5f);
    float minDelay = cfg->getFloat("slideDoor.minDelayBetween", 1.0f);
    float maxDelay = cfg->getFloat("slideDoor.maxDelayBetween", 2.5f);
    float randomDelay = cocos2d::RandomHelper::random_real(minDelay, maxDelay);
    
    isSlideOpen = false;
    
    for (int i = 1; i <= 2; i++)
    {
        Img_SlideDoor[i]->runAction(Sequence::create(
                                                     ScaleTo::create(closeDuration, 0.9f, 0.9f),
                                                     nullptr
                                                     ));
    }
    
    Img_CharStool->runAction(Sequence::create(
                                              DelayTime::create(randomDelay),
                                              CallFunc::create([this] { slideOpen(); }),
                                              nullptr
                                              ));
}
void counting::resetTapCount()
{
    tapObj=0;

    for (int i = 1; i <= totalCntNo; i++)
    {
        Img_Objects[i]->setOpacity(255);
    }
}
void counting::countingEffect()
{
    phaseCmp=true;
    unschedule(SEL_SCHEDULE(&counting::idealEffectObj));
    
    loopObjNo=0;
    float scaleRatio=Img_Objects[1]->getScaleX();
    for (int i = 1; i <= countGenNo; i++)
    {
        Img_Objects[i]->runAction(Sequence::create(DelayTime::create(0.3),DelayTime::create(i * 1.0f),ScaleTo::create(0.2, scaleRatio+0.2),CallFunc::create([this]{
            loopObjNo++;
            SimpleAudioEngine::getInstance()->playEffect(__String::createWithFormat("%d.mp3",loopObjNo)->getCString());

        }),ScaleTo::create(0.2, scaleRatio),CallFunc::create([this]{
            
            if(loopObjNo==countGenNo)
            {
                scheduleOnce(SEL_SCHEDULE(&counting::levelCmpEffect), 1.0);
                scheduleOnce(SEL_SCHEDULE(&counting::genRandomObj), 2.2);//1.9
            }
            
        }),NULL));
    }
}
void counting::showSuccessParticle()
{
    ParticleSystemQuad *prtcl = ParticleSystemQuad::create("successMiniGame (2).plist");
    prtcl->setPosition(Img_Box->getPosition());
    prtcl->setLocalZOrder(20);
    prtcl->setDuration(0.5);
    this->addChild(prtcl);
    
    playAppreciationSound();
}
void counting::playAppreciationSound()
{
    SimpleAudioEngine::getInstance()->playEffect(__String::createWithFormat("appreciation_%d.mp3",RandomHelper::random_int(2, 6))->getCString());
}

void counting::levelCmpEffect()
{
    ParticleSystemQuad *prtcl = ParticleSystemQuad::create("fallingConfeti (9).plist");
    prtcl->setPosition(Vec2(691.42,753.60));
    prtcl->setLocalZOrder(20);
    prtcl->setDuration(0.5);
    this->addChild(prtcl);

    std::string tapSound = cfg->getString("sounds.yay", "tap.mp3");

    
    SimpleAudioEngine::getInstance()->playEffect("yay.mp3");
}
void counting::EntrySound()
{
    SimpleAudioEngine::getInstance()->playEffect("CountTitleSnd.mp3");
}
void counting::TouchOn()
{
    this->getEventDispatcher()->setEnabled(true);
}
void counting::TouchOff()
{
    this->getEventDispatcher()->setEnabled(false);
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void counting::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
      //  SimpleAudioEngine::getInstance()->stopAllEffects();
      //  this->unscheduleUpdate();
      //  unscheduleAllCallbacks();
      //  Director::getInstance()->replaceScene(TransitionFade::create(0.5, LevelScreen::createScene()));
    }
}
#endif
