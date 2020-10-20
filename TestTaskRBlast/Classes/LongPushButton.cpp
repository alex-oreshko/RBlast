
#include "LongPushButton.hpp"
#include "Defines.h"

LongPushButton* LongPushButton::create(Rect content, Rect expand, Rect safe)
{
    LongPushButton *button = new (std::nothrow) LongPushButton;
    if (button && button->init(content, expand, safe))
    {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return nullptr;
}

bool LongPushButton::init(Rect content, Rect expand, Rect safe)
{
    if (Button::init(content, expand, safe))
    {
        drawNode = DrawNode::create();
        addChild(drawNode, 1);
        setTimerInterval(2.5);
        return true;
    }
    else
        return false;
}

void LongPushButton::setLongPushAction(std::function<void()> callback)
{
    _longPushCallback = callback;
}

void LongPushButton::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if (touchInExpandZone(pTouch) && eligibleTouch)
    {
        if (_state == LONGPUSH)
            return;
        setState(State::PUSHED);
    }
    else if (touchInSafeZone(pTouch) && eligibleTouch)
    {
        if (_state == LONGPUSH)
            return;
        setState(State::DRAGOUT);
    }
    else
    {
        eligibleTouch = false;
        setState(State::IDLE);
    }
        
}

void LongPushButton::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (_state == LONGPUSH)
    {
        sendEvent();
        if (_longPushCallback)
            _longPushCallback();
    }
    Button::onTouchEnded(pTouch, pEvent);
}

void LongPushButton::setTimerInterval(float seconds)
{
    interval = seconds;
}

void LongPushButton::startTimer()
{
    timerRunning = true;
    timer = 0;
    this->schedule(schedule_selector(LongPushButton::onTimer));

    drawNode->clear();
}

void LongPushButton::stopTimer()
{
    timerRunning = false;
    this->unschedule(schedule_selector(LongPushButton::onTimer));
    
    drawNode->clear();
}

void LongPushButton::onTimer(float dt)
{
    timer += dt;
    
    float progress = timer / interval;
    
    drawNode->drawLine(_content.origin, _content.origin + _content.size  * progress, Color4F(1, 1, 1, 1));
    
    if (timer >= interval)
        setState(LONGPUSH);
}

void LongPushButton::setState(State state)
{
    Button::setState(state);
   
    if (state == LONGPUSH)
    {
        stopTimer();
    }
    else if (state == IDLE)
    {
        if (timerRunning)
            stopTimer();
    }
    else if (state == PUSHED)
    {
        if (!timerRunning)
            startTimer();
    }
    
}




