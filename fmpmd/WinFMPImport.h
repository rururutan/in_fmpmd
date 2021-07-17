//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	ＦＭＰ－ＳＹＳＴＥＭ　常駐型ＦＭ音源ドライバ
//			DLL Import Header
//			Windows Converted by C60
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef WINFMPIMPORT_H
#define WINFMPIMPORT_H

#include	<windows.h>
#include	<stdlib.h>
#include	"PCMMusDriver.h"

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
#ifndef __cplusplus
typedef unsigned char bool;
#endif


//=============================================================================
//	バージョン情報
//=============================================================================
#define	FMP_InterfaceVersion		 10		// 上１桁：major, 下２桁：minor version


//=============================================================================
//	DLL の戻り値
//=============================================================================
#define	WINFMP_OK				 		  0		// 正常終了
#define	FMP_ERR_OPEN_MUSIC_FILE			  1		// 曲 データを開けなかった
#define	FMP_ERR_WRONG_MUSIC_FILE		  2		// FMP の曲データではなかった
#define	FMP_ERR_OPEN_PVI_FILE		 	  3		// PVI を開けなかった
#define	FMP_ERR_OPEN_PPZ1_FILE		 	  6		// PPZ1 を開けなかった
#define	FMP_ERR_WRONG_PVI_FILE		 	  8		// PVI ではなかった
#define	FMP_ERR_WRONG_PPZ1_FILE		 	 11		// PVI ではなかった(PPZ1)
#define	FMP_WARNING_PVI_ALREADY_LOAD	 13		// PVI はすでに読み込まれている
#define	FMP_WARNING_PPZ1_ALREADY_LOAD	 16		// PPZ1 はすでに読み込まれている

#define	FMP_ERR_WRONG_PARTNO			 30		// パート番号が不適
//#define	FMP_ERR_ALREADY_MASKED			 31		// 指定パートはすでにマスクされている
#define	FMP_ERR_NOT_MASKED				 32		// 指定パートはマスクされていない
#define	FMP_ERR_MUSIC_STOPPED			 33		// 曲が止まっているのにマスク操作をした

#define	FMP_ERR_OUT_OF_MEMORY			 99		// メモリを確保できなかった

#define	FMP_MUSDATASIZE				  65536		// 最大曲データサイズ
#define	FMP_COMMENTDATASIZE	 		   8192		// 最大３行コメントサイズ



//=============================================================================
//	ＷＩＮＦＭＰ専用の定義
//=============================================================================
#define	SOUND_55K					  55555
#define	SOUND_48K					  48000
#define	SOUND_44K					  44100
#define	SOUND_22K					  22050
#define	SOUND_11K					  11025

#define	PPZ8_i0						  44100
#define	PPZ8_i1						  33080
#define	PPZ8_i2						  22050
#define	PPZ8_i3						  16540
#define	PPZ8_i4						  11025
#define	PPZ8_i5						   8270
#define	PPZ8_i6						   5513
#define	PPZ8_i7						   4135

#define	FMP_MAX_PCMDIR					 64
#define FMP_DEFAULT_REG_WAIT		  15000

//=============================================================================
//	その他定義
//=============================================================================
#define	FMP_NumOfFMPart				     6
#define	FMP_NumOfSSGPart				 3
#define	FMP_NumOfADPCMPart				 1
#define	FMP_NumOFOPNARhythmPart			 1
#define	FMP_NumOfExtPart				 3
#define	FMP_NumOfPPZ8Part			     8
#define	FMP_NumOfAllPart	(FMP_NumOfFMPart+FMP_NumOfSSGPart+FMP_NumOfADPCMPart+FMP_NumOFOPNARhythmPart+FMP_NumOfExtPart+FMP_NumOfPPZ8Part)


//----------------------------------------------------------------------------
//	ＦＭＰ各種状態保持ｂｉｔ
//
//            fedcba98 76543210
// FMP_sysbit xxxxxxxx xxxxxxxx
//            |||||||| |||||||+---　
//            |||||||| ||||||+----　
//            |||||||| |||||+-----　
//            |||||||| ||||+------　ＰＰＺ８でＰＶＩエミュレート中
//            |||||||| |||+-------　
//            |||||||| ||+--------　
//            |||||||| |+---------　ＰＰＺ８演奏中
//            |||||||| +----------　
//            ||||||||
//            |||||||+------------　
//            ||||||+-------------　
//            |||||+--------------　
//            ||||+---------------　
//            |||+----------------　
//            ||+-----------------　フェードアウト中
//            |+------------------　ループした
//            +-------------------　演奏停止中
//
//----------------------------------------------------------------------------

#define	FMP_SYS_PPZ8PVI		0x0008		// ＰＰＺ８エミュレート中
#define	FMP_SYS_PPZ8USE		0x0040		// ＰＰＺ８ファイル使用中
#define	FMP_SYS_FADE		0x2000		// フェードアウト中
#define	FMP_SYS_LOOP		0x4000		// ループした
#define	FMP_SYS_STOP		0x8000		// 演奏停止中
#define	FMP_SYS_INIT		FMP_SYS_STOP


//----------------------------------------------------------------------------
//	ＰＣＭ定義フラグ
//----------------------------------------------------------------------------
#define	FMP_PCM_USEV1		0x0001		// ＰＶＩ１使用中
#define	FMP_PCM_USEZ1		0x0010		// ＰＰＺ１使用中


//============================================================================
//	ＬＦＯワーク構造体定義
//============================================================================
//----------------------------------------------------------------------------
//	ビブラートワーク構造体
//----------------------------------------------------------------------------
typedef struct lfostag
{
	int		LfoSdelay		; // ビブラート ディレイ値
	int		LfoSspeed		; // ビブラート スピード
	int		LfoScnt_dly		; // ビブラート ディレイカウンタ
	int		LfoScnt_spd		; // ビブラート スピードカウンタ
	int		LfoSdepth		; // ビブラート ずらしカウント値
	int		LfoScnt_dep		; // ビブラート ずらしカウンタ
	int		LfoSrate1		; // ビブラート かかり値
	int		LfoSrate2		; // ビブラート かかり値（サブ）
	int		LfoSwave		; // ビブラート 波形
} LFOS;

#define	S_LFOS		sizeof(LFOS)


//----------------------------------------------------------------------------
//	トレモロワーク構造体
//----------------------------------------------------------------------------
typedef struct alfostag
{
	int		AlfoSdelay		; // トレモロ ディレイ値
	int		AlfoSspeed		; // トレモロ スピード
	int		AlfoScnt_dly	; // トレモロ ディレイカウンタ
	int		AlfoScnt_spd	; // トレモロ スピード
	int		AlfoSdepth		; // トレモロ 変化量
	int		AlfoScnt_dep	; // トレモロ 変化量カウンタ
	int		AlfoSrate		; // トレモロ かかり値
	int		AlfoSrate_org	; // トレモロ かかり値
} ALFOS;

#define	S_ALFOS		sizeof(ALFOS)


//----------------------------------------------------------------------------
//	ワウワウワーク構造体
//----------------------------------------------------------------------------
typedef struct wlfostag
{
	int		WlfoSdelay		; // ワウワウ ディレイ値
	int		WlfoSspeed		; // ワウワウ スピード
	int		WlfoScnt_dly	; // ワウワウ ディレイカウンタ
	int		WlfoScnt_spd	; // ワウワウ スピードカウンタ
	int		WlfoSdepth		; // ワウワウ 変化量
	int		WlfoScnt_dep	; // ワウワウ 変化量カウンタ
	int		WlfoSrate		; // ワウワウ かかり値
	int		WlfoSrate_org	; // ワウワウ 現在のずらし値
	int		WlfoSrate_now	; // ワウワウ 現在のずらし値
	int		WlfoSsync		; // ワウワウ シンクロ／マスク
} WLFOS;

#define	S_WLFOS		sizeof(WLFOS)

#define	FMP_WLFO_SYNC	0x80	// シンクロビット
								// 下位４ビットがスロットマスク


//============================================================================
//	ピッチベンドワーク構造体定義
//============================================================================
typedef struct pitstag
{
	int		PitSdat			; // ピッチベンド変化値
	int		PitSdelay		; // ディレイ値
	int		PitSspeed		; // スピード
	int		PitScnt			; // スピードカウンタ
	int		PitSwave		; // 目標周波数
	int		PitStarget		; // 目標音階
} PITS;

#define	S_PITS		sizeof(PITS)


//----------------------------------------------------------------------------
//	ＳＳＧエンベロープワーク構造体
//----------------------------------------------------------------------------
typedef struct envstag
{
	int		EnvSsv			; // スタートヴォユーム
	int		EnvSar			; // アタックレート
	int		EnvSdr			; // ディケイレート
	int		EnvSsl			; // サスティンレベル
	int		EnvSsr			; // サスティンレート
	int		EnvSrr			; // リリースレート
} ENVS;

#define	S_ENVS		sizeof(ENVS)


//============================================================================
//	パートワーク構造体定義
//============================================================================
typedef struct cpatstag
{
	int		PartSlfo_f		; // ＬＦＯ状態フラグ
	int		PartSdeflen		; // デフォルトの音長

	int		PartSvol		; // 現在の音量
	int		PartSdat_q		; // ゲート処理比較値

	int		PartScnt		; // 音長基準カウンタ
	int		PartSorg_q		; // ゲート処理用カウンタ

	int		PartStmpvol		; // 実際の出力音量
	int		PartSdat_k		; // Ｋｅｙｏｎを遅らせる値

	int		PartScnt_k		; // Ｋｅｙｏｎ遅らせ処理用カウンタ
	int		PartSbefore		; // １つ前の音程

	int		PartSstatus		; // 状態フラグ
	int		PartSsync		; // シンクロフラグ

	int		PartSdetune		; // デチューン値
	PITS	PartSpitch		; // ピッチベンド用ワーク
	LFOS	PartSlfo[3]		; // ビブラートＬＦＯ（＃０，＃１，＃２）
	int		PartSwave		; // 実際の出力周波数
	
	int		PartSwave2		; // １つ前の出力周波数

	int		PartSxtrns		; // 音階のずれ用
	int		PartStone		; // 現在の音色番号

	int		PartSkeyon		; // 外部Ｋｅｙｏｎ取得用

	int		PartSpan		; // パン取得用
	int		PartSalg		; // 現在のアルゴリズム番号

	int		PartSio			; // 出力Ｉ／Ｏアドレス
	uchar	*PartSpoint		; // 読み込みポインタ
	uchar	*PartSloop		; // くり返しポインタ
	int		PartSchan		; // チャンネル識別用
	int		PartSbit		; // チャンネル制御bit
	int		PartSport		; // ＦＭ裏アドレスアクセス用
} CPATS;


//----------------------------------------------------------------------------
//	ＦＭパートワーク構造体
//----------------------------------------------------------------------------
typedef struct fpatstag
{
	uchar	*FpatSaddr		; // ＴＬアドレス
	ALFOS	FpatSalfo		; // トレモロ用ワーク
	WLFOS	FpatSwlfo		; // ワウワウ用ワーク
	int		FpatS_hdly		; // ＨＬＦＯディレイ
	int		FpatS_hdlycnt	; // ＨＬＦＯカウンタ
	int		FpatS_hfreq		; // ＨＬＦＯ　ｆｒｅｑ
	int		FpatS_hapms		; // ＨＬＦＯ　ＰＭＳ／ＡＭＳ
	int		FpatSextend		; // extendモード
	int		FpatSslot_v[4]	; // スロットごとの相対値
} FPATS;


//----------------------------------------------------------------------------
//	ＳＳＧパートワーク構造体
//----------------------------------------------------------------------------
typedef struct spatstag
{
	int		SpatSnow_vol	; // 現在の音量
	int		SpatSflg		; // エンヴェロープ状態フラグ
	int		SpatSoct		; // オクターブ
	int		SpatSvol		; // 現在の出力音量
	ENVS	SpatSenv		; // ソフトウェアエンベロープ
	ENVS	*SpatSenvadr	; // SSG Env pattern Address
} SPATS;


//----------------------------------------------------------------------------
//	ＡＤＰＣＭパートワーク構造体
//----------------------------------------------------------------------------
typedef struct apatstag
{
	int		ApatSstart		; // ＰＣＭ スタートアドレス
	int		ApatSend		; // ＰＣＭ エンドアドレス
	int		ApatSdelta		; // ＰＣＭ ΔＮ値
} APATS;

#define	S_APATS		sizeof(APATS)


//----------------------------------------------------------------------------
//	パートワーク構造体
//----------------------------------------------------------------------------
typedef struct patstag
{
	CPATS	CPatS			; // 共通ワーク
	union {
		FPATS	FPatS		; // ＦＭパートワーク構造体
		SPATS	SPatS		; // ＳＳＧパートワーク構造体
		APATS	APatS		; // ＡＤＰＣＭパートワーク構造体
	} EPatS;
} PARTS;

#define	S_PARTS		sizeof(PARTS)


//----------------------------------------------------------------------------
//	フェードアウトデータ構造体
//----------------------------------------------------------------------------
typedef struct fadestag
{
	int		FadeSfm			;
	int		FadeSssg		;
	int		FadeSrhy		;
	int		FadeSapcm		;
} FADES;

#define	S_FADES		sizeof(FADES)


//----------------------------------------------------------------------------
//	外部同期ワーク構造体
//----------------------------------------------------------------------------
typedef struct syncstag
{
	int		SyncSdat		; // 同期データ
	int		SyncScnt		; // 同期カウント
} SYNCS;

#define	S_SYNCS		sizeof(SYNCS)


//----------------------------------------------------------------------------
//	ＦＭＰ内部ワーク構造体
//----------------------------------------------------------------------------
typedef struct fmpstag
{
	int		FmpStempo		; // 00 現在のテンポ
	SYNCS	FmpSsync		; // 01 外部同期データ
	int		FmpSsysbit		; // 0a ＦＭＰステータスｂｉｔ
	int		FmpScnt_c		; // 0c 曲演奏中クロック
	int		FmpScnt_t		; // 0e 待ちカウンタ
	FADES	FmpSfade		; // 0f フェードアウト音量
	FADES	FmpSfade_o		; // 13 フェードアウト音量（オリジナル）
	int		FmpSloop_c		; // 14 曲ループ回数
	int		FmpStempo_t		; // 17 現在のテンポ（予備）
	int		FmpSmix_s		; // 18 ??FEDFED
	int		FmpStimer		; // 19 タイマーロード値
	int		FmpSnoise		; // 1a ＳＳＧのノイズ周波数
	int		FmpSsho			; // 1b 小節カウンタ
	int		FmpSpcmuse		; // 1d ＰＣＭ使用状況
	int		FmpScnt_ct		; // 1f 曲全体カウント数
	int		FmpScnt_cl		; // 23 曲ループカウント数
	int		FmpSmix_e		; // 29 効果音処理中のmixer
	int		FmpStempo_e		; // 2a 効果音時のデフォルトテンポ
} FMPS;

#define	S_FMPS		sizeof(FMPS)


//----------------------------------------------------------------------------
//	全体ワーク
//----------------------------------------------------------------------------
typedef struct workstag
{
	FMPS	ExtBuff;				// 外部参照許可ワーク
	PARTS	_F[FMP_NumOfFMPart];	// ＦＭ音源ワーク
	PARTS	_A;						// ＡＤＰＣＭ音源ワーク
	PARTS	_X[FMP_NumOfExtPart];	// ＦＭextendワーク
	PARTS	_P[FMP_NumOfPPZ8Part];	// ＰＣＭ(ppz8)音源ワーク
	PARTS	_S[FMP_NumOfSSGPart];	// ＳＳＧ音源ワーク
	
	//------------------------------------------------------------------------
	//	リズム音源部ワーク
	//------------------------------------------------------------------------
	int		R_key[16];				// 絶対に変えられない
	int		R_mask;
	int		R_Oncho_cnt;
	int		R_Oncho_def;
	int		RTL_vol;
	int		R_vol[6];
	int		R_pan[6];
	int		R_Loop_now;
	int		R_Sync_flg;
	int		R_State_flg;			// イネーブルフラグ
	PARTS	_R;

	//------------------------------------------------------------------------
	//	ｂｙｔｅデータワーク
	//------------------------------------------------------------------------
	int		TotalLoop;				// ループ終了カウンタ
	int		Loop_cnt;				// ループ終了回数
	int		Int_fcT;				// 割り込みフェードカウンタ
	int		Int_fc;					// 割り込みフェードカウンタ
	int		TimerA_cnt;				// Ｔｉｍｅｒディレイ
	int		Ver;					// 曲データバージョン
	int		NowPPZmode;				// 現在のＰＰＺの再生モード
	int		MusicClockCnt;			// 曲のクロックカウント(C??)
	int		ClockCnt;				// クロックカウント
	int		PcmHardVol;				// ＰＣＭハード音量
	int		ExtendKeyon;			// extend状態の3chのkeyon
	int		ExtendAlg;				// extendチャンネルのアルゴリズム

	//------------------------------------------------------------------------
	//	ｗｏｒｄデータワーク
	//------------------------------------------------------------------------
	int		FM_effect_dat[4];		// 効果音モードずらし値
	int		Play_flg;				// 演奏中フラグ
	int		Loop_flg;				// ループフラグ
	int		Int_CX;					// 割り込みフェードカウンタ
	
	//------------------------------------------------------------------------
	//	チャンネル別ワークアドレステーブル
	//------------------------------------------------------------------------
	PARTS	*Chan_tbl_R;					// リズム
	PARTS	*Chan_tbl[FMP_NumOfFMPart+FMP_NumOfSSGPart+FMP_NumOfPPZ8Part+1+FMP_NumOfExtPart+1];

	//------------------------------------------------------------------------
	//	ＳＳＧエンベロープワーク
	//------------------------------------------------------------------------
	ENVS	EnvAddr[FMP_NumOfSSGPart];
	
	//------------------------------------------------------------------------
	//	演奏中の曲のファイル名
	//------------------------------------------------------------------------
	char	Music_name[_MAX_PATH];	// 演奏中の曲のファイル名

	//------------------------------------------------------------------------
	//	現在のＰＶＩファイル名
	//------------------------------------------------------------------------
	char	PVI_name[_MAX_PATH];	// 現在のＰＣＭファイル名
	
	//------------------------------------------------------------------------
	//	ＷＩＮＦＭＰ専用データ
	//------------------------------------------------------------------------
	int		rate;				//	PCM 出力周波数(11k, 22k, 44k, 55k)
	int		ppzrate;			//	PPZ 出力周波数
	bool	fmcalc55k;							// FM で 55kHz 合成をするか？
	bool	ppz8ip;								// PPZ8 で補完するか
	int		fadeout2_speed;						// fadeout(高音質)speed(>0で fadeout)
	int		lastSyncExtTime;					// 最後に Sync_Ext を実行した時間(ms/カラオケ用）
	char	pcmdir[FMP_MAX_PCMDIR+1][_MAX_PATH];	//	PCM 検索ディレクトリ
} WORKS;

#define	S_WORKS			sizeof(WORKS)



//============================================================================
//	COM 風 interface class
//============================================================================
interface IWINFMP : public IFMPMD {
	virtual int WINAPI maskon(bool rhythm_flag, int ah) = 0;
	virtual int WINAPI maskoff(bool rhythm_flag, int ah) = 0;
	virtual void WINAPI setfmvoldown(int voldown) = 0;
	virtual void WINAPI setssgvoldown(int voldown) = 0;
	virtual void WINAPI setrhythmvoldown(int voldown) = 0;
	virtual void WINAPI setadpcmvoldown(int voldown) = 0;
	virtual void WINAPI setppzvoldown(int voldown) = 0;
	virtual int WINAPI getfmvoldown(void) = 0;
	virtual int WINAPI getssgvoldown(void) = 0;
	virtual int WINAPI getrhythmvoldown(void) = 0;
	virtual int WINAPI getadpcmvoldown(void) = 0;
	virtual int WINAPI getppzvoldown(void) = 0;
	virtual char* WINAPI getcomment(char *dest, uchar *musdata, int size) = 0;
	virtual char* WINAPI getcomment2(char *dest, uchar *musdata, int size) = 0;
	virtual char** WINAPI getcomment3(char *dest[], uchar *musdata, int size) = 0;
	virtual int WINAPI fgetcomment(char *dest, char *filename) = 0;
	virtual int WINAPI fgetcomment2(char *dest, char *filename) = 0;
	virtual int WINAPI fgetcomment3(char *dest[], char *filename) = 0;
	virtual char* WINAPI getdefinedpcmfilename(char *dest, uchar *musdata, int size) = 0;
	virtual char* WINAPI getdefinedppzfilename(char *dest, uchar *musdata, int size, int bufnum) = 0;
	virtual int WINAPI fgetdefinedpcmfilename(char *dest, char *filename) = 0;
	virtual int WINAPI fgetdefinedppzfilename(char *dest, char *filename, int bufnum) = 0;
	virtual int WINAPI getsyncscnt(void) = 0;
	virtual int WINAPI getlastsyncexttime(void) = 0;
	virtual WORKS* WINAPI getworks(void) = 0;
	virtual void WINAPI setadpcmppz8emulate(bool flag) = 0;
};


//=============================================================================
//	Interface ID(IID) & Class ID(CLSID)
//=============================================================================

// GUID of IWINFMP Interface ID
interface	__declspec(uuid("B7910277-0295-4052-9A65-5AD48D0F3477")) IWINFMP;

// GUID of WINFMP Class ID
class		__declspec(uuid("3E7816B4-EB8F-435F-BC37-01CEDBF42287")) WINFMP;

const IID	IID_IWINFMP		= _uuidof(IWINFMP);	// IWINFMP Interface ID
const CLSID	CLSID_WINFMP	= _uuidof(WINFMP);	// WINFMP Class ID


//=============================================================================
//	DLL Export Functions
//=============================================================================

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllimport) int WINAPI fmp_getversion(void);
__declspec(dllimport) int WINAPI fmp_getinterfaceversion(void);
__declspec(dllimport) bool WINAPI fmp_init(char *path);
__declspec(dllimport) int WINAPI fmp_load(char *filename);
__declspec(dllimport) int WINAPI fmp_load2(uchar *musdata, int size);
__declspec(dllimport) void WINAPI fmp_start(void);
__declspec(dllimport) int WINAPI fmp_stop(void);
__declspec(dllimport) void WINAPI fmp_getpcmdata(short *buf, int nsamples);
__declspec(dllimport) int WINAPI fmp_maskon(bool rhythm_flag, int ah);
__declspec(dllimport) int WINAPI fmp_maskoff(bool rhythm_flag, int ah);
__declspec(dllimport) bool WINAPI fmp_loadrhythmsample(char *path);
__declspec(dllimport) bool WINAPI fmp_setpcmdir(char **path);
__declspec(dllimport) void WINAPI fmp_setpcmrate(int rate);
__declspec(dllimport) void WINAPI fmp_setppzrate(int rate);
__declspec(dllimport) void WINAPI fmp_fadeout(int speed);
__declspec(dllimport) void WINAPI fmp_fadeout2(int speed);
__declspec(dllimport) void WINAPI fmp_setfmcalc55k(bool flag);
__declspec(dllimport) void WINAPI fmp_setppzinterpolation(bool ip);
__declspec(dllimport) void WINAPI fmp_setadpcmppz8emulate(bool flag);
__declspec(dllimport) char * WINAPI fmp_getcomment(char *dest, uchar *musdata, int size);
__declspec(dllimport) char * WINAPI fmp_getcomment2(char *dest, uchar *musdata, int size);
__declspec(dllimport) char ** WINAPI fmp_getcomment3(char *dest[], uchar *musdata, int size);
__declspec(dllimport) int WINAPI fmp_fgetcomment(char *dest, char *filename);
__declspec(dllimport) int WINAPI fmp_fgetcomment2(char *dest, char *filename);
__declspec(dllimport) int WINAPI fmp_fgetcomment3(char *dest[], char *filename);
__declspec(dllimport) char * WINAPI fmp_getdefinedpcmfilename(char *dest, uchar *musdata, int size);
__declspec(dllimport) char * WINAPI fmp_getdefinedppzfilename(char *dest, uchar *musdata, int size, int bufnum);
__declspec(dllimport) int WINAPI fmp_fgetdefinedpcmfilename(char *dest, char *filename);
__declspec(dllimport) int WINAPI fmp_fgetdefinedppzfilename(char *dest, char *filename, int bufnum);
__declspec(dllimport) char * WINAPI fmp_getmusicfilename(char *dest);
__declspec(dllimport) char * WINAPI fmp_getpcmfilename(char *dest);
__declspec(dllimport) char * WINAPI fmp_getppzfilename(char *dest, int bufnum);
__declspec(dllimport) void WINAPI fmp_setfmvoldown(int voldown);
__declspec(dllimport) void WINAPI fmp_setssgvoldown(int voldown);
__declspec(dllimport) void WINAPI fmp_setrhythmvoldown(int voldown);
__declspec(dllimport) void WINAPI fmp_setadpcmvoldown(int voldown);
__declspec(dllimport) void WINAPI fmp_setppzvoldown(int voldown);
__declspec(dllimport) int WINAPI fmp_getfmvoldown(void);
__declspec(dllimport) int WINAPI fmp_getssgvoldown(void);
__declspec(dllimport) int WINAPI fmp_getrhythmvoldown(void);
__declspec(dllimport) int WINAPI fmp_getadpcmvoldown(void);
__declspec(dllimport) int WINAPI fmp_getppzvoldown(void);
__declspec(dllimport) void WINAPI fmp_setpos(int pos);
__declspec(dllimport) void WINAPI fmp_setpos2(int pos);
__declspec(dllimport) int WINAPI fmp_getpos(void);
__declspec(dllimport) int WINAPI fmp_getpos2(void);
__declspec(dllimport) bool WINAPI fmp_getlength(char *filename, int *length, int *loop);
__declspec(dllimport) bool WINAPI fmp_getlength2(char *filename, int *length, int *loop);
__declspec(dllimport) int WINAPI fmp_getloopcount(void);
__declspec(dllimport) void WINAPI fmp_setfmwait(int nsec);
__declspec(dllimport) void WINAPI fmp_setssgwait(int nsec);
__declspec(dllimport) void WINAPI fmp_setrhythmwait(int nsec);
__declspec(dllimport) void WINAPI fmp_setadpcmwait(int nsec);
__declspec(dllimport) int WINAPI fmp_getsyncscnt(void);
__declspec(dllimport) int WINAPI fmp_getlastsyncexttime(void);
__declspec(dllimport) WORKS * WINAPI fmp_getworks(void);
__declspec(dllimport) HRESULT WINAPI fmp_CoCreateInstance(
  REFCLSID rclsid,     //Class identifier (CLSID) of the object
  LPUNKNOWN pUnkOuter, //Pointer to whether object is or isn't part 
                       // of an aggregate
  DWORD dwClsContext,  //Context for running executable code
  REFIID riid,         //Reference to the identifier of the interface
  LPVOID * ppv         //Address of output variable that receives 
                       // the interface pointer requested in riid
);


#ifdef __cplusplus
}
#endif


#endif // WINFMPIMPORT_H
