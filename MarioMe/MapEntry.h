#pragma once
#include "GameObject.h"
#include "Utils.h"

class MapEntry :
    public CGameObject
{
public:
    string sceneId;
    int nodeId;

    int enter = 0;
    int finished = 0;
    int worldNumber = 0;

    bool isStartPos = false;

    unordered_map<string, int> adjacentNodes;
    unordered_map<string, LPSPRITE> sprites;

    MapEntry(float xPos, float yPos, int nodeID, string sceneID,
        string checkedSprite, string uncheckedSprite,
        string adjList, string adjWeight, int worldNumber, bool isStartPos);

    virtual void Update(DWORD dt) override;
    virtual void InitAnimations() override;
    virtual void Render() override;
    virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;
    void Reset();
    unordered_map<string, int> GetAdjacentNodes();

    int GetNodeId();
    void Enter();

    virtual int GetObjectType();
    static const int ObjectType = 77;
};

