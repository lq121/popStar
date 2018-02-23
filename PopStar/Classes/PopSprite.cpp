//
//  PopSprite.cpp
//  MyProject-mobile
//
//  Created by admin on 2018/2/8.
//

#include "PopSprite.hpp"

USING_NS_CC;

PopSprite * PopSprite::createPopSprite(int numbers,int width,int height,int posX,int posY)
{
    PopSprite * sprite = new PopSprite();
    if(sprite && sprite->init())
    {
        sprite->autorelease();
        sprite->entityInit(numbers,width,height,posX,posY);
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

bool PopSprite::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    return true;
}

void PopSprite::entityInit(int numbers, int width, int height, int posX, int posY)
{
    this->setPosition(Vec2(posX, posY));
    number = numbers;
    popX = 0;
    popY = 0;
    spwidth = width;
    spheight = height;
    
    SpriteFrameCache* frameCache = SpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("images/popModePlist.plist","images/popModePlist.pvr.ccz");
    //通过一个镇图片生产精灵
    sprite = Scale9Sprite::createWithSpriteFrameName("pop_mode_1_1.png");
    sprite->setAnchorPoint(Vec2(0,0));
    sprite->setCapInsets(CCRectMake(3, 3, (width-4)/2, (height-4)/2));
    sprite->setContentSize(CCSizeMake(width-1,height-1));
    sprite->setPosition(Vec2(popX, popY));
    this->addChild(sprite);
}

Vec2 PopSprite::getColorBGPoint()
{
    return sprite->getPosition();
}

int PopSprite::getNumber()
{
    return number;
}

int PopSprite::getPopX()
{
    return popX;
}

int PopSprite::getPopY()
{
    return popY;
}

void PopSprite::setNumber(int num)
{
    number = num;
    sprite->setVisible(true);
    switch (num) {
        case -1:
            sprite->setVisible(false);
            sprite->setStrechEnabled(false);
            break;
        case 0:
             sprite->setSpriteFrame("pop_mode_1_1.png");
            break;
        case 1:
             sprite->setSpriteFrame("pop_mode_1_2.png");
            break;
        case 2:
             sprite->setSpriteFrame("pop_mode_1_3.png");
            break;
        case 3:
             sprite->setSpriteFrame("pop_mode_1_4.png");
            break;
        case 4:
             sprite->setSpriteFrame("pop_mode_1_5.png");
            break;
        default:
            break;
    }
    sprite->setCapInsets(CCRectMake(3, 3, (spwidth-4)/2, (spheight-4)/2));
    sprite->setContentSize(CCSizeMake(spwidth-1,spheight-1));
}

void PopSprite::setPopX(int x)
{
    popX = x;
    
}
void PopSprite::setPopY(int y)
{
    popY = y;
}


