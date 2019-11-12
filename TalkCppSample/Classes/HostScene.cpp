//
//  HostScene.cpp
//  TalkCppSample
//
//  Created by 杜红 on 2017/3/21.
//
//

#include "HostScene.hpp"
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
extern YouMeUserRole g_roleType;


Scene* HostScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HostScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HostScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    loadCommonUI();
    
    if( g_roleType ==  YOUME_USER_HOST ){
        loadHostUI();
        
        //进入房间后，立刻设置麦克风，扬声器状态
        //主播模式，都需要
        IYouMeVoiceEngine::getInstance()->setMicrophoneMute(true);
        IYouMeVoiceEngine::getInstance()->setSpeakerMute( false );
        IYouMeVoiceEngine::getInstance()->setVolume( 100 );

    }
    else{
        //进入房间后，立刻设置麦克风，扬声器状态
        //听众模式，不需要麦克风
        IYouMeVoiceEngine::getInstance()->setMicrophoneMute(true);
        IYouMeVoiceEngine::getInstance()->setSpeakerMute( false );
        IYouMeVoiceEngine::getInstance()->setVolume( 100 );
    }
    
    addVoiceEvent();
    
    return true;
}

void HostScene::addVoiceEvent(){
    //显示来自声音引擎的信息
    auto tipsListener = EventListenerCustom::create("Tips", [=](EventCustom* event){
        //进入房间成功，切换场景
        string* pTips = (string*)event->getUserData();
        addTips( *pTips );
        
        delete pTips;
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(tipsListener, 1 );
}

void HostScene::addTips( string  strTips){
	m_labelTips->setString(UTEXT(strTips.c_str()) );
}

//播放和停止音乐
void HostScene::onBtnPlayMusic(Ref * pSender ){
    FileUtils* f = FileUtils::getInstance();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //android直接播放资源路径下的文件有坑，所以这里写死个路径
    //调用的时候请自己选择合适的路径
    std::string strPath = "/sdcard/temp/shinian.mp3";
#else
    std::string dir = "music/shinian.mp3";
    std::string strPath = f->fullPathForFilename( dir );
#endif

    /**
     *  功能描述: 播放背景音乐，并允许选择混合到扬声器输出麦克风输入
     *  @param pFilePath 音乐文件的路径
     *  @param bRepeat 是否重复播放
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    YouMeErrorCode errorcode = IYouMeVoiceEngine::getInstance()->playBackgroundMusic(strPath.c_str(), false );
     addTips("播放音乐");
    if( YOUME_SUCCESS == errorcode)
    {
        YouMe_Log("调用播放音乐成功，%s\n", strPath.c_str());
    }
    else
    {
        YouMe_Log("调用播放音乐失败，错误码：%d, %s\n",  errorcode,strPath.c_str());
    }

    
}
void HostScene::onBtnStopMusic(Ref * pSender ){
    /**
     *  功能描述: 如果当前正在播放背景音乐的话，停止播放
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    YouMeErrorCode errorcode = IYouMeVoiceEngine::getInstance()->stopBackgroundMusic();
    addTips("停止音乐");
    if( YOUME_SUCCESS == errorcode)
    {
        YouMe_Log("调用停止音乐成功\n");
    }
    else
    {
        YouMe_Log("调用停止音乐失败，错误码：%d\n",  errorcode);
    }
}

void HostScene::onBtnOpenMic(Ref *pSender ){
    /**
     *  功能描述:设置麦克风静音
     *
     *  @param bOn:true——静音，false——取消静音
     *  @return 无
     */
    IYouMeVoiceEngine::getInstance()->setMicrophoneMute(false);
    addTips("上麦");
    YouMe_Log("上麦\n");
}

void HostScene::onBtnCloseMic(Ref * pSender ){
    /**
     *  功能描述:设置麦克风静音
     *
     *  @param bOn:true——静音，false——取消静音
     *  @return 无
     */
    IYouMeVoiceEngine::getInstance()->setMicrophoneMute( true );

    addTips("下麦");
    YouMe_Log("下麦\n");
}

void HostScene::onBtnLeave(Ref * pSender ){
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


void HostScene::valueChanged(Ref *sender, Control::EventType controlEvent){
    ControlSlider* pSlider = (ControlSlider*)sender;
    // Change value of label.
    int vol = (int)pSlider->getValue();
    /**
     *  功能描述: 设置背景音乐播放的音量
     *  @param vol 背景音乐的音量，范围 0-100
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    YouMeErrorCode errorcode = IYouMeVoiceEngine::getInstance()->setBackgroundMusicVolume(vol);
    YouMe_Log("设置背景音乐音量:%d，错误码：%d\n", vol,  errorcode);
    addTips("设置音乐音量");
}

void HostScene::loadCommonUI(){
    auto visibleSize = Director::getInstance()->getVisibleSize();

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
    
    auto btnExit = Button::create();
    btnExit->setTitleFontSize( 80 );
	btnExit->setTitleText(UTEXT("退出"));
    btnExit->setPosition( Vec2(visibleSize.width - 100, 10 ) );
    btnExit->setAnchorPoint( Vec2(0.5, 0) );
    btnExit->addClickEventListener( CC_CALLBACK_1( HostScene::onBtnLeave, this  ) );
    addChild( btnExit );
    
    m_labelTips = Label::createWithSystemFont("Tips", "Arial", 40);
    m_labelTips->setPosition( 0, 0 );
    m_labelTips->setAnchorPoint( Vec2(0,0)) ;
    addChild( m_labelTips );
}


void HostScene::loadHostUI(){
    const float labelX = 50;
    auto visibleSize = Director::getInstance()->getVisibleSize();

    //button
    auto btnOpenMic = Button::create();
    btnOpenMic->setTitleFontSize( 80 );
    btnOpenMic->setPosition(Vec2(300 ,280 ));
    btnOpenMic->setAnchorPoint( Vec2(0.5, 0) );
	btnOpenMic->setTitleText(UTEXT("上麦"));
    btnOpenMic->addClickEventListener( CC_CALLBACK_1( HostScene::onBtnOpenMic, this  ) );
    addChild( btnOpenMic);
    
    auto btnCloseMic = Button::create();
    btnCloseMic->setTitleFontSize( 80 );
	btnCloseMic->setTitleText(UTEXT("下麦"));
    btnCloseMic->setPosition( Vec2(300, 180 ) );
    btnCloseMic->setAnchorPoint( Vec2(0.5, 0) );
    btnCloseMic->addClickEventListener( CC_CALLBACK_1( HostScene::onBtnCloseMic, this  ) );
    addChild( btnCloseMic );
    
    auto btnPlayMusic = Button::create();
    btnPlayMusic->setTitleFontSize( 80 );
    btnPlayMusic->setPosition(Vec2(700,280 ));
    btnPlayMusic->setAnchorPoint( Vec2(0.5, 0) );
	btnPlayMusic->setTitleText(UTEXT("播放音乐"));
    btnPlayMusic->addClickEventListener( CC_CALLBACK_1( HostScene::onBtnPlayMusic, this  ) );
    addChild( btnPlayMusic);
    
    auto btnStopMusic = Button::create();
    btnStopMusic->setTitleFontSize( 80 );
	btnStopMusic->setTitleText(UTEXT("停止音乐"));
    btnStopMusic->setPosition( Vec2(700, 180 ) );
    btnStopMusic->setAnchorPoint( Vec2(0.5, 0) );
    btnStopMusic->addClickEventListener( CC_CALLBACK_1( HostScene::onBtnStopMusic, this  ) );
    addChild( btnStopMusic );
    
    auto slider = ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    slider->setAnchorPoint(Vec2(0.5f, 1.0f));
    slider->setMinimumValue(1.0f); // Sets the min value of range
    slider->setMaximumValue(100.0f); // Sets the max value of range
    slider->setPosition( 700, 100 );
    slider->setTag(1);
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(HostScene::valueChanged), Control::EventType::VALUE_CHANGED);
    addChild( slider );
    //slider->setContentSize( Size( 300, 50 ) );
    //初识音量设置为
    int vol = 80;
    slider->setValue( vol );
    /**
     *  功能描述: 设置背景音乐播放的音量
     *  @param vol 背景音乐的音量，范围 0-100
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    YouMeErrorCode errorcode = IYouMeVoiceEngine::getInstance()->setBackgroundMusicVolume( vol );
    YouMe_Log("设置背景音乐音量:%d，错误码：%d\n", vol,  errorcode);

    
}
