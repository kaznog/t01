#include "HelloWorldScene.h"
#include "GameOverScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::HelloWorld()
: _projectileDestroyed(0)
{
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(255, 255, 255, 255)) )
    {
        return false;
    }
    auto director = Director::getInstance();
    director->getOpenGLView()->setDesignResolutionSize(480, 320, ResolutionPolicy::SHOW_ALL);
    _winSize = director->getWinSize();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto player = Sprite::create("player.png");
    player->setPosition(Vec2(player->getContentSize().width/2, _winSize.height/2));
    this->addChild(player);
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    _monsters.clear();
    _projectiles.clear();

    this->enableTouchEvent(true);
    this->schedule(schedule_selector(HelloWorld::gameLogic), 1.0f);
    this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt)
{
    std::vector<Sprite*> projectileToDelete;
    projectileToDelete.clear();
    for (std::vector<Sprite*>::iterator it = _projectiles.begin(); it != _projectiles.end(); it++) {
        auto projectile = dynamic_cast<Sprite*>(*it);
        Rect projectileRect = Rect(projectile->getPosition().x - projectile->getContentSize().width/2,
                                   projectile->getPosition().y - projectile->getContentSize().height/2,
                                   projectile->getContentSize().width,
                                   projectile->getContentSize().height);
        std::vector<Sprite*> targetToDelete;
        targetToDelete.clear();
        for (std::vector<Sprite*>::iterator mit = _monsters.begin(); mit != _monsters.end(); mit++) {
            auto monster = dynamic_cast<Sprite*>(*mit);
            Rect monsterRect = Rect(monster->getPosition().x - monster->getContentSize().width/2,
                                    monster->getPosition().y - monster->getContentSize().height/2,
                                    monster->getContentSize().width,
                                    monster->getContentSize().height);
            if (projectileRect.intersectsRect(monsterRect)) {
                targetToDelete.push_back(monster);
            }
        }
        for (std::vector<Sprite*>::iterator dit = targetToDelete.begin(); dit != targetToDelete.end(); dit++) {
            auto monster = dynamic_cast<Sprite*>(*dit);
            this->delete_monster(monster);
            
            _projectileDestroyed++;
            if (_projectileDestroyed >= 5) {
                auto gameOverScene = GameOverScene::create();
                gameOverScene->getGameOverLayer()->getLabel()->setString("You Win !");
                Director::getInstance()->replaceScene(gameOverScene);
            }
        }
        if (targetToDelete.size() > 0) {
            projectileToDelete.push_back(projectile);
        }
        targetToDelete.clear();
    }
    for (std::vector<Sprite*>::iterator pit = projectileToDelete.begin(); pit != projectileToDelete.end(); pit++) {
        auto projectile = dynamic_cast<Sprite*>(*pit);
        this->delete_projectile(projectile);
    }
    projectileToDelete.clear();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::addMonster()
{
    auto monster = Sprite::create("monster.png");
    
    int minY = monster->getContentSize().height / 2;
    int maxY = _winSize.height - monster->getContentSize().height / 2;
    int rangeY = maxY - minY;
    int actualY = (rand() % rangeY) + minY;
    
    monster->setPosition(Vec2(_winSize.width + monster->getContentSize().width / 2, actualY));
    monster->setTag(1);
    _monsters.push_back(monster);
    
    this->addChild(monster);
    
    int minDuration = 2;
    int maxDuration = 4;
    int rangeDuration = maxDuration - minDuration;
    int actualDuration = (rand() % rangeDuration) + minDuration;
    auto actionMove = MoveTo::create(actualDuration, Vec2(- monster->getContentSize().width / 2, actualY));
    monster->runAction(Sequence::create(actionMove, CallFunc::create(CC_CALLBACK_0(HelloWorld::monsterMoveFinished, this, monster)), NULL));
    
}

void HelloWorld::gameLogic(float dt)
{
    this->addMonster();
}

#pragma mark -
#pragma mark touch events
void HelloWorld::enableTouchEvent(bool enabled)
{
    if (this->_touchListener != nullptr) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(this->_touchListener);
        this->_touchListener = nullptr;
    }
    if (enabled) {
        this->_touchListener = EventListenerTouchOneByOne::create();
        _touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
    
    return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event)
{
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
    auto location = touch->getLocationInView();
    location = Director::getInstance()->convertToGL(location);
    
    auto projectile = Sprite::create("projectile.png");
    projectile->setPosition(Vec2(20, _winSize.height/2));
    
    Vec2 offset = location - projectile->getPosition();
    
    if (offset.x <= 0) {
        return;
    }
    
    projectile->setTag(2);
    _projectiles.push_back(projectile);

    this->addChild(projectile);
    
    float realX = _winSize.width + projectile->getContentSize().width/2;
    float ratio = offset.y / offset.x;
    float realY = (realX * ratio) + projectile->getPosition().y;
    Vec2 realDest = Vec2(realX, realY);
    
    float distance = realDest.distance(projectile->getPosition());
//    float offRealX = realX - projectile->getPosition().x;
//    float offRealY = realY - projectile->getPosition().y;
//    float length = sqrtf((offRealX*offRealX) + (offRealY*offRealY));
    float velocity = 480 / 1;
    float realDuration = distance / velocity;
    
    projectile->runAction(Sequence::create(MoveTo::create(realDuration, realDest), CallFunc::create(CC_CALLBACK_0(HelloWorld::delete_projectile, this, projectile)), NULL));
}

void HelloWorld::onTouchCancelled(Touch *touch, Event *event)
{
}

#pragma mark -
void HelloWorld::monsterMoveFinished(Node* Sender)
{
    auto gameOverScene = GameOverScene::create();
    gameOverScene->getGameOverLayer()->getLabel()->setString("You Lose :[");
    Director::getInstance()->replaceScene(gameOverScene);
}

void HelloWorld::delete_monster(Node* Sender)
{
    auto monster = (Sprite*)Sender;
    std::vector<Sprite*> tmplist;
    tmplist.clear();
    for (std::vector<Sprite*>::iterator it = _monsters.begin(); it != _monsters.end(); it++) {
        auto object = (Sprite*)*it;
        if (object != monster) {
            tmplist.push_back(object);
        }
    }
    _monsters.clear();
    for (std::vector<Sprite*>::iterator it = tmplist.begin(); it != tmplist.end(); it++) {
        auto object = (Sprite*)*it;
        _monsters.push_back(object);
    }
    tmplist.clear();
    monster->removeFromParentAndCleanup(true);
}

void HelloWorld::delete_projectile(Node* Sender)
{
    auto projectile = (Sprite*)Sender;
    std::vector<Sprite*> tmplist;
    tmplist.clear();
    for (std::vector<Sprite*>::iterator it = _projectiles.begin(); it != _projectiles.end(); it++) {
        auto object = (Sprite*)*it;
        if (object != projectile) {
            tmplist.push_back(object);
        }
    }
    _projectiles.clear();
    for (std::vector<Sprite*>::iterator it = tmplist.begin(); it != tmplist.end(); it++) {
        auto object = (Sprite*)*it;
        _projectiles.push_back(object);
    }
    tmplist.clear();
    projectile->removeFromParentAndCleanup(true);
}
