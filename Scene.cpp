#include "Scene.h"


CScene::CScene()
{
}


CScene::~CScene()
{
}

CBaseModel* CScene::GetModel(size_t i)
{
	if(i>=models.size())
		return nullptr;
	return models[i];
}

void CScene::AddModel(CBaseModel* model)
{
	models.push_back(model);
}

