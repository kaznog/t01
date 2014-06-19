//
//  GameOverScene.cpp
//  t01
//
//  Created by 野口一也 on 2014/06/06.
//
//

#include "GameOverScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

GameOverScene::~GameOverScene()
{
    if (this->_gameoverLayer) {
        this->_gameoverLayer->release();
        this->_gameoverLayer = nullptr;
    }
}

bool GameOverScene::init()
{
    if ( Scene::init()) {
        this->_gameoverLayer = GameOverLayer::create();
        this->_gameoverLayer->retain();
        this->addChild(this->_gameoverLayer);
        
        return true;
    } else {
        return false;
    }
}


GameOverLayer::~GameOverLayer()
{
    if (_label) {
        _label->release();
        _label = nullptr;
    }
}

bool GameOverLayer::init()
{
    if (LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
        Size winSize = Director::getInstance()->getWinSize();
        this->_label = LabelTTF::create("", "Arial", 32);
        this->_label->retain();
        this->_label->setColor(Color3B(0, 0, 0));
        this->_label->setPosition(Vec2(winSize.width/2, winSize.height/2));
        this->addChild(this->_label);
        
        this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create(CC_CALLBACK_0(GameOverLayer::gameOverDone, this)), NULL));
        return true;
    } else {
        return false;
    }
}

void GameOverLayer::gameOverDone()
{
    Director::getInstance()->replaceScene(HelloWorld::createScene());
}