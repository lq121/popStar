//
//  StartScene.cpp
//  PopStar-mobile
//
//  Created by admin on 2018/2/10.
//

#include "StartScene.hpp"
#include "HelloWorldScene.h"




Scene* StartScene::createScene()
{
    return StartScene::create();
}


bool StartScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    
    Size size = Director::getInstance()->getWinSize();
    //创建游戏背景
    Sprite *BGSprite = Sprite::create("images/xingzuoBg.png",Rect(0,0,size.width,size.height));
    BGSprite->setAnchorPoint(Vec2(0,0));
    this->addChild(BGSprite);
    
    // 最高分
    int highScore = UserDefault::sharedUserDefault()->getIntegerForKey("highSocre");
    auto scoreLabel = LabelBMFont::create(String::createWithFormat("最高分:%ld",highScore)->getCString(), "fonts/label30.fnt");
    scoreLabel->setScale(3);
    scoreLabel->setPosition(Vec2(size.width/2,size.height-20));
    this->addChild(scoreLabel);
    
    // logo
    SpriteFrameCache* frameCache = SpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("images/popPlist1.plist","images/popPlist1.pvr.ccz");
    frameCache->addSpriteFramesWithFile("images/imagesPop1.plist","images/imagesPop1.pvr.ccz");
    auto logo = Scale9Sprite::createWithSpriteFrameName("task_img_english.png",Rect(0,0,386,274));
    logo->setScale(2.5);
    logo->setPosition(Vec2(size.width/2,size.height-150));
    this->addChild(logo);
    // 开始按钮
    auto startButtonBg = Scale9Sprite::createWithSpriteFrameName("begin_game.png");
    startButtonBg->setScale(2);
    startButton = ControlButton::create(startButtonBg);
    startButton->addTargetWithActionForControlEvents(this,cccontrol_selector(StartScene::touchUpInside),Control::EventType::TOUCH_UP_INSIDE);
    startButton->setPosition(Vec2(size.width/2,200));
    this->addChild(startButton);
    // 继续按钮
    auto continueBg = Scale9Sprite::createWithSpriteFrameName("continue_game.png");
    continueBg->setScale(2);
    continueButton = ControlButton::create(continueBg);
    continueButton->addTargetWithActionForControlEvents(this,cccontrol_selector(StartScene::touchUpInside),Control::EventType::TOUCH_UP_INSIDE);
    continueButton->setPosition(Vec2(size.width/2,140));
    
    this->addChild(continueButton);
    return true;
    
}

void StartScene::touchUpInside(Ref* pSender,Control::EventType event)
{
    if(pSender == startButton)
    {
        UserDefault::sharedUserDefault()->setStringForKey("startFalg","start");
         Director::getInstance()->pushScene(HelloWorld::createScene());
    }
    else if(pSender == continueButton)
    {
        UserDefault::sharedUserDefault()->setStringForKey("startFalg","continue");
        Director::getInstance()->pushScene(HelloWorld::createScene());
    }
}




