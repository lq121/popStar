#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PopSprite.hpp"
#include "cocos-ext.h"

using namespace cocos2d::extension;


using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);
    
    // 自动创建10*10矩形方阵
    void autoCreatePopSprite(Size size);
    
    // 监听手势识别
    virtual bool onTouchBegan(Touch* touch,Event* event);
    String value;
private:
    // pop数组
    PopSprite* popStarSprite[10][10];
    // 创建数组存放点击的pop
    cocos2d::__Array*popArr;
    cocos2d::LabelBMFont *scoreLabel;
    cocos2d::LabelBMFont *lastScoreLabel;
    int score;
    LabelBMFont* tollgateLabel;
    LabelBMFont* targetLabel;
    Scale9Sprite* winSprite;
    int sceneNum;
    
private:
    // 点击得到pop对象
    PopSprite* getPopStarSprite(Size size,Vec2 touch);
    // 通过传递点击的pop对象返回上下左右的pop
    cocos2d::__Array*checkPopUDLR(PopSprite* pop);
    // 通过检测到的上下左右数据循环检测每一个的上下左右
    void checkPopInNew(cocos2d::__Array* arrOld);
    // 让popstar消失
    void dismiss(int num);
    // 播放音效
    void PlayEffectWithCount(int count);
    // 让空白部分被上面的填充
    void reDiessPopStar();
    // 上每一列执行被上面填充
    void reDiessPopStarRun(int x);
    // 当空白的一列时需要将右边的往左移动
    void reDiessPopSatrTurnLeft();
    // 判断是否可以继续进行游戏
    void gameOver();
    void touchUpInside(Ref* pSender,Control::EventType event);
    void setStringWithScreeningNum(int num);
    int  getStringWithScreeningNum(int num);
    void setScoreLabel();
};

#endif // __HELLOWORLD_SCENE_H__

