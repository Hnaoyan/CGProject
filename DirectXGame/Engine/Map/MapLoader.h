#pragma once
#include <string>
#include <map>
#include <variant>

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


private:
	// 変数の型管理
	// ステージのデータ
	using Item = std::variant<int**, float>;

	// 管理した変数と文字列を結ぶ
	using Group = std::map<std::string, Item>;

	// 全てのデータ
	std::map<std::string, Group> datas_;

};

