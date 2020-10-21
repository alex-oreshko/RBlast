#include "Button.hpp"
#include "Defines.h"

Button* Button::create(Rect content, Rect expand, Rect safe)
{
    Button *button = new (std::nothrow) Button;
    if (button && button->initWithZoneSizes(content, expand, safe))
    {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return nullptr;
}

bool Button::initWithZoneSizes(Rect content, Rect expand, Rect safe)
{
    // invoke an overridden init() at first
    if (!Widget::init()) {
        return false;
    }
    
    setZone(content, CONTENT);
    setZone(expand, EXPAND);
    setZone(safe, SAFE);
        
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
    LOG("~Button\n");
    for (auto node : items)
    {
        char* states = (char*)node->getUserData();
        free(states);
    }
    items.clear();
}

void Button::setZone(Rect rect, Zone zone)
{
    
    if (zone == CONTENT)
        _content = rect;
    else if (zone == EXPAND)
        _expand = rect;
    else if (zone == SAFE)
        _safe = rect;
    
#if defined(DEBUG)
    for (auto child : getChildren())
        if (child->getTag() == zone + 1)
        {
            removeChild(child);
            break;
        }

    DrawNode* drawNode = DrawNode::create();
    drawNode->setTag(zone + 1);
    drawNode->drawRect(rect.origin, rect.origin + rect.size, Color4F(1, 1, 1, 1));
    addChild(drawNode);
#endif
}

Rect Button::getZone(Zone zone)
{
    Rect rect;
    if (zone == CONTENT)
        rect = _content;
    else if (zone == EXPAND)
        rect = _expand;
    else if (zone == SAFE)
        rect = _safe;
    return rect;
}

void Button::setState(State state)
{
    if (state == _state)
        return;
    
    _state = state;
    
    for (auto node : items)
    {
        char* states = (char*)node->getUserData();
        node->setVisible(*(states + state) == 1);
    }
}

void Button::addItem(Node* item, State state)
{
    if (!items.contains(item))
    {
        items.pushBack(item);
        char* states = (char*)calloc(sizeof(char), (LONGPUSH + 1));
        *(states + state) = 1;
        item->setUserData(states);
        item->setVisible(state == _state);
        addChild(item);
    }
    else
    {
        char *states = (char*)item->getUserData();
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
        
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    
    std::string name;
    if (_state == IDLE)
        name = "button: idle";
    else if (_state == DRAGOUT)
        name = "button: dragout";
    else if (_state == PUSHED)
        name = "button: push";
    else if (_state == LONGPUSH)
        name = "button: long push";
    
    EventCustom event(name);
    event.setUserData(button);

    dispatcher->dispatchEvent(&event);
}

bool Button::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (touchInZone(pTouch, EXPAND))
    {
        setState(State::PUSHED);
        return true;
    }
    else
        return false;
}

void Button::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if (touchInZone(pTouch, EXPAND) && _state != IDLE)
        setState(State::PUSHED);
    else if (touchInZone(pTouch, SAFE) && _state != IDLE)
        setState(State::DRAGOUT);
    else
        setState(State::IDLE);        
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

bool Button::touchInZone(Touch *pTouch, Zone zone)
{
    Point p = convertTouchToNodeSpace(pTouch);
    Rect rect;
    if (zone == CONTENT)
        rect = _content;
    else if (zone == EXPAND)
        rect = _expand;
    else if (zone == SAFE)
        rect = _safe;
    return rect.containsPoint(p);
}

