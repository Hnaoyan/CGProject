#include "UIManager.h"

UIManager* UIManager::GetInstance()
{
	static UIManager instance;
	return &instance;
}

void UIManager::Draw()
{
	for (UIBase* ui : uiSprites_) {
		ui->Draw();
	}
}

void UIManager::AddUI(int texHandle, const Vector2& position, const Vector2& anchor, const std::string& name)
{
	UIBase* newUi = new UIBase(name);
	newUi->SetPosition(position);
	newUi->SetAnchor(anchor);
	newUi->Initialize(texHandle);
	uiSprites_.push_back(newUi);
}

void UIManager::DeleteUI(const std::string& name)
{
	for (auto it = uiSprites_.begin(); it != uiSprites_.end(); ++it) {
		if ((*it)->GetTagName() == name) {
			delete *it;
			it = uiSprites_.erase(it);
		}
	}
}

UIBase* UIManager::GetUI(const std::string& name)
{
	for (auto ui : uiSprites_) {
		if (ui->GetTagName() == name) {
			return ui;
		}
	}
	return nullptr;
}
