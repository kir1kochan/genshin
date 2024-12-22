#include "StoryManager.h"

StoryManager::StoryManager() {
    storyLabel = nullptr;
}

void StoryManager::startStory(const std::string& storyText) {
    displayStory(storyText);
}

void StoryManager::displayStory(const std::string& text) {
    if (!storyLabel) {
        storyLabel = cocos2d::Label::createWithTTF(text, "fonts/Marker Felt.ttf", 44);
        storyLabel->setPosition(cocos2d::Vec2(5812, 250));  // ������ʾλ��
        this->addChild(storyLabel);
    }
    else {
        storyLabel->setString(text);  // ���¾����ı�
    }
}

StoryManager* StoryManager::create() {
    StoryManager* sm = new StoryManager();
    if (sm) {
        sm->autorelease();
        return sm;
    }
    return nullptr;
}
