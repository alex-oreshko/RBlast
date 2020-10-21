#ifndef Button_hpp
#define Button_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "Defines.h"

USING_NS_CC;

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
    
    enum Zone
    {
        CONTENT = 0,
        EXPAND,
        SAFE
    };
        
    static Button* create(Rect content, Rect expand, Rect safe);
    
    void setZone(Rect rect, Zone zone);
    Rect getZone(Zone zone);

    void setAction(std::function<void()> callback);
    
    void addItem(Node* item, State state);
    
    ~Button();
    
    State getState();
    
protected:
    
    bool initWithZoneSizes(Rect content, Rect expand, Rect safe);
    
    virtual void setState(State state);
    State _state;

    std::function<void()> _callback;
    void sendEvent();
    
    Vector <Node*> items;
    
    Rect _content;
    Rect _expand;
    Rect _safe;
    
    bool touchInZone(Touch *pTouch, Zone zone);
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *pTouch, Event *pEvent);
};

#endif /* Button_hpp */
