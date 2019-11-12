#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"

#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
#include "IYouMeVoiceEngine.h"
USING_NS_CC;

#include <string>
using namespace std;

#include "cocos-ext.h"


using namespace cocos2d::extension;

class EditBox;

class LoginScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void loadUI();
    //注册声音引擎事件
    void addVoiceEvent();
    
    void addTips( string  strTips);
    
    //小队频道
    void onBtnEnterTeam(Ref *pSender );
    //主播频道，听众
    void onBtnEnterListener(Ref * pSender );
    //主播频道,主播
    void onBtnEnterHost(Ref * pSender );
    
    void loginChatRoom( YouMeUserRole roleType );

    cocos2d::extension::EditBox *  idInput;
    cocos2d::extension::EditBox *  roomInput;
    
    bool isKeyValid( std::string );
    
    Label*   m_labelTips;
    
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(LoginScene);
};

#endif // __LOGIN_SCENE_H__
