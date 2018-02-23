#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "StartScene.hpp"
#include "PopSprite.hpp"
#include "GCCsvHelper.hpp"
#include <cstring>
using namespace std;

USING_NS_CC;
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    score = 0;
    sceneNum = 1;
    Size size = Director::getInstance()->getWinSize();
    //创建游戏背景
    Sprite *BGSprite = Sprite::create("images/xingzuoBg.png",Rect(0,0,size.width,size.height));
    BGSprite->setAnchorPoint(Vec2(0,0));
    this->addChild(BGSprite);
    
    // 顶部的Sprite
    SpriteFrameCache* frameCache = SpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("images/popPlist1.plist","images/popPlist1.pvr.ccz");
    
    tollgateLabel = LabelBMFont::create("关卡:1", "fonts/label30.fnt");
    tollgateLabel->setAnchorPoint(Vec2(0,0));
    tollgateLabel->setScale(3);
    tollgateLabel->setPosition(Vec2(30,size.height-20));
    this->addChild(tollgateLabel);
    
    targetLabel = LabelBMFont::create("目标:2000", "fonts/label30.fnt");
    targetLabel->setScale(3);
    targetLabel->setPosition(Vec2(size.width/2+20,size.height-10));
    this->addChild(targetLabel);
    
    
    scoreLabel = LabelBMFont::create(String::createWithFormat("%d",score)->getCString(), "fonts/dial_num.fnt");
    scoreLabel->setScale(5);
    scoreLabel->setPosition(Vec2(size.width/2,size.height-50));
    this->addChild(scoreLabel);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    auto eventDispatch = Director::getInstance()->getEventDispatcher();
    eventDispatch->addEventListenerWithSceneGraphPriority(touchListener, this);
    auto audioengine = CocosDenshion::SimpleAudioEngine::getInstance();
    audioengine->preloadBackgroundMusic("sound/bg.mp3");
    
    
    // 自动创建10*10 的popStar矩阵
    autoCreatePopSprite(size);
    std::string startFlag = UserDefault::sharedUserDefault()->getStringForKey("startFalg");
    if (strcmp(startFlag.c_str(),"start") == 0) {
        setStringWithScreeningNum(sceneNum);
        UserDefault::sharedUserDefault()->setIntegerForKey("screening",sceneNum);
        UserDefault::sharedUserDefault()->setIntegerForKey("score",score);
    } else {
        int screening = UserDefault::sharedUserDefault()->getIntegerForKey("screening");
        setStringWithScreeningNum(screening);
        sceneNum = screening;
        int lastScore = UserDefault::sharedUserDefault()->getIntegerForKey("score");
        score = lastScore;
        scoreLabel->setString(String::createWithFormat("%d",score)->getCString());
    }
    return true;
}

void HelloWorld::setStringWithScreeningNum(int num)
{
    GCCsvHelper *csv = new GCCsvHelper();
    csv->openAndResolveFile("screening.csv");
    
    std::string line = "";
    line = csv->getData(num,0);
    unsigned long index = line.find(",");
    std::string temp1 = line.substr(0, index);
    std::string temp2 = line.substr(index+1, line.length());
    tollgateLabel->setString(String::createWithFormat("关卡:%s",temp1.c_str())->getCString());
    targetLabel->setString(String::createWithFormat("目标:%s",temp2.c_str())->getCString());
}


cocos2d::__Array* HelloWorld::checkPopUDLR(PopSprite* pop)
{
    cocos2d::__Array*arr = cocos2d::__Array::create();
    bool isDoublePop = false;
    // 判断上面的相同数
    int up = pop->getPopY();
    for (int i = up+1; i < 10; i++)
    {
        if (popStarSprite[pop->getPopX()][i]->getNumber() == popStarSprite[pop->getPopX()][pop->getPopY()]->getNumber()) {
            arr->addObject(popStarSprite[pop->getPopX()][i]);
            isDoublePop = true;
        }
        else
        {
            break;
        }
    }
    // 下面的
    int down = pop->getPopY();
    for (int i = down-1; i >= 0; i--)
    {
        if (popStarSprite[pop->getPopX()][i]->getNumber() == popStarSprite[pop->getPopX()][pop->getPopY()]->getNumber()) {
            arr->addObject(popStarSprite[pop->getPopX()][i]);
            isDoublePop = true;
        }
        else
        {
            break;
        }
    }
    //左边
    int left = pop->getPopX();
    for (int i = left-1; i >= 0; i--)
    {
        if (popStarSprite[i][pop->getPopY()]->getNumber() == popStarSprite[pop->getPopX()][pop->getPopY()]->getNumber()) {
            arr->addObject(popStarSprite[i][pop->getPopY()]);
            isDoublePop = true;
        }
        else
        {
            break;
        }
    }
    //左边
    int right = pop->getPopX();
    for (int i = right+1; i < 10; i++)
    {
        if (popStarSprite[i][pop->getPopY()]->getNumber() == popStarSprite[pop->getPopX()][pop->getPopY()]->getNumber()) {
            arr->addObject(popStarSprite[i][pop->getPopY()]);
            isDoublePop = true;
        }
        else
        {
            break;
        }
    }
    
    // 自身
    if(isDoublePop)
    {
        
        arr->addObject(pop);
    }
    return arr;
}

// 监听手势识别
bool HelloWorld::onTouchBegan(Touch* touch,Event* event)
{
    popArr = cocos2d::__Array::create();
    Vec2 getTouch = touch->getLocation();
    Size size = Director::getInstance()->getWinSize();
    PopSprite * pop  = getPopStarSprite(size, getTouch);
    if(pop != NULL && pop->getNumber() != -1)
    {
        //点击了色块检测上下左右
        popArr = checkPopUDLR(pop);
        if(popArr->count()>= 2)
        {
            checkPopInNew(popArr);
        }
    }
    return true;
}

// 传入获取到的上下左右的相同的popSprite然后循环遍历获取其他的popstar(如T字型的)
void HelloWorld::checkPopInNew(cocos2d::__Array* arrOld)
{
    cocos2d::__Array* newArr = cocos2d::__Array::create();
    for(int i = 0; i < arrOld->count();i++)
    {
        PopSprite* sprite = (PopSprite*)arrOld->getObjectAtIndex(i);
        cocos2d::__Array* oldArr = checkPopUDLR(sprite);
        for (int j = 0; j < oldArr->count(); j++)
        {
            bool isSave = false;
            PopSprite *oldSprite = (PopSprite*)oldArr->getObjectAtIndex(j);
            for (int k = 0; k< popArr->count(); k++)
            {
                PopSprite * popOld = (PopSprite*)popArr->getObjectAtIndex(k);
                if (oldSprite->getPopY() == popOld->getPopY() && oldSprite->getPopX() == popOld->getPopX())
                {
                    isSave = true;
                    break;
                }
            }
            if (!isSave) {
                newArr->addObject(oldSprite);
            }
        }
    }
    if (newArr->count() > 0)
    {
        for (int f = 0; f< newArr->count(); f++)
        {
            popArr->addObject(newArr->getObjectAtIndex(f));
        }
        checkPopInNew(newArr);
    }
    else
    {
        for(int i = (int)(popArr->count())-1;i>=0;i--)
        {
            auto delayTime = DelayTime::create(0.1 *i);
            PopSprite* sp = (PopSprite*)popArr->getObjectAtIndex(i);
            int count = (int)(popArr->count())-1;
            auto func = CallFunc::create([this,i,sp,count](){
                sp->setNumber(-1);
                auto audioengine = CocosDenshion::SimpleAudioEngine::getInstance();
                if (i+1 >= 14) {
                    audioengine->preloadEffect("sound/combo14.mp3");
                    audioengine->playEffect("sound/combo14.mp3");
                } else
                {
                    String *str = String::createWithFormat("sound/combo%zd.mp3",i+1);
                    
                    audioengine->preloadEffect(str->getCString());
                    audioengine->playEffect(str->getCString());
                }
                CCLog("开始的score---------%d",score);
                score += count/ 5 * 50 + count % 5 * 20;
                CCLog("结束的score---------%d",score);
                
                scoreLabel->setString(String::createWithFormat("%d",score)->getCString());
                if(i == count)
                {
                    if(count >= 6)
                    {
                        audioengine->preloadEffect("sound/cheer.mp3");
                        audioengine->playEffect("sound/cheer.mp3");
                    }
                    else if (count >= 4)
                    {
                        audioengine->preloadEffect("sound/rainbow.mp3");
                        audioengine->playEffect("sound/rainbow.mp3");
                    }
                    //让空白部分被上面的填充
                    reDiessPopStar();
                }
                
            });
            auto seq = Sequence::create(delayTime, func, nullptr);
            this->runAction(seq);
        }
    }
    
}


void HelloWorld::gameOver()
{
    bool isGameOver = true;
    Size size = Director::getInstance()->getWinSize();
    for (int i =0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (checkPopUDLR(popStarSprite[i][j])->count() > 0 && popStarSprite[i][j]->getNumber() != -1)
            {
                isGameOver = false;
                break;
            }
            if (isGameOver == false) {
                break;
            }
        }
        
    }
    GCCsvHelper *csv = new GCCsvHelper();
    csv->openAndResolveFile("screening.csv");
    
    std::string line = "";
    line = csv->getData(sceneNum,0);
    unsigned long index = line.find(",");
    std::string temp2 = line.substr(index+1, line.length());
    int needScore =std::atoi(temp2.c_str());
    
    if(isGameOver)
    {
        // 游戏结束 让余下的闪烁
        for(int i = 0;i< 10;i++)
        {
            for (int j = 0; j < 10; j++)
            {
                PopSprite * popSprite =popStarSprite[i][j];
                Blink* blink =  Blink::create(0.3f, 3);// 代表闪烁的时间、闪烁的次数
                popSprite->runAction(blink);
            }
        }
        Size size = Director::getInstance()->getWinSize();
        lastScoreLabel = LabelBMFont::create(String::createWithFormat("%d",score)->getCString(), "fonts/dial_num.fnt");
        lastScoreLabel->setScale(7);
        lastScoreLabel->setPosition(Vec2(size.width/2,size.height/2));
        this->addChild(lastScoreLabel);
        // 存储最高分、关卡，分数
        int hightScore = UserDefault::sharedUserDefault()->getIntegerForKey("highSocre");
        if (hightScore < score) {
            UserDefault::sharedUserDefault()->setIntegerForKey("highSocre",score);
        }
        UserDefault::sharedUserDefault()->setIntegerForKey("screening",sceneNum+1);
        UserDefault::sharedUserDefault()->setIntegerForKey("score",score);
        CCLog("%d----------------------------***************%d",score,needScore);
        if(score < needScore)
        {
            // 分数小于彻底结束显示重玩按钮
            auto delayTime = DelayTime::create(0.3f);
            auto func = CallFunc::create([this,size](){
                auto startButtonBg = Scale9Sprite::createWithSpriteFrameName("game_replay.png");
                startButtonBg->setScale(2);
                auto startButton = ControlButton::create(startButtonBg);
                startButton->addTargetWithActionForControlEvents(this,cccontrol_selector(HelloWorld::touchUpInside),Control::EventType::TOUCH_UP_INSIDE);
                startButton->setPosition(Vec2(size.width/2,200));
                this->addChild(startButton);
            });
            
            auto seq = Sequence::create(delayTime, func, nullptr);
            this->runAction(seq);
        }
        else
        {
            int screening2 = UserDefault::sharedUserDefault()->getIntegerForKey("screening");
            SpriteFrameCache* frameCache = SpriteFrameCache::sharedSpriteFrameCache();
            frameCache->addSpriteFramesWithFile("images/popModePlist.plist","images/popModePlist.pvr.ccz");
            //通过一个镇图片生产精灵
            winSprite = Scale9Sprite::createWithSpriteFrameName("adventure_win2_english.png");
            winSprite->setPosition(Vec2(size.width/2,size.height-40));
            this->addChild(winSprite);
            ScaleTo* scaleTo = ScaleTo::create(0.1, 3);
            RotateBy* rotateBy = RotateBy::create(0.1, 10);
            RotateBy* rotateBy2 = RotateBy::create(0.1, -10);
            CallFunc* callFunc = CallFunc::create(this, callfunc_selector(HelloWorld::setScoreLabel));
            Action* actions = Sequence::create(scaleTo,rotateBy,rotateBy2,callFunc, NULL);
            winSprite->runAction(actions);
            auto audioengine = CocosDenshion::SimpleAudioEngine::getInstance();
            audioengine->preloadEffect("sound/cheer.mp3");
            audioengine->playEffect("sound/cheer.mp3");
        }
    }
}

void HelloWorld::setScoreLabel()
{
    winSprite->setVisible(false);
    lastScoreLabel->setVisible(false);
    Size size = Director::getInstance()->getWinSize();
    autoCreatePopSprite(size);
    sceneNum = sceneNum + 1;
    setStringWithScreeningNum(sceneNum);
}

void HelloWorld::touchUpInside(Ref* pSender,Control::EventType event)
{
    UserDefault::sharedUserDefault()->setIntegerForKey("screening",1);
    UserDefault::sharedUserDefault()->setIntegerForKey("score",0);
    Director::getInstance()->pushScene(StartScene::createScene());
}


void HelloWorld::reDiessPopStar()
{
    for (int i = 0 ; i <  10 ; i++)
    {
        reDiessPopStarRun(i);
    }
    // 当空白的一列时需要将右边的往左移动
    reDiessPopSatrTurnLeft();
    gameOver();
}

void HelloWorld::reDiessPopStarRun(int x)
{
    int number = 0;
    // 检测每一列中的空格数
    for(int i = 0; i < 10; i++)
    {
        if (popStarSprite[x][i]->getNumber() == -1)
        {
            number ++;
        }
    }
    // 循环替换(当有空白格时)
    if(number > 0)
    {
        for(int y = 0;y<10;y++)
        {
            // 判断是否需要回调自己
            if(popStarSprite[x][y]->getNumber() == -1)
            {
                if (10 - popStarSprite[x][y]->getPopY() > number)
                {
                    for (int s = y; s< 10; s++)
                    {
                        if (s+1 == 10)
                        {
                            popStarSprite[x][s]->setNumber(-1);
                        }
                        else
                        {
                            popStarSprite[x][s]->setNumber(popStarSprite[x][s+1]->getNumber());
                        }
                    }
                    reDiessPopStarRun(x);
                }
                else
                {
                    break;
                }
            }
        }
    }
}

// 当空白的一列时需要将右边的往左移动
void HelloWorld::reDiessPopSatrTurnLeft()
{
    for (int x = 0; x < 10; x++)
    {
        int number = 0;
        // 检测每一列中的空格数
        for(int i = 0; i < 10; i++)
        {
            if (popStarSprite[x][i]->getNumber() == -1)
            {
                number ++;
            }
        }
        if(number == 10)
        {
            for (int y = 0; y < 10; y++)
            {
                for (int s = x; s<9; s++)
                {
                    popStarSprite[s][y]->setNumber(popStarSprite[s+1][y]->getNumber());
                    popStarSprite[s+1][y]->setNumber(-1);
                }
            }
        }
    }
}

// 通过点击获取当前点击的popSprite
PopSprite* HelloWorld::getPopStarSprite(Size size,Vec2 touch)
{
    int lon = (size.width - 60)/10;
    // 取出点击的X位置
    float x = (touch.x - 30)/lon;
    // 取出点击的Y位置
    float y = (touch.y - size.height*0.1)/lon;
    
    if (x < 10 && y < 10 && x >= 0 && y >= 0)
    {
        int x1 = (touch.x - 30)/lon;
        int y1 = (touch.y - (size.height*0.1))/lon;
        return popStarSprite[x1][y1];
    }
    return NULL;
}


// 自动创建10*10 的popStar矩阵
void HelloWorld::autoCreatePopSprite(Size size)
{
    // 加入随机种子
    srand((unsigned)time(NULL));
    // 获取pop的宽高
    int lon = (size.width - 60)/10;
    for (int i = 0; i < 10; i++)
    {
        for(int j = 0;j< 10 ; j++)
        {
            int maxX = 5;
            int minX = 0;
            int rangeX = maxX-minX;
            int ramdomX = (rand() % rangeX)+minX;
            PopSprite* pop = PopSprite::createPopSprite(-1, lon, lon, lon * i+30, lon*j+size.height*0.1);
            pop->setPopX(i);
            pop->setPopY(j);
            pop->setNumber(ramdomX);
            this->addChild(pop);
            // pop 加入数组
            popStarSprite[i][j] = pop;
        }
    }
}



