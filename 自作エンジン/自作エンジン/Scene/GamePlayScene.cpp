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
	//particle.reset(ParticleManager::Create("Default/effect1.png"));

	// �X�v���C�g
	player.reset(Object3d::Create("PlayerStay"));
	player->SetPosition({ 0, 5.2f * (-4.9f), 0 });
	player->SetRotation({ -95, 0, 0 });
	player->SetScale({ 2.25f, 1, 2.25f });
	player->Update();
	ball.reset(Object3d::Create("BallMove_01"));
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
			block[y][x]->block->Update();
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
			scoreNum[n][x]->SetPosition({ x * scoreNum[n][x]->GetScale().x - (4.0f * x) + 21, 24.3f, 0});
			scoreNum[n][x]->SetRotation({ -90, 0, 0 });
			scoreNumEND[n][x]->SetScale({ 5.5f * 1.5f, 1, 5.5f * 1.5f });
			scoreNumEND[n][x]->Update();
			scoreNumEND[n][x]->SetPosition({ x * scoreNum[n][x]->GetScale().x - (0.5f * x) - 5, -5.2f, 0});
			scoreNumEND[n][x]->SetRotation({ -90, 0, 0 });
		}
	}
	comboText[0].reset(Object3d::Create("Combo_01"));
	comboText[1].reset(Object3d::Create("Combo_02"));
	comboText[2].reset(Object3d::Create("Combo_03"));
	comboText[3].reset(Object3d::Create("Combo_04"));
	comboText[4].reset(Object3d::Create("Combo_05"));
	comboText[5].reset(Object3d::Create("Combo_05"));
	for (int i = 0; i < 6; i++)
	{
		comboText[i]->SetScale({0, 1, 0});
		comboText[i]->Update();
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
	score = 745;

	stage = TITLE;

	addB = false;

	addX = 0;

	playerS = 0.15f;
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

	comboTimer = 0;
	comboLimit = 60;
	comboNum = 0;
	isActiveCT = false;

	heartCounter = 3;

	titleTimer = 0;

	titleA = 1;

	endTimer = 0;

	buttonTimer = 0;

	spawTimer = 1;
	spawnerScale = 0;
	spawnerSA = 0.05;

	buttonA = 1;

	effectTimer = 0;

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

		enemySpawner[x]->spawner->SetPosition({ 5.2f * (x - 6), 5.2f * (7 - 4), 0 });
		enemySpawner[x]->spawner->SetRotation({ -90, 0, 0 });
		enemySpawner[x]->spawner->SetScale({ 0, 1, 0 });
		enemySpawner[x]->spawner->Update();
		enemySpawner[x]->active = false;
	}
}

void GamePlayScene::Update()
{
#pragma region �Q�[�����C���V�X�e��
	
	// �^�C�g��
	if (stage == TITLE)
	{
		// ����̑傫���łȂ����
		if (title->GetScale().x < 30 * 1.7f && titleButton->GetScale().x < 20)
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
		// �傫���Ȃ�^�C�}�[������
		else
		{
			// �C�[�W���O
			XMFLOAT3 bScale = titleButton->GetScale();
			float t = powf((float)titleTimer / 40, 2);
			bScale.x = 20.0f * (1.0f - t) + 23.0f * t;
			bScale.z = bScale.x;
			titleButton->SetScale(bScale);

			// �^�C�}�[
			titleTimer += titleA;

			// ���̎��Ԃ������
			if (titleTimer >= 60)
			{
				titleA = -abs(titleA);
			}
			else if (titleTimer <= 0)
			{
				titleA = abs(titleA);
			}
		}
	}

	// �u���b�N
	if (stage == TITLE || stage == GAME)
	{
		// HP�X�V
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
					// 0�Ȃ���J�艺��
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
						block[y][x]->block->SetColor({ 1, 1, 1, 1 });
					}
				}
			}
		}

		// ��񂻂������
		for (int x = 0; x < 13; x++)
		{
			if (addB == true)
			{
				break;
			}
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
				if (stage == GAME)
				{
					isActiveCT = true;
					// �^�C�}�[��1�ɂ���
					comboTimer = 1;
					// �R���{�J�E���^�[
					comboNum++;
					// �e�L�X�g�̍��W
					comboText[comboNum - 1]->SetPosition({ 5.2f * (x - 6), 5.2f * (6 - 4), 0 });
					comboText[comboNum - 1]->Update();
					effectTimer = 0;
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

		// �R���{����
		if (comboTimer > 0 && stage == GAME)
		{
			// �R���{�̗P�\����
			comboTimer++;
			// �P�\���߂�����
			if (comboTimer > comboLimit)
			{
				// ���Z�b�g
				comboTimer = 0;
				comboNum = 0;
			}
		}

		if (isActiveCT == true)
		{
			// �g�k
			if (effectTimer < 60)
			{
				effectTimer++;
			}

			XMFLOAT3 cScale = comboText[comboNum - 1]->GetScale();
			float t = ((float)effectTimer / 60) * (2 - (float)effectTimer / 60);
			cScale.x = 10.0f * (1.0f - t) + 15.0f * t;
			cScale.z = cScale.x;
			comboText[comboNum - 1]->SetScale(cScale);
		}
	}

	// �v���C���[
	if (stage == TITLE || stage == GAME)
	{
		// �W�����v
		if (input->TriggerKey(DIK_SPACE) && isPJ == false)
		{
			playerJS = 0.05f;
			isPJ = true;
		}

		// �ړ�
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

		// �W�����v����
		if (isPJ == true)
		{
			// ���Z
			pPos.y += playerJS;
			// ����
			playerJS *= 2;
			//�u���b�N�ɂԂ�������
			if (-23.4f < pPos.y + 0.5f * player->GetScale().x - 0.7f)
			{
				pPos.y = -23.4f - 0.5f * player->GetScale().x + 0.7f;
				playerJS = 0.05f;
				playerJS = -playerJS;
				// X�����ǂ��̉���
				int x = static_cast<int>((pPos.x + block[0][0]->block->GetScale().x * 6 + 0.5f * block[0][0]->block->GetScale().x) / block[0][0]->block->GetScale().x);
				//���ɋ󂫂����邩
				for (int y = 6; 0 <= y; y--)
				{
					if (addB == true)
					{
						break;
					}
					if (y < 7 && block[y][x]->map == true && block[y + 1][x]->map == false && block[1][x]->HP > 0)
					{
						block[y + 1][x]->map = true;
						for (int i = 1; i < 8; i++)
						{
							if (i < 7)
							{
								block[i + 1][x]->HP = block[i][x]->HP;
								block[i][x]->HP = 15;
							}
							XMFLOAT3 pos = block[i][x]->block->GetPosition();
							pos.y -= 5.2f;
							block[i][x]->block->SetPosition(pos);
						}
						addB = true;
						addX = x;
						if (isBJ == false && static_cast<int>((ball->GetPosition().x + block[0][0]->block->GetScale().x * 6 + 0.5f * block[0][0]->block->GetScale().x) / block[0][0]->block->GetScale().x) == x)
						{
							block[y + 1][x]->map = true;
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
			//���ɂ�����
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
				pos.y += 0.52f;
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
		player->SetPosition(pPos);
		player->Update();
	}

	// �{�[��
	if (stage == GAME)
	{
		// �ړ�
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
		// �d��
		if (isBJ == false)
		{
			bPos.y -= ballG;
			ballG += gravity;
		}
		// �W�����v
		else
		{
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
							if (bPos.y - 0.5f * ball->GetScale().x < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
							{
								bPos.y = block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x + 0.5f * ball->GetScale().x;
								ballG = gravity;
								isBJ = false;
								// ���x����
								if (ballS == 0)
								{
									ballS = 0.2f;
								}
							}
						}
					}
					// ��
					if (fabsf(bPos.x - block[y][x]->block->GetPosition().x) < 0.5f * ball->GetScale().x + 0.5f * block[y][x]->block->GetScale().x)
					{
						if (block[y][x]->block->GetPosition().y - 0.5f * block[y][x]->block->GetScale().x < bPos.y && bPos.y < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
						{
							if (bPos.x - block[y][x]->block->GetPosition().x < 0)
							{
								bPos.x = block[y][x]->block->GetPosition().x - 0.5f * ball->GetScale().x - 0.5f * block[y][x]->block->GetScale().x;
								block[y][x]->HP--;
								if (isBJ == false)
								{
									ballS = -abs(ballS);
									ballR = abs(ballR);
								}
							}
							else
							{
								bPos.x = block[y][x]->block->GetPosition().x + 0.5f * ball->GetScale().x + 0.5f * block[y][x]->block->GetScale().x;
								block[y][x]->HP--;
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
		// ��]
		XMFLOAT3 bRot = ball->GetRotation();
		bRot.z += ballR;
		if (bRot.z >= 360 || bRot.z <= -360)
		{
			bRot.z = 0;
		}
		ball->SetRotation(bRot);
	}

	// �G�l�~�[
	if (stage == GAME)
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
			// �X�|�i�[
			if (enemySpawner[i]->active == true)
			{
				// �g��
				XMFLOAT3 scale = enemySpawner[i]->spawner->GetScale();
				scale.x += spawnerSA;
				scale.z += spawnerSA;
				// �G�l�~�[�̐���
				if (scale.x > 2.25f && enemy[i]->alive == false)
				{
					enemy[i]->alive = true;
					enemy[i]->enemy->SetPosition({ 5.2f * (i - 6), 5.2f * (7 - 4), 0});
					enemy[i]->enemy->SetScale({ 0, 1, 0 });
					enemy[i]->enemy->Update();
					enemy[i]->enemyG = 0.5f;
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
			if (enemy[i]->alive == true)
			{
				// �X�|�i�[�ɍ��킹�Ċg��
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
				// �X�V
				else
				{
					// �ړ�
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
					// �d��
					ePos.y -= enemy[i]->enemyG;
					enemy[i]->enemyG += gravity;
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
										if (ePos.y - 0.5f * ball->GetScale().x < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
										{
											ePos.y = block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x + 0.5f * ball->GetScale().x;
											enemy[i]->enemyG = gravity;
											// ���x����
											if (enemy[i]->enemyS == 0)
											{
												enemy[i]->enemyS = 0.2f;
											}
										}
									}
								}
								// ��
								if (fabsf(ePos.x - block[y][x]->block->GetPosition().x) < 0.5f * ball->GetScale().x + 0.5f * block[y][x]->block->GetScale().x)
								{
									if (block[y][x]->block->GetPosition().y - 0.5f * block[y][x]->block->GetScale().x < ePos.y && ePos.y < block[y][x]->block->GetPosition().y + 0.5f * block[y][x]->block->GetScale().x)
									{
										if (ePos.x - block[y][x]->block->GetPosition().x < 0)
										{
											ePos.x = block[y][x]->block->GetPosition().x - 0.5f * ball->GetScale().x - 0.5f * block[y][x]->block->GetScale().x;
											enemy[i]->enemyS = -enemy[i]->enemyS;
											enemy[i]->enemyR = -enemy[i]->enemyR;
											block[y][x]->HP--;
										}
										else
										{
											ePos.x = block[y][x]->block->GetPosition().x + 0.5f * ball->GetScale().x + 0.5f * block[y][x]->block->GetScale().x;
											enemy[i]->enemyS = -enemy[i]->enemyS;
											enemy[i]->enemyR = -enemy[i]->enemyR;
											block[y][x]->HP--;
										}
									}
								}
							}
						}
					}
					enemy[i]->enemy->SetPosition(ePos);
					// ��]
					XMFLOAT3 eRot = enemy[i]->enemy->GetRotation();
					eRot.z += enemy[i]->enemyR;
					if (eRot.z >= 360 || eRot.z <= -360)
					{
						eRot.z = 0;
					}
					enemy[i]->enemy->SetRotation(eRot);
					// �v���C���[�̃{�[���Ƃ̓����蔻��
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

	// ���U���g
	if (stage == END)
	{
		// �I������u�Ԉ�u�~�܂�
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
			// ���̑傫���ɖ����Ȃ����
			if (end->GetScale().x < 20 * 1.5f && endButton->GetScale().x < 25 && gameover->GetScale().x < 30 * 1.7f)
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
			}
			else
			{
				// �C�[�W���O
				XMFLOAT3 bScale = endButton->GetScale();
				float t = powf((float)buttonTimer / 40, 2);
				bScale.x = 25.0f * (1.0f - t) + 28.0f * t;
				bScale.z = bScale.x;
				endButton->SetScale(bScale);

				// �^�C�}�[
				buttonTimer += buttonA;

				// ���̎��Ԃ������
				if (buttonTimer >= 60)
				{
					buttonA = -abs(buttonA);
				}
				else if (buttonTimer <= 0)
				{
					buttonA = abs(buttonA);
				}

				// TITLE�ɖ߂�
				if (input->TriggerKey(DIK_SPACE))
				{
					InitializeVariable();
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
	//DrawEffect(cmdList);
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

	for (int y = 7; y >= 0; y--)
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
	if (isActiveCT == true)
	{
		comboText[comboNum - 1]->Draw();
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