#include "ContactRecord.h"
#include <algorithm>

void ContactRecord::AddHistory(uint32_t number)
{
	historys_.push_back(number);
}

bool ContactRecord::CheckHistory(uint32_t number)
{
	for (uint32_t history : historys_) {
		if (history == number) {
			return true;
		}
	}

	//return std::any_of(historys_.begin(), historys_.end(), [](int num) {return num == number; });

	return false;
}
