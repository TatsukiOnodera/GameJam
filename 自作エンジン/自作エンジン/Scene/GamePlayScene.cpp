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
	player.reset(Object3d::Create("Player"));
	ball.reset(Object3d::Create("BallMove_01"));
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

	player->SetPosition({ 0, 4.5f * (-5), 0 });
	player->SetRotation({ -90, 0, 0 });
	player->SetScale({ 2.25f, 1, 2.25f });
	player->Update();

	ball->SetPosition({ 0, 5, 0 });
	ball->SetRotation({ -90, 0, 0 });
	ball->SetScale({ 2.25f, 1, 2.25f });
	ball->Update();
}

void GamePlayScene::Update()
{
#pragma region �Q�[�����C���V�X�e��
	
	// �v���C���[
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
			if (27 < pPos.x)
			{
				pPos.x = 27;
			}
			else if (pPos.x < -27)
			{
				pPos.x = -27;
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
			if (-22.5f + 0.5f * player->GetScale().x < pPos.y)
			{
				pPos.y = -22.5f + 0.5f * player->GetScale().x;
				playerJS = 0.05f;
				playerJS = -playerJS;
				// X�����ǂ��̉���
				int x = static_cast<int>((pPos.x + 29.25f) / block[0][0]->block->GetScale().x);
				//���ɋ󂫂����邩
				for (int y = 6; 0 <= y; y--)
				{
					if (y < 7 && block[y][x]->map == true && block[y + 1][x]->map == false)
					{
						block[y + 1][x]->map = true;
						break;
					}
				}
				if (isBJ == false && static_cast<int>((ball->GetPosition().x + 29.25f) / block[0][0]->block->GetScale().x) == x)
				{
					isBJ = true;
					ballJS = 2.0f;
					XMFLOAT3 bPos = ball->GetPosition();
					bPos.y += 4.5f;
					ball->SetPosition(bPos);
					ball->Update();
				}
			}
			//���ɂ�����
			else if (pPos.y < -22.5f)
			{
				pPos.y = -22.5f;
				isPJ = false;
			}
		}
		player->SetPosition(pPos);
	}

	// �{�[��
	{
		// �ړ�
		XMFLOAT3 bPos = ball->GetPosition();
		bPos.x += ballS;
		if (28.275f < bPos.x)
		{
			bPos.x = 28.275f;
			ballS = -ballS;
			ballR = -ballR;
		}
		else if (bPos.x < -28.275f)
		{
			bPos.x = -28.275f;
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
								ballS = -ballS;
								ballR = -ballR;
							}
							else
							{
								bPos.x = block[y][x]->block->GetPosition().x + 0.5f * ball->GetScale().x + 0.5f * block[y][x]->block->GetScale().x;
								ballS = -ballS;
								ballR = -ballR;
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
		// ��]
		XMFLOAT3 bRot = ball->GetRotation();
		bRot.z += ballR;
		if (bRot.z >= 360 || bRot.z <= -360)
		{
			bRot.z = 0;
		}
		ball->SetRotation(bRot);
	}

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
						block[y][x]->map = block[y + 1][x]->map;
						block[y][x]->HP = block[y + 1][x]->HP;
						block[y + 1][x]->map = false;
						block[y + 1][x]->HP = 15;
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
