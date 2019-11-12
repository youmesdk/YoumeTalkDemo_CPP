#include "AppDelegate.h"
#include "LoginScene.h"

#include "IYouMeVoiceEngine.h"
#include  "YouMeVoiceEngineImp.hpp"

USING_NS_CC;
#include <stdlib.h>
extern string g_roomName;
extern string g_userID;

static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("TalkCppSample", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("TalkCppSample");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
//    if (frameSize.height > mediumResolutionSize.height)
//    {        
//        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
//    }
//    // if the frame's height is larger than the height of small size.
//    else if (frameSize.height > smallResolutionSize.height)
//    {        
//        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
//    }
//    // if the frame's height is smaller than the height of medium size.
//    else
//    {        
//        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
//    }

    register_all_packages();
    
    /**
     *  功能描述:初始化引擎
     *
     *  @param pEventCallback:回调类地址，需要继承IYouMeEventCallback并实现其中的回调函数
     *  @param strAPPKey:在申请SDK注册时得到的App Key，也可凭账号密码到http://gmx.dev.net/createApp.html查询
     *  @param strAPPSecret:在申请SDK注册时得到的App Secret，也可凭账号密码到http://gmx.dev.net/createApp.html查询
     *  @param serverRegionId: 服务器区域(YOUME_RTC_SERVER_REGION)
     *  @param pExtServerRegionName:
     *  @return 错误码，详见YouMeConstDefine.h定义
     */
    YouMeErrorCode errorcode = IYouMeVoiceEngine::getInstance()->init( YouMeVoiceEngineImp::getInstance() ,
                                           "YOUME670584CA1F7BEF370EC7780417B89BFCC4ECBF78",
                                           "yYG7XY8BOVzPQed9T1/jlnWMhxKFmKZvWSFLxhBNe0nR4lbm5OUk3pTAevmxcBn1mXV9Z+gZ3B0Mv/MxZ4QIeDS4sDRRPzC+5OyjuUcSZdP8dLlnRV7bUUm29E2CrOUaALm9xQgK54biquqPuA0ZTszxHuEKI4nkyMtV9sNCNDMBAAE=",
                                                                      RTC_CN_SERVER, "");
    if( YOUME_SUCCESS == errorcode)
    {
        YouMe_Log("调用初始化成功\n");
    }
    else
    {
        YouMe_Log("调用初始化失败，错误码：%d\n",  errorcode);
    }
    
    //初始化一个房间ID和用户ID
    g_roomName = "123";
    //随机生成一个用户ID
    auto curTime = time(0);
    int userID = curTime % 1000;
    stringstream ss;
    ss<<"u"<<userID;
    g_userID = ss.str();



    // create a scene. it's an autorelease object
    auto scene = LoginScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
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


    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    
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

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
