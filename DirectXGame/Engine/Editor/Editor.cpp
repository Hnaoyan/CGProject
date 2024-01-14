#include "Editor.h"
#include "ImGuiManager.h"
#include <json.hpp>
#include <fstream>

using namespace nlohmann;

void Editor::Update()
{
	if (!ImGui::Begin("Editors")) {
		ImGui::End();
		return;
	}
	//if (!ImGui::BeginMenuBar())
	//	return;
	// グループ区画
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin();
		itGroup != datas_.end(); ++itGroup) {
		// 名を取得
		const std::string& groupName = itGroup->first + std::to_string(kLoadMapNumber_);
		// 参照を取得
		Group& group = itGroup->second;

		//if (!ImGui::BeginMenu(groupName.c_str()))
		//	continue;

		if (ImGui::CollapsingHeader(groupName.c_str())) {
			// セクション区画
			for (std::map<std::string, Section>::iterator itSection = group.begin();
				itSection != group.end(); ++itSection) {
				// 名を取得
				const std::string& sectionName = itSection->first;
				// 参照を取得
				Section& section = itSection->second;

				//if (!ImGui::BeginMenu(sectionName.c_str()))
				//	continue;

				std::string fullPath = groupName + ":" + sectionName;
				if (ImGui::TreeNode(fullPath.c_str())) {

					// アイテム区画
					for (std::map<std::string, Item>::iterator itItem = section.begin();
						itItem != section.end(); ++itItem) {
						// 項目名を取得
						const std::string& itemName = itItem->first;
						// 項目の参照を取得
						Item& item = itItem->second;
						// int32_t型の値を保持していれば
						if (std::holds_alternative<int32_t>(item)) {
							int32_t* ptr = std::get_if<int32_t>(&item);
							ImGui::DragInt(itemName.c_str(), ptr, 0.1f, -kFabsValue_i, kFabsValue_i);
						}
						else if (std::holds_alternative<float>(item)) {
							float* ptr = std::get_if<float>(&item);
							ImGui::DragFloat(itemName.c_str(), ptr, 0.01f, -kFabsValue_f, kFabsValue_f);
						}
						else if (std::holds_alternative<Vector2>(item)) {
							Vector2* ptr = std::get_if<Vector2>(&item);
							ImGui::DragFloat2(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.1f, -kFabsValue_f, kFabsValue_f);
						}
						else if (std::holds_alternative<Vector3>(item)) {
							Vector3* ptr = std::get_if<Vector3>(&item);
							ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.1f, -kFabsValue_f, kFabsValue_f);
						}
					}


					// 改行
					ImGui::Text("\n");

					HierarchicalName prevName = { groupName,sectionName };

					if (ImGui::Button("Save")) {
						SaveFile(prevName);
						std::string message = std::format("{}.json saved.", groupName);
						MessageBoxA(nullptr, message.c_str(), "Editors", 0);
					}
					ImGui::TreePop();
				}
				//ImGui::EndMenu();
			}
			//ImGui::EndMenu();
		}

	}
	//ImGui::EndMenuBar();
	ImGui::End();
}

void Editor::CreateHierarchy(const HierarchicalName& names)
{
	// なければ追加
	datas_[names.kGroup][names.kSection];
}

void Editor::SaveFile(const HierarchicalName& names)
{
	// グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(names.kGroup);

	const std::string& groupName = itGroup->first;

	// 未登録チェック
	assert(itGroup != datas_.end());

	json root;
	root = json::object();

	// jsonオブジェクト登録
	root[groupName] = json::object();

	// 各項目について
	for (std::map<std::string, Section>::iterator itSection = itGroup->second.begin();
		itSection != itGroup->second.end(); ++itSection) {

		// 項目名を取得
		const std::string& sectionName = itSection->first;
		// 項目の参照を取得
		//Section& section = itSection->second;

		root[groupName][sectionName];

		for (std::map<std::string, Item>::iterator itItem = itSection->second.begin();
			itItem != itSection->second.end(); ++itItem) {
			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目の参照を取得
			Item& item = itItem->second;

			// int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item)) {
				// int32_t型の値を登録
				root[groupName][sectionName][itemName] = std::get<int32_t>(item);
			}
			else if (std::holds_alternative<float>(item)) {
				// float型の値を登録
				root[groupName][sectionName][itemName] = std::get<float>(item);
			}
			else if (std::holds_alternative<Vector2>(item)) {
				// float型のjson配列登録
				Vector2 value = std::get<Vector2>(item);
				root[groupName][sectionName][itemName] = json::array({ value.x, value.y });
			}
			else if (std::holds_alternative<Vector3>(item)) {
				// float型のjson配列登録
				Vector3 value = std::get<Vector3>(item);
				root[groupName][sectionName][itemName] = json::array({ value.x, value.y, value.z });
			}
		}
	}
	// ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::filesystem::create_directories(kDirectoryPath);
	}
	// 書き込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込み用に開く
	ofs.open(filePath);

	// ファイルオープン失敗？
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "Editors", 0);
		assert(0);
		return;
	}
	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	// ファイルを閉じる
	ofs.close();

}

void Editor::LoadFiles()
{
	std::string saveDirectryPath = kDirectoryPath;
	// ディレクトリがなければスキップする
	if (!std::filesystem::exists(saveDirectryPath)) {
		return;
	}
	std::filesystem::directory_iterator dir_it(saveDirectryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		// ファイル読み込み
		LoadFile(filePath.stem().string());
	}

}

void Editor::LoadFile(const std::string& groupName)
{
	// 読み込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に開く
	ifs.open(filePath);
	// ファイルオープン失敗
	if (!std::filesystem::exists(filePath)) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
	}
	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	while (1) {
		std::string newGroupName = groupName + std::to_string(kLoadMapNumber_);

		// グループを検索
		json::iterator itGroup = root.find(newGroupName);

		// 未登録チェック
		//assert(itGroup != root.end());
		if (itGroup == root.end()) {
			break;
		}
		else {
			kLoadMapNumber_++;
		}

		// セクション区画
		for (json::iterator itSection = itGroup->begin(); itSection != itGroup->end(); ++itSection) {
			// 名を取得
			const std::string& sectionName = itSection.key();

			// アイテム区画
			for (json::iterator itItem = itSection->begin();
				itItem != itSection->end(); ++itItem) {
				// アイテム名を取得
				const std::string& itemName = itItem.key();

				HierarchicalName prevNames = { groupName ,sectionName };

				// int32_t型
				if (itItem->is_number_integer()) {
					// int型の値を登録
					int32_t value = itItem->get<int32_t>();
					SetValue(prevNames, itemName, value);
				}
				// float型
				else if (itItem->is_number_float()) {
					// int型の値を登録
					double value = itItem->get<double>();
					SetValue(prevNames, itemName, static_cast<float>(value));
				}
				// 要素数が2の配列であれば
				else if (itItem->is_array() && itItem->size() == 2) {
					// float型のjson配列登録
					Vector2 value = { itItem->at(0), itItem->at(1) };
					SetValue(prevNames, itemName, value);
				}
				// 要素数が3の配列であれば
				else if (itItem->is_array() && itItem->size() == 3) {
					// float型のjson配列登録
					Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
					SetValue(prevNames, itemName, value);
				}

			}
		}

	}
}

void Editor::SetValue(const HierarchicalName& names, const std::string& key, int32_t value)
{
	// グループの参照を取得
	Section& section = datas_[names.kGroup][names.kSection];
	// 新しい項目のデータを設定
	Item newItem = value;
	// 設定した項目をstd::mapに追加
	section[key] = newItem;
}

void Editor::SetValue(const HierarchicalName& names, const std::string& key, float value)
{
	// グループの参照を取得
	Section& section = datas_[names.kGroup][names.kSection];
	// 新しい項目のデータを設定
	Item newItem = value;
	// 設定した項目をstd::mapに追加
	section[key] = newItem;
}

void Editor::SetValue(const HierarchicalName& names, const std::string& key, const Vector2& value)
{
	// グループの参照を取得
	Section& section = datas_[names.kGroup][names.kSection];
	// 新しい項目のデータを設定
	Item newItem = value;
	// 設定した項目をstd::mapに追加
	section[key] = newItem;
}

void Editor::SetValue(const HierarchicalName& names, const std::string& key, const Vector3& value)
{
	// グループの参照を取得
	Section& section = datas_[names.kGroup][names.kSection];
	// 新しい項目のデータを設定
	Item newItem = value;
	// 設定した項目をstd::mapに追加
	section[key] = newItem;
}

void Editor::SetValue(const HierarchicalName& names, const std::string& key, const EnemyInfo& value)
{
	// グループの参照を取得
	Section& section = datas_[names.kGroup][names.kSection];
	// 新しい項目のデータを設定
	Item newItem = value;
	// 設定した項目をstd::mapに追加
	section[key] = newItem;
}

void Editor::AddItem(const HierarchicalName& names, const std::string& key, int32_t value)
{
	// 項目が未登録なら
	if (datas_[names.kGroup][names.kSection].find(key) ==
		datas_[names.kGroup][names.kSection].end()) {
		SetValue(names, key, value);
	}
}

void Editor::AddItem(const HierarchicalName& names, const std::string& key, float value)
{
	// 項目が未登録なら
	if (datas_[names.kGroup][names.kSection].find(key) ==
		datas_[names.kGroup][names.kSection].end()) {
		SetValue(names, key, value);
	}
}

void Editor::AddItem(const HierarchicalName& names, const std::string& key, const Vector2& value)
{
	// 項目が未登録なら
	if (datas_[names.kGroup][names.kSection].find(key) ==
		datas_[names.kGroup][names.kSection].end()) {
		SetValue(names, key, value);
	}
}

void Editor::AddItem(const HierarchicalName& names, const std::string& key, const Vector3& value)
{
	// 項目が未登録なら
	if (datas_[names.kGroup][names.kSection].find(key) ==
		datas_[names.kGroup][names.kSection].end()) {
		SetValue(names, key, value);
	}
}

void Editor::AddItem(const HierarchicalName& names, const std::string& key, const EnemyInfo& value)
{
	// 項目が未登録なら
	if (datas_[names.kGroup][names.kSection].find(key) ==
		datas_[names.kGroup][names.kSection].end()) {
		SetValue(names, key, value);
	}
}

int32_t Editor::GetIntValue(const HierarchicalName& names, const std::string& key)
{
	// 指定グループが存在するか
	assert(datas_.find(names.kGroup) != datas_.end());
	// セクション探し
	assert(datas_[names.kGroup].find(names.kSection) != datas_[names.kGroup].end());
	// セクションの参照を取得
	Section& section = datas_[names.kGroup][names.kSection];

	// 指定グループに指定キーが存在するか
	assert(section.find(key) != section.end());
	// 指定グループから指定のキーの値を取得
	return std::get<0>(section[key]);
}

float Editor::GetFloatValue(const HierarchicalName& names, const std::string& key)
{
	// 指定グループが存在するか
	assert(datas_.find(names.kGroup) != datas_.end());
	// セクション探し
	assert(datas_[names.kGroup].find(names.kSection) != datas_[names.kGroup].end());
	// セクションの参照を取得
	Section& section = datas_[names.kGroup][names.kSection];

	// 指定グループに指定キーが存在するか
	assert(section.find(key) != section.end());
	// 指定グループから指定のキーの値を取得
	return std::get<1>(section[key]);
}

Vector2 Editor::GetVector2Value(const HierarchicalName& names, const std::string& key)
{
	// 指定グループが存在するか
	assert(datas_.find(names.kGroup) != datas_.end());
	// セクション探し
	assert(datas_[names.kGroup].find(names.kSection) != datas_[names.kGroup].end());
	// セクションの参照を取得
	Section& section = datas_[names.kGroup][names.kSection];

	// 指定グループに指定キーが存在するか
	assert(section.find(key) != section.end());
	// 指定グループから指定のキーの値を取得
	return std::get<2>(section[key]);
}

Vector3 Editor::GetVector3Value(const HierarchicalName& names, const std::string& key)
{
	// 指定グループが存在するか
	assert(datas_.find(names.kGroup) != datas_.end());
	// セクション探し
	assert(datas_[names.kGroup].find(names.kSection) != datas_[names.kGroup].end());
	// セクションの参照を取得
	Section& section = datas_[names.kGroup][names.kSection];

	// 指定グループに指定キーが存在するか
	assert(section.find(key) != section.end());
	// 指定グループから指定のキーの値を取得
	return std::get<3>(section[key]);
}

EnemyInfo Editor::GetEnemyInfoValue(const HierarchicalName& names, const std::string& key)
{
	// 指定グループが存在するか
	assert(datas_.find(names.kGroup) != datas_.end());
	// セクション探し
	assert(datas_[names.kGroup].find(names.kSection) != datas_[names.kGroup].end());
	// セクションの参照を取得
	Section& section = datas_[names.kGroup][names.kSection];

	// 指定グループに指定キーが存在するか
	assert(section.find(key) != section.end());
	// 指定グループから指定のキーの値を取得
	return std::get<4>(section[key]);
}
