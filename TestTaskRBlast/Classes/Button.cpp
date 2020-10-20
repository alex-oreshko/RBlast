#include "Button.hpp"
#include "Defines.h"

Button* Button::create(Rect content, Rect expand, Rect safe)
{
    Button *button = new (std::nothrow) Button;
    if (button && button->init(content, expand, safe))
    {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return nullptr;
}

bool Button::init(Rect content, Rect expand, Rect safe)
{
    // invoke an overridden init() at first
    if (!Widget::init()) {
        return false;
    }
    
    setContentZone(content);
    setExpandZone(expand);
    setSafeZone(safe);
    
    _state = State::IDLE;
        
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = CC_CALLBACK_2(Button::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Button::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Button::onTouchEnded, this);

    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
    return true;
}

Button::~Button()
{
    for (Vector<Node*>::iterator it = items.begin() ; it != items.end(); ++it)
    {
        Node* node = *it;
        int* states = (int*)node->getUserData();
        free(states);
    }
    items.clear();
}

void Button::setContentZone(Rect content)
{
    _content = content;
    
    #if defined(DEBUG)
    
        DrawNode* drawNode = DrawNode::create();
        drawNode->drawRect(content.origin, content.origin + content.size, Color4F(1, 1, 1, 1));
        addChild(drawNode);
        
    #endif
}

void Button::setExpandZone(Rect expand)
{
    _expand = expand;
    
    #if defined(DEBUG)
    
        DrawNode* drawNode = DrawNode::create();
        drawNode->drawRect(expand.origin, expand.origin + expand.size, Color4F(1, 1, 1, 1));
        addChild(drawNode);
        
    #endif
}

void Button::setSafeZone(Rect safe)
{
    _safe = safe;
    
    #if defined(DEBUG)
    
        DrawNode* drawNode = DrawNode::create();
        drawNode->drawRect(safe.origin, safe.origin + safe.size, Color4F(1, 1, 1, 1));
        addChild(drawNode);
        
    #endif

}

void Button::setState(State state)
{
    if (state == _state)
        return;
    
    _state = state;
    
    for (Vector<Node*>::iterator it = items.begin() ; it != items.end(); ++it)
    {
        Node* node = *it;
        int* states = (int*)node->getUserData();
        node->setVisible(*(states + state) == 1);
    }
}

void Button::addItem(Node* item, State state)
{
    int *states;
    if (!items.contains(item))
    {
        items.pushBack(item);
        int* states = (int*)malloc(sizeof(int) * (LONGPUSH + 1));
        for (int i = 0; i <= LONGPUSH; i++)
            *(states + i) = (i == state)?1:0;
        item->setUserData(states);
        item->setVisible(state == _state);
        addChild(item);
    }
    else
    {
        states = (int*)item->getUserData();
        *(states + state) = 1;
    }
}

Button::State Button::getState()
{
    return _state;
}

void Button::setAction(std::function<void()> callback)
{
    _callback = callback;
}

void Button::sendEvent()
{
    Button* button = this;
    
    char name[128];
    if (_state == IDLE)
        sprintf(name, "%s", "button: idle");
    else if (_state == DRAGOUT)
        sprintf(name, "%s", "button: dragout");
    else if (_state == PUSHED)
        sprintf(name, "%s", "button: push");
    else if (_state == LONGPUSH)
        sprintf(name, "%s", "button: long push");
        
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    
    EventCustom event(name);
    event.setUserData(button);

    dispatcher->dispatchEvent(&event);
}


bool Button::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (touchInExpandZone(pTouch))
    {
        eligibleTouch = true;
        setState(State::PUSHED);
    }
    else
        eligibleTouch = false;
    return true;
}

void Button::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if (touchInExpandZone(pTouch) && eligibleTouch)
    {
        setState(State::PUSHED);
    }
    else if (touchInSafeZone(pTouch) && eligibleTouch)
    {
        setState(State::DRAGOUT);
    }
    else
    {
        eligibleTouch = false;
        setState(State::IDLE);
    }
        
}

void Button::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (_state == PUSHED)
    {
        sendEvent();
        if (_callback)
            _callback();
    }
    setState(State::IDLE);
}

void Button::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    setState(State::IDLE);
}

bool Button::touchInContentZone(Touch *pTouch)
{
    Point p = convertTouchToNodeSpace(pTouch);
    Rect r(_content.origin, _content.size);
    return r.containsPoint(p);
}

bool Button::touchInExpandZone(Touch *pTouch)
{
    Point p = convertTouchToNodeSpace(pTouch);
    Rect r(_expand.origin, _expand.size);
    return r.containsPoint(p);
}

bool Button::touchInSafeZone(Touch *pTouch)
{
    Point p = convertTouchToNodeSpace(pTouch);
    Rect r(_safe.origin, _safe.size);
    return r.containsPoint(p);
}
