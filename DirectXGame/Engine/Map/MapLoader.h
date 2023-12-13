#pragma once
#include <string>
#include <map>

class MapLoader
{
public:

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="groupName"></param>
	void Load(const std::string& groupName);
	/// <summary>
	/// 保存
	/// </summary>
	/// <param name="groupName"></param>
	void Save(const std::string& groupName);

private:
	/// <summary>
	/// マップ情報の保存先ファイルパス
	/// </summary>
	const std::string kDirectoryPath = "Resources/MapData/";

};

