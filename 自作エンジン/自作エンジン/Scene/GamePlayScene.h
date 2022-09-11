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
public: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 定数
	// デバッグテキスト用テクスチャの番号
	const int fontNumber = 0;

private: // メモリ置き場
	// DIrectXCommon
	DirectXCommon* dx_cmd = nullptr;
	// 操作系
	Input* input = nullptr;
	// オーディオ
	//Audio* audio = nullptr;
	// カメラ
	Camera* camera = nullptr;
	// デバッグテキスト
	DebugText debugText;

private: //サブクラス
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

private: // インスタンス
	// ライト
	std::unique_ptr<Light> light = nullptr;
	// パーティクル
	std::unique_ptr<ParticleManager> playerWalkEffect;
	std::unique_ptr<ParticleManager> playerJumpEffect;
	std::unique_ptr<ParticleManager> ballJumpEffect;
	std::unique_ptr<ParticleManager> ballBounceEffect;
	std::unique_ptr<ParticleManager> enemyBounceEffect;
	std::unique_ptr<ParticleManager> enemySpawnEffect;
	// スプライト
	
	// OBJオブジェクト
	// プレイヤー
	std::unique_ptr<Object3d> playerStay = nullptr;
	std::unique_ptr<Object3d> playerJump = nullptr;
	std::unique_ptr<Object3d> playerMove01 = nullptr;
	std::unique_ptr<Object3d> playerMove02 = nullptr;
	std::unique_ptr<Object3d> playerMove03 = nullptr;
	std::unique_ptr<Object3d> playerMove04 = nullptr;
	// ブロック
	std::unique_ptr<BLOCK> block[8][13] = {};
	// ボール
	std::unique_ptr<Object3d> ball01 = nullptr;
	std::unique_ptr<Object3d> ball02 = nullptr;
	std::unique_ptr<Object3d> ball03 = nullptr;
	std::unique_ptr<Object3d> ball04 = nullptr;
	// エネミー
	std::unique_ptr<ENEMYSTAY> enemyStay[13] = {};
	std::unique_ptr<ENEMY01> enemy01[13] = {};
	std::unique_ptr<ENEMY02> enemy02[13] = {};
	// エネミースポナー
	std::unique_ptr<ENEMYSPAWNER> enemySpawner[13] = {};
	// フレーム
	std::unique_ptr<Object3d> frame = nullptr;
	// ニードル
	std::unique_ptr<Object3d> needle[13] = {};
	// ハート
	std::unique_ptr<Object3d> heart[3] = {};
	// FBXオブジェクト


private: // メンバ変数
	// プレイヤー
	// 速さ
	float playerS;
	// ジャンプの速さ
	float playerJS;
	// ジャンプフラグ
	bool isPJ;

	int playerWalkEffectTimer = 10;
	// ボール
	// 速さ
	float ballS;
	// 回転
	float ballR;
	// ジャンプの速さ
	float ballJS;
	// ジャンプの加速度
	float ballJA;
	// ジャンプフラグ
	bool isBJ;
	//	重力落下速度
	float ballG;
	// 重力
	float gravity = 0.1f;

	// コンボ
	// タイマー
	int comboTimer;
	// コンボの猶予時間
	int comboLimit;
	// コンボカウンター
	int comboNum;

	// スポーン
	// タイマー
	int spawTimer;
	// 拡大率
	float spawnerScale;
	// 加速度
	float spawnerSA;

	// ハート
	// ハートカウンター
	int heartCounter;

	bool switchingMove = true;
	int moveCount = 0;
	bool isBlink = false;
	int blinkCount = 0;

public: // メンバ関数
	~GamePlayScene() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 変数初期化
	/// </summary>
	void InitializeVariable();

	/// <summary>
	/// 背景スプライト描画
	/// </summary>
	void DrawBackSprite(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画
	/// </summary>
	void DrawObjects(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// エフェクト描画
	/// </summary>
	void DrawEffect(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// デバッグテキスト描画
	/// </summary>
	void DrawDebugText(ID3D12GraphicsCommandList* cmdList);

	const float LenAB(XMFLOAT3 posA, XMFLOAT3 posB);
};
