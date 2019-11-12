//
//  TeamScene.hpp
//  TalkCppSample
//
//  Created by 杜红 on 2017/3/21.
//
//

#ifndef TeamScene_hpp
#define TeamScene_hpp

#include <stdio.h>

#include "cocos2d.h"

#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
USING_NS_CC;

#include <string>
using namespace std;

class TeamScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void loadUI();
    //注册声音引擎事件
    void addVoiceEvent();
    
    void addTips( string  strTips);
    
    //暂停
    void onBtnPause(Ref *pSender );
    //恢复
    void onBtnResume(Ref * pSender );
    //离开频道
    void onBtnLeave(Ref * pSender );

    
    Label*   m_labelTips;
    
    // implement the "static create()" method manually
    CREATE_FUNC(TeamScene);
};

#endif /* TeamScene_hpp */
