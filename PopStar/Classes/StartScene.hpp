//
//  StartScene.hpp
//  PopStar-mobile
//
//  Created by admin on 2018/2/10.
//

#ifndef StartScene_hpp
#define StartScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;
using namespace cocos2d::extension;

class StartScene: public cocos2d::Scene{
    
public:
    
    virtual bool init();
    static Scene* createScene();
    void touchUpInside(Ref* pSender,Control::EventType event);
    CREATE_FUNC(StartScene);
    ControlButton *startButton;
    ControlButton *continueButton;
};

#endif /* StartScene_hpp */
