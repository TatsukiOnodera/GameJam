#include "GamePlayScene.h"
#include "SceneManager.h"

#include <SafeDelete.h>
#include <cassert>
#include <time.h>
#include <stdlib.h>
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
	ballJumpEffect.reset(ParticleManager::Create("star.png"));
	ballBounceEffect.reset(ParticleManager::Create("stareffect_01.png"));
	ballDeadEffect.reset(ParticleManager::Create("balldeadeffect.png"));
	enemyBounceEffect.reset(ParticleManager::Create("stareffect_03.png"));
	enemySpawnEffect.reset(ParticleManager::Create("balldeadeffect.png"));

	// スプライト
	playerStay.reset(Object3d::Create("PlayerStay"));
	playerJump.reset(Object3d::Create("PlayerJump"));
	playerMove01.reset(Object3d::Create("PlayerMove_01"));
	playerMove02.reset(Object3d::Create("PlayerMove_02"));
	playerMove03.reset(Object3d::Create("PlayerMove_03"));
	playerMove04.reset(Object3d::Create("PlayerMove_04"));
	ball01.reset(Object3d::Create("BallMove_01"));
	ball02.reset(Object3d::Create("BallMove_02"));
	ball03.reset(Object3d::Create("SmileBall_01"));
	ball04.reset(Object3d::Create("SmileBall_02"));
	frame.reset(Object3d::Create("GameFrame"));
	frame->SetPosition({ 0, 0, 0 });
	frame->SetRotation({ -90, 0, 0 });
	frame->SetScale({ 10 * 7.23, 1, 8 * 7.23 });
	frame->Update();
	title.reset(Object3d::Create("TitleText"));
	title->SetPosition({ 0, 8, 0 });
	title->SetRotation({ -90, 0, 0 });
	title->SetScale({ 30, 1, 30 });
	title->Update();
	titleButton.reset(Object3d::Create("Button_01"));
	titleButton->SetPosition({ 0, -9, 0 });
	titleButton->SetRotation({ -90, 0, 0 });
	titleButton->SetScale({ 20, 1, 20 });
	titleButton->Update();
	end.reset(Object3d::Create("Text_02"));
	end->SetPosition({ -10, -2, 0 });
	end->SetRotation({ -90, 0, 0 });
	end->SetScale({ 0, 1, 0 });
	end->Update();
	endButton.reset(Object3d::Create("Button_02"));
	endButton->SetPosition({ 0, -10, 0 });
	endButton->SetRotation({ -90, 0, 0 });
	endButton->SetScale({ 0, 1, 0 });
	endButton->Update();
	gameover.reset(Object3d::Create("GameoverText"));
	gameover->SetPosition({ 0, 8, 0 });
	gameover->SetRotation({ -90, 0, 0 });
	gameover->SetScale({ 0, 1, 0 });
	gameover->Update();
	for (auto& m : enemyStay)
	{
		m.reset(new ENEMYSTAY(Object3d::Create("EnemyStay")));
	}
	for (auto& m : enemy01)
	{
		m.reset(new ENEMY01(Object3d::Create("EnemyMove_01")));
	}
	for (auto& m : enemy02)
	{
		m.reset(new ENEMY02(Object3d::Create("EnemyMove_02")));
	}
	for (auto& m : enemySpawner)
	{
		m.reset(new ENEMYSPAWNER(Object3d::Create("EnemySpawner")));
	}
	for (int x = 0; x < 13; x++)
	{
		needle[x].reset(Object3d::Create("Needle"));
		needle[x]->SetPosition({ 5.2f * (x - 6), 15.35f, 0 });
		needle[x]->SetRotation({ -90, 0, 0 });
		needle[x]->SetScale({ 5.2f, 1, 5.2f });
		needle[x]->Update();
	}
	for (int x = 0; x < 3; x++)
	{
		heart[x].reset(Object3d::Create("Heart"));
		heart[x]->SetPosition({ x * 2.5f + 4.5f * (x - 1) - 22.5f, 24.25f, 0 });
		heart[x]->SetRotation({ -90, 0, 0 });
		heart[x]->SetScale({ 4.5f, 1, 4.5f });
		heart[x]->Update();
	}
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			block[y][x].reset(new BLOCK(Object3d::Create("Block")));
			if (y == 0)
			{
				block[y][x]->map = true;
			}
			else if (y == 1)
			{
				if (x == 1 || x == 2 || x == 4 || x == 8 || x == 10 || x == 11)
				{
					block[y][x]->map = true;
				}
			}
			else if (y == 2)
			{
				if (x == 1 || x == 2 || x == 10 || x == 11)
				{
					block[y][x]->map = true;
				}
			}
			else if (y == 3)
			{
				if (x == 1 || x == 11)
				{
					block[y][x]->map = true;
				}
			}
			else if (y == 4)
			{
				if (x == 1 || x == 11)
				{
					block[y][x]->map = true;
				}
			}
			else if (y == 5)
			{
				if (x == 11)
				{
					block[y][x]->map = true;
				}
			}
			else
			{
				block[y][x]->map = false;
			}
			block[y][x]->block->SetPosition({ 5.2f * (x - 6), 5.2f * (y - 4) , 0 });
			block[y][x]->block->SetRotation({ -90, 0, 0 });
			block[y][x]->block->SetScale({ 5.2f, 1, 5.2f });
			block[y][x]->block->SetColor({ 0.4f, 1, 0.5f, 1 });
			block[y][x]->block->Update();
		}
	}

	// OBJオブジェクト


	// FBXオブェクト


	// オーディオ
	//audio->Initialize();

	// 変数の初期化
	InitializeVariable();

	// 乱数初期化
	srand(time(NULL));
}

void GamePlayScene::InitializeVariable()
{
	score = 0;

	stage = TITLE;

	playerS = 0.15f;
	playerJS = 0;
	isPJ = false;

	ballS = 0;
	ballR = -5;
	ballJS = 0;
	ballJA = -0.1f;
	ballG = 0.5f;
	isBJ = false;
	isAliveB = true;

	comboTimer = 0;
	comboLimit = 60;
	comboNum = 0;

	heartCounter = 3;

	titleTimer = 0;

	endTimer = 0;

	buttonTimer = 0;

	spawTimer = 1;
	spawnerScale = 0;
	spawnerSA = 0.05;

	playerStay->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	playerStay->SetRotation({ -90, 0, 0 });
	playerStay->SetScale({ 2.25f, 1, 2.25f });
	playerStay->Update();

	playerJump->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	playerJump->SetRotation({ -90, 0, 0 });
	playerJump->SetScale({ 2.25f, 1, 2.25f });
	playerJump->Update();

	playerMove01->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	playerMove01->SetRotation({ -90, 0, 0 });
	playerMove01->SetScale({ 2.25f, 1, 2.25f });
	playerMove01->Update();

	playerMove02->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	playerMove02->SetRotation({ -90, 0, 0 });
	playerMove02->SetScale({ 2.25f, 1, 2.25f });
	playerMove02->Update();

	playerMove03->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	playerMove03->SetRotation({ -90, 0, 0 });
	playerMove03->SetScale({ 2.25f, 1, 2.25f });
	playerMove03->Update();

	playerMove04->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	playerMove04->SetRotation({ -90, 0, 0 });
	playerMove04->SetScale({ 2.25f, 1, 2.25f });
	playerMove04->Update();

	ball01->SetPosition({ 0, 5.2f * 2, 0 });
	ball01->SetRotation({ -90, 0, 0 });
	ball01->SetScale({ 2.25f, 1, 2.25f });
	ball01->Update();

	ball02->SetPosition({ 0, 5.2f * 2, 0 });
	ball02->SetRotation({ -90, 0, 0 });
	ball02->SetScale({ 2.25f, 1, 2.25f });
	ball02->Update();

	ball03->SetPosition({ 0, 5.2f * 2, 0 });
	ball03->SetRotation({ -90, 0, 0 });
	ball03->SetScale({ 2.25f, 1, 2.25f });
	ball03->Update();

	ball04->SetPosition({ 0, 5.2f * 2, 0 });
	ball04->SetRotation({ -90, 0, 0 });
	ball04->SetScale({ 2.25f, 1, 2.25f });
	ball04->Update();

	for (int x = 0; x < 13; x++)
	{
		needle[x]->SetPosition({ 4.5f * (x - 6), 14.5f, 0 });
		needle[x]->SetRotation({ -90, 0, 0 });
		needle[x]->SetScale({ 4.5f, 1, 4.5f });
		needle[x]->Update();
	}

	for (int x = 0; x < heartCounter; x++)
	{
		heart[x]->SetPosition({ x * 2.5f + 4.5f * (x - 1) - 22.5f, 24.25f, 0 });
		heart[x]->SetRotation({ -90, 0, 0 });
		heart[x]->SetScale({ 4.5f, 1, 4.5f });
		heart[x]->Update();
	}

	for (int x = 0; x < 13; x++)
	{
		enemyStay[x]->enemyStay->SetPosition({ 0, 0, 0 });
		enemyStay[x]->enemyStay->SetRotation({ -90, 0, 0 });
		enemyStay[x]->enemyStay->SetScale({ 0, 1, 0 });
		enemyStay[x]->enemyStay->Update();
		enemyStay[x]->enemyS = 0;
		enemyStay[x]->enemyR = -5;
		enemyStay[x]->enemyG = 0.5f;
		enemyStay[x]->alive = false;
		enemyStay[x]->isEnemyLanding = false;

		enemy01[x]->enemy01->SetPosition({ 0, 0, 0 });
		enemy01[x]->enemy01->SetRotation({ -90, 0, 0 });
		enemy01[x]->enemy01->SetScale({ 0, 1, 0 });
		enemy01[x]->enemy01->Update();
		enemy01[x]->enemyS = 0;
		enemy01[x]->enemyR = -5;
		enemy01[x]->enemyG = 0.5f;
		enemy01[x]->alive = false;

		enemy02[x]->enemy02->SetPosition({ 0, 0, 0 });
		enemy02[x]->enemy02->SetRotation({ -90, 0, 0 });
		enemy02[x]->enemy02->SetScale({ 0, 1, 0 });
		enemy02[x]->enemy02->Update();
		enemy02[x]->enemyS = 0;
		enemy02[x]->enemyR = -5;
		enemy02[x]->enemyG = 0.5f;
		enemy02[x]->alive = false;

		enemySpawner[x]->spawner->SetPosition({ 5.2f * (x - 6), 13.5f, 0 });
		enemySpawner[x]->spawner->SetRotation({ -90, 0, 0 });
		enemySpawner[x]->spawner->SetScale({ 0, 1, 0 });
		enemySpawner[x]->spawner->Update();
		enemySpawner[x]->active = false;
	}
}

void GamePlayScene::Update()
{
#pragma region ゲームメインシステム

	// タイトル
	if (stage == TITLE)
	{
		// 既定の大きさでなければ
		if (title->GetScale().x < 30 && titleButton->GetScale().x < 20)
		{
			XMFLOAT3 tScale = title->GetScale();
			XMFLOAT3 bScale = titleButton->GetScale();
			tScale.x += 0.75f;
			tScale.z += 0.75f;
			bScale.x += 0.5f;
			bScale.z += 0.5f;
			if (tScale.x >= 30)
			{
				tScale.x = 30.0f;
				tScale.z = 30.0f;
			}
			if (bScale.x >= 20)
			{
				bScale.x = 20.0f;
				bScale.z = 20.0f;
			}
			title->SetScale(tScale);
			titleButton->SetScale(bScale);
		}
		// 大きさならタイマーを刻む
		else
		{
			titleTimer++;
		}
		// 一定時間たつと
		if (titleTimer > 60)
		{
			titleTimer = 0;
			XMFLOAT3 tScale = title->GetScale();
			XMFLOAT3 bScale = {};
			if (tScale.x < 30.25f)
			{
				tScale.x = 30.5f;
				tScale.z = 30.5f;
				bScale.x = 20.5f;
				bScale.z = 20.5f;
			}
			else
			{
				tScale.x = 30.0f;
				tScale.z = 30.0f;
				bScale.x = 20.0f;
				bScale.z = 20.0f;
			}
			title->SetScale(tScale);
			titleButton->SetScale(bScale);
		}
	}

	// ブロック
	if (stage == TITLE || stage == GAME)
	{
		// HP更新
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 13; x++)
			{
				if (block[y][x]->map == true)
				{
					if (stage == TITLE)
					{
						block[y][x]->HP = 15;
					}
					// 0なら一列繰り下げ
					if (block[y][x]->HP <= 0)
					{
						for (int h = 1; h < 8; h++)
						{
							XMFLOAT3 bPos = block[h + 1][x]->block->GetPosition();
							bPos.y -= 0.45f;
							block[h + 1][x]->block->SetPosition(bPos);
							if (5.2f < 5.2f * (h - 3) - bPos.y && h < 7)
							{
								block[y][x]->HP = 15;
								block[h][x]->map = block[h + 1][x]->map;
								block[h + 1][x]->map = false;
								block[h][x]->block->SetPosition({ 5.2f * (x - 6), 5.2f * (h - 4) , 0 });
								if (h == 6)
								{
									block[7][x]->block->SetPosition({ 5.2f * (x - 6), 5.2f * (7 - 4) , 0 });
								}
							}
						}
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
					ballDeadEffect->Add(60, block[y][x]->block->GetPosition(), { 0,0,0 }, { 0,-0.005f,0 }, 3.0f, 7.0f);
					camera->SetShakeFlag(true, 10);
				}
				if (stage == GAME)
				{
					// タイマーを1にする
					comboTimer = 1;
					// コンボカウンター
					comboNum++;
				}
				else if (stage == TITLE)
				{
					title->SetScale({ 0, 1, 0 });
					titleButton->SetScale({ 0, 1, 0 });
					stage = GAME;
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
						}
					}
					break;
				}
				else if (stage == TITLE)
				{
					title->SetScale({ 0, 1, 0 });
					titleButton->SetScale({ 0, 1, 0 });
					stage = GAME;
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
						}
					}
					break;
				}
			}
		}

		// コンボ処理
		if (comboTimer > 0 && stage == GAME)
		{
			// コンボの猶予時間
			comboTimer++;
			if (comboTimer > 60)
			{
				// コンボ時
				debugText.Print("COMBO", 10, 10, comboNum);
			}
			// 猶予を過ぎたか
			if (comboTimer > comboLimit)
			{
				// リセット
				comboTimer = 0;
				comboNum = 0;
			}
		}
	}

	// プレイヤー
	if (stage == TITLE || stage == GAME)
	{
		XMFLOAT3 pPos = playerStay->GetPosition();
		// ジャンプ
		if (input->TriggerKey(DIK_SPACE) && isPJ == false)
		{
			playerJS = 0.05f;
			isPJ = true;
		}

		// 移動
		//XMFLOAT3 pPos = player->GetPosition();
		XMFLOAT3 walkParticlePos = playerStay->GetPosition();
		XMFLOAT3 jumpParticlePos = playerStay->GetPosition();
		XMFLOAT3 velocity = { 0,0,0 };
		XMFLOAT3 walkParticleAccel = { 0,0,0 };
		XMFLOAT3 jumpParticleAccel = { 0,0,0 };
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			moveCount++;
			if (moveCount >= 5 && switchingMove) {
				switchingMove = false;
				moveCount = 0;
			}
			else if (moveCount >= 5 && !switchingMove) {
				switchingMove = true;
				moveCount = 0;
			}

			pPos.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * playerS;
			if (31.2f < pPos.x)
			{
				pPos.x = 31.2f;
			}
			else if (pPos.x < -31.2f)
			{
				pPos.x = -31.2f;
			}

			playerWalkEffectTimer--;

			if (playerWalkEffectTimer <= 0 && !isPJ) {
				walkParticlePos.x = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + pPos.x;
				walkParticlePos.y = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + pPos.y;
				velocity.x -= (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.2f;
				walkParticleAccel.x -= (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.001f;
				walkParticleAccel.y = (float)rand() / RAND_MAX * 0.005f;
				playerWalkEffect->Add(30, { walkParticlePos.x, walkParticlePos.y - 1.5f, walkParticlePos.z - 1.0f }, velocity, walkParticleAccel, 0.5f, 3.5f, { 1,1,1,1 }, { 0.5,0.5,0.5,0.3 });
				playerWalkEffectTimer = 10;
				playerWalkEffect->Update();
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
			if (-23.4f < pPos.y + 0.5f * playerStay->GetScale().x - 0.7f)
			{
				pPos.y = -23.4f - 0.5f * playerStay->GetScale().x + 0.7f;
				playerJS = 0.05f;
				playerJS = -playerJS;
				jumpParticleAccel.y = -0.005f;
				playerJumpEffect->Add(30, { jumpParticlePos.x,jumpParticlePos.y - 0.5f,jumpParticlePos.z - 1.0f }, { 0,0,0 }, jumpParticleAccel, 4.0f, 6.0f, { 1,1,1,1 }, { 1,1,1,1 });
				// Xが今どこの横列か
				int x = static_cast<int>((pPos.x + block[0][0]->block->GetScale().x * 6 + 0.5f * block[0][0]->block->GetScale().x) / block[0][0]->block->GetScale().x);
				//一つ上に空きがあるか
				for (int y = 6; 0 <= y; y--)
				{
					if (y < 7 && block[y][x]->map == true && block[y + 1][x]->map == false && block[1][x]->HP > 0)
					{
						block[y + 1][x]->map = true;
						if (isBJ == false && static_cast<int>((ball01->GetPosition().x + block[0][0]->block->GetScale().x * 6 + 0.5f * block[0][0]->block->GetScale().x) / block[0][0]->block->GetScale().x) == x)
						{
							isBJ = true;
							ballJS = 2.0f;
							XMFLOAT3 bPos = ball01->GetPosition();
							bPos.y += 4.5f;
							ball01->SetPosition(bPos);
							ball02->SetPosition(bPos);
							ball03->SetPosition(bPos);
							ball04->SetPosition(bPos);
							ball01->Update();
							ball02->Update();
							ball03->Update();
							ball04->Update();
						}
						for (int i = 0; i < 13; i++)
						{
							if (isBJ == false && static_cast<int>((enemy01[i]->enemy01->GetPosition().x + block[0][0]->block->GetScale().x * 6 + 0.5f * block[0][0]->block->GetScale().x) / block[0][0]->block->GetScale().x) == x && enemy01[i]->alive == true)
							{
								enemySpawnEffect->Add(60, enemy01[i]->enemy01->GetPosition(), { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
								enemyStay[i]->alive = false;
								enemyStay[i]->isEnemyLanding = false;
								enemy01[i]->alive = false;
								enemy02[i]->alive = false;
							}
						}
						break;
					}
				}
			}
			//床についたか
			else if (pPos.y < -25.48f)
			{
				pPos.y = -25.48f;
				isPJ = false;
			}
		}
		playerStay->SetPosition(pPos);
		playerJump->SetPosition(pPos);
		playerMove01->SetPosition(pPos);
		playerMove02->SetPosition(pPos);
		playerMove03->SetPosition(pPos);
		playerMove04->SetPosition(pPos);
		playerStay->Update();
	}

	// ボール
	if (stage == GAME)
	{
		// 移動
		XMFLOAT3 bPos = ball01->GetPosition();
		bPos.x += ballS;
		blinkCount++;
		if (blinkCount >= 120 && !isBlink) {
			isBlink = true;
			blinkCount = 0;
		}
		else if (blinkCount >= 15 && isBlink) {
			isBlink = false;
			blinkCount = 0;
		}
		if (32.75f < bPos.x)
		{
			bPos.x = 32.75f;
			ballS = -ballS;
			ballR = -ballR;
			ballBounceEffect->Add(60, bPos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
		}
		else if (bPos.x < -32.75f)
		{
			bPos.x = -32.75f;
			ballS = -ballS;
			ballR = -ballR;
			ballBounceEffect->Add(60, bPos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
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
			ballJumpEffect->Add(30, bPos, { 0,0,0 }, { 0,0,0 }, 1.0f, 3.0f);
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
					// 縦
					if (y < 7 && block[y + 1][x]->map == false)
					{
						if (block[y][x]->block->GetPosition().x - 0.5f * block[y][x]->block->GetScale().x <= bPos.x && bPos.x <= block[y][x]->block->GetPosition().x + 0.5f * block[y][x]->block->GetScale().x)
						{
							if (bPos.y - 0.5f * ball01->GetScale().x < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
							{
								bPos.y = block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x + 0.5f * ball01->GetScale().x;
								ballG = gravity;
								isBJ = false;
								// 速度決定
								if (ballS == 0)
								{
									ballS = 0.2f;
								}
							}
						}
					}
					// 横
					if (fabsf(bPos.x - block[y][x]->block->GetPosition().x) < 0.5f * ball01->GetScale().x + 0.5f * block[y][x]->block->GetScale().x)
					{
						if (block[y][x]->block->GetPosition().y - 0.5f * block[y][x]->block->GetScale().x < bPos.y && bPos.y < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
						{
							if (bPos.x - block[y][x]->block->GetPosition().x < 0)
							{
								bPos.x = block[y][x]->block->GetPosition().x - 0.5f * ball01->GetScale().x - 0.5f * block[y][x]->block->GetScale().x;
								if (isBJ == false)
								{
									ballS = -abs(ballS);
									ballR = abs(ballR);
									ballBounceEffect->Add(60, bPos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
								}
							}
							else
							{
								bPos.x = block[y][x]->block->GetPosition().x + 0.5f * ball01->GetScale().x + 0.5f * block[y][x]->block->GetScale().x;
								if (isBJ == false)
								{
									ballS = abs(ballS);
									ballR = -abs(ballR);
									ballBounceEffect->Add(60, bPos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
								}
							}
						}
					}
				}
			}
		}
		ball01->SetPosition(bPos);
		ball02->SetPosition(bPos);
		ball03->SetPosition(bPos);
		ball04->SetPosition(bPos);
		// 回転
		XMFLOAT3 bRot = ball01->GetRotation();
		bRot.z += ballR;
		if (bRot.z >= 360 || bRot.z <= -360)
		{
			bRot.z = 0;
		}
		ball01->SetRotation(bRot);
		ball02->SetRotation(bRot);
		ball03->SetRotation(bRot);
		ball04->SetRotation(bRot);
	}

	// エネミー
	if (stage == GAME)
	{
		// スポーンタイマー
		if (spawTimer > 0)
		{
			spawTimer++;
		}
		// 一定時間たったら生成
		if (spawTimer > 600)
		{
			spawTimer = 1;
			while (true)
			{
				int n = rand() % 13;
				if (enemy01[n]->alive == false)
				{
					enemySpawner[n]->active = true;
					break;
				}
				else
				{
					bool hit = false;
					for (auto& m : enemy01)
					{
						if (m->alive == false)
						{
							hit = true;
						}
					}
					if (hit == false)
					{
						break;
					}
				}
			}
		}
		for (int i = 0; i < 13; i++)
		{
			// スポナー
			if (enemySpawner[i]->active == true)
			{
				// 拡大
				XMFLOAT3 scale = enemySpawner[i]->spawner->GetScale();
				scale.x += spawnerSA;
				scale.z += spawnerSA;
				// エネミーの生成
				if (scale.x > 2.25f && enemy01[i]->alive == false)
				{
					enemyStay[i]->alive = true;
					enemyStay[i]->enemyStay->SetPosition({ 5.2f * (i - 6), 13.5f, 0 });
					enemyStay[i]->enemyStay->SetRotation({ -90, 0, 0 });
					enemyStay[i]->enemyStay->SetScale({ 0, 1, 0 });
					enemyStay[i]->enemyStay->Update();
					enemyStay[i]->enemyG = 0.5f;

					enemy01[i]->alive = true;
					enemy01[i]->enemy01->SetPosition({ 5.2f * (i - 6), 13.5f, 0 });
					enemy01[i]->enemy01->SetRotation({ -90, 0, 0 });
					enemy01[i]->enemy01->SetScale({ 0, 1, 0 });
					enemy01[i]->enemy01->Update();
					enemy01[i]->enemyG = 0.5f;

					enemy02[i]->alive = true;
					enemy02[i]->enemy02->SetPosition({ 5.2f * (i - 6), 13.5f, 0 });
					enemy02[i]->enemy02->SetRotation({ -90, 0, 0 });
					enemy02[i]->enemy02->SetScale({ 0, 1, 0 });
					enemy02[i]->enemy02->Update();
					enemy02[i]->enemyG = 0.5f;
				}
				// 最大なら縮小
				else if (scale.x > 4.5f)
				{
					scale.x = 4.5f;
					scale.z = 4.5f;
					spawnerSA = -spawnerSA;
				}
				// 0なら消失
				else if (scale.x <= 0)
				{
					scale.x = 0;
					scale.z = 0;
					spawnerSA = -spawnerSA;
					enemySpawner[i]->active = false;
					enemySpawnEffect->Add(60, enemyStay[i]->enemyStay->GetPosition(), { 0,0,0 }, { 0,0,0 }, 3.0f, 10.0f);
				}
				enemySpawner[i]->spawner->SetScale(scale);
			}
			// エネミー
			if (enemy01[i]->alive == true)
			{
				// スポナーに合わせて拡大
				if (enemySpawner[i]->spawner->GetScale().x != 0)
				{
					XMFLOAT3 scale = enemy01[i]->enemy01->GetScale();
					scale.x += 0.05f;
					scale.z += 0.05f;
					if (scale.x > 2.25f)
					{
						scale.x = 2.25f;
						scale.z = 2.25f;
					}
					enemyStay[i]->enemyStay->SetScale(scale);
					enemy01[i]->enemy01->SetScale(scale);
					enemy02[i]->enemy02->SetScale(scale);
				}
				// 更新
				else
				{
					// 移動
					XMFLOAT3 ePos = enemy01[i]->enemy01->GetPosition();
					ePos.x += enemy01[i]->enemyS;
					if (32.75f < ePos.x)
					{
						ePos.x = 32.75f;
						enemy01[i]->enemyS = -abs(enemy01[i]->enemyS);
						enemy01[i]->enemyR = abs(enemy01[i]->enemyR);
						enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
					}
					else if (ePos.x < -32.75f)
					{
						ePos.x = -32.75f;
						enemy01[i]->enemyS = abs(enemy01[i]->enemyS);
						enemy01[i]->enemyR = -abs(enemy01[i]->enemyR);
						enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
					}
					// 重力
					ePos.y -= enemy01[i]->enemyG;
					enemy01[i]->enemyG += gravity;
					//当たり判定
					for (int y = 0; y < 7; y++)
					{
						for (int x = 0; x < 13; x++)
						{
							if (block[y][x]->map == true)
							{
								// 縦
								if (y < 7 && block[y + 1][x]->map == false)
								{
									if (block[y][x]->block->GetPosition().x - 0.5f * block[y][x]->block->GetScale().x <= ePos.x && ePos.x <= block[y][x]->block->GetPosition().x + 0.5f * block[y][x]->block->GetScale().x)
									{
										if (ePos.y - 0.5f * ball01->GetScale().x < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
										{
											ePos.y = block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x + 0.5f * ball01->GetScale().x;
											enemy01[i]->enemyG = gravity;
											enemyStay[i]->isEnemyLanding = true;
											// 速度決定
											if (enemy01[i]->enemyS == 0)
											{
												enemy01[i]->enemyS = 0.2f;
												enemy02[i]->enemyS = 0.2f;
											}
										}
									}
								}
								// 横
								if (fabsf(ePos.x - block[y][x]->block->GetPosition().x) < 0.5f * ball01->GetScale().x + 0.5f * block[y][x]->block->GetScale().x)
								{
									if (block[y][x]->block->GetPosition().y - 0.5f * block[y][x]->block->GetScale().x < ePos.y && ePos.y < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
									{
										if (ePos.x - block[y][x]->block->GetPosition().x < 0)
										{
											ePos.x = block[y][x]->block->GetPosition().x - 0.5f * ball01->GetScale().x - 0.5f * block[y][x]->block->GetScale().x;
											block[y][x]->HP--;
											enemy01[i]->enemyS = -enemy01[i]->enemyS;
											enemy01[i]->enemyR = -enemy01[i]->enemyR;
											enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
										}
										else
										{
											ePos.x = block[y][x]->block->GetPosition().x + 0.5f * ball01->GetScale().x + 0.5f * block[y][x]->block->GetScale().x;
											block[y][x]->HP--;
											enemy01[i]->enemyS = -enemy01[i]->enemyS;
											enemy01[i]->enemyR = -enemy01[i]->enemyR;
											enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
										}
									}
								}
							}
						}
					}
					enemyStay[i]->enemyStay->SetPosition(ePos);
					enemy01[i]->enemy01->SetPosition(ePos);
					enemy02[i]->enemy02->SetPosition(ePos);
					// 回転
					XMFLOAT3 eRot = enemy01[i]->enemy01->GetRotation();
					eRot.z += enemy01[i]->enemyR;
					if (eRot.z >= 360 || eRot.z <= -360)
					{
						eRot.z = 0;
					}
					enemyStay[i]->enemyStay->SetRotation(eRot);
					enemy01[i]->enemy01->SetRotation(eRot);
					enemy02[i]->enemy02->SetRotation(eRot);
					// プレイヤーのボールとの当たり判定
					if (LenAB(ball01->GetPosition(), ePos) < ball01->GetScale().x * 0.5f + 0.5f * enemy01[i]->enemy01->GetScale().x)
					{
						heartCounter--;
						if (heartCounter <= 0)
						{
							heartCounter = 0;
							stage = END;
						}
						else
						{
							enemySpawnEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
							enemyStay[i]->alive = false;
							enemyStay[i]->isEnemyLanding = false;
							enemy01[i]->alive = false;
							enemy02[i]->alive = false;
						}
					}
				}
			}
		}
	}

	// リザルト
	if (stage == END)
	{
		// 終わった瞬間一瞬止まる
		if (isAliveB == true)
		{
			endTimer++;
			XMFLOAT3 bPos = ball01->GetPosition();
			XMFLOAT3 bDeadPos = ball01->GetPosition();
			bPos.x = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + bPos.x;
			bPos.y = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + bPos.y;
			ballBounceEffect->Add(30, { bPos.x, bPos.y, bPos.z }, { 0,0,0 }, { 0,0,0 }, 0.5f, 3.5f, { 1,1,1,1 }, { 0.5,0.5,0.5,0.3 });
			if (endTimer > 60)
			{
				isAliveB = false;
				ballDeadEffect->Add(120, { bDeadPos.x, bDeadPos.y, bDeadPos.z }, { 0,0,0 }, { 0,0,0 }, 1.0f, 5.0f);
				for (int i = 0; i < 13; i++)
				{
					enemyStay[i]->alive = false;
					enemy01[i]->alive = false;
					enemy02[i]->alive = false;
				}
			}
		}
		else
		{
			// 一定の大きさに満たなければ
			if (end->GetScale().x < 20 && endButton->GetScale().x < 25 && gameover->GetScale().x < 30)
			{
				XMFLOAT3 a = end->GetScale();
				XMFLOAT3 b = endButton->GetScale();
				XMFLOAT3 c = gameover->GetScale();
				a.x += 0.5f;
				a.z += 0.5f;
				b.x += 0.625f;
				b.z += 0.625f;
				c.x += 0.75f;
				c.z += 0.75f;
				if (a.x >= 20)
				{
					a.x = 20;
					a.z = 20;
					b.x = 25;
					b.z = 25;
					c.x = 30;
					c.z = 30;
				}
				end->SetScale(a);
				endButton->SetScale(b);
				gameover->SetScale(c);
			}
			else
			{
				// タイマー
				buttonTimer++;
				// 一定の時間すぎると
				if (buttonTimer > 60)
				{
					buttonTimer = 0;
					XMFLOAT3 bScale = endButton->GetScale();
					if (bScale.x < 25.25f)
					{
						bScale.x = 25.5f;
						bScale.z = 25.5f;
					}
					else
					{
						bScale.x = 25.0f;
						bScale.z = 25.0f;
					}
					endButton->SetScale(bScale);
				}

				// TITLEに戻る
				if (input->TriggerKey(DIK_SPACE))
				{
					InitializeVariable();
				}
			}
		}
	}

#pragma endregion

#pragma region カメラとライトの更新
	camera->CameraShake();
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
	for (auto& m : needle)
	{
		m->Draw();
	}
	for (int i = 0; i < 13; i++)
	{
		if (enemySpawner[i]->active == true)
		{
			enemySpawner[i]->spawner->Draw();
		}
		if (enemy01[i]->alive == true && !enemyStay[i]->isEnemyLanding) {
			enemyStay[i]->enemyStay->Draw();
		}
		else if (enemy01[i]->alive == true && !isBlink)
		{
			enemy01[i]->enemy01->Draw();
		}
		else if (enemy01[i]->alive == true && isBlink)
		{
			enemy02[i]->enemy02->Draw();
		}
	}

	if (input->PushKey(DIK_D)) {
		if (switchingMove) {
			playerMove01->Draw();
		}
		else if (!switchingMove) {
			playerMove02->Draw();
		}
	}
	else if (input->PushKey(DIK_A)) {
		if (switchingMove) {
			playerMove03->Draw();
		}
		else if (!switchingMove) {
			playerMove04->Draw();
		}
	}
	else if (isPJ) {
		playerJump->Draw();
	}
	else {
		playerStay->Draw();
	}
	if (stage == GAME || stage == END)
	{
		if (isAliveB == true)
		{
			if (!isBlink) {
				if (!isBJ) {
					ball01->Draw();
				}
				else if (isBJ) {
					ball03->Draw();
				}
			}
			if (isBlink) {
				if (!isBJ) {
					ball02->Draw();
				}
				else if (isBJ) {
					ball04->Draw();
				}
			}
		}
	}
	frame->Draw();
	for (int i = 0; i < heartCounter; i++)
	{
		heart[i]->Draw();
	}
	if (stage == TITLE)
	{
		title->Draw();
		titleButton->Draw();
	}
	else if (stage == END)
	{
		if (isAliveB == false)
		{
			gameover->Draw();
			end->Draw();
			endButton->Draw();
		}
	}

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
	if (!isPJ) {
		playerWalkEffect->Draw();
	}
	playerJumpEffect->Draw();
	ballJumpEffect->Draw();
	ballBounceEffect->Draw();
	ballDeadEffect->Draw();
	enemyBounceEffect->Draw();
	enemySpawnEffect->Draw();

	ParticleManager::PostDraw();
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	// デバッグテキスト描画
	debugText.Draw(cmdList);
}

const float GamePlayScene::LenAB(XMFLOAT3 posA, XMFLOAT3 posB)
{
	return sqrtf(powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2));
}