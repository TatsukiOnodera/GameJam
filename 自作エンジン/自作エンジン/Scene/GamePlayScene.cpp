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
	Sprite::LoadTexture(1, L"Resources/Player.png");

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
	player.reset(Sprite::Create(1, { 0.0f, 0.0f }, {0.5f, 0.5f}));

	// OBJ�I�u�W�F�N�g


	// FBX�I�u�F�N�g


	// �I�[�f�B�I
	//audio->Initialize();

	// �ϐ��̏�����
	InitializeVariable();
}

void GamePlayScene::InitializeVariable()
{
	playerS = 2;
	playerJS = 2;
	isPJ = false;

	player->SetPosition({ WinApp::window_width / 2, 660 });
	player->SetColor({ 1, 0.6f, 0.9f, 1 });
}

void GamePlayScene::Update()
{
#pragma region �Q�[�����C���V�X�e��
	
	// �v���C���[
	{
		// �W�����v
		if (input->TriggerKey(DIK_SPACE) && isPJ == false)
		{
			isPJ = true;
		}

		// �ړ�
		XMFLOAT2 pos = player->GetPostion();
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			pos.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * playerS;
		}
		if (input->PushKey(DIK_S) || input->PushKey(DIK_W))
		{
			pos.y += (input->PushKey(DIK_S) - input->PushKey(DIK_W)) * playerS;
		}
		// �W�����v
		if (isPJ == true)
		{
			pos.y -= playerJS;
			isPJ = false;
		}
		player->SetPosition(pos);
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

	

	Object3d::PostDraw();

	// FBX�I�u�W�F�N�g
	/*FbxObject3d::PreDraw(cmdList);

	

	FbxObject3d::PostDraw();*/

	// �X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	player->Draw();

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
