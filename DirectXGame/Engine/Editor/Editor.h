#pragma once
#include <variant>
#include <map>
#include <string>
#include "StructManager.h"

class Editor
{
public:
	struct HierarchicalName {
		std::string kGroup;
		std::string kSection;
	};

public:
	static Editor* GetInstance() {
		static Editor instance;
		return &instance;
	}

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

public:
	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const std::string& groupName);

	void CreateSection(const HierarchicalName& names);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const HierarchicalName& names);

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	//void LoadFiles();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName"></param>
	//void LoadFile(const HierarchicalName& groupName);

public:

	/// <summary>
	/// 値のセット(int)
	/// </summary>
	void SetValue(const HierarchicalName& names, const std::string& key, int32_t value);

	/// <summary>
	/// 値のセット(float)
	/// </summary>
	//void SetValue(const std::string& groupName, const std::string& key, float value);

	/// <summary>
	/// 値のセット(Vector2)
	/// </summary>
	//void SetValue(const std::string& groupName, const std::string& key, const Vector2& value);

	/// <summary>
	/// 値のセット(Vector3)
	/// </summary>
	//void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// 項目の追加(int)
	/// </summary>
	void AddItem(const HierarchicalName& names, const std::string& key, int32_t value);

	/// <summary>
	/// 項目の追加(float)
	/// </summary>
	//void AddItem(const std::string& groupName, const std::string& key, float value);

	/// <summary>
	/// 項目の追加(Vector2)
	/// </summary>
	//void AddItem(const std::string& groupName, const std::string& key, const Vector2& value);

	/// <summary>
	/// 項目の追加(Vector3)
	/// </summary>
	//void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);

	// 値の取得
	int32_t GetIntValue(const HierarchicalName& names, const std::string& key);
	//float GetFloatValue(const std::string& groupName, const std::string& key);
	//Vector2 GetVector2Value(const std::string& groupName, const std::string& key);
	//Vector3 GetVector3Value(const std::string& groupName, const std::string& key);



private:
	Editor() = default;
	~Editor() = default;
	Editor(const Editor& variable) = default;
	Editor& operator=(const Editor& variable);

private:
	//// 項目
	using Item = std::variant<int32_t, float, Vector2, Vector3>;
	using Section = std::map<std::string, Item>;
	using Group = std::map<std::string, Section>;

	// 全データ
	std::map<std::string, Group> datas_;

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/Editor/";
	const float kFabsValue_f = 2000.0f;
	const int kFabsValue_i = 1000;

};

