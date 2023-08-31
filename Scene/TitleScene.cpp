#include "TitleScene.h"
#include "TextureManager.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	uint32_t textureTitle = TextureManager::Load("title/startInfo.png");
	title_ = Sprite::Create( textureTitle, { 480 ,480 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.0f,0.0f }, false, false);

	uint32_t texture = TextureManager::Load("title/title.png");
	titleFont_ = Sprite::Create(texture, { 1280 / 2 - 200,720 / 2 - 100 }, { 1,1,1,1 }, { 0,0 }, 0, 0);

	uint32_t tileTexture = TextureManager::Load("ground/ground3.png");
	tiles_[0] = Sprite::Create(tileTexture, { 0,0 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[1] = Sprite::Create(tileTexture, { 200,0 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[2] = Sprite::Create(tileTexture, { 400,0 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[3] = Sprite::Create(tileTexture, { 600,0 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[4] = Sprite::Create(tileTexture, { 800,0 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[5] = Sprite::Create(tileTexture, { 1000,0 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[6] = Sprite::Create(tileTexture, { 1200,0 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[7] = Sprite::Create(tileTexture, { 0,200 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[8] = Sprite::Create(tileTexture, { 200,200 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[9] = Sprite::Create(tileTexture, { 400,200 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[10] = Sprite::Create(tileTexture, { 600,200 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[11] = Sprite::Create(tileTexture, { 800,200 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[12] = Sprite::Create(tileTexture, { 1000,200 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[13] = Sprite::Create(tileTexture, { 1200,200 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[14] = Sprite::Create(tileTexture, { 0,400 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[15] = Sprite::Create(tileTexture, { 200,400 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[16] = Sprite::Create(tileTexture, { 400,400 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[17] = Sprite::Create(tileTexture, { 600,400 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[18] = Sprite::Create(tileTexture, { 800,400 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[19] = Sprite::Create(tileTexture, { 1000,400 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[20] = Sprite::Create(tileTexture, { 1200,400 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[21] = Sprite::Create(tileTexture, { 0,600 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[22] = Sprite::Create(tileTexture, { 200,600 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[23] = Sprite::Create(tileTexture, { 400,600 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[24] = Sprite::Create(tileTexture, { 600,600 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[25] = Sprite::Create(tileTexture, { 800,600 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[26] = Sprite::Create(tileTexture, { 1000,600 }, { 1,1,1,1 }, { 0,0 }, 0, 0);
	tiles_[27] = Sprite::Create(tileTexture, { 1200,600 }, { 1,1,1,1 }, { 0,0 }, 0, 0);

	// 地面
	ground_ = std::make_unique<Ground>();
	groundModel_.reset(Model::CreateFromObj("ground", true));
	ground_->Initialize(groundModel_.get());

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	titleCamera_ = std::make_unique<TitleCamera>();
	titleCamera_->Initialize();

}

void TitleScene::Update()
{
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			sceneNum = GAMESCENE;
		}
	}
	ground_->Update();
	followCamera_->Update();
	titleCamera_->Update();

	//viewProjection_.matView = followCamera_->GetView().matView;
	//viewProjection_.matProjection = followCamera_->GetView().matProjection;
	viewProjection_.matView = titleCamera_->GetView().matView;
	viewProjection_.matProjection = titleCamera_->GetView().matProjection;
	viewProjection_.TransferMatrix();
	
}

void TitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	
	titleFont_->Draw();

	title_->Draw();

	for (int i = 0; i < 28; i++) {
		tiles_[i]->Draw();
	}


	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	ground_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion

}
