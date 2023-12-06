#pragma once
#include <vector>

/// <summary>
/// 接触記録
/// </summary>
class ContactRecord
{
private:
	// 履歴
	std::vector<uint32_t> historys_;

public:
	// 履歴追加
	void AddHistory(uint32_t number);
	// 履歴チェック
	bool CheckHistory(uint32_t number);
	// 履歴抹消
	void Clear() { historys_.clear(); }
};

