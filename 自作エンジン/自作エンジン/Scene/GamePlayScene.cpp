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

	// �X�v���C�g�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");

	// ���C�g����
	light.reset(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({ -5, -5, 0, 0 });
	Object3d::SetLight(light.get());

	// �O�i�X�v���C�g
	debugText.Initialize(fontNumber);

	// �p�[�e�B�N��
	playerWalkEffect.reset(ParticleManager::Create("star.png"));
	playerJumpEffect.reset(ParticleManager::Create("stareffect_02.png"));
	ballJumpEffect.reset(ParticleManager::Create("star.png"));
	ballBounceEffect.reset(ParticleManager::Create("stareffect_01.png"));
	enemyBounceEffect.reset(ParticleManager::Create("stareffect_03.png"));
	enemySpawnEffect.reset(ParticleManager::Create("enemyspowneffect.png"));

	// �X�v���C�g
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

	// OBJ�I�u�W�F�N�g


	// FBX�I�u�F�N�g


	// �I�[�f�B�I
	//audio->Initialize();

	// �ϐ��̏�����
	InitializeVariable();

	// ����������
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

	playerStay->SetPosition({ 0, 4.5f * (-5), 0 });
	playerStay->SetRotation({ -90, 0, 0 });
	playerStay->SetScale({ 2.25f, 1, 2.25f });
	playerStay->Update();

	playerJump->SetPosition({ 0, 4.5f * (-5), 0 });
	playerJump->SetRotation({ -90, 0, 0 });
	playerJump->SetScale({ 2.25f, 1, 2.25f });
	playerJump->Update();

	playerMove01->SetPosition({ 0, 4.5f * (-5), 0 });
	playerMove01->SetRotation({ -90, 0, 0 });
	playerMove01->SetScale({ 2.25f, 1, 2.25f });
	playerMove01->Update();

	playerMove02->SetPosition({ 0, 4.5f * (-5), 0 });
	playerMove02->SetRotation({ -90, 0, 0 });
	playerMove02->SetScale({ 2.25f, 1, 2.25f });
	playerMove02->Update();

	playerMove03->SetPosition({ 0, 4.5f * (-5), 0 });
	playerMove03->SetRotation({ -90, 0, 0 });
	playerMove03->SetScale({ 2.25f, 1, 2.25f });
	playerMove03->Update();

	playerMove04->SetPosition({ 0, 4.5f * (-5), 0 });
	playerMove04->SetRotation({ -90, 0, 0 });
	playerMove04->SetScale({ 2.25f, 1, 2.25f });
	playerMove04->Update();

	ball01->SetPosition({ 0, 5, 0 });
	ball01->SetRotation({ -90, 0, 0 });
	ball01->SetScale({ 2.25f, 1, 2.25f });
	ball01->Update();

	ball02->SetPosition({ 0, 5, 0 });
	ball02->SetRotation({ -90, 0, 0 });
	ball02->SetScale({ 2.25f, 1, 2.25f });
	ball02->Update();

	ball03->SetPosition({ 0, 5, 0 });
	ball03->SetRotation({ -90, 0, 0 });
	ball03->SetScale({ 2.25f, 1, 2.25f });
	ball03->Update();

	ball04->SetPosition({ 0, 5, 0 });
	ball04->SetRotation({ -90, 0, 0 });
	ball04->SetScale({ 2.25f, 1, 2.25f });
	ball04->Update();

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
		enemyStay[x]->enemyStay->SetPosition({ 4.5f * (x - 6), 13.5f, 0});
		enemyStay[x]->enemyStay->SetRotation({ -90, 0, 0 });
		enemyStay[x]->enemyStay->SetScale({ 0, 1, 0 });
		enemyStay[x]->enemyStay->Update();
		enemyStay[x]->enemyS = 0.2f;
		enemyStay[x]->enemyR = -5;
		enemyStay[x]->enemyG = 0.5f;
		enemyStay[x]->alive = false;

		enemy01[x]->enemy01->SetPosition({ 4.5f * (x - 6), 13.5f, 0 });
		enemy01[x]->enemy01->SetRotation({ -90, 0, 0 });
		enemy01[x]->enemy01->SetScale({ 0, 1, 0 });
		enemy01[x]->enemy01->Update();
		enemy01[x]->enemyS = 0.2f;
		enemy01[x]->enemyR = -5;
		enemy01[x]->enemyG = 0.5f;
		enemy01[x]->alive = false;

		enemy02[x]->enemy02->SetPosition({ 4.5f * (x - 6), 13.5f, 0 });
		enemy02[x]->enemy02->SetRotation({ -90, 0, 0 });
		enemy02[x]->enemy02->SetScale({ 0, 1, 0 });
		enemy02[x]->enemy02->Update();
		enemy02[x]->enemyS = 0.2f;
		enemy02[x]->enemyR = -5;
		enemy02[x]->enemyG = 0.5f;
		enemy02[x]->alive = false;

		enemySpawner[x]->spawner->SetPosition({ 4.5f * (x - 6), 13.5f, 0 });
		enemySpawner[x]->spawner->SetRotation({ -90, 0, 0 });
		enemySpawner[x]->spawner->SetScale({ 0, 1, 0 });
		enemySpawner[x]->spawner->Update();
		enemySpawner[x]->active = false;
	}
}

void GamePlayScene::Update()
{
#pragma region �Q�[�����C���V�X�e��
	
	// �u���b�N
	{
		// HP�X�V
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 13; x++)
			{
				if (block[y][x]->map == true)
				{
					// 0�Ȃ���J�艺��
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
					// 5�ȉ���
					else if (block[y][x]->HP <= 5)
					{
						block[y][x]->block->SetColor({ 1, 0.5f, 0.4f, 1 });
					}
					// 10�ȉ���
					else if (block[y][x]->HP <= 10)
					{
						block[y][x]->block->SetColor({ 1, 1, 0.45f, 1 });
					}
					// 15�ȉ���
					else
					{
						block[y][x]->block->SetColor({ 0.4f, 1, 0.5f, 1 });
					}
				}
			}
		}

		// ��񂻂������
		for (int x = 0; x < 13; x++)
		{
			bool isComplete = true;
			for (int y = 0; y < 8; y++)
			{
				// �c��񂸂��ׂ�
				if (block[y][x]->map == false)
				{
					isComplete = false;
				}
			}
			// ��񂻂���Ă�����
			if (isComplete == true)
			{
				// ���ׂď���
				for (int y = 1; y < 8; y++)
				{
					block[y][x]->map = false;
					block[y][x]->HP = 15;
				}
				// �^�C�}�[��1�ɂ���
				comboTimer = 1;
				// �R���{�J�E���^�[
				comboNum++;
			}
		}

		// �R���{����
		if (comboTimer > 0)
		{
			// �R���{�̗P�\����
			comboTimer++;
			if (comboTimer > 20)
			{
				// �R���{��
				debugText.Print("COMBO", 10, 10, comboNum);
			}
			// �P�\���߂�����
			if (comboTimer > comboLimit)
			{
				// ���Z�b�g
				comboTimer = 0;
				comboNum = 0;
			}
		}
	}

	// �v���C���[
	{
		XMFLOAT3 pPos = playerStay->GetPosition();
		// �W�����v
		if (input->TriggerKey(DIK_SPACE) && isPJ == false)
		{
			playerJS = 0.05f;
			isPJ = true;
		}

		// �ړ�
		//XMFLOAT3 pPos = player->GetPosition();
		XMFLOAT3 walkParticlePos = playerStay->GetPosition();
		XMFLOAT3 jumpParticlePos = playerStay->GetPosition();
		XMFLOAT3 velocity = { 0,0,0 };
		XMFLOAT3 walkParticleAccel = { 0,0,0 };
		XMFLOAT3 jumpParticleAccel = { 0,0,0 };
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			moveCount++;
			if (moveCount>=5 && switchingMove) {
				switchingMove = false;
				moveCount = 0;
			}
			else if (moveCount >= 5 && !switchingMove) {
				switchingMove = true;
				moveCount = 0;
			}

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

		// �W�����v����
		if (isPJ == true)
		{
			// ���Z
			pPos.y += playerJS;
			// ����
			playerJS *= 2;
			//�u���b�N�ɂԂ�������
			if (-22.0f + 0.5f * playerStay->GetScale().x < pPos.y)
			{
				pPos.y = -22.0f + 0.5f * playerStay->GetScale().x;
				playerJS = 0.05f;
				playerJS = -playerJS;
				jumpParticleAccel.y = -0.005f;
				playerJumpEffect->Add(30, { jumpParticlePos.x,jumpParticlePos.y-0.5f,jumpParticlePos.z - 1.0f }, { 0,0,0 }, jumpParticleAccel, 4.0f, 6.0f, { 1,1,1,1 }, { 1,1,1,1 });
				// X�����ǂ��̉���
				int x = static_cast<int>((pPos.x + 29.25f) / block[0][0]->block->GetScale().x);
				//���ɋ󂫂����邩
				for (int y = 6; 0 <= y; y--)
				{
					if (y < 7 && block[y][x]->map == true && block[y + 1][x]->map == false && block[1][x]->HP > 0)
					{
						block[y + 1][x]->map = true;
						if (isBJ == false && static_cast<int>((ball01->GetPosition().x + 29.25f) / block[0][0]->block->GetScale().x) == x)
						{
							isBJ = true;
							ballJS = 2.0f;
							XMFLOAT3 bPos = ball01->GetPosition();
							bPos.y += 4.5f;
							ball01->SetPosition(bPos);
							ball01->Update();
						}
						for (int i = 0; i < 13; i++)
						{
							if (isBJ == false && static_cast<int>((enemy01[i]->enemy01->GetPosition().x + 29.25f) / block[0][0]->block->GetScale().x) == x && enemy01[i]->alive == true)
							{
								enemy01[i]->alive = false;
							}
						}
						break;
					}
				}
			}
			//���ɂ�����
			else if (pPos.y < -22.5f)
			{
				pPos.y = -22.5f;
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

	// �{�[��
	{
		// �ړ�
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
		// �d��
		if (isBJ == false)
		{
			bPos.y -= ballG;
			ballG += gravity;
		}
		// �W�����v
		else
		{
			ballJumpEffect->Add(30, bPos, { 0,0,0 }, { 0,0,0 }, 1.0f, 3.0f);
			bPos.y += ballJS;
			ballJS += ballJA;
		}
		//�����蔻��
		for (int y = 0; y < 7; y++)
		{
			for (int x = 0; x < 13; x++)
			{
				if (block[y][x]->map == true)
				{
					// �c
					if (y < 7 && block[y + 1][x]->map == false)
					{
						if (block[y][x]->block->GetPosition().x - 0.5f * block[y][x]->block->GetScale().x <= bPos.x && bPos.x <= block[y][x]->block->GetPosition().x + 0.5f * block[y][x]->block->GetScale().x)
						{
							if (bPos.y - 0.5f * ball01->GetScale().x < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
							{
								bPos.y = block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x + 0.5f * ball01->GetScale().x;
								ballG = gravity;
								isBJ = false;
							}
						}
					}
					// ��
					if (fabsf(bPos.x - block[y][x]->block->GetPosition().x) < 0.5f * ball01->GetScale().x + 0.5f * block[y][x]->block->GetScale().x)
					{
						if (block[y][x]->block->GetPosition().y - 0.5f * block[y][x]->block->GetScale().x < bPos.y && bPos.y < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
						{
							if (bPos.x - block[y][x]->block->GetPosition().x < 0)
							{
								bPos.x = block[y][x]->block->GetPosition().x - 0.5f * ball01->GetScale().x - 0.5f * block[y][x]->block->GetScale().x;
								if (isBJ == false)
								{
									ballS = -ballS;
									ballR = -ballR;
								}
							}
							else
							{
								bPos.x = block[y][x]->block->GetPosition().x + 0.5f * ball01->GetScale().x + 0.5f * block[y][x]->block->GetScale().x;
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
		ball01->SetPosition(bPos);
		// ��]
		XMFLOAT3 bRot = ball01->GetRotation();
		bRot.z += ballR;
		if (bRot.z >= 360 || bRot.z <= -360)
		{
			bRot.z = 0;
		}
		ball01->SetRotation(bRot);
	}

	// �G�l�~�[
	{
		// �X�|�[���^�C�}�[
		if (spawTimer > 0)
		{
			spawTimer++;
		}
		// ��莞�Ԃ������琶��
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
			// �X�|�i�[
			if (enemySpawner[i]->active == true)
			{
				// �g��
				XMFLOAT3 scale = enemySpawner[i]->spawner->GetScale();
				scale.x += spawnerSA;
				scale.z += spawnerSA;
				// �G�l�~�[�̐���
				if (scale.x > 2.25f && enemy01[i]->alive == false)
				{
					enemyStay[i]->alive = true;
					enemyStay[i]->enemyStay->SetPosition({ 4.5f * (i - 6), 13.5f, 0 });
					enemyStay[i]->enemyStay->SetScale({ 0, 1, 0 });
					enemyStay[i]->enemyStay->Update();
					enemyStay[i]->enemyG = 0.5f;

					enemy01[i]->alive = true;
					enemy01[i]->enemy01->SetPosition({ 4.5f * (i - 6), 13.5f, 0 });
					enemy01[i]->enemy01->SetScale({ 0, 1, 0 });
					enemy01[i]->enemy01->Update();
					enemy01[i]->enemyG = 0.5f;

					enemy02[i]->alive = true;
					enemy02[i]->enemy02->SetPosition({ 4.5f * (i - 6), 13.5f, 0 });
					enemy02[i]->enemy02->SetScale({ 0, 1, 0 });
					enemy02[i]->enemy02->Update();
					enemy02[i]->enemyG = 0.5f;
				}
				// �ő�Ȃ�k��
				else if (scale.x > 4.5f)
				{
					scale.x = 4.5f;
					scale.z = 4.5f;
					spawnerSA = -spawnerSA;
				}
				// 0�Ȃ����
				else if (scale.x <= 0)
				{
					scale.x = 0;
					scale.z = 0;
					spawnerSA = -spawnerSA;
					enemySpawner[i]->active = false;
				}
				enemySpawner[i]->spawner->SetScale(scale);
			}
			// �G�l�~�[
			if (enemy01[i]->alive == true)
			{
				// �X�|�i�[�ɍ��킹�Ċg��
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
				// �X�V
				else
				{
					// �ړ�
					XMFLOAT3 ePos = enemy01[i]->enemy01->GetPosition();
					ePos.x += enemy01[i]->enemyS;
					if (28.275f < ePos.x)
					{
						ePos.x = 28.275f;
						enemy01[i]->enemyS = -enemy01[i]->enemyS;
						enemy01[i]->enemyR = -enemy01[i]->enemyR;
						enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
					}
					else if (ePos.x < -28.275f)
					{
						ePos.x = -28.275f;
						enemy01[i]->enemyS = -enemy01[i]->enemyS;
						enemy01[i]->enemyR = -enemy01[i]->enemyR;
						enemyBounceEffect->Add(60, ePos, { 0,0,0 }, { 0,0,0 }, 6.0f, 0.0f);
					}
					// �d��
					ePos.y -= enemy01[i]->enemyG;
					enemy01[i]->enemyG += gravity;
					//�����蔻��
					for (int y = 0; y < 7; y++)
					{
						for (int x = 0; x < 13; x++)
						{
							if (block[y][x]->map == true)
							{
								// �c
								if (y < 7 && block[y + 1][x]->map == false)
								{
									if (block[y][x]->block->GetPosition().x - 0.5f * block[y][x]->block->GetScale().x <= ePos.x && ePos.x <= block[y][x]->block->GetPosition().x + 0.5f * block[y][x]->block->GetScale().x)
									{
										if (ePos.y - 0.5f * ball01->GetScale().x < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
										{
											ePos.y = block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x + 0.5f * ball01->GetScale().x;
											enemy01[i]->enemyG = gravity;
											enemyStay[i]->isLanding = true;
										}
									}
								}
								// ��
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
					if (ePos.y < -14.625f)
					{
						ePos.y = -14.625f;
						enemy01[i]->enemyG = gravity;
					}
					enemyStay[i]->enemyStay->SetPosition(ePos);
					enemy01[i]->enemy01->SetPosition(ePos);
					enemy02[i]->enemy02->SetPosition(ePos);
					// ��]
					XMFLOAT3 eRot = enemy01[i]->enemy01->GetRotation();
					eRot.z += enemy01[i]->enemyR;
					if (eRot.z >= 360 || eRot.z <= -360)
					{
						eRot.z = 0;
					}
					enemyStay[i]->enemyStay->SetRotation(eRot);
					enemy01[i]->enemy01->SetRotation(eRot);
					enemy02[i]->enemy02->SetRotation(eRot);
					if (LenAB(ball01->GetPosition(), ePos) < ball01->GetScale().x * 0.5f + 0.5f * enemy01[i]->enemy01->GetScale().x)
					{
						enemyStay[i]->alive = false;
						enemy01[i]->alive = false;
						enemy02[i]->alive = false;
					}
				}
			}
		}
	}
	
#pragma endregion

#pragma region �J�����ƃ��C�g�̍X�V

	light->Update();
	camera->Update();

#pragma endregion
}

void GamePlayScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	// �e�`��
	//DrawBackSprite(cmdList);
	DrawObjects(cmdList);
	DrawEffect(cmdList);
	DrawUI(cmdList);
	DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	// �O�i�X�v���C�g�`��
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::DrawObjects(ID3D12GraphicsCommandList* cmdList)
{
	// OBJ�I�u�W�F�N�g�`��
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
		if (enemy01[i]->alive == true && !enemyStay[i]->isLanding) {
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
	ball01->Draw();
	frame->Draw();
	for (int i = 0; i < heartCounter; i++)
	{
		heart[i]->Draw();
	}

	Object3d::PostDraw();

	// FBX�I�u�W�F�N�g
	/*FbxObject3d::PreDraw(cmdList);

	

	FbxObject3d::PostDraw();*/

	// �X�v���C�g�`��
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void GamePlayScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	// UI�`��
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void GamePlayScene::DrawEffect(ID3D12GraphicsCommandList* cmdList)
{
	// �p�[�e�B�N���`��
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
	// �f�o�b�O�e�L�X�g�`��
	debugText.Draw(cmdList);
}

const float GamePlayScene::LenAB(XMFLOAT3 posA, XMFLOAT3 posB)
{
	return sqrtf(powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2));
}