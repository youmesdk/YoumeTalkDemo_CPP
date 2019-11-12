//
//  HostScene.hpp
//  TalkCppSample
//
//  Created by 杜红 on 2017/3/21.
//
//

#ifndef HostScene_hpp
#define HostScene_hpp

#include <stdio.h>

#include <stdio.h>

#include "cocos2d.h"

#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
#include "cocos-ext.h"
using namespace cocos2d::extension;
USING_NS_CC;

#include <string>
using namespace std;

class HostScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void loadHostUI();
    void loadCommonUI();
    //注册声音引擎事件
    void addVoiceEvent();
    
    void addTips( string  strTips);
    //离开频道
    void onBtnLeave(Ref * pSender );
    
    //主播相关的操作
    //上麦
    void onBtnOpenMic(Ref * pSender );
    //下麦
    void onBtnCloseMic(Ref * pSender );
    //播放和停止音乐
    void onBtnPlayMusic(Ref * pSender );
    void onBtnStopMusic(Ref * pSender );
    void valueChanged(Ref *sender, Control::EventType controlEvent);
    
    Label* m_labelTips;
    
    // implement the "static create()" method manually
    CREATE_FUNC(HostScene);
};

#endif /* HostScene_hpp */
