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

private: // インスタンス
	// ライト
	std::unique_ptr<Light> light = nullptr;

	// パーティクル
	//std::unique_ptr<ParticleManager> particle = nullptr;
	
	// スプライト

	
	// OBJオブジェクト
	// プレイヤー
	std::unique_ptr<Object3d> player = nullptr;
	// ブロック
	std::unique_ptr<BLOCK> block[8][13] = {};
	// ボール
	std::unique_ptr<Object3d> ball = nullptr;
	// エネミー
	std::unique_ptr<ENEMY> enemy[13] = {};
	// エネミースポナー
	std::unique_ptr<ENEMYSPAWNER> enemySpawner[13] = {};
	// フレーム
	std::unique_ptr<Object3d> frame = nullptr;
	// ニードル
	std::unique_ptr<Object3d> needle[13] = {};
	// ハート
	std::unique_ptr<Object3d> heart[3] = {};
	// タイトル
	std::unique_ptr<Object3d> title = nullptr;
	// タイトルボタン
	std::unique_ptr<Object3d> titleButton = nullptr;
	// リザルト
	std::unique_ptr<Object3d> end = nullptr;
	// リザルトボタン
	std::unique_ptr<Object3d> endButton = nullptr;
	// ゲームオーバー
	std::unique_ptr<Object3d> gameover = nullptr;
	// HP 
	std::unique_ptr<Object3d> HP = nullptr;
	// スコア文字
	std::unique_ptr<Object3d> scoreList = nullptr;
	// スコア
	std::unique_ptr<Object3d> scoreNum[10][6] = {};
	std::unique_ptr<Object3d> scoreNumEND[10][6] = {};
	// コンボ
	std::unique_ptr<Object3d> comboText[6] = {};
	// 背景
	std::unique_ptr<Object3d> backGround[2] = {};
	// FBXオブジェクト


private: // メンバ変数
	// プレイヤー
	// 速さ
	float playerS;
	// ジャンプの速さ
	float playerJS;
	// ジャンプフラグ
	bool isPJ;
	// ブロック追加
	bool addB;
	// 横列のどれか
	int addX;
	// イージングタイマー
	int blockTimer;
	// 加速度
	int timerA;

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
	// フラグ
	float isAliveB;

	// コンボ
	// タイマー
	int comboTimer;
	// コンボの猶予時間
	int comboLimit;
	// コンボカウンター
	int comboNum;
	// テキスト
	bool isActiveCT;
	int effectTimer;
	int textNum;

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

	// スコア
	int score;

	// ステージ
	int stage;

	// タイトル
	// ロゴタイマー
	int titleTimer;
	// ロゴの加速度
	int titleA;

	// リザルト
	// 終わるまでのラグタイマー
	int endTimer;
	// ボタンのタイマー
	int buttonTimer;
	// ボタンタイマーの加速度
	int buttonA;

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
