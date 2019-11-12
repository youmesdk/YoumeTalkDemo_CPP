//
//  TeamScene.cpp
//  TalkCppSample
//
//  Created by 杜红 on 2017/3/21.
//
//

#include "TeamScene.hpp"

#include "LoginScene.h"

#include "SimpleAudioEngine.h"
#include "IYouMeVoiceEngine.h"
#include "YouMeVoiceEngineImp.hpp"
#include "iconv.h"


#include <string>
using namespace std;
USING_NS_CC;

extern string g_roomName;
extern string g_userID;


Scene* TeamScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TeamScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TeamScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    loadUI();
    
    addVoiceEvent();
    
    //进入房间后，立刻设置麦克风，扬声器状态
    //小队模式，都需要
    IYouMeVoiceEngine::getInstance()->setMicrophoneMute(false);
    IYouMeVoiceEngine::getInstance()->setSpeakerMute( false );
    IYouMeVoiceEngine::getInstance()->setVolume( 100 );

    
    return true;
}

void TeamScene::addVoiceEvent(){
    //显示来自声音引擎的信息
    auto tipsListener = EventListenerCustom::create("Tips", [=](EventCustom* event){
        string* pTips = (string*)event->getUserData();
        addTips( *pTips );
        
        delete pTips;
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(tipsListener, 1 );
}

void TeamScene::addTips( string  strTips){
	m_labelTips->setString(UTEXT(strTips.c_str()));
}

void TeamScene::onBtnPause(Ref *pSender ){
    /**
     *  功能描述: 暂停通话，释放麦克风等设备资源
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    YouMeErrorCode errorcode = IYouMeVoiceEngine::getInstance()->pauseChannel();
    if( YOUME_SUCCESS == errorcode)
    {
        YouMe_Log("调用暂停成功\n");
    }
    else
    {
        YouMe_Log("调用暂停失败，错误码：%d\n",  errorcode);
    }
}

void TeamScene::onBtnResume(Ref * pSender ){
    /**
     *  功能描述: 恢复通话
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    YouMeErrorCode errorcode = IYouMeVoiceEngine::getInstance()->resumeChannel();
    if( YOUME_SUCCESS == errorcode)
    {
        YouMe_Log("调用恢复成功\n");
    }
    else
    {
        YouMe_Log("调用恢复失败，错误码：%d\n",  errorcode);
    }
}

void TeamScene::onBtnLeave(Ref * pSender ){
    //清除注册的事件
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("Tips");
    
    /**
     *  功能描述:退出所有语音频道
     *
     *  @return 错误码，详见YouMeConstDefine.h定义
     */
    YouMeErrorCode errorcode = IYouMeVoiceEngine::getInstance()->leaveChannelAll();
    if( YOUME_SUCCESS == errorcode)
    {
        YouMe_Log("调用退出所有频道成功\n");
    }
    else
    {
        YouMe_Log("调用退出所有频道失败，错误码：%d\n",  errorcode);
    }
    
    //退回登录界面
    auto director = Director::getInstance();
    Scene* scene = LoginScene::createScene();
    director->replaceScene(scene);
}

void TeamScene::loadUI(){
    const float labelX = 50;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    m_labelTips = Label::createWithSystemFont("Tips", "Arial", 40);
    m_labelTips->setPosition( 0, 0 );
    m_labelTips->setAnchorPoint( Vec2(0,0)) ;
    addChild( m_labelTips );
    
    
    //label
    auto labelID = Label::createWithSystemFont("", "Arial", 40);
    labelID->setPosition( 20, visibleSize.height - 80 );
    labelID->setAnchorPoint( Vec2(0,0)) ;
    stringstream suser;
	suser << UTEXT("用户:") << g_userID;
    labelID->setString(suser.str() );
    addChild( labelID );
    
    auto labelRoom = Label::createWithSystemFont("", "Arial", 40);
    labelRoom->setPosition( 20 , visibleSize.height - 160 );
    labelRoom->setAnchorPoint( Vec2(0,0)) ;
    stringstream sroom;
	sroom << UTEXT("频道:") << g_roomName;
    labelRoom->setString(sroom.str() );
    addChild( labelRoom );
    
    //button
    auto btnPause = Button::create();
    btnPause->setTitleFontSize( 80 );
    btnPause->setPosition(Vec2(visibleSize.width/2,280 ));
    btnPause->setAnchorPoint( Vec2(0.5, 0) );
	btnPause->setTitleText(UTEXT("暂停"));
    btnPause->addClickEventListener( CC_CALLBACK_1( TeamScene::onBtnPause, this  ) );
    addChild( btnPause);
    
    auto btnResume = Button::create();
    btnResume->setTitleFontSize( 80 );
	btnResume->setTitleText(UTEXT("恢复"));
    btnResume->setPosition( Vec2(visibleSize.width/2, 180 ) );
    btnResume->setAnchorPoint( Vec2(0.5, 0) );
    btnResume->addClickEventListener( CC_CALLBACK_1( TeamScene::onBtnResume, this  ) );
    addChild( btnResume );
    
    auto btnExit = Button::create();
    btnExit->setTitleFontSize( 80 );
	btnExit->setTitleText(UTEXT("退出"));
    btnExit->setPosition( Vec2(visibleSize.width - 100, 10 ) );
    btnExit->setAnchorPoint( Vec2(0.5, 0) );
    btnExit->addClickEventListener( CC_CALLBACK_1( TeamScene::onBtnLeave, this  ) );
    addChild( btnExit );

}

