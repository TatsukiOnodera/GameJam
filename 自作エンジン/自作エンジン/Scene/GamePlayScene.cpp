#include "GamePlayScene.h"
#include "SceneManager.h"

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
	//particle.reset(ParticleManager::Create("Default/effect1.png"));

	// スプライト
	player.reset(Object3d::Create("Player"));
	ball.reset(Object3d::Create("BallMove_01"));
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			block[y][x].reset(new BLOCK(Object3d::Create("Block")));
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
			block[y][x]->block->SetColor({ 0.4f, 1, 0.5f, 1 });
			block[y][x]->block->Update();
		}
	}

	playerS = 0.1f;
	playerJS = 0;
	isPJ = false;

	ballS = 0.2f;
	ballR = -5;
	ballJS = 0;
	ballJA = -0.45f;
	ballG = 0.5f;
	isBJ = false;

	comboTimer = 0;
	comboLimit = 60;

	player->SetPosition({ 0, 4.5f * (-5), 0 });
	player->SetRotation({ -90, 0, 0 });
	player->SetScale({ 4.5f, 1, 4.5f });
	player->Update();

	ball->SetPosition({ 0, 5, 0 });
	ball->SetRotation({ -90, 0, 0 });
	ball->SetScale({ 4.5f, 1, 4.5f });
	ball->Update();
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
		XMFLOAT3 pPos = player->GetPosition();
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			pPos.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * playerS;
			if (27 < pPos.x)
			{
				pPos.x = 27;
			}
			else if (pPos.x < -27)
			{
				pPos.x = -27;
			}
		}

		// ジャンプ処理
		if (isPJ == true)
		{
			// 加算
			pPos.y += playerJS; 
			// 加速
			playerJS *= 2;
			//ブロックにぶつかったか
			if (-20.25f < pPos.y)
			{
				pPos.y = -20.25f;
				playerJS = 0.05f;
				playerJS = -playerJS;
				// Xが今どこの横列か
				int x = static_cast<int>((pPos.x + 29.25f) / 4.5f);
				//一つ上に空きがあるか
				for (int y = 6; 0 <= y; y--)
				{
					if (y < 7 && block[y][x]->map == true && block[y + 1][x]->map == false)
					{
						block[y + 1][x]->map = true;
						break;
					}
				}
				if (isBJ == false && static_cast<int>((ball->GetPosition().x + 29.25f) / 4.5f) == x)
				{
					isBJ = true;
					ballJS = 4.5f;
				}
			}
			//床についたか
			else if (pPos.y < -22.5f)
			{
				pPos.y = -22.5f;
				isPJ = false;
			}
		}
		player->SetPosition(pPos);
	}

	// ボール
	{
		// 移動
		XMFLOAT3 bPos = ball->GetPosition();
		bPos.x += ballS;
		if (27 < bPos.x)
		{
			bPos.x = 27;
			ballS = -ballS;
			ballR = -ballR;
		}
		else if (bPos.x < -27)
		{
			bPos.x = -27;
			ballS = -ballS;
			ballR = -ballR;
		}
		// 重力
		if (isBJ == false)
		{
			bPos.y -= ballG;
			ballG += gravity;
		}
		// ジャンプ
		else
		{
			bPos.y += ballJS;
			ballJS += ballJA;
		}
		//当たり判定
		for (int y = 0; y < 7; y++)
		{
			for (int x = 0; x < 13; x++)
			{
				if (block[y][x]->map == true)
				{
					if (bPos.y - 2.25f < block[y][x]->block->GetPosition().y + 2.25f)
					{
						bPos.y = block[y][x]->block->GetPosition().y + 4.5f;
						ballG = gravity;
						isBJ = false;
					}
				}
			}
		}
		ball->SetPosition(bPos);
		// 回転
		XMFLOAT3 bRot = ball->GetRotation();
		bRot.z += ballR;
		if (bRot.z >= 360 || bRot.z <= -360)
		{
			bRot.z = 0;
		}
		ball->SetRotation(bRot);
	}

	// ブロック
	{
		// HP更新
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 13; x++)
			{
				if (block[y][x]->map == true)
				{
					// 0なら一列繰り下げ
					if (block[y][x]->HP <= 0)
					{
						block[y][x]->map = block[y + 1][x]->map;
						block[y][x]->HP = block[y + 1][x]->HP;
						block[y + 1][x]->map = false;
						block[y + 1][x]->HP = 15;
					}
					// 5以下で
					else if (block[y][x]->HP <= 5)
					{
						block[y][x]->block->SetColor({ 1, 0.5f, 0.4f, 1 });
					}
					// 10以下で
					else if (block[y][x]->HP <= 10)
					{
						block[y][x]->block->SetColor({ 1, 1, 0.45f, 1 });
					}
					// 15以下で
					else
					{
						block[y][x]->block->SetColor({ 0.4f, 1, 0.5f, 1 });
					}
				}
			}
		}

		// 一列そろったか
		for (int x = 0; x < 13; x++)
		{
			bool isComplete = true;
			for (int y = 0; y < 8; y++)
			{
				// 縦一列ずつ調べる
				if (block[y][x]->map == false)
				{
					isComplete = false;
				}
			}
			// 一列そろっていたら
			if (isComplete == true)
			{
				// すべて消す
				for (int y = 1; y < 8; y++)
				{
					block[y][x]->map = false;
					block[y][x]->HP = 15;
				}
				// コンボが成立するか
				if (comboTimer > 0)
				{
					// コンボ時

				}
				//タイマーを1にする
				comboTimer = 1;
			}
		}

		// コンボ処理
		if (comboTimer > 0)
		{
			// コンボの猶予時間
			comboTimer++;
			// 猶予を過ぎたか
			if (comboTimer > comboLimit)
			{
				// リセット
				comboTimer = 0;
			}
		}
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
	//DrawEffect(cmdList);
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
	ball->Draw();

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

	ParticleManager::PostDraw();
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	// デバッグテキスト描画
	debugText.Draw(cmdList);
}
