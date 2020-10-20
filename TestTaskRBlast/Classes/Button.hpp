#ifndef Button_hpp
#define Button_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "Defines.h"

USING_NS_CC;

#define kExpandZone 20
#define kSafeZone   40

class Button : public ui::Widget
{
public:
    
    enum State
    {
        IDLE = 0,
        PUSHED,
        DRAGOUT,
        LONGPUSH
    };
        
    static Button* create(Rect content, Rect expand, Rect safe);
    
    void setContentZone(Rect content);
    void setExpandZone(Rect expand);
    void setSafeZone(Rect safe);
    
    void setAction(std::function<void()> callback);
    
    void addItem(Node* item, State state);
    
    ~Button();
    
    State getState();
    
protected:
    
    bool init(Rect content, Rect expand, Rect safe);
    
    virtual void setState(State state);
    State _state;

    std::function<void()> _callback;
    void sendEvent();
    
    Vector <Node*> items;
    
    Rect _content;
    Rect _expand;
    Rect _safe;
    
    bool eligibleTouch;
    
    bool touchInContentZone(Touch *pTouch);
    bool touchInExpandZone(Touch *pTouch);
    bool touchInSafeZone(Touch *pTouch);
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *pTouch, Event *pEvent);
};

#endif /* Button_hpp */
