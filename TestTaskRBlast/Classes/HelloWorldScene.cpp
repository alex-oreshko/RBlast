/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "Defines.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    setupScane();
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

void HelloWorld::setupScane()
{
    square = Sprite::create("square.png");
    square->setPosition(Point(260, 400));
    addChild(square);
    
    auto idle = Sprite::create("idle.png");
    auto pushed = Sprite::create("pushed.png");
    auto dragout = Sprite::create("dragout.png");
    
    Rect content = Rect(idle->getContentSize() / -2, idle->getContentSize());
    Rect expand = Rect(content.origin - Point(20, 20), content.size + Size(40, 40));
    Rect safe = Rect(content.origin - Point(40, 40), content.size + Size(80, 80));
    
    button = Button::create(content, expand, safe);
    button->setPosition(Vec2(150, 150));
    button->addItem(idle, Button::State::IDLE);
    button->addItem(pushed, Button::State::PUSHED);
    button->addItem(dragout, Button::State::DRAGOUT);
    addChild(button);
    
    button->setAction([&](){
        auto big = ScaleTo::create(0.1f, 1.2f);
        auto small = ScaleTo::create(0.1f, 1.0f);
        auto action = Sequence::createWithTwoActions(big, small);
        square->runAction(action);
        auto rotate = RotateBy::create(0.1f, rand() % 720 - 360);
        auto scale = ScaleTo::create(0.1, 0.8 + (rand() % 40) / 100.0, 0.8 + (rand() % 40) / 100.0);
        auto spawn = Spawn::createWithTwoActions(rotate, scale);
        button->runAction(spawn);
    });
    
    auto idleAndPushed = Label::createWithSystemFont("idle or pushed", "Tahoma", 15);
    idleAndPushed->setPosition(Point(10, 80));
    button->addItem(idleAndPushed, Button::State::IDLE);
    button->addItem(idleAndPushed, Button::State::PUSHED);
    
    auto pushedAndDragout = Label::createWithSystemFont("pushed or dragout", "Tahoma", 15);
    pushedAndDragout->setPosition(Point(10, 100));
    button->addItem(pushedAndDragout, Button::State::DRAGOUT);
    button->addItem(pushedAndDragout, Button::State::PUSHED);
    
    auto dragoutAndIdle = Label::createWithSystemFont("dragou or idle", "Tahoma", 15);
    dragoutAndIdle->setPosition(Point(10, 120));
    button->addItem(dragoutAndIdle, Button::State::IDLE);
    button->addItem(dragoutAndIdle, Button::State::DRAGOUT);

    
    button->setRotation(10);
    
    // long push button
    
    Sprite* lpbImage = Sprite::create("lpb.png");
    content = Rect(lpbImage->getContentSize() / -2, lpbImage->getContentSize());
    expand = Rect(content.origin - Point(20, 20), content.size + Size(40, 40));
    safe = Rect(content.origin - Point(40, 40), content.size + Size(80, 80));
    
    longPushButton = LongPushButton::create(content, expand, safe);
    longPushButton->setPosition(Vec2(150, 350));
    longPushButton->addItem(lpbImage, Button::IDLE);
    longPushButton->addItem(lpbImage, Button::PUSHED);
    longPushButton->addItem(lpbImage, Button::DRAGOUT);
    
    Sprite* lpbImage2 = Sprite::create("lpb2.png");
    longPushButton->addItem(lpbImage2, Button::LONGPUSH);
    
    addChild(longPushButton);
    
    longPushButton->setAction([&](){
        auto rotate = RotateBy::create(0.1f, 360);
        square->runAction(rotate);
        
        Rect expand = longPushButton->getZone(Button::EXPAND);
        Rect safe = longPushButton->getZone(Button::SAFE);
        expand.origin = expand.origin - Vec2(1, 1);
        expand.size = expand.size + Size(2, 2);
        safe.origin = safe.origin - Vec2(2, 2);
        safe.size = safe.size + Size(4, 4);
        longPushButton->setZone(expand, Button::EXPAND);
        longPushButton->setZone(safe, Button::SAFE);
    });
    
    longPushButton->setLongPushAction([&](){
        auto rotateSelf = RotateBy::create(0.1f, rand() % 720 - 360);
        auto scale = ScaleTo::create(0.1, 0.8 + (rand() % 40) / 100.0, 0.8 + (rand() % 40) / 100.0);
        auto spawn = Spawn::createWithTwoActions(rotateSelf, scale);
        longPushButton->runAction(spawn);
    });
    

}
