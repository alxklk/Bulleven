#pragma once

#include "BaseModel.h"

#include <vector>

class CScene
{
	std::vector<CBaseModel*> models;
public:
	CScene();
	CBaseModel* GetModel(size_t i);
	void AddModel(CBaseModel*);
	~CScene();
};

