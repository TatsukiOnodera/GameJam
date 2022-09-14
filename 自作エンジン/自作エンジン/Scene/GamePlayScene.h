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
	Audio* audio = nullptr;
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
		bool isEnemyLanding = false;
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
		float groundE = false;
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
		float groundE = false;
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

	enum STAGE
	{
		TITLE, GAME, END
	};

private: // �C���X�^���X
	// ���C�g
	std::unique_ptr<Light> light = nullptr;

	// �p�[�e�B�N��
	std::unique_ptr<ParticleManager> playerWalkEffect;
	std::unique_ptr<ParticleManager> playerJumpEffect;
	std::unique_ptr<ParticleManager> ballJumpEffect;
	std::unique_ptr<ParticleManager> ballBounceEffect;
	std::unique_ptr<ParticleManager> deleteBlockEffect;
	std::unique_ptr<ParticleManager> enemyBounceEffect;
	std::unique_ptr<ParticleManager> enemySpawnEffect;
	std::unique_ptr<ParticleManager> deadEffect;
	// �X�v���C�g
	std::unique_ptr<Sprite> animationBack = nullptr;
	std::unique_ptr<Sprite> animationLogo = nullptr;
	
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
	std::unique_ptr<Object3d> buffBlock[8][13] = {};
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
	// �E�H�[�NSE�J�E���g
	int workSE;
	// �_���[�W�^�C�}�[
	int damageTimer;

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
	// �t���O
	float isAliveB;
	// �n�ʂɂ��邩
	bool groundB;
	// buff
	bool isBuff;

	// �u���b�N
	int blockHP = 9; // TODO �u���b�N��HP

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
	// �C���^�[�o��
	int spawnInterval;

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

	bool switchingMove = true;
	int moveCount = 0;
	bool isBlink = false;
	int blinkCount = 0;
	int restoreCameraTimer = 0;

	bool endToTitle;
	bool titleToGame;

	int addBlock;

	int heartBeat;
	int heartA;

	//�{�����[��
	float wav1 = 0.4f; // �v���C���[�̓���
	float wav2 = 0.5f; // �v���C���[�̃W�����v
	float wav3 = 0.5f; // �u���b�N��������
	float wav4 = 0.5f; // �{�[���̃W�����v
	float wav5 = 0.5f; // �{�[�����W�����v���Ē��n
	float wav6 = 0.5f; // �{�[�����G���ǂ�u���b�N�ɓ�����
	float wav7 = 0.5f; // �{�[�����G�ɓ�����
	float wav8 = 0.5f; // �u���b�N������
	float wav9 = 0.5f; // �u���b�N���ő�ɓ��B����
	float wav10 = 0.5f; // �u���b�N���ǉ�
	float wav11 = 0.5f; // �G���|�����
	float wav12 = 0.5f; // �G�̃X�|�[��
	float wav13 = 0.5f; // ���U���g��SPACE
	float wav14 = 0.5f; // �{�[�����G�����n�����Ƃ�
	float wavCombo = 0.5f; // �R���{
	float BGM = 0.75f;

	// �A�j���[�V����
	bool animation = true;
	float addAlpha = 0.01;
	float animationRot = 0;
	bool animationEND = false;
	int animationTimer = 0;

	// SE
	bool playWav15;

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
