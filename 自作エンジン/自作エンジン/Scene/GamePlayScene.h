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
		BLOCK(Object3d *obj, int HP = 15)
		{
			this->block.reset(obj);
			this->HP = HP;
		}
	};

	struct ENEMY
	{
		std::unique_ptr<Object3d> enemy = nullptr;
		bool alive = false;
		float enemyS = 0;
		float enemyR = 0;
		float enemyG = 0;
		ENEMY(Object3d* obj)
		{
			this->enemy.reset(obj);
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

	enum STAGE
	{
		TITLE, GAME, END
	};

private: // �C���X�^���X
	// ���C�g
	std::unique_ptr<Light> light = nullptr;

	// �p�[�e�B�N��
	//std::unique_ptr<ParticleManager> particle = nullptr;
	
	// �X�v���C�g

	
	// OBJ�I�u�W�F�N�g
	// �v���C���[
	std::unique_ptr<Object3d> player = nullptr;
	// �u���b�N
	std::unique_ptr<BLOCK> block[8][13] = {};
	// �{�[��
	std::unique_ptr<Object3d> ball = nullptr;
	// �G�l�~�[
	std::unique_ptr<ENEMY> enemy[13] = {};
	// �G�l�~�[�X�|�i�[
	std::unique_ptr<ENEMYSPAWNER> enemySpawner[13] = {};
	// �t���[��
	std::unique_ptr<Object3d> frame = nullptr;
	// �j�[�h��
	std::unique_ptr<Object3d> needle[13] = {};
	// �n�[�g
	std::unique_ptr<Object3d> heart[3] = {};
	// �^�C�g��
	std::unique_ptr<Object3d> title = nullptr;
	// �^�C�g���{�^��
	std::unique_ptr<Object3d> titleButton = nullptr;
	// ���U���g
	std::unique_ptr<Object3d> end = nullptr;
	// ���U���g�{�^��
	std::unique_ptr<Object3d> endButton = nullptr;
	// �Q�[���I�[�o�[
	std::unique_ptr<Object3d> gameover = nullptr;
	// HP 
	std::unique_ptr<Object3d> HP = nullptr;
	// �X�R�A����
	std::unique_ptr<Object3d> scoreList = nullptr;
	// �X�R�A
	std::unique_ptr<Object3d> scoreNum[10][6] = {};
	std::unique_ptr<Object3d> scoreNumEND[10][6] = {};
	// �R���{
	std::unique_ptr<Object3d> comboText[6] = {};
	// �w�i
	std::unique_ptr<Object3d> backGround[2] = {};
	// FBX�I�u�W�F�N�g


private: // �����o�ϐ�
	// �v���C���[
	// ����
	float playerS;
	// �W�����v�̑���
	float playerJS;
	// �W�����v�t���O
	bool isPJ;
	// �u���b�N�ǉ�
	bool addB;
	// ����̂ǂꂩ
	int addX;
	// �C�[�W���O�^�C�}�[
	int blockTimer;
	// �����x
	int timerA;

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
	// �t���O
	float isAliveB;

	// �R���{
	// �^�C�}�[
	int comboTimer;
	// �R���{�̗P�\����
	int comboLimit;
	// �R���{�J�E���^�[
	int comboNum;
	// �e�L�X�g
	bool isActiveCT;
	int effectTimer;
	int textNum;

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

	// �X�R�A
	int score;

	// �X�e�[�W
	int stage;

	// �^�C�g��
	// ���S�^�C�}�[
	int titleTimer;
	// ���S�̉����x
	int titleA;

	// ���U���g
	// �I���܂ł̃��O�^�C�}�[
	int endTimer;
	// �{�^���̃^�C�}�[
	int buttonTimer;
	// �{�^���^�C�}�[�̉����x
	int buttonA;

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
