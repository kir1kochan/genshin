#ifndef STORY_MANAGER_H
#define STORY_MANAGER_H

#include "cocos2d.h"

class StoryManager : public cocos2d::Node {
public:
    StoryManager();
    void startStory(const std::string& storyText);
    void displayStory(const std::string& text);
    static StoryManager* create();

private:
    cocos2d::Label* storyLabel;
};

#endif // STORY_MANAGER_H
