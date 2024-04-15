#pragma once
#include "Model.h"

class ObjectLoader 
{
	// .mtlファイルの読み取り
	static Model::MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName);

	// .objファイルの読み取り
	static Model::ModelData LoadObjFile(const std::string& directoryPath, const std::string& fileName);

};
