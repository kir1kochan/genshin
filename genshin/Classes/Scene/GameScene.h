#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"
#include "Player.h"
#include "InputManager.h"
#include "SpiritManager.h"
#include "BlockManager.h"
#include "TPAnchor.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    ~GameScene();  // �����������ͷ���Դ

    bool init();

    // ������Ϸ״̬
    void update(float deltaTime);

    // ���ص�ͼ
    void loadBackgroundMap();

    // ��ʼ����Ϸ������
    void initGameManagers();

    // ��ʼ�����������
    void setupInputManager();

    // ���� TPAnchors
    void loadTPAnchors();

    CREATE_FUNC(GameScene);

protected:
    // ��Ϸ�����������
    Player* player;               // �����ָ��
    InputManager* inputManager;    // ���������
    SpiritManager* spiritManager;  // ���������
    BlockManager* blockManager;    // ���������
    TPAnchor* tpAnchor;            // ���͵���ָ��
};

#endif // GAMESCENE_H
