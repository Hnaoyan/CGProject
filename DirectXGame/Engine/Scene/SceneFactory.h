#pragma once

#include "AbstractSceneFactory.h"
#include "SceneList.h"

class SceneFactory : public AbstractSceneFactory
{
public:

	IScene* CreateScene(const std::string& sceneName) override;

};

