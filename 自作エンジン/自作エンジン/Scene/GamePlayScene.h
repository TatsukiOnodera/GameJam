#pragma once
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Audio.h"
#include "DebugText.h"
#include "ParticleManager.h"
#include "Collision.h"
#include "Camera.h"
#include "DirectXCommon.h"
#include "BaseScene.h"
#include "Light.h"
//#include "FbxLoader.h"
//#include "FbxObject3d.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <memory>
#include <array>

class GamePlayScene : public BaseScene
{
public: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �萔
	// �f�o�b�O�e�L�X�g�p�e�N�X�`���̔ԍ�
	const int fontNumber = 0;

private: // �������u����
	// DIrectXCommon
	DirectXCommon* dx_cmd = nullptr;
	// ����n
	Input* input = nullptr;
	// �I�[�f�B�I
	//Audio* audio = nullptr;
	// �J����
	Camera* camera = nullptr;
	// �f�o�b�O�e�L�X�g
	DebugText debugText;

private: //�T�u�N���X
	struct BLOCK
	{
		std::unique_ptr<Object3d> block = nullptr;
		bool map = false;
		int HP = 15;
		bool buff = false;
		BLOCK(Object3d *obj, int HP = 15)
		{
			this->block.reset(obj);
			this->HP = HP;
		}
	};

	struct ENEMYSTAY
	{
		std::unique_ptr<Object3d> enemyStay = nullptr;
		bool alive = false;
		float enemyS = 0;
		float enemyR = 0;
		float enemyG = 0;
		bool isLanding = false;
		ENEMYSTAY(Object3d* obj)
		{
			this->enemyStay.reset(obj);
		}
	};

	struct ENEMY01
	{
		std::unique_ptr<Object3d> enemy01 = nullptr;
		bool alive = false;
		float enemyS = 0;
		float enemyR = 0;
		float enemyG = 0;
		ENEMY01(Object3d* obj)
		{
			this->enemy01.reset(obj);
		}
	};

	struct ENEMY02
	{
		std::unique_ptr<Object3d> enemy02 = nullptr;
		bool alive = false;
		float enemyS = 0;
		float enemyR = 0;
		float enemyG = 0;
		ENEMY02(Object3d* obj)
		{
			this->enemy02.reset(obj);
		}
	};

	struct ENEMYSPAWNER
	{
		std::unique_ptr<Object3d> spawner = nullptr;
		bool active = false;
		ENEMYSPAWNER(Object3d* obj)
		{
			this->spawner.reset(obj);
		}
	};

private: // �C���X�^���X
	// ���C�g
	std::unique_ptr<Light> light = nullptr;
	// �p�[�e�B�N��
	std::unique_ptr<ParticleManager> playerWalkEffect;
	std::unique_ptr<ParticleManager> playerJumpEffect;
	std::unique_ptr<ParticleManager> ballJumpEffect;
	std::unique_ptr<ParticleManager> ballBounceEffect;
	std::unique_ptr<ParticleManager> enemyBounceEffect;
	std::unique_ptr<ParticleManager> enemySpawnEffect;
	// �X�v���C�g
	
	// OBJ�I�u�W�F�N�g
	// �v���C���[
	std::unique_ptr<Object3d> playerStay = nullptr;
	std::unique_ptr<Object3d> playerJump = nullptr;
	std::unique_ptr<Object3d> playerMove01 = nullptr;
	std::unique_ptr<Object3d> playerMove02 = nullptr;
	std::unique_ptr<Object3d> playerMove03 = nullptr;
	std::unique_ptr<Object3d> playerMove04 = nullptr;
	// �u���b�N
	std::unique_ptr<BLOCK> block[8][13] = {};
	// �{�[��
	std::unique_ptr<Object3d> ball01 = nullptr;
	std::unique_ptr<Object3d> ball02 = nullptr;
	std::unique_ptr<Object3d> ball03 = nullptr;
	std::unique_ptr<Object3d> ball04 = nullptr;
	// �G�l�~�[
	std::unique_ptr<ENEMYSTAY> enemyStay[13] = {};
	std::unique_ptr<ENEMY01> enemy01[13] = {};
	std::unique_ptr<ENEMY02> enemy02[13] = {};
	// �G�l�~�[�X�|�i�[
	std::unique_ptr<ENEMYSPAWNER> enemySpawner[13] = {};
	// �t���[��
	std::unique_ptr<Object3d> frame = nullptr;
	// �j�[�h��
	std::unique_ptr<Object3d> needle[13] = {};
	// �n�[�g
	std::unique_ptr<Object3d> heart[3] = {};
	// FBX�I�u�W�F�N�g


private: // �����o�ϐ�
	// �v���C���[
	// ����
	float playerS;
	// �W�����v�̑���
	float playerJS;
	// �W�����v�t���O
	bool isPJ;

	int playerWalkEffectTimer = 10;
	// �{�[��
	// ����
	float ballS;
	// ��]
	float ballR;
	// �W�����v�̑���
	float ballJS;
	// �W�����v�̉����x
	float ballJA;
	// �W�����v�t���O
	bool isBJ;
	//	�d�͗������x
	float ballG;
	// �d��
	float gravity = 0.1f;

	// �R���{
	// �^�C�}�[
	int comboTimer;
	// �R���{�̗P�\����
	int comboLimit;
	// �R���{�J�E���^�[
	int comboNum;

	// �X�|�[��
	// �^�C�}�[
	int spawTimer;
	// �g�嗦
	float spawnerScale;
	// �����x
	float spawnerSA;

	// �n�[�g
	// �n�[�g�J�E���^�[
	int heartCounter;

	bool switchingMove = true;
	int moveCount = 0;
	bool isBlink = false;
	int blinkCount = 0;

public: // �����o�֐�
	~GamePlayScene() override;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �ϐ�������
	/// </summary>
	void InitializeVariable();

	/// <summary>
	/// �w�i�X�v���C�g�`��
	/// </summary>
	void DrawBackSprite(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��
	/// </summary>
	void DrawObjects(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �G�t�F�N�g�`��
	/// </summary>
	void DrawEffect(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// UI�`��
	/// </summary>
	void DrawUI(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �f�o�b�O�e�L�X�g�`��
	/// </summary>
	void DrawDebugText(ID3D12GraphicsCommandList* cmdList);

	const float LenAB(XMFLOAT3 posA, XMFLOAT3 posB);
};
