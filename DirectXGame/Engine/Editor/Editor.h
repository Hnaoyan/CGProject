#pragma once
#include <variant>
#include <map>
#include <string>
#include "StructManager.h"

class Editor
{
public: // サブクラス
	struct HierarchicalName {
		std::string kGroup;
		std::string kSection;
	};
private:
	Editor() = default;
	~Editor() = default;
	Editor(const Editor& variable) = default;
	Editor& operator=(const Editor& variable);

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
	/// ヒエラルキーで作成
	/// </summary>
	/// <param name="names"></param>
	void CreateHierarchy(const HierarchicalName& names);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const HierarchicalName& names);

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName"></param>
	void LoadFile(const std::string& groupName);

public: // アクセッサ
#pragma region 設定
	/// <summary>
	/// 値のセット(int)
	/// </summary>
	void SetValue(const HierarchicalName& names, const std::string& key, int32_t value);

	/// <summary>
	/// 値のセット(float)
	/// </summary>
	void SetValue(const HierarchicalName& names, const std::string& key, float value);

	/// <summary>
	/// 値のセット(Vector2)
	/// </summary>
	void SetValue(const HierarchicalName& names, const std::string& key, const Vector2& value);

	/// <summary>
	/// 値のセット(Vector3)
	/// </summary>
	void SetValue(const HierarchicalName& names, const std::string& key, const Vector3& value);
#pragma endregion

#pragma region 追加
	/// <summary>
	/// 項目の追加(int)
	/// </summary>
	void AddItem(const HierarchicalName& names, const std::string& key, int32_t value);

	/// <summary>
	/// 項目の追加(float)
	/// </summary>
	void AddItem(const HierarchicalName& names, const std::string& key, float value);

	/// <summary>
	/// 項目の追加(Vector2)
	/// </summary>
	void AddItem(const HierarchicalName& names, const std::string& key, const Vector2& value);

	/// <summary>
	/// 項目の追加(Vector3)
	/// </summary>
	void AddItem(const HierarchicalName& names, const std::string& key, const Vector3& value);
#pragma endregion

#pragma region 取得
	/// <summary>
	/// 値の取得(int)
	/// </summary>
	int32_t GetIntValue(const HierarchicalName& names, const std::string& key);
	/// <summary>
	/// 値の取得(float)
	/// </summary>
	float GetFloatValue(const HierarchicalName& names, const std::string& key);
	/// <summary>
	/// 値の取得(Vector2)
	/// </summary>
	Vector2 GetVector2Value(const HierarchicalName& names, const std::string& key);
	/// <summary>
	/// 値の取得(Vector3)
	/// </summary>
	Vector3 GetVector3Value(const HierarchicalName& names, const std::string& key);
#pragma endregion

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

