#include "LoginScene.h"
#include "SimpleAudioEngine.h"
#include "IYouMeVoiceEngine.h"
#include "YouMeVoiceEngineImp.hpp"

#include "TeamScene.hpp"
#include "HostScene.hpp"
#include "iconv.h"

#include <string>
using namespace std;
USING_NS_CC;

extern string g_roomName;
extern string g_userID;
extern YouMeUserRole g_roleType;


Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoginScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    loadUI();
    
    addVoiceEvent();
    
    return true;
}

void LoginScene::addVoiceEvent(){
    //进入房间成功事件
    auto enterListener = EventListenerCustom::create("EnterRoom", [=](EventCustom* event){
        //清除注册的事件
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("EnterRoom");
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("Tips");

        
        //进入房间成功，切换场景
        auto director = Director::getInstance();
        
        Scene* scene = NULL;
        switch( g_roleType ){
            case YOUME_USER_TALKER_FREE:{
                scene = TeamScene::createScene();
                break;
            }
            case YOUME_USER_LISTENER:{
                //通过g_roleType决定场景模式
                scene = HostScene::createScene();
            }
            break;
            case YOUME_USER_HOST:{
                //通过g_roleType决定场景模式
                scene = HostScene::createScene();
            }
            default:
                break;
        }
        
        if( scene ){
            director->replaceScene(scene);
        }
     });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(enterListener, 1 );
    
    //显示来自声音引擎的信息
    auto tipsListener = EventListenerCustom::create("Tips", [=](EventCustom* event){
        string* pTips = (string*)event->getUserData();
        addTips( *pTips );
        
        delete pTips;
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(tipsListener, 1 );
}

void LoginScene::addTips( string  strTips){
	m_labelTips->setString(UTEXT(strTips.c_str()));
}

bool LoginScene::isKeyValid( std::string strID ){
    if( strID.length() == 0 ){
        return false;
    }
    
    const char* p = strID.c_str();
    for( int i = 0; i < strID.length(); i++ ){
        const char c = *(p+i);
        if( c ==  '_' ||
           ( c >= 'a' && c <= 'z' ) ||
           ( c >= 'A' && c <= 'Z') ||
           ( c >= '0' && c <= '9') ){
            continue;
        }
        else{
            return false;
        }
    }
    return true;
}


void LoginScene::loginChatRoom( YouMeUserRole roleType ){
    g_userID = idInput->getText();
    g_roomName = roomInput->getText();
    g_roleType = roleType;
    
    if( !isKeyValid( g_userID )){
		addTips(UTEXT("用户ID不合法"));
        return ;
    }
    
    if( !isKeyValid( g_roomName )){
		addTips(UTEXT("房间名字不合法"));
        return;
    }

    //YOUME
    /*!
     *  进入房间
     *
     *  @param pUserID   用户ID
     *  @param pChannelID 频道ID
     *  @param eUserRole 用户角色（YouMeUserRole）
     *  @return 错误码
     */
    
    YouMeErrorCode errorcode = IYouMeVoiceEngine::getInstance()->joinChannelSingleMode( g_userID.c_str(),
                                                                                       g_roomName.c_str(),
                                                                                       g_roleType );
    if( YOUME_SUCCESS == errorcode)
    {
        YouMe_Log("调用进入房间成功\n");
    }
    else
    {
        YouMe_Log("调用进入房间失败，错误码：%d\n",  errorcode);
    }
}


//小队频道
void LoginScene::onBtnEnterTeam(Ref *pSender ){
    loginChatRoom(YOUME_USER_TALKER_FREE );
}
//主播频道，听众
void LoginScene::onBtnEnterListener(Ref * pSender ){
    loginChatRoom(YOUME_USER_LISTENER );
}
//主播频道,主播
void LoginScene::onBtnEnterHost(Ref * pSender ){
    loginChatRoom(YOUME_USER_HOST );
}

void LoginScene::loadUI(){
    const float labelX = 50;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    m_labelTips = Label::createWithSystemFont("Tips", "Arial", 40);
    m_labelTips->setPosition( 0, 0 );
    m_labelTips->setAnchorPoint( Vec2(0,0)) ;
    addChild( m_labelTips );

    
    //id
	auto labelID = Label::createWithSystemFont(UTEXT("输入用户ID"), "Arial", 80);
    labelID->setPosition(labelX, 600);
    labelID->setAnchorPoint( Vec2(0,0)) ;
    addChild( labelID );
    //room
	auto labelRoom = Label::createWithSystemFont(UTEXT("输入频道ID"), "Arial", 80);
    labelRoom->setPosition(labelX, 450 );
    labelRoom->setAnchorPoint( Vec2(0,0)) ;
    addChild( labelRoom );
    
    //input
    idInput = cocos2d::extension::EditBox::create( Size( 400, 100 ), "chat_bottom_textfield.png");
    idInput->setTextHorizontalAlignment( TextHAlignment::LEFT );
    idInput->setInputMode( cocos2d::extension::EditBox::InputMode::SINGLE_LINE );
    idInput->setAnchorPoint( Vec2(0,0) );
    idInput->setFontColor( Color3B::BLACK );
    idInput->setPosition( Vec2( labelX + 450,  600 ) );
    idInput->setText( g_userID.c_str() );
    addChild( idInput );
    
    roomInput = cocos2d::extension::EditBox::create( Size( 400, 100 ), "chat_bottom_textfield.png");
    roomInput->setTextHorizontalAlignment( TextHAlignment::LEFT );
    roomInput->setInputMode( cocos2d::extension::EditBox::InputMode::SINGLE_LINE );
    roomInput->setAnchorPoint( Vec2(0,0) );
    roomInput->setFontColor( Color3B::BLACK );
    roomInput->setPosition( Vec2( labelX + 450,  450 ) );
    roomInput->setText( g_roomName.c_str() );
    addChild( roomInput );
    
    //button
    auto btnEnterTeam = Button::create();
    btnEnterTeam->setTitleFontSize( 80 );
    btnEnterTeam->setPosition(Vec2(visibleSize.width/2,280 ));
    btnEnterTeam->setAnchorPoint( Vec2(0.5, 0) );
	btnEnterTeam->setTitleText(UTEXT("小队频道"));
    btnEnterTeam->addClickEventListener( CC_CALLBACK_1( LoginScene::onBtnEnterTeam, this  ) );
    addChild( btnEnterTeam);
    
    auto btnEnterListener = Button::create();
    btnEnterListener->setTitleFontSize( 80 );
	btnEnterListener->setTitleText(UTEXT("主播频道——听众"));
    btnEnterListener->setPosition( Vec2(visibleSize.width/2, 180 ) );
    btnEnterListener->setAnchorPoint( Vec2(0.5, 0) );
    btnEnterListener->addClickEventListener( CC_CALLBACK_1( LoginScene::onBtnEnterListener, this  ) );
    addChild( btnEnterListener );
    
    auto btnEnterHost = Button::create();
    btnEnterHost->setTitleFontSize( 80 );
	btnEnterHost->setTitleText(UTEXT("主播频道——主播"));
    btnEnterHost->setPosition( Vec2(visibleSize.width/2, 80 ) );
    btnEnterHost->setAnchorPoint( Vec2(0.5, 0) );
    btnEnterHost->addClickEventListener( CC_CALLBACK_1( LoginScene::onBtnEnterHost, this  ) );
    addChild( btnEnterHost );
    
}




