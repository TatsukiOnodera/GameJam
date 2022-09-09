#include "GamePlayScene.h"
#include "SceneManager.h"
#include <time.h>

#include <SafeDelete.h>
#include <cassert>
//#include <fbxsdk.h>

using namespace DirectX;

GamePlayScene::~GamePlayScene()
{
	
}

void GamePlayScene::Initialize()
{
	dx_cmd = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	//audio = Audio::GetInstance();
	camera = Camera::GetInstance();

	// スプライトテクスチャ読み込み
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");

	// ライト生成
	light.reset(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({ -5, -5, 0, 0 });
	Object3d::SetLight(light.get());

	// 前景スプライト
	debugText.Initialize(fontNumber);

	// パーティクル
	playerWalkEffect.reset(ParticleManager::Create("star.png"));
	playerJumpEffect.reset(ParticleManager::Create("stareffect_02.png"));

	// スプライト
	player.reset(Object3d::Create("Player"));
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			block[y][x].reset(new BLOCK(Object3d::Create("Block"), false));
		}
	}

	// OBJオブジェクト


	// FBXオブェクト


	// オーディオ
	//audio->Initialize();

	// 変数の初期化
	InitializeVariable();
}

void GamePlayScene::InitializeVariable()
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			if (y == 0)
			{
				block[y][x]->map = true;
			}
			else
			{
				block[y][x]->map = false;
			}
			block[y][x]->block->SetPosition({ 4.5f * (x - 6), 4.5f * (y - 4) , 0 });
			block[y][x]->block->SetRotation({ -90, 0, 0 });
			block[y][x]->block->SetScale({ 4.5f, 1, 4.5f });
			block[y][x]->block->SetColor({ 0.6f, 1, 0.7f, 1 });
			block[y][x]->block->Update();
		}
	}

	playerS = 0.1f;
	isPJ = false;

	player->SetPosition({ 0, 4.5f * (-5), 0 });
	player->SetRotation({ -90, 0, 0 });
	player->SetScale({ 4.5f, 1, 4.5f });
	player->SetColor({ 1, 0.6f, 0.9f, 1 });
	player->Update();
}

void GamePlayScene::Update()
{
#pragma region ゲームメインシステム
	
	// プレイヤー
	{
		// ジャンプ
		if (input->TriggerKey(DIK_SPACE) && isPJ == false)
		{
			playerJS = 0.05f;
			isPJ = true;
		}

		// 移動
		XMFLOAT3 pos = player->GetPosition();
		XMFLOAT3 particlePos = player->GetPosition();
		XMFLOAT3 velocity = { 0,0,0 };
		XMFLOAT3 accel = { 0,0,0 };
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			pos.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * playerS;
			/*this->position.x = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.x;
			this->position.y = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.y;*/
			if (27 < pos.x)
			{
				pos.x = 27;
			}
			else if (pos.x < -27)
			{
				pos.x = -27;
			}

			playerWalkEffectTimer--;
			if (playerWalkEffectTimer <= 0) {
				particlePos.x = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + pos.x;
				particlePos.y = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + pos.y;
				velocity.x -= (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.2f;
				accel.x -= (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.001f;
				accel.y = (float)rand() / RAND_MAX * 0.005f;
				playerWalkEffect->Add(30, { particlePos.x,particlePos.y - 1.5f,particlePos.z - 1.0f }, velocity, accel, 0.5f, 3.5f,{1,1,1,1},{0.5,0.5,0.5,0.3});
				playerWalkEffectTimer = 10;
			}			
		}
		// ジャンプ
		if (isPJ == true)
		{
			pos.y += playerJS; 
			playerJS *= 2;
			if (-20.25f < pos.y)
			{
				pos.y = -20.25f;
				playerJS = 0.05f;
				playerJS = -playerJS;
				accel.y = -0.005f;
				playerJumpEffect->Add(30, { particlePos.x,particlePos.y-0.5f,particlePos.z - 1.0f }, { 0,0,0 }, accel, 4.0f, 6.0f, { 1,1,1,1 }, { 1,1,1,1 });
			}
			else if (pos.y < -22.5f)
			{
				pos.y = -22.5f;
				isPJ = false;
			}
		}
		player->SetPosition(pos);
	}

#pragma endregion

#pragma region カメラとライトの更新

	light->Update();
	camera->Update();

#pragma endregion
}

void GamePlayScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	// 各描画
	//DrawBackSprite(cmdList);
	DrawObjects(cmdList);
	DrawEffect(cmdList);
	DrawUI(cmdList);
	DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	// 前景スプライト描画
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::DrawObjects(ID3D12GraphicsCommandList* cmdList)
{
	// OBJオブジェクト描画
	Object3d::PreDraw(cmdList);

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			if (block[y][x]->map == true)
			{
				block[y][x]->block->Draw();
			}
		}
	}
	player->Draw();

	Object3d::PostDraw();

	// FBXオブジェクト
	/*FbxObject3d::PreDraw(cmdList);

	

	FbxObject3d::PostDraw();*/

	// スプライト描画
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void GamePlayScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	// UI描画
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void GamePlayScene::DrawEffect(ID3D12GraphicsCommandList* cmdList)
{
	// パーティクル描画
	ParticleManager::PreDraw(cmdList);

	//particle->Draw();
	playerWalkEffect->Draw();
	playerJumpEffect->Draw();

	ParticleManager::PostDraw();
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	// デバッグテキスト描画
	debugText.Draw(cmdList);
}
