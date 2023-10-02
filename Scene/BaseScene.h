#pragma once

enum Scene {
	TITLE,GAMESCENE,CLEAR,
};

class BaseScene 
{
public:

	virtual void Initialize(){};
	virtual void Update(){};
	virtual void Draw(){};

	virtual ~BaseScene() = default;

	int GetSceneNum() { return sceneNum; }

protected:
	static int sceneNum;

};
