//
//  PopSprite.hpp
//  MyProject-mobile
//
//  Created by admin on 2018/2/8.
//

#ifndef PopSprite_hpp
#define PopSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d::extension;
using namespace cocos2d;

class PopSprite : public Sprite {
public:
    // 初始化精灵 数值，宽度，高度，x,y
    static PopSprite *createPopSprite(int numbers,int width,int height,int posX,int posY);
    virtual bool init();
    CREATE_FUNC(PopSprite);
    // 获取数字
    int getNumber();
    // 设置数字
    void setNumber(int num);
    // 获取色块的坐标
    Vec2 getColorBGPoint();
    
    
    int getPopX();
    int getPopY();
    
    void setPopX(int x);
    void setPopY(int y);
private:
    // 获取pop位置
    int popX,popY; 
    int number;
    void entityInit(int numbers,int width,int height,int posX,int posY);
    Scale9Sprite * sprite;
    int spwidth, spheight;
};

#endif /* PopSprite_hpp */
