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
	//particle.reset(ParticleManager::Create("Default/effect1.png"));

	// スプライト
	player.reset(Object3d::Create("PlayerStay"));
	ball.reset(Object3d::Create("BallMove_01"));
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
	for (auto& m : enemy)
	{
		m.reset(new ENEMY(Object3d::Create("EnemyStay")));
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
				if (x == 1 ||  x == 11)
				{
					block[y][x]->map = true;
				}
			}
			else if (y == 4)
			{
				if (x == 1 ||  x == 11)
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

	player->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	player->SetRotation({ -90, 0, 0 });
	player->SetScale({ 2.25f, 1, 2.25f });
	player->Update();

	ball->SetPosition({ 0, 5.2f * 2, 0 });
	ball->SetRotation({ -90, 0, 0 });
	ball->SetScale({ 2.25f, 1, 2.25f });
	ball->Update();

	for (int x = 0; x < 13; x++)
	{
		enemy[x]->enemy->SetPosition({ 0, 0, 0});
		enemy[x]->enemy->SetRotation({ -90, 0, 0 });
		enemy[x]->enemy->SetScale({ 0, 1, 0 });
		enemy[x]->enemy->Update();
		enemy[x]->enemyS = 0;
		enemy[x]->enemyR = -5;
		enemy[x]->enemyG = 0.5f;
		enemy[x]->alive = false;

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
			if (31.2f < pPos.x)
			{
				pPos.x = 31.2f;
			}
			else if (pPos.x < -31.2f)
			{
				pPos.x = -31.2f;
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
			if (-23.4f < pPos.y + 0.5f * player->GetScale().x - 0.7f)
			{
				pPos.y = -23.4f - 0.5f * player->GetScale().x + 0.7f;
				playerJS = 0.05f;
				playerJS = -playerJS;
				// Xが今どこの横列か
				int x = static_cast<int>((pPos.x + block[0][0]->block->GetScale().x * 6 + 0.5f * block[0][0]->block->GetScale().x) / block[0][0]->block->GetScale().x);
				//一つ上に空きがあるか
				for (int y = 6; 0 <= y; y--)
				{
					if (y < 7 && block[y][x]->map == true && block[y + 1][x]->map == false && block[1][x]->HP > 0)
					{
						block[y + 1][x]->map = true;
						if (isBJ == false && static_cast<int>((ball->GetPosition().x + block[0][0]->block->GetScale().x * 6 + 0.5f * block[0][0]->block->GetScale().x) / block[0][0]->block->GetScale().x) == x)
						{
							isBJ = true;
							ballJS = 2.0f;
							XMFLOAT3 bPos = ball->GetPosition();
							bPos.y += 4.5f;
							ball->SetPosition(bPos);
							ball->Update();
						}
						for (int i = 0; i < 13; i++)
						{
							if (isBJ == false && static_cast<int>((enemy[i]->enemy->GetPosition().x + block[0][0]->block->GetScale().x * 6 + 0.5f * block[0][0]->block->GetScale().x) / block[0][0]->block->GetScale().x) == x && enemy[i]->alive == true)
							{
								enemy[i]->alive = false;
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
		player->SetPosition(pPos);
		player->Update();
	}

	// ボール
	if (stage == GAME)
	{
		// 移動
		XMFLOAT3 bPos = ball->GetPosition();
		bPos.x += ballS;
		if (32.75f < bPos.x)
		{
			bPos.x = 32.75f;
			ballS = -ballS;
			ballR = -ballR;
		}
		else if (bPos.x < -32.75f)
		{
			bPos.x = -32.75f;
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
					// 縦
					if (y < 7 && block[y + 1][x]->map == false)
					{
						if (block[y][x]->block->GetPosition().x - 0.5f * block[y][x]->block->GetScale().x <= bPos.x && bPos.x <= block[y][x]->block->GetPosition().x + 0.5f * block[y][x]->block->GetScale().x)
						{
							if (bPos.y - 0.5f * ball->GetScale().x < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
							{
								bPos.y = block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x + 0.5f * ball->GetScale().x;
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
					if (fabsf(bPos.x - block[y][x]->block->GetPosition().x) < 0.5f * ball->GetScale().x + 0.5f * block[y][x]->block->GetScale().x)
					{
						if (block[y][x]->block->GetPosition().y - 0.5f * block[y][x]->block->GetScale().x < bPos.y && bPos.y < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
						{
							if (bPos.x - block[y][x]->block->GetPosition().x < 0)
							{
								bPos.x = block[y][x]->block->GetPosition().x - 0.5f * ball->GetScale().x - 0.5f * block[y][x]->block->GetScale().x;
								if (isBJ == false)
								{
									ballS = -abs(ballS);
									ballR = abs(ballR);
								}
							}
							else
							{
								bPos.x = block[y][x]->block->GetPosition().x + 0.5f * ball->GetScale().x + 0.5f * block[y][x]->block->GetScale().x;
								if (isBJ == false)
								{
									ballS = abs(ballS);
									ballR = -abs(ballR);
								}
							}
						}
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
				if (enemy[n]->alive == false)
				{
					enemySpawner[n]->active = true;
					break;
				}
				else
				{
					bool hit = false;
					for (auto& m : enemy)
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
				if (scale.x > 2.25f && enemy[i]->alive == false)
				{
					enemy[i]->alive = true;
					enemy[i]->enemy->SetPosition({ 5.2f * (i - 6), 13.5f, 0 });
					enemy[i]->enemy->SetScale({ 0, 1, 0 });
					enemy[i]->enemy->Update();
					enemy[i]->enemyG = 0.5f;
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
				}
				enemySpawner[i]->spawner->SetScale(scale);
			}
			// エネミー
			if (enemy[i]->alive == true)
			{
				// スポナーに合わせて拡大
				if (enemySpawner[i]->spawner->GetScale().x != 0)
				{
					XMFLOAT3 scale = enemy[i]->enemy->GetScale();
					scale.x += 0.05f;
					scale.z += 0.05f;
					if (scale.x > 2.25f)
					{
						scale.x = 2.25f;
						scale.z = 2.25f;
					}
					enemy[i]->enemy->SetScale(scale);
				}
				// 更新
				else
				{
					// 移動
					XMFLOAT3 ePos = enemy[i]->enemy->GetPosition();
					ePos.x += enemy[i]->enemyS;
					if (32.75f < ePos.x)
					{
						ePos.x = 32.75f;
						enemy[i]->enemyS = -abs(enemy[i]->enemyS);
						enemy[i]->enemyR = abs(enemy[i]->enemyR);
					}
					else if (ePos.x < -32.75f)
					{
						ePos.x = -32.75f;
						enemy[i]->enemyS = abs(enemy[i]->enemyS);
						enemy[i]->enemyR = -abs(enemy[i]->enemyR);
					}
					// 重力
					ePos.y -= enemy[i]->enemyG;
					enemy[i]->enemyG += gravity;
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
										if (ePos.y - 0.5f * ball->GetScale().x < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
										{
											ePos.y = block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x + 0.5f * ball->GetScale().x;
											enemy[i]->enemyG = gravity;
											// 速度決定
											if (enemy[i]->enemyS == 0)
											{
												enemy[i]->enemyS = 0.2f;
											}
										}
									}
								}
								// 横
								if (fabsf(ePos.x - block[y][x]->block->GetPosition().x) < 0.5f * ball->GetScale().x + 0.5f * block[y][x]->block->GetScale().x)
								{
									if (block[y][x]->block->GetPosition().y - 0.5f * block[y][x]->block->GetScale().x < ePos.y && ePos.y < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
									{
										if (ePos.x - block[y][x]->block->GetPosition().x < 0)
										{
											ePos.x = block[y][x]->block->GetPosition().x - 0.5f * ball->GetScale().x - 0.5f * block[y][x]->block->GetScale().x;
											block[y][x]->HP--;
											enemy[i]->enemyS = -enemy[i]->enemyS;
											enemy[i]->enemyR = -enemy[i]->enemyR;
										}
										else
										{
											ePos.x = block[y][x]->block->GetPosition().x + 0.5f * ball->GetScale().x + 0.5f * block[y][x]->block->GetScale().x;
											block[y][x]->HP--;
											enemy[i]->enemyS = -enemy[i]->enemyS;
											enemy[i]->enemyR = -enemy[i]->enemyR;
										}
									}
								}
							}
						}
					}
					enemy[i]->enemy->SetPosition(ePos);
					// 回転
					XMFLOAT3 eRot = enemy[i]->enemy->GetRotation();
					eRot.z += enemy[i]->enemyR;
					if (eRot.z >= 360 || eRot.z <= -360)
					{
						eRot.z = 0;
					}
					enemy[i]->enemy->SetRotation(eRot);
					// プレイヤーのボールとの当たり判定
					if (LenAB(ball->GetPosition(), ePos) < ball->GetScale().x * 0.5f + 0.5f * enemy[i]->enemy->GetScale().x)
					{
						heartCounter--;
						if (heartCounter <= 0)
						{
							heartCounter = 0;
							stage = END;
						}
						else
						{
							enemy[i]->alive = false;
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
			if (endTimer > 60)
			{
				isAliveB = false;
				for (int i = 0; i < 13; i++)
				{
					enemy[i]->alive = false;
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
		if (enemy[i]->alive == true)
		{
			enemy[i]->enemy->Draw();
		}
	}
	if (stage == GAME || stage == END)
	{
		if (isAliveB == true)
		{
			ball->Draw();
		}
	}
	player->Draw();
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