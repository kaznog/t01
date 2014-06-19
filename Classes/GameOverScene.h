//
//  GameOverScene.h
//  t01
//
//  Created by 野口一也 on 2014/06/06.
//
//

#ifndef __t01__GameOverScene__
#define __t01__GameOverScene__

#include "cocos2d.h"

USING_NS_CC;

class GameOverLayer : public cocos2d::LayerColor
{
public:
    GameOverLayer() : _label(nullptr) {};
    virtual ~GameOverLayer();
    virtual bool init();
    
    CREATE_FUNC(GameOverLayer);
    
    void gameOverDone();
    
    CC_SYNTHESIZE(LabelTTF*, _label, Label);
};

class GameOverScene : public cocos2d::Scene
{
public:
    GameOverScene() : _gameoverLayer(nullptr) {};
    ~GameOverScene();
    virtual bool init();
    
    CREATE_FUNC(GameOverScene);
    CC_SYNTHESIZE_READONLY(GameOverLayer*, _gameoverLayer, GameOverLayer);
};

#endif /* defined(__t01__GameOverScene__) */
