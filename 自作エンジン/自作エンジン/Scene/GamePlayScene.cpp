#include "GamePlayScene.h"
#include "SceneManager.h"
#include <time.h>

#include <SafeDelete.h>
#include <cassert>
#include <time.h>
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
	enemyBounceEffect.reset(ParticleManager::Create("stareffect_03.png"));
	enemySpawnEffect.reset(ParticleManager::Create("enemyspowneffect.png"));

	// スプライト
	player.reset(Object3d::Create("Player"));
	ball.reset(Object3d::Create("BallMove_01"));
	frame.reset(Object3d::Create("GameFrame"));
	for (auto& m : enemy)
	{
		m.reset(new ENEMY(Object3d::Create("EnemyStay")));
	}
	for (auto& m : enemySpawner)
	{
		m.reset(new ENEMYSPAWNER(Object3d::Create("EnemySpawner")));
	}
	for (auto& m : needle)
	{
		m.reset(Object3d::Create("Needle"));
	}
	for (auto& m : heart)
	{
		m.reset(Object3d::Create("Heart"));
	}
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

	// 乱数初期化
	srand(time(NULL));
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

	playerS = 0.15f;
	playerJS = 0;
	isPJ = false;

	ballS = 0.2f;
	ballR = -5;
	ballJS = 0;
	ballJA = -0.1f;
	ballG = 0.5f;
	isBJ = false;

	comboTimer = 0;
	comboLimit = 60;
	comboNum = 0;

	heartCounter = 3;

	spawTimer = 1;
	spawnerScale = 0;
	spawnerSA = 0.05;

	player->SetPosition({ 0, 4.5f * (-5), 0 });
	player->SetRotation({ -90, 0, 0 });
	player->SetScale({ 2.25f, 1, 2.25f });
	player->Update();

	ball->SetPosition({ 0, 5, 0 });
	ball->SetRotation({ -90, 0, 0 });
	ball->SetScale({ 2.25f, 1, 2.25f });
	ball->Update();

	frame->SetPosition({ 0, 0, 0 });
	frame->SetRotation({ -90, 0, 0 });
	frame->SetScale({ 10 * 7.23, 1, 8 * 7.23 });
	frame->Update();

	for (int x = 0; x < 13; x++)
	{
		needle[x]->SetPosition({4.5f * (x - 6), 14.5f, 0});
		needle[x]->SetRotation({ -90, 0, 0 });
		needle[x]->SetScale({ 4.5f, 1, 4.5f });
		needle[x]->Update();
	}

	for (int x = 0; x < heartCounter; x++)
	{
		heart[x]->SetPosition({ x * 2.5f + 4.5f * (x - 1) - 22.5f, 24.25f, 0});
		heart[x]->SetRotation({ -90, 0, 0 });
		heart[x]->SetScale({ 4.5f, 1, 4.5f });
		heart[x]->Update();
	}

	for (int x = 0; x < 13; x++)
	{
		enemy[x]->enemy->SetPosition({ 4.5f * (x - 6), 13.5f, 0});
		enemy[x]->enemy->SetRotation({ -90, 0, 0 });
		enemy[x]->enemy->SetScale({ 0, 1, 0 });
		enemy[x]->enemy->Update();
		enemy[x]->enemyS = 0.2f;
		enemy[x]->enemyR = -5;
		enemy[x]->enemyG = 0.5f;
		enemy[x]->alive = false;

		enemySpawner[x]->spawner->SetPosition({ 4.5f * (x - 6), 13.5f, 0 });
		enemySpawner[x]->spawner->SetRotation({ -90, 0, 0 });
		enemySpawner[x]->spawner->SetScale({ 0, 1, 0 });
		enemySpawner[x]->spawner->Update();
		enemySpawner[x]->active = false;
	}
}

void GamePlayScene::Update()
{
#pragma region ゲームメインシステム
	
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
						for (int h = 1; h < 8; h++)
						{
							XMFLOAT3 bPos = block[h + 1][x]->block->GetPosition();
							bPos.y -= 0.45f;
							block[h + 1][x]->block->SetPosition(bPos);
							if (4.5f < 4.5f * (h - 3) - bPos.y && h < 7)
							{
								block[y][x]->HP = 15;
								block[h][x]->map = block[h + 1][x]->map;
								block[h + 1][x]->map = false;
								block[h][x]->block->SetPosition({ 4.5f * (x - 6), 4.5f * (h - 4) , 0 });
								if (h == 6)
								{
									block[7][x]->block->SetPosition({ 4.5f * (x - 6), 4.5f * (7 - 4) , 0 });
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
				// タイマーを1にする
				comboTimer = 1;
				// コンボカウンター
				comboNum++;
			}
		}

		// コンボ処理
		if (comboTimer > 0)
		{
			// コンボの猶予時間
			comboTimer++;
			if (comboTimer > 20)
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
	{
		// ジャンプ
		if (input->TriggerKey(DIK_SPACE) && isPJ == false)
		{
			playerJS = 0.05f;
			isPJ = true;
		}

		// 移動
		XMFLOAT3 pPos = player->GetPosition();
		XMFLOAT3 walkParticlePos = player->GetPosition();
		XMFLOAT3 jumpParticlePos = player->GetPosition();
		XMFLOAT3 velocity = { 0,0,0 };
		XMFLOAT3 walkParticleAccel = { 0,0,0 };
		XMFLOAT3 jumpParticleAccel = { 0,0,0 };
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			pPos.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * playerS;
			if (27 < pPos.x)
			{
				pPos.x = 27;
			} else if (pPos.x < -27)
			{
				pPos.x = -27;
			}

			playerWalkEffectTimer--;

			if (playerWalkEffectTimer <= 0 && !isPJ) {
				walkParticlePos.x = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + pPos.x;
				walkParticlePos.y = ((float)rand() / RAND_MAX * 1 - 1 / 2.0f) + pPos.y;
				velocity.x -= (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.2f;
				walkParticleAccel.x -= (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.001f;
				walkParticleAccel.y = (float)rand() / RAND_MAX * 0.005f;
				playerWalkEffect->Add(30, { walkParticlePos.x, walkParticlePos.y - 1.5f, walkParticlePos.z - 1.0f }, velocity, walkParticleAccel, 0.5f, 3.5f,{1,1,1,1},{0.5,0.5,0.5,0.3});
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
			if (-22.0f + 0.5f * player->GetScale().x < pPos.y)
			{
				pPos.y = -22.0f + 0.5f * player->GetScale().x;
				playerJS = 0.05f;
				playerJS = -playerJS;
				jumpParticleAccel.y = -0.005f;
				playerJumpEffect->Add(30, { jumpParticlePos.x,jumpParticlePos.y-0.5f,jumpParticlePos.z - 1.0f }, { 0,0,0 }, jumpParticleAccel, 4.0f, 6.0f, { 1,1,1,1 }, { 1,1,1,1 });
				// Xが今どこの横列か
				int x = static_cast<int>((pPos.x + 29.25f) / block[0][0]->block->GetScale().x);
				//一つ上に空きがあるか
				for (int y = 6; 0 <= y; y--)
				{
					if (y < 7 && block[y][x]->map == true && block[y + 1][x]->map == false && block[1][x]->HP > 0)
					{
						block[y + 1][x]->map = true;
						if (isBJ == false && static_cast<int>((ball->GetPosition().x + 29.25f) / block[0][0]->block->GetScale().x) == x)
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
							if (isBJ == false && static_cast<int>((enemy[i]->enemy->GetPosition().x + 29.25f) / block[0][0]->block->GetScale().x) == x && enemy[i]->alive == true)
							{
								enemy[i]->alive = false;
							}
						}
						break;
					}
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
		player->Update();
	}

	// ボール
	{
		// 移動
		XMFLOAT3 bPos = ball->GetPosition();
		bPos.x += ballS;
		if (28.275f < bPos.x)
		{
			bPos.x = 28.275f;
			ballS = -ballS;
			ballR = -ballR;
			ballBounceEffect->Add(60, bPos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
		}
		else if (bPos.x < -28.275f)
		{
			bPos.x = -28.275f;
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
							if (bPos.y - 0.5f * ball->GetScale().x < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
							{
								bPos.y = block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x + 0.5f * ball->GetScale().x;
								ballG = gravity;
								isBJ = false;
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
									ballS = -ballS;
									ballR = -ballR;
								}
							}
							else
							{
								bPos.x = block[y][x]->block->GetPosition().x + 0.5f * ball->GetScale().x + 0.5f * block[y][x]->block->GetScale().x;
								if (isBJ == false)
								{
									ballS = -ballS;
									ballR = -ballR;
								}
							}
						}
					}
				}
			}
		}
		if (bPos.y < -14.625f)
		{
			bPos.y = -14.625f;
			ballG = gravity;
			isBJ = false;
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
					enemy[i]->enemy->SetPosition({ 4.5f * (i - 6), 13.5f, 0 });
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
					if (28.275f < ePos.x)
					{
						ePos.x = 28.275f;
						enemy[i]->enemyS = -enemy[i]->enemyS;
						enemy[i]->enemyR = -enemy[i]->enemyR;
						enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
					}
					else if (ePos.x < -28.275f)
					{
						ePos.x = -28.275f;
						enemy[i]->enemyS = -enemy[i]->enemyS;
						enemy[i]->enemyR = -enemy[i]->enemyR;
						enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
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
											enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
										}
										else
										{
											ePos.x = block[y][x]->block->GetPosition().x + 0.5f * ball->GetScale().x + 0.5f * block[y][x]->block->GetScale().x;
											block[y][x]->HP--;
											enemy[i]->enemyS = -enemy[i]->enemyS;
											enemy[i]->enemyR = -enemy[i]->enemyR;
											enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
										}
									}
								}
							}
						}
					}
					if (ePos.y < -14.625f)
					{
						ePos.y = -14.625f;
						enemy[i]->enemyG = gravity;
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
					if (LenAB(ball->GetPosition(), ePos) < ball->GetScale().x * 0.5f + 0.5f * enemy[i]->enemy->GetScale().x)
					{
						enemy[i]->alive = false;
					}
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
		if (enemy[i]->alive == true)
		{
			enemy[i]->enemy->Draw();
		}
	}
	player->Draw();
	ball->Draw();
	frame->Draw();
	for (int i = 0; i < heartCounter; i++)
	{
		heart[i]->Draw();
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