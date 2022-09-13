#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <cstdint>
#include <map>
#include <string>

/// <summary>
/// オーディオコールバック
/// </summary>
class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	// ボイス処理パスの開始時
	//STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {};
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
	// ボイス処理パスの終了時
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
	// バッファストリームの再生が終了した時
	STDMETHOD_(void, OnStreamEnd) (THIS) {};
	// バッファの使用開始時
	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
	// バッファの末尾に達した時
	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) {
		// バッファを解放する
		delete[] pBufferContext;
	};
	// 再生がループ位置に達した時
	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
	// ボイスの実行エラー時
	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
};

/// <summary>
/// オーディオ
/// </summary>
class Audio
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public: // サブクラス
	// チャンクヘッダ
	struct Chunk
	{
		char	id[4]; // チャンク毎のID
		int		size;  // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		Chunk	chunk;   // "RIFF"
		char	type[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		Chunk		chunk; // "fmt "
		WAVEFORMAT	fmt;   // 波形フォーマット
	};

	// サウンドデータ
	struct SoundData
	{
		// 波形フォーマット
		WAVEFORMATEX wfex;
		// バッファの先頭アドレス
		BYTE* buffer;
		// バッファのサイズ
		unsigned int bufferSize;
		// 波形フォーマットを元にSourceVoiceの生成する奴
		IXAudio2SourceVoice* sourceVoice;
		// ループ
		bool loop = false;
		// 再生状態
		bool playNow = false;
	};

public: // 静的メンバ関数
	static Audio* GetInstance();

private: // メンバ変数
	ComPtr<IXAudio2> xAudio2;
	XAudio2VoiceCallback voiceCallback;
	std::map<std::string, SoundData> soundList;
	XAUDIO2_BUFFER buf{};

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Audio();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Audio();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	// サウンドファイルの再生
	void PlayWave(const std::string& fileName, bool loop, float volume);

	// サウンドファイルの読み込み
	void LoadSound(const std::string& fileName);

	void StopSound(const std::string& fileName);

	void CreateSoundData(SoundData& soundData);

	void Unload(SoundData* soundList);

	void Finalize();
};

