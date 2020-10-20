#ifndef LongPushButton_hpp
#define LongPushButton_hpp

#include "Button.hpp"
#include "Defines.h"


class LongPushButton : public Button
{
public:
    
    static LongPushButton* create(Rect content, Rect expand, Rect safe);
    
    void setTimerInterval(float seconds);
    
    void setLongPushAction(std::function<void()> callback);

    ~LongPushButton() {LOG("~LongPushButton\n");}
    
protected:
    
    bool init(Rect content, Rect expand, Rect safe);
    
    void setState(State state);
    
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    std::function<void()> _longPushCallback;

private:
    
    float interval;
    float timer;
    bool timerRunning;
    void onTimer(float dt);
    
    void startTimer();
    void stopTimer();
    
    DrawNode* drawNode;

};

#endif /* LongPushButton_hpp */
