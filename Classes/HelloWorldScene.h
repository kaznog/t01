#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::LayerColor
{
public:
    HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void update(float dt);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    /** Touch listener */
    void enableTouchEvent(bool enabled);
    CC_SYNTHESIZE(EventListenerTouchOneByOne*, _touchListener, TouchListener);
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchCancelled(Touch *touch, Event *event);
    
    void monsterMoveFinished(Node* Sender);
    void delete_monster(Node* Sender);
    void delete_projectile(Node* Sender);
    std::vector<Sprite*> _monsters;
    std::vector<Sprite*> _projectiles;
protected:
    CC_SYNTHESIZE(Size, _winSize, WinSize);
    CC_SYNTHESIZE(int, _projectileDestroyed, ProjectileDestroyed);
    void addMonster();
    void gameLogic(float dt);
};

#endif // __HELLOWORLD_SCENE_H__
