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
	audio = Audio::GetInstance();
	audio->Initialize();
	camera = Camera::GetInstance();

	// スプライトテクスチャ読み込み
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/Animation.png");
	Sprite::LoadTexture(2, L"Resources/TeamLogo.png");

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
	deleteBlockEffect.reset(ParticleManager::Create("balldeadeffect.png"));
	enemyBounceEffect.reset(ParticleManager::Create("stareffect_03.png"));
	enemySpawnEffect.reset(ParticleManager::Create("enemyspowneffect.png"));
	deadEffect.reset(ParticleManager::Create("deadeffect.png"));

	// OBJオブジェクト
	playerStay.reset(Object3d::Create("PlayerStay"));
	playerStay->SetPosition({ 0, 5.2f * (-4.9), 0 });
	playerStay->SetRotation({ -95, 0, 0 });
	playerStay->SetScale({ 2.25f, 1, 2.25f });
	playerStay->Update();
	playerJump.reset(Object3d::Create("PlayerJump"));
	playerJump->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	playerJump->SetRotation({ -95, 0, 0 });
	playerJump->SetScale({ 2.25f, 1, 2.25f });
	playerJump->Update();
	playerMove01.reset(Object3d::Create("PlayerMove_01"));
	playerMove01->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	playerMove01->SetRotation({ -95, 0, 0 });
	playerMove01->SetScale({ 2.25f, 1, 2.25f });
	playerMove01->Update();
	playerMove02.reset(Object3d::Create("PlayerMove_02"));
	playerMove02->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	playerMove02->SetRotation({ -95, 0, 0 });
	playerMove02->SetScale({ 2.25f, 1, 2.25f });
	playerMove02->Update();
	playerMove03.reset(Object3d::Create("PlayerMove_03"));
	playerMove03->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	playerMove03->SetRotation({ -95, 0, 0 });
	playerMove03->SetScale({ 2.25f, 1, 2.25f });
	playerMove03->Update();
	playerMove04.reset(Object3d::Create("PlayerMove_04"));
	playerMove04->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	playerMove04->SetRotation({ -95, 0, 0 });
	playerMove04->SetScale({ 2.25f, 1, 2.25f });
	playerMove04->Update();
	ball01.reset(Object3d::Create("BallMove_01"));
	ball02.reset(Object3d::Create("BallMove_02"));
	ball03.reset(Object3d::Create("SmileBall_01"));
	ball04.reset(Object3d::Create("SmileBall_02"));
	frame.reset(Object3d::Create("GameFrame"));
	frame->SetPosition({ 0, 0, 0 });
	frame->SetRotation({ -90, 0, 0 });
	frame->SetScale({ 10 * 7.22, 1, 8 * 7.22 });
	frame->Update();
	title.reset(Object3d::Create("TitleText"));
	title->SetPosition({ 0, 4, 0 });
	title->SetRotation({ -90, 0, 0 });
	title->SetScale({ 30 * 1.7f, 1, 30 * 1.7f });
	title->Update();
	titleButton.reset(Object3d::Create("Button_01"));
	titleButton->SetPosition({ 0, -13, 0 });
	titleButton->SetRotation({ -90, 0, 0 });
	titleButton->SetScale({ 20, 1, 20 });
	titleButton->Update();
	end.reset(Object3d::Create("Text_02"));
	end->SetPosition({ -20, -6, 0 });
	end->SetRotation({ -90, 0, 0 });
	end->SetScale({ 0, 1, 0 });
	end->Update();
	endButton.reset(Object3d::Create("Button_02"));
	endButton->SetPosition({ 0, -14, 0 });
	endButton->SetRotation({ -90, 0, 0 });
	endButton->SetScale({ 0, 1, 0 });
	endButton->Update();
	gameover.reset(Object3d::Create("GameoverText"));
	gameover->SetPosition({ 0, 7, 0 });
	gameover->SetRotation({ -90, 0, 0 });
	gameover->SetScale({ 0, 1, 0 });
	gameover->Update();
	HP.reset(Object3d::Create("Text_01"));
	HP->SetPosition({ -28, 24.25f, 0 });
	HP->SetRotation({ -90, 0, 0 });
	HP->SetScale({ 25, 1, 25 });
	HP->Update();
	scoreList.reset(Object3d::Create("Text_02"));
	scoreList->SetPosition({ 13.4, 23.4f, 0 });
	scoreList->SetRotation({ -90, 0, 0 });
	scoreList->SetScale({ 20, 1, 20 });
	scoreList->Update();
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
		heart[x]->SetPosition({ 4.5f * (x - 1) - 17, 24.25f, 0 });
		heart[x]->SetRotation({ -90, 0, 0 });
		heart[x]->SetScale({ 3.5f, 1, 3.5f });
		heart[x]->Update();
	}
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			block[y][x].reset(new BLOCK(Object3d::Create("Block")));
			buffBlock[y][x].reset(Object3d::Create("BuffBlock"));
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
			block[y][x]->block->Update();
			buffBlock[y][x]->SetPosition({ 5.2f * (x - 6), 5.2f * (y - 4) , 0 });
			buffBlock[y][x]->SetRotation({ -90, 0, 0 });
			buffBlock[y][x]->SetScale({ 5.2f, 1, 5.2f });
			buffBlock[y][x]->Update();
		}
	}
	for (int x = 0; x < 6; x++)
	{
		scoreNum[0][x].reset(Object3d::Create("NumberText_0"));
		scoreNum[1][x].reset(Object3d::Create("NumberText_1"));
		scoreNum[2][x].reset(Object3d::Create("NumberText_2"));
		scoreNum[3][x].reset(Object3d::Create("NumberText_3"));
		scoreNum[4][x].reset(Object3d::Create("NumberText_4"));
		scoreNum[5][x].reset(Object3d::Create("NumberText_5"));
		scoreNum[6][x].reset(Object3d::Create("NumberText_6"));
		scoreNum[7][x].reset(Object3d::Create("NumberText_7"));
		scoreNum[8][x].reset(Object3d::Create("NumberText_8"));
		scoreNum[9][x].reset(Object3d::Create("NumberText_9"));
		scoreNumEND[0][x].reset(Object3d::Create("NumberText_0"));
		scoreNumEND[1][x].reset(Object3d::Create("NumberText_1"));
		scoreNumEND[2][x].reset(Object3d::Create("NumberText_2"));
		scoreNumEND[3][x].reset(Object3d::Create("NumberText_3"));
		scoreNumEND[4][x].reset(Object3d::Create("NumberText_4"));
		scoreNumEND[5][x].reset(Object3d::Create("NumberText_5"));
		scoreNumEND[6][x].reset(Object3d::Create("NumberText_6"));
		scoreNumEND[7][x].reset(Object3d::Create("NumberText_7"));
		scoreNumEND[8][x].reset(Object3d::Create("NumberText_8"));
		scoreNumEND[9][x].reset(Object3d::Create("NumberText_9"));
	}
	for (int n = 0; n < 10; n++)
	{
		for (int x = 0; x < 6; x++)
		{
			scoreNum[n][x]->SetScale({ 4 * 1.5f, 1, 4 * 1.5f });
			scoreNum[n][x]->Update();
			scoreNum[n][x]->SetPosition({ x * scoreNum[n][x]->GetScale().x - (4.0f * x) + 21, 24.3f, 0 });
			scoreNum[n][x]->SetRotation({ -90, 0, 0 });
			scoreNumEND[n][x]->SetScale({ 0, 1, 0 });
			scoreNumEND[n][x]->Update();
			scoreNumEND[n][x]->SetPosition({ x * scoreNum[n][x]->GetScale().x - (0.5f * x) - 5, -5.2f, 0 });
			scoreNumEND[n][x]->SetRotation({ -90, 0, 0 });
		}
	}
	comboText[0].reset(Object3d::Create("Combo_01"));
	comboText[1].reset(Object3d::Create("Combo_02"));
	comboText[2].reset(Object3d::Create("Combo_03"));
	comboText[3].reset(Object3d::Create("Combo_04"));
	comboText[4].reset(Object3d::Create("Combo_05"));
	comboText[5].reset(Object3d::Create("Combo_06"));
	for (int i = 0; i < 6; i++)
	{
		comboText[i]->SetScale({ 0, 1, 0 });
		comboText[i]->SetRotation({ -90, 0, 0 });
		comboText[i]->Update();
	}
	for (int i = 0; i < 2; i++)
	{
		backGround[i].reset(Object3d::Create("Background"));
		backGround[i]->SetScale({ 50, 1, 50 });
		backGround[i]->Update();
		backGround[i]->SetPosition({ i * backGround[i]->GetScale().x - backGround[i]->GetScale().x * 0.5f, -2, 0 });
		backGround[i]->SetRotation({ -90, 0, 0 });
		backGround[i]->SetColor({ 1, 1, 1, 0.7f });
		backGround[i]->Update();
	}

	// スプライト
	animationBack.reset(Sprite::Create(1, { 0, 0 }, { 0.5f, 0.5f }));
	animationBack->SetPosition({ WinApp::window_width / 2, WinApp::window_height / 2 });
	animationLogo.reset(Sprite::Create(2, { 0, 0 }, { 0.5f, 0.5f }));
	animationLogo->SetPosition({ WinApp::window_width / 2, WinApp::window_height / 2 });
	animationLogo->SetColor({ 1, 1, 1, 0 });

	//FBXオブェクト


	// オーディオ
	//audio->Initialize();

	// 変数の初期化
	InitializeVariable();

	// 乱数初期化
	srand(time(NULL));
}

void GamePlayScene::InitializeVariable()
{
	workSE = 0;

	score = 0;

	addBlock = 1;

	stage = TITLE;

	addB = false;

	addX = 0;

	playerS = 0.18f;
	playerJS = 0;
	isPJ = false;

	blockTimer = 0;

	timerA = 1;

	ballS = 0;
	ballR = -5;
	ballJS = 0;
	ballJA = -0.1f;
	ballG = 0.5f;
	isBJ = false;
	isAliveB = true;
	groundB = false;
	isBuff = false;

	comboTimer = 0;
	comboLimit = 60;
	comboNum = 0;
	isActiveCT = false;

	textNum = 0;

	heartCounter = 3;

	titleTimer = 0;

	titleA = 1;

	endTimer = 0;

	buttonTimer = 0;
	effectTimer = 0;
	buttonA = 1;

	spawTimer = 1;
	spawnerScale = 0;
	spawnerSA = 0.05;

	endToTitle = false;
	titleToGame = false;
	restoreCameraTimer = 0;

	playerStay->SetScale({ 2.25f, 1, 2.25f });
	playerStay->Update();

	playerJump->SetScale({ 2.25f, 1, 2.25f });
	playerJump->Update();

	playerMove01->SetScale({ 2.25f, 1, 2.25f });
	playerMove01->Update();

	playerMove02->SetScale({ 2.25f, 1, 2.25f });
	playerMove02->Update();

	playerMove03->SetScale({ 2.25f, 1, 2.25f });
	playerMove03->Update();

	playerMove04->SetScale({ 2.25f, 1, 2.25f });
	playerMove04->Update();

	ball01->SetPosition({ 0, 5.2f * 3, 0 });
	ball01->SetRotation({ -90, 0, 0 });
	ball01->SetScale({ 2.25f, 1, 2.25f });
	ball01->Update();

	ball02->SetPosition({ 0, 5.2f * 3, 0 });
	ball02->SetRotation({ -90, 0, 0 });
	ball02->SetScale({ 2.25f, 1, 2.25f });
	ball02->Update();

	ball03->SetPosition({ 0, 5.2f * 3, 0 });
	ball03->SetRotation({ -90, 0, 0 });
	ball03->SetScale({ 2.25f, 1, 2.25f });
	ball03->Update();

	ball04->SetPosition({ 0, 5.2f * 3, 0 });
	ball04->SetRotation({ -90, 0, 0 });
	ball04->SetScale({ 2.25f, 1, 2.25f });
	ball04->Update();

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

		enemySpawner[x]->spawner->SetPosition({ 5.2f * (x - 6), 5.2f * (7 - 4), 0 });
		enemySpawner[x]->spawner->SetRotation({ -90, 0, 0 });
		enemySpawner[x]->spawner->SetScale({ 0, 1, 0 });
		enemySpawner[x]->spawner->Update();
		enemySpawner[x]->active = false;
	}

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			block[y][x]->block->SetPosition({ 5.2f * (x - 6), 5.2f * (y - 4) , 0 });
			block[y][x]->block->SetScale({ 5.2f, 1, 5.2f });
			block[y][x]->block->Update();
			buffBlock[y][x]->SetPosition({ 5.2f * (x - 6), 5.2f * (y - 4) , 0 });
			buffBlock[y][x]->SetScale({ 5.2f, 1, 5.2f });
			buffBlock[y][x]->Update();
		}
	}
}

void GamePlayScene::Update()
{
#pragma region ?ｿｽQ?ｿｽ[?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽC?ｿｽ?ｿｽ?ｿｽV?ｿｽX?ｿｽe?ｿｽ?ｿｽ

	if (animation == true)
	{
		if (animationEND == false)
		{
			if (animationTimer == 0)
			{
				XMFLOAT4 color = animationLogo->GetColor();
				color.w += addAlpha;
				if (color.w <= 0)
				{
					color.w = 0;
					animationEND = true;
				}
				else if (color.w >= 1)
				{
					color.w = 1;
					animationTimer = 1;
				}
				animationLogo->SetColor(color);
			}
			else
			{
				animationTimer++;
				if (animationTimer > 60)
				{
					animationTimer = 0;
					addAlpha = -abs(addAlpha);
				}
			}
		}
		else
		{
			animationRot += 1;
			if (animationRot >= 360)
			{
				animationRot = 0;
			}
			animationBack->SetRotation(animationRot);
			XMFLOAT2 aScale = animationBack->GetSize();
			aScale.x -= (float)10 * 1.0f;
			aScale.y -= (float)8 * 1.0f;
			if (aScale.x <= 0)
			{
				animation = false;
			}
			animationBack->SetSize(aScale);
		}
	}
	else
	{
		//audio->PlayWave("Resources/BGM/.wav", true, BGM);
	}

	// タイトル
	if (stage == TITLE && animation == false)
	{
		// 既定の大きさでなければ
		if (title->GetScale().x < 30 * 1.7f && titleButton->GetScale().x < 20 && titleToGame == false)
		{
			XMFLOAT3 tScale = title->GetScale();
			XMFLOAT3 bScale = titleButton->GetScale();
			tScale.x += 0.75f * 1.7f;
			tScale.z += 0.75f * 1.7f;
			bScale.x += 0.5f;
			bScale.z += 0.5f;
			if (tScale.x >= 30 * 1.7f)
			{
				tScale.x = 30.0f * 1.7f;
				tScale.z = 30.0f * 1.7f;
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
			if (titleToGame == false)
			{
				// イージング
				XMFLOAT3 bScale = titleButton->GetScale();
				float t = powf((float)titleTimer / 40, 2);
				bScale.x = 20.0f * (1.0f - t) + 23.0f * t;
				bScale.z = bScale.x;
				titleButton->SetScale(bScale);

				// タイマー
				titleTimer += titleA;

				// 一定の時間すぎると
				if (titleTimer >= 60)
				{
					titleA = -abs(titleA);
				}
				else if (titleTimer <= 0)
				{
					titleA = abs(titleA);
				}
			}
			else
			{
				XMFLOAT3 tScale = title->GetScale();
				XMFLOAT3 bScale = titleButton->GetScale();
				tScale.x -= 0.75f * 1.7f;
				tScale.z -= 0.75f * 1.7f;
				bScale.x -= 0.5f;
				bScale.z -= 0.5f;
				if (tScale.x <= 0)
				{
					tScale.x = 0;
					tScale.z = 0;
				}
				if (bScale.x <= 0)
				{
					bScale.x = 0;
					bScale.z = 0;
				}
				if (tScale.x == 0 && bScale.x == 0)
				{
					stage = GAME;
				}
				title->SetScale(tScale);
				title->Update();
				titleButton->SetScale(bScale);
				titleButton->Update();
			}
		}
	}

	// ブロック
	if ((stage == TITLE || stage == GAME) && animation == false)
	{
		// HP更新
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 13; x++)
			{
				if (block[y][x]->map == true)
				{
					if (stage == TITLE || y == 0)
					{
						block[y][x]->HP = 15;
					}
					// 0なら一列繰り下げ
					if (block[y][x]->HP <= 0)
					{
						for (int h = 1; h < 7; h++)
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
						block[y][x]->block->SetColor({ 1, 1, 1, 1 });
					}
				}
			}
		}

		// 一列そろったか
		bool deleteB = false;
		int w = 0;
		for (int x = 0; x < 13; x++)
		{
			if (addB == true)
			{
				break;
			}
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
				camera->SetShakeFlag(true, 10);
				// すべて消す
				for (int y = 1; y < 8; y++)
				{
					XMFLOAT3 bScale = block[y][x]->block->GetScale();
					bScale.x -= 0.52f;
					bScale.z = bScale.x;
					block[y][x]->block->SetScale(bScale);
					if (bScale.x <= 0)
					{
						block[y][x]->map = false;
						block[y][x]->HP = 15;
						block[y][x]->block->SetScale({ 5.2f, 1, 5.2f });
						deleteBlockEffect->Add(60, block[y][x]->block->GetPosition(), { 0,0,0 }, { 0,-0.005f,0 }, 3.0f, 7.0f);
						deleteB = true;
						w = x;
					}
					if (stage == TITLE)
					{
						for (int i = 0; i < 13; i++)
						{
							if (i != x && block[y][i]->map == true)
							{
								XMFLOAT3 bScale = block[y][i]->block->GetScale();
								bScale.x -= 0.52f;
								if (bScale.x <= 0)
								{
									bScale.x = 5.2f;
									block[y][i]->map = false;
									block[y][i]->HP = 15;
								}
								bScale.z = bScale.x;
								block[y][i]->block->SetScale(bScale);
								block[y][i]->block->Update();
							}
						}
					}
				}
			}
		}
		if (deleteB == true)
		{
			if (stage == GAME)
			{
				isActiveCT = true;
				// タイマーを1にする
				comboTimer = 1;
				// コンボカウンター
				comboNum++;
				if (comboNum == 1)
				{
					audio->PlayWave("Resources/SE/combose_01.wav", false, wavCombo);
				}
				else if (comboNum == 2)
				{
					audio->PlayWave("Resources/SE/combose_02.wav", false, wavCombo);
				}
				else if (comboNum == 3)
				{
					audio->PlayWave("Resources/SE/combose_03.wav", false, wavCombo);
				}
				else if (comboNum == 4)
				{
					audio->PlayWave("Resources/SE/combose_04.wav", false, wavCombo);
				}
				else if (comboNum == 5)
				{
					audio->PlayWave("Resources/SE/combose_05.wav", false, wavCombo);
				}
				else
				{
					audio->PlayWave("Resources/SE/combose_06.wav", false, wavCombo);
				}
				score += 100 * comboNum * comboNum;
				// テキストの座標
				textNum = comboNum - 1;
				if (comboNum > 5)
				{
					textNum = 5;
				}
				comboText[textNum]->SetPosition({ 5.2f * (w - 6), 5.2f * (6 - 4), 0 });
				comboText[textNum]->Update();
				effectTimer = 0;
			}
			else if (stage == TITLE)
			{
				for (int y = 1; y < 8; y++)
				{
					for (int x = 0; x < 13; x++)
					{
						block[y][x]->map = false;
						titleToGame = true;
					}
				}
			}
		}

		// コンボ処理
		if (comboTimer > 0 && stage == GAME)
		{
			// コンボの猶予時間
			comboTimer++;
			// 猶予を過ぎたか
			if (comboTimer > comboLimit)
			{
				// リセット
				comboTimer = 0;
				comboNum = 0;
			}
		}

		// コンボテキスト
		if (isActiveCT == true)
		{
			XMFLOAT3 pos = comboText[textNum]->GetPosition();
			if (pos.y < 5.2f * (7 - 4))
			{
				pos.y += 5.2f / 60.0f;
				if (pos.y >= 5.2f * (7 - 4))
				{
					pos.y = 5.2f * (7 - 4);
				}
				comboText[textNum]->SetPosition(pos);

				// 拡縮
				if (effectTimer < 60)
				{
					effectTimer++;
				}
				XMFLOAT3 cScale = { 0, 1, 0 };
				float t = ((float)effectTimer / 60) * (2 - (float)effectTimer / 60);
				cScale.x = 10.0f * (1.0f - t) + 15.0f * t;
				cScale.z = cScale.x;
				comboText[textNum]->SetScale(cScale);
			}
			else
			{
				XMFLOAT3 cScale = comboText[textNum]->GetScale();
				cScale.x -= 0.52f;
				cScale.z = cScale.x;
				if (cScale.x <= 0.0f)
				{
					cScale.x = 0;
					cScale.z = cScale.x;
					isActiveCT = false;
				}
				comboText[textNum]->SetScale(cScale);
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
			audio->PlayWave("Resources/SE/se_02.wav", false, wav2);
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

			workSE++;
			// インターバル
			if (workSE > 7)
			{
				audio->PlayWave("Resources/SE/se_01.wav", false, wav1);
				workSE = 0;
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
		else
		{
			workSE = 7;
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
				audio->PlayWave("Resources/SE/se_03.wav", false, wav3);
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
					if (addB == true)
					{
						break;
					}
					if (y < 7 && block[y][x]->map == true && block[y + 1][x]->map == false && block[1][x]->HP > 0)
					{
						if (block[0][x]->buff == true)
						{
							block[0][x]->buffTimer = 0;
							block[0][x]->buff = false;
							addBlock = 3;
							for (int i = 0; i < addBlock; i++)
							{
								if (y + 1 + i > 7)
								{
									break;
								}
								block[y + 1 + i][x]->map = true;
								if (y + 1 + i == 7)
								{
									audio->PlayWave("Resources/SE/se_09.wav", false, wav9);
								}
							}
							for (int i = 6; i > 0; i--)
							{
								block[i + 1][x]->HP = block[i][x]->HP;
								block[i][x]->HP = 15;
								XMFLOAT3 pos = block[i][x]->block->GetPosition();
								pos.y -= 5.2f * addBlock;
								block[i][x]->block->SetPosition(pos);
							}
						}
						else
						{
							block[y + 1][x]->map = true;
							for (int i = 6; i > 0; i--)
							{
								block[i + 1][x]->HP = block[i][x]->HP;
								block[i][x]->HP = 15;
								XMFLOAT3 pos = block[i][x]->block->GetPosition();
								pos.y -= 5.2f;
								block[i][x]->block->SetPosition(pos);
							}
							if (y + 1 == 7)
							{
								audio->PlayWave("Resources/SE/se_09.wav", false, wav9);
							}
						}
						audio->PlayWave("Resources/SE/se_10.wav", false, wav10);
						addB = true;
						addX = x;
						if (isBJ == false && static_cast<int>((ball01->GetPosition().x + block[0][0]->block->GetScale().x * 6 + 0.5f * block[0][0]->block->GetScale().x) / block[0][0]->block->GetScale().x) == x && stage == GAME)
						{
							audio->PlayWave("Resources/SE/se_04.wav", false, wav4);
							block[y + 1][x]->map = true;
							isBJ = true;
							ballJS = 1.5f;
							isBuff = true;
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
								XMFLOAT3 ePos = enemy01[i]->enemy01->GetPosition();
								XMFLOAT3 deadEffectPos = { 0,0,0 };
								XMFLOAT3 velocity = { 0,0,0 };
								XMFLOAT3 accel = { 0,0,0 };
								for (int i = 0; i < 75; i++) {
									deadEffectPos.x = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + ePos.x;
									deadEffectPos.y = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + ePos.y;
									velocity.x = ((float)rand() / RAND_MAX * 0.5f - 0.5f / 2.0f);
									velocity.y = (float)rand() / RAND_MAX * 0.8f;
									accel.y =  -0.03f;
									deadEffect->Add(60, { ePos.x, ePos.y, ePos.z }, velocity, accel, 0.25f, 1.0f);
								}
								enemyStay[i]->alive = false;
								enemyStay[i]->isEnemyLanding = false;
								enemy01[i]->alive = false;
								enemy02[i]->alive = false;
								audio->PlayWave("Resources/SE/se_11.wav", false, wav11);
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
		if (addB == true)
		{
			blockTimer += timerA;
			for (int y = 1; y < 8; y++)
			{
				XMFLOAT3 pos = block[y][addX]->block->GetPosition();
				pos.y += 0.52f * 3;
				if (5.2f * (y - 4) <= pos.y)
				{
					pos.y = 5.2f * (y - 4);
				}
				block[y][addX]->block->SetPosition(pos);
			}
			XMFLOAT3 bScale = block[0][addX]->block->GetScale();
			float t = ((float)blockTimer / 5) * (2 - (float)blockTimer / 5);
			bScale.x = 5.2f * (1.0f - t) + 6.2f * t;
			bScale.z = bScale.x;
			if (blockTimer >= 5)
			{
				timerA = -abs(timerA);
			}
			else if (blockTimer <= 0)
			{
				timerA = abs(timerA);
				addB = false;
			}
			block[0][addX]->block->SetScale(bScale);
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
			audio->PlayWave("Resources/SE/se_06.wav", false, wav6);
		}
		else if (bPos.x < -32.75f)
		{
			bPos.x = -32.75f;
			ballS = -ballS;
			ballR = -ballR;
			ballBounceEffect->Add(60, bPos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
			audio->PlayWave("Resources/SE/se_06.wav", false, wav6);
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
			if (15.6f + 0.5f * 5.2f < bPos.y)
			{
				bPos.y = 15.6f + 0.5f * 5.2f;
				ballJS = 0;
			}
		}
		//当たり判定
		for (int y = 0; y < 7; y++)
		{
			for (int x = 0; x < 13; x++)
			{
				if (block[y][x]->buff == true)
				{
					block[y][x]->buffTimer++;
					if (block[y][x]->buffTimer > 240)
					{
						block[y][x]->buffTimer = 0;
						block[y][x]->buff = false;
					}
				}
				if (block[y][x]->map == true)
				{
					// 縦
					if (block[y + 1][x]->map == false)
					{
						if (block[y][x]->block->GetPosition().x - 0.5f * block[y][x]->block->GetScale().x <= bPos.x && bPos.x <= block[y][x]->block->GetPosition().x + 0.5f * block[y][x]->block->GetScale().x)
						{
							if (bPos.y - 0.5f * ball01->GetScale().x < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
							{
								bPos.y = block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x + 0.5f * ball01->GetScale().x;
								ballG = gravity;
								if (isBJ == true)
								{
									audio->PlayWave("Resources/SE/se_05.wav", false, wav5);
								}
								else if (groundB == false)
								{
									audio->PlayWave("Resources/SE/se_14.wav", false, wav14);
								}
								if (isBuff == true && isBJ == true)
								{
									isBuff = false;
									block[0][x]->buff = true;
								}
								groundB = true;
								isBJ = false;
								// 速度決定
								if (ballS == 0)
								{
									ballS = 0.2f;
								}
							}
							else
							{
								groundB = false;
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
								if (groundB == true)
								{
									block[y][x]->HP--;
								}
								if (block[y][x]->HP <= 0)
								{
									audio->PlayWave("Resources/SE/se_08.wav", false, wav8);
								}
								else if (groundB == true)
								{
									audio->PlayWave("Resources/SE/se_06.wav", false, wav6);
								}
								ballS = -abs(ballS);
								ballR = abs(ballR);
								ballBounceEffect->Add(60, bPos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
							}
							else
							{
								bPos.x = block[y][x]->block->GetPosition().x + 0.5f * ball01->GetScale().x + 0.5f * block[y][x]->block->GetScale().x;
								if (groundB == true)
								{
									block[y][x]->HP--;
								}
								if (block[y][x]->HP <= 0)
								{
									audio->PlayWave("Resources/SE/se_08.wav", false, wav8);
								}
								else if (groundB == true)
								{
									audio->PlayWave("Resources/SE/se_06.wav", false, wav6);
								}
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
					audio->PlayWave("Resources/SE/se_12.wav", false, wav12);
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
					enemyStay[i]->enemyStay->SetPosition({ 5.2f * (i - 6), 5.2f * (7 - 4), 0 });
					enemyStay[i]->enemyStay->SetRotation({ -90, 0, 0 });
					enemyStay[i]->enemyStay->SetScale({ 0, 1, 0 });
					enemyStay[i]->enemyStay->Update();
					enemyStay[i]->enemyG = 0.5f;

					enemy01[i]->alive = true;
					enemy01[i]->enemy01->SetPosition({ 5.2f * (i - 6), 5.2f * (7 - 4), 0 });
					enemy01[i]->enemy01->SetRotation({ -90, 0, 0 });
					enemy01[i]->enemy01->SetScale({ 0, 1, 0 });
					enemy01[i]->enemy01->Update();
					enemy01[i]->enemyG = 0.5f;

					enemy02[i]->alive = true;
					enemy02[i]->enemy02->SetPosition({ 5.2f * (i - 6), 5.2f * (7 - 4), 0 });
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
						audio->PlayWave("Resources/SE/se_06.wav", false, wav6);
					}
					else if (ePos.x < -32.75f)
					{
						ePos.x = -32.75f;
						enemy01[i]->enemyS = abs(enemy01[i]->enemyS);
						enemy01[i]->enemyR = -abs(enemy01[i]->enemyR);
						enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
						audio->PlayWave("Resources/SE/se_06.wav", false, wav6);
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
											if (enemy01[i]->groundE == false)
											{
												audio->PlayWave("Resources/SE/se_14.wav", false, wav14);
											}
											enemy01[i]->groundE = true;
											enemy02[i]->groundE = true;
										}
										else
										{
											enemy01[i]->groundE = false;
											enemy02[i]->groundE = false;
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
											enemy01[i]->enemyS = -enemy01[i]->enemyS;
											enemy01[i]->enemyR = -enemy01[i]->enemyR;
											block[y][x]->HP--;
											if (block[y][x]->HP <= 0)
											{
												audio->PlayWave("Resources/SE/se_08.wav", false, wav8);
											} 
											else if (enemy01[i]->groundE == true)
											{
												audio->PlayWave("Resources/SE/se_06.wav", false, wav6);
											}
											enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
										}
										else
										{
											ePos.x = block[y][x]->block->GetPosition().x + 0.5f * ball01->GetScale().x + 0.5f * block[y][x]->block->GetScale().x;
											enemy01[i]->enemyS = -enemy01[i]->enemyS;
											enemy01[i]->enemyR = -enemy01[i]->enemyR;
											enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
											block[y][x]->HP--;
											if (block[y][x]->HP <= 0)
											{
												audio->PlayWave("Resources/SE/se_08.wav", false, wav8);
											}
											else if (enemy01[i]->groundE == true)
											{
												audio->PlayWave("Resources/SE/se_06.wav", false, wav6);
											}
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
						audio->PlayWave("Resources/SE/se_07.wav", false, wav7);
						heartCounter--;
						if (heartCounter <= 0)
						{
							heartCounter = 0;
							stage = END;
						}
						else
						{
							XMFLOAT3 deadEffectPos = { 0,0,0 };
							XMFLOAT3 velocity = { 0,0,0 };
							XMFLOAT3 accel = { 0,0,0 };
							for (int i = 0; i < 75; i++) {
								deadEffectPos.x = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + ePos.x;
								deadEffectPos.y = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + ePos.y;
								velocity.x = ((float)rand() / RAND_MAX * 0.5f - 0.5f / 2.0f);
								velocity.y = (float)rand() / RAND_MAX * 0.8f;
								accel.y = -0.03f;
								deadEffect->Add(60, {ePos.x, ePos.y, ePos.z }, velocity, accel, 0.25f, 1.0f);
							}
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
			if (endTimer < 60) {
				float elapsedTimeRate = static_cast<float>(endTimer) / 60.0f;
				XMFLOAT3 defaultEyePos = camera->GetDefaultEye();
				XMFLOAT3 defaultTargetPos = camera->GetDefaultTarget();
				camera->SetEye({
					static_cast<float>((bDeadPos.x - defaultEyePos.x) * (1 - pow(1 - elapsedTimeRate,4)) + defaultEyePos.x),
					static_cast<float>((bDeadPos.y - defaultEyePos.y) * (1 - pow(1 - elapsedTimeRate,4)) + defaultEyePos.y),
					static_cast<float>((-10 - defaultEyePos.z) * (1 - pow(1 - elapsedTimeRate,4)) + defaultEyePos.z) });
				camera->SetTarget({
					static_cast<float>((bDeadPos.x - defaultTargetPos.x) * (1 - pow(1 - elapsedTimeRate,4)) + defaultTargetPos.x),
					static_cast<float>((bDeadPos.y - defaultTargetPos.y) * (1 - pow(1 - elapsedTimeRate,4)) + defaultTargetPos.x),
					0 });
			}

			if (endTimer > 60)
			{
				isAliveB = false;
				XMFLOAT3 deadEffectPos = { 0,0,0 };
				XMFLOAT3 velocity = { 0,0,0 };
				XMFLOAT3 accel = { 0,0,0 };
				for (int i = 0; i < 75; i++) {
					deadEffectPos.x = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + bDeadPos.x;
					deadEffectPos.y = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + bDeadPos.y;
					velocity.x = ((float)rand() / RAND_MAX * 0.5f - 0.5f / 2.0f);
					velocity.y = (float)rand() / RAND_MAX * 0.8f;
					accel.y = -0.03f;
					deadEffect->Add(120, { bDeadPos.x, bDeadPos.y, bDeadPos.z }, velocity, accel, 0.25f, 1.0f);
				}
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
			if (restoreCameraTimer < 60) {
				restoreCameraTimer++;
				float elapsedTimeRate = static_cast<float>(restoreCameraTimer) / 60.0f;
				XMFLOAT3 defaultEyePos = camera->GetDefaultEye();
				XMFLOAT3 defaultTargetPos = camera->GetDefaultTarget();
				XMFLOAT3 bDeadPos = ball01->GetPosition();
				camera->SetEye({
					static_cast<float>((defaultEyePos.x - bDeadPos.x) * (1 - pow(1 - elapsedTimeRate,3)) + bDeadPos.x),
					static_cast<float>((defaultEyePos.y - bDeadPos.y) * (1 - pow(1 - elapsedTimeRate,3)) + bDeadPos.y),
					static_cast<float>((defaultEyePos.z - -10) * (1 - pow(1 - elapsedTimeRate,3)) + -10) });
				camera->SetTarget({
					static_cast<float>((defaultTargetPos.x - bDeadPos.x) * (1 - pow(1 - elapsedTimeRate,3)) + bDeadPos.x),
					static_cast<float>((defaultTargetPos.y - bDeadPos.y) * (1 - pow(1 - elapsedTimeRate,3)) + bDeadPos.y),
					0 });
			}
			// 一定の大きさに満たなければ
			if (end->GetScale().x < 20 * 1.5f && endButton->GetScale().x < 25 && gameover->GetScale().x < 30 * 1.7f && endToTitle == false)
			{
				XMFLOAT3 a = end->GetScale();
				XMFLOAT3 b = endButton->GetScale();
				XMFLOAT3 c = gameover->GetScale();
				a.x += 0.5f * 1.5f;
				a.z += 0.5f * 1.5f;
				b.x += 0.625f;
				b.z += 0.625f;
				c.x += 0.75f * 1.7f;
				c.z += 0.75f * 1.7f;
				if (a.x >= 20 * 1.5f)
				{
					a.x = 20 * 1.5f;
					a.z = 20 * 1.5f;
					b.x = 25;
					b.z = 25;
					c.x = 30 * 1.7f;
					c.z = 30 * 1.7f;
				}
				end->SetScale(a);
				endButton->SetScale(b);
				gameover->SetScale(c);
				for (int y = 0; y < 10; y++)
				{
					for (int x = 0; x < 6; x++)
					{
						XMFLOAT3 d = scoreNumEND[y][x]->GetScale();
						d.x += 0.1375f * 1.5f;
						d.z = d.x;
						scoreNumEND[y][x]->SetScale(d);
					}
				}
			}
			else
			{
				if (endToTitle == false)
				{
					// イージング
					XMFLOAT3 bScale = endButton->GetScale();
					float t = powf((float)buttonTimer / 40, 2);
					bScale.x = 25.0f * (1.0f - t) + 28.0f * t;
					bScale.z = bScale.x;
					endButton->SetScale(bScale);
					endButton->Update();

					// タイマー
					buttonTimer += buttonA;

					// 一定の時間すぎると
					if (buttonTimer >= 60)
					{
						buttonA = -abs(buttonA);
					}
					else if (buttonTimer <= 0)
					{
						buttonA = abs(buttonA);
					}
				}
				else
				{
					XMFLOAT3 a = end->GetScale();
					XMFLOAT3 b = endButton->GetScale();
					XMFLOAT3 c = gameover->GetScale();
					a.x -= 0.5f * 1.5f;
					a.z -= 0.5f * 1.5f;
					b.x -= 0.7f;
					b.z -= 0.7f;
					if (b.x <= 0)
					{
						b.x = 0;
						b.z = b.x;
					}
					c.x -= 0.75f * 1.7f;
					c.z -= 0.75f * 1.7f;
					if (a.x <= 0)
					{
						InitializeVariable();
						a.x = 0;
						a.z = a.x;
						c.x = 0;
						c.z = c.x;
					}
					end->SetScale(a);
					endButton->SetScale(b);
					gameover->SetScale(c);
					for (int y = 0; y < 10; y++)
					{
						for (int x = 0; x < 6; x++)
						{
							XMFLOAT3 d = scoreNumEND[y][x]->GetScale();
							d.x -= 0.1375f * 1.5f;
							if (d.x <= 0)
							{
								d.x = 0;
							}
							d.z = d.x;
							scoreNumEND[y][x]->SetScale(d);
							scoreNumEND[y][x]->Update();
						}
					}
					end->Update();
					endButton->Update();
					gameover->Update();
				}

				// TITLEに戻る
				if (input->TriggerKey(DIK_SPACE) && endToTitle == false)
				{
					endToTitle = true;
					audio->PlayWave("Resources/SE/se_13.wav", false, wav13);
				}
			}
		}
	}

#pragma endregion

#pragma region ?ｿｽJ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾆ??ｿｽ?ｿｽC?ｿｽg?ｿｽﾌ更?ｿｽV
	if (stage == TITLE || stage == GAME) {
		camera->CameraShake();
	}
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

	for (int i = 0; i < 2; i++)
	{
		backGround[i]->Draw();
	}
	for (int y = 7; y >= 0; y--)
	{
		for (int x = 0; x < 13; x++)
		{
			if (block[y][x]->map == true)
			{
				if (block[y][x]->block->GetPosition().y >= 5.2f * -4)
				{
					if (block[y][x]->buff == false)
					{
						block[y][x]->block->Draw();
					}
					else
					{
						block[y][x]->block->Update();
						buffBlock[y][x]->SetPosition(block[y][x]->block->GetPosition());
						buffBlock[y][x]->SetScale(block[y][x]->block->GetScale());
						buffBlock[y][x]->SetColor(block[y][x]->block->GetColor());
						buffBlock[y][x]->Draw();
					}
				}
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
	if (isActiveCT == true && stage == GAME)
	{
		comboText[textNum]->Draw();
	}
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
			int s = score;
			scoreNumEND[s / 100000][0]->Draw();
			s = s % 100000;
			scoreNumEND[s / 10000][1]->Draw();
			s = s % 10000;
			scoreNumEND[s / 1000][2]->Draw();
			s = s % 1000;
			scoreNumEND[s / 100][3]->Draw();
			s = s % 100;
			scoreNumEND[s / 10][4]->Draw();
			s = s % 10;
			scoreNumEND[s][5]->Draw();
		}
	}
	int s = score;
	scoreNum[s / 100000][0]->Draw();
	s = s % 100000;
	scoreNum[s / 10000][1]->Draw();
	s = s % 10000;
	scoreNum[s / 1000][2]->Draw();
	s = s % 1000;
	scoreNum[s / 100][3]->Draw();
	s = s % 100;
	scoreNum[s / 10][4]->Draw();
	s = s % 10;
	scoreNum[s][5]->Draw();
	scoreList->Draw();
	HP->Draw();

	Object3d::PostDraw();

	// FBXオブジェクト
	/*FbxObject3d::PreDraw(cmdList);



	FbxObject3d::PostDraw();*/

	// スプライト描画
	Sprite::PreDraw(cmdList);

	if (animation == true)
	{
		animationBack->Draw();
		animationLogo->Draw();
	}

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
	deleteBlockEffect->Draw();
	enemyBounceEffect->Draw();
	enemySpawnEffect->Draw();
	deadEffect->Draw();

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