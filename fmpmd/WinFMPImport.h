//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	�e�l�o�|�r�x�r�s�d�l�@�풓�^�e�l�����h���C�o
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
//	�o�[�W�������
//=============================================================================
#define	FMP_InterfaceVersion		 10		// ��P���Fmajor, ���Q���Fminor version


//=============================================================================
//	DLL �̖߂�l
//=============================================================================
#define	WINFMP_OK				 		  0		// ����I��
#define	FMP_ERR_OPEN_MUSIC_FILE			  1		// �� �f�[�^���J���Ȃ�����
#define	FMP_ERR_WRONG_MUSIC_FILE		  2		// FMP �̋ȃf�[�^�ł͂Ȃ�����
#define	FMP_ERR_OPEN_PVI_FILE		 	  3		// PVI ���J���Ȃ�����
#define	FMP_ERR_OPEN_PPZ1_FILE		 	  6		// PPZ1 ���J���Ȃ�����
#define	FMP_ERR_WRONG_PVI_FILE		 	  8		// PVI �ł͂Ȃ�����
#define	FMP_ERR_WRONG_PPZ1_FILE		 	 11		// PVI �ł͂Ȃ�����(PPZ1)
#define	FMP_WARNING_PVI_ALREADY_LOAD	 13		// PVI �͂��łɓǂݍ��܂�Ă���
#define	FMP_WARNING_PPZ1_ALREADY_LOAD	 16		// PPZ1 �͂��łɓǂݍ��܂�Ă���

#define	FMP_ERR_WRONG_PARTNO			 30		// �p�[�g�ԍ����s�K
//#define	FMP_ERR_ALREADY_MASKED			 31		// �w��p�[�g�͂��łɃ}�X�N����Ă���
#define	FMP_ERR_NOT_MASKED				 32		// �w��p�[�g�̓}�X�N����Ă��Ȃ�
#define	FMP_ERR_MUSIC_STOPPED			 33		// �Ȃ��~�܂��Ă���̂Ƀ}�X�N���������

#define	FMP_ERR_OUT_OF_MEMORY			 99		// ���������m�ۂł��Ȃ�����

#define	FMP_MUSDATASIZE				  65536		// �ő�ȃf�[�^�T�C�Y
#define	FMP_COMMENTDATASIZE	 		   8192		// �ő�R�s�R�����g�T�C�Y



//=============================================================================
//	�v�h�m�e�l�o��p�̒�`
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
//	���̑���`
//=============================================================================
#define	FMP_NumOfFMPart				     6
#define	FMP_NumOfSSGPart				 3
#define	FMP_NumOfADPCMPart				 1
#define	FMP_NumOFOPNARhythmPart			 1
#define	FMP_NumOfExtPart				 3
#define	FMP_NumOfPPZ8Part			     8
#define	FMP_NumOfAllPart	(FMP_NumOfFMPart+FMP_NumOfSSGPart+FMP_NumOfADPCMPart+FMP_NumOFOPNARhythmPart+FMP_NumOfExtPart+FMP_NumOfPPZ8Part)


//----------------------------------------------------------------------------
//	�e�l�o�e���ԕێ�������
//
//            fedcba98 76543210
// FMP_sysbit xxxxxxxx xxxxxxxx
//            |||||||| |||||||+---�@
//            |||||||| ||||||+----�@
//            |||||||| |||||+-----�@
//            |||||||| ||||+------�@�o�o�y�W�ło�u�h�G�~�����[�g��
//            |||||||| |||+-------�@
//            |||||||| ||+--------�@
//            |||||||| |+---------�@�o�o�y�W���t��
//            |||||||| +----------�@
//            ||||||||
//            |||||||+------------�@
//            ||||||+-------------�@
//            |||||+--------------�@
//            ||||+---------------�@
//            |||+----------------�@
//            ||+-----------------�@�t�F�[�h�A�E�g��
//            |+------------------�@���[�v����
//            +-------------------�@���t��~��
//
//----------------------------------------------------------------------------

#define	FMP_SYS_PPZ8PVI		0x0008		// �o�o�y�W�G�~�����[�g��
#define	FMP_SYS_PPZ8USE		0x0040		// �o�o�y�W�t�@�C���g�p��
#define	FMP_SYS_FADE		0x2000		// �t�F�[�h�A�E�g��
#define	FMP_SYS_LOOP		0x4000		// ���[�v����
#define	FMP_SYS_STOP		0x8000		// ���t��~��
#define	FMP_SYS_INIT		FMP_SYS_STOP


//----------------------------------------------------------------------------
//	�o�b�l��`�t���O
//----------------------------------------------------------------------------
#define	FMP_PCM_USEV1		0x0001		// �o�u�h�P�g�p��
#define	FMP_PCM_USEZ1		0x0010		// �o�o�y�P�g�p��


//============================================================================
//	�k�e�n���[�N�\���̒�`
//============================================================================
//----------------------------------------------------------------------------
//	�r�u���[�g���[�N�\����
//----------------------------------------------------------------------------
typedef struct lfostag
{
	int		LfoSdelay		; // �r�u���[�g �f�B���C�l
	int		LfoSspeed		; // �r�u���[�g �X�s�[�h
	int		LfoScnt_dly		; // �r�u���[�g �f�B���C�J�E���^
	int		LfoScnt_spd		; // �r�u���[�g �X�s�[�h�J�E���^
	int		LfoSdepth		; // �r�u���[�g ���炵�J�E���g�l
	int		LfoScnt_dep		; // �r�u���[�g ���炵�J�E���^
	int		LfoSrate1		; // �r�u���[�g ������l
	int		LfoSrate2		; // �r�u���[�g ������l�i�T�u�j
	int		LfoSwave		; // �r�u���[�g �g�`
} LFOS;

#define	S_LFOS		sizeof(LFOS)


//----------------------------------------------------------------------------
//	�g���������[�N�\����
//----------------------------------------------------------------------------
typedef struct alfostag
{
	int		AlfoSdelay		; // �g������ �f�B���C�l
	int		AlfoSspeed		; // �g������ �X�s�[�h
	int		AlfoScnt_dly	; // �g������ �f�B���C�J�E���^
	int		AlfoScnt_spd	; // �g������ �X�s�[�h
	int		AlfoSdepth		; // �g������ �ω���
	int		AlfoScnt_dep	; // �g������ �ω��ʃJ�E���^
	int		AlfoSrate		; // �g������ ������l
	int		AlfoSrate_org	; // �g������ ������l
} ALFOS;

#define	S_ALFOS		sizeof(ALFOS)


//----------------------------------------------------------------------------
//	���E���E���[�N�\����
//----------------------------------------------------------------------------
typedef struct wlfostag
{
	int		WlfoSdelay		; // ���E���E �f�B���C�l
	int		WlfoSspeed		; // ���E���E �X�s�[�h
	int		WlfoScnt_dly	; // ���E���E �f�B���C�J�E���^
	int		WlfoScnt_spd	; // ���E���E �X�s�[�h�J�E���^
	int		WlfoSdepth		; // ���E���E �ω���
	int		WlfoScnt_dep	; // ���E���E �ω��ʃJ�E���^
	int		WlfoSrate		; // ���E���E ������l
	int		WlfoSrate_org	; // ���E���E ���݂̂��炵�l
	int		WlfoSrate_now	; // ���E���E ���݂̂��炵�l
	int		WlfoSsync		; // ���E���E �V���N���^�}�X�N
} WLFOS;

#define	S_WLFOS		sizeof(WLFOS)

#define	FMP_WLFO_SYNC	0x80	// �V���N���r�b�g
								// ���ʂS�r�b�g���X���b�g�}�X�N


//============================================================================
//	�s�b�`�x���h���[�N�\���̒�`
//============================================================================
typedef struct pitstag
{
	int		PitSdat			; // �s�b�`�x���h�ω��l
	int		PitSdelay		; // �f�B���C�l
	int		PitSspeed		; // �X�s�[�h
	int		PitScnt			; // �X�s�[�h�J�E���^
	int		PitSwave		; // �ڕW���g��
	int		PitStarget		; // �ڕW���K
} PITS;

#define	S_PITS		sizeof(PITS)


//----------------------------------------------------------------------------
//	�r�r�f�G���x���[�v���[�N�\����
//----------------------------------------------------------------------------
typedef struct envstag
{
	int		EnvSsv			; // �X�^�[�g���H���[��
	int		EnvSar			; // �A�^�b�N���[�g
	int		EnvSdr			; // �f�B�P�C���[�g
	int		EnvSsl			; // �T�X�e�B�����x��
	int		EnvSsr			; // �T�X�e�B�����[�g
	int		EnvSrr			; // �����[�X���[�g
} ENVS;

#define	S_ENVS		sizeof(ENVS)


//============================================================================
//	�p�[�g���[�N�\���̒�`
//============================================================================
typedef struct cpatstag
{
	int		PartSlfo_f		; // �k�e�n��ԃt���O
	int		PartSdeflen		; // �f�t�H���g�̉���

	int		PartSvol		; // ���݂̉���
	int		PartSdat_q		; // �Q�[�g������r�l

	int		PartScnt		; // ������J�E���^
	int		PartSorg_q		; // �Q�[�g�����p�J�E���^

	int		PartStmpvol		; // ���ۂ̏o�͉���
	int		PartSdat_k		; // �j����������x�点��l

	int		PartScnt_k		; // �j���������x�点�����p�J�E���^
	int		PartSbefore		; // �P�O�̉���

	int		PartSstatus		; // ��ԃt���O
	int		PartSsync		; // �V���N���t���O

	int		PartSdetune		; // �f�`���[���l
	PITS	PartSpitch		; // �s�b�`�x���h�p���[�N
	LFOS	PartSlfo[3]		; // �r�u���[�g�k�e�n�i���O�C���P�C���Q�j
	int		PartSwave		; // ���ۂ̏o�͎��g��
	
	int		PartSwave2		; // �P�O�̏o�͎��g��

	int		PartSxtrns		; // ���K�̂���p
	int		PartStone		; // ���݂̉��F�ԍ�

	int		PartSkeyon		; // �O���j���������擾�p

	int		PartSpan		; // �p���擾�p
	int		PartSalg		; // ���݂̃A���S���Y���ԍ�

	int		PartSio			; // �o�͂h�^�n�A�h���X
	uchar	*PartSpoint		; // �ǂݍ��݃|�C���^
	uchar	*PartSloop		; // ����Ԃ��|�C���^
	int		PartSchan		; // �`�����l�����ʗp
	int		PartSbit		; // �`�����l������bit
	int		PartSport		; // �e�l���A�h���X�A�N�Z�X�p
} CPATS;


//----------------------------------------------------------------------------
//	�e�l�p�[�g���[�N�\����
//----------------------------------------------------------------------------
typedef struct fpatstag
{
	uchar	*FpatSaddr		; // �s�k�A�h���X
	ALFOS	FpatSalfo		; // �g�������p���[�N
	WLFOS	FpatSwlfo		; // ���E���E�p���[�N
	int		FpatS_hdly		; // �g�k�e�n�f�B���C
	int		FpatS_hdlycnt	; // �g�k�e�n�J�E���^
	int		FpatS_hfreq		; // �g�k�e�n�@��������
	int		FpatS_hapms		; // �g�k�e�n�@�o�l�r�^�`�l�r
	int		FpatSextend		; // extend���[�h
	int		FpatSslot_v[4]	; // �X���b�g���Ƃ̑��Βl
} FPATS;


//----------------------------------------------------------------------------
//	�r�r�f�p�[�g���[�N�\����
//----------------------------------------------------------------------------
typedef struct spatstag
{
	int		SpatSnow_vol	; // ���݂̉���
	int		SpatSflg		; // �G�����F���[�v��ԃt���O
	int		SpatSoct		; // �I�N�^�[�u
	int		SpatSvol		; // ���݂̏o�͉���
	ENVS	SpatSenv		; // �\�t�g�E�F�A�G���x���[�v
	ENVS	*SpatSenvadr	; // SSG Env pattern Address
} SPATS;


//----------------------------------------------------------------------------
//	�`�c�o�b�l�p�[�g���[�N�\����
//----------------------------------------------------------------------------
typedef struct apatstag
{
	int		ApatSstart		; // �o�b�l �X�^�[�g�A�h���X
	int		ApatSend		; // �o�b�l �G���h�A�h���X
	int		ApatSdelta		; // �o�b�l ���m�l
} APATS;

#define	S_APATS		sizeof(APATS)


//----------------------------------------------------------------------------
//	�p�[�g���[�N�\����
//----------------------------------------------------------------------------
typedef struct patstag
{
	CPATS	CPatS			; // ���ʃ��[�N
	union {
		FPATS	FPatS		; // �e�l�p�[�g���[�N�\����
		SPATS	SPatS		; // �r�r�f�p�[�g���[�N�\����
		APATS	APatS		; // �`�c�o�b�l�p�[�g���[�N�\����
	} EPatS;
} PARTS;

#define	S_PARTS		sizeof(PARTS)


//----------------------------------------------------------------------------
//	�t�F�[�h�A�E�g�f�[�^�\����
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
//	�O���������[�N�\����
//----------------------------------------------------------------------------
typedef struct syncstag
{
	int		SyncSdat		; // �����f�[�^
	int		SyncScnt		; // �����J�E���g
} SYNCS;

#define	S_SYNCS		sizeof(SYNCS)


//----------------------------------------------------------------------------
//	�e�l�o�������[�N�\����
//----------------------------------------------------------------------------
typedef struct fmpstag
{
	int		FmpStempo		; // 00 ���݂̃e���|
	SYNCS	FmpSsync		; // 01 �O�������f�[�^
	int		FmpSsysbit		; // 0a �e�l�o�X�e�[�^�X������
	int		FmpScnt_c		; // 0c �ȉ��t���N���b�N
	int		FmpScnt_t		; // 0e �҂��J�E���^
	FADES	FmpSfade		; // 0f �t�F�[�h�A�E�g����
	FADES	FmpSfade_o		; // 13 �t�F�[�h�A�E�g���ʁi�I���W�i���j
	int		FmpSloop_c		; // 14 �ȃ��[�v��
	int		FmpStempo_t		; // 17 ���݂̃e���|�i�\���j
	int		FmpSmix_s		; // 18 ??FEDFED
	int		FmpStimer		; // 19 �^�C�}�[���[�h�l
	int		FmpSnoise		; // 1a �r�r�f�̃m�C�Y���g��
	int		FmpSsho			; // 1b ���߃J�E���^
	int		FmpSpcmuse		; // 1d �o�b�l�g�p��
	int		FmpScnt_ct		; // 1f �ȑS�̃J�E���g��
	int		FmpScnt_cl		; // 23 �ȃ��[�v�J�E���g��
	int		FmpSmix_e		; // 29 ���ʉ���������mixer
	int		FmpStempo_e		; // 2a ���ʉ����̃f�t�H���g�e���|
} FMPS;

#define	S_FMPS		sizeof(FMPS)


//----------------------------------------------------------------------------
//	�S�̃��[�N
//----------------------------------------------------------------------------
typedef struct workstag
{
	FMPS	ExtBuff;				// �O���Q�Ƌ����[�N
	PARTS	_F[FMP_NumOfFMPart];	// �e�l�������[�N
	PARTS	_A;						// �`�c�o�b�l�������[�N
	PARTS	_X[FMP_NumOfExtPart];	// �e�lextend���[�N
	PARTS	_P[FMP_NumOfPPZ8Part];	// �o�b�l(ppz8)�������[�N
	PARTS	_S[FMP_NumOfSSGPart];	// �r�r�f�������[�N
	
	//------------------------------------------------------------------------
	//	���Y�����������[�N
	//------------------------------------------------------------------------
	int		R_key[16];				// ��΂ɕς����Ȃ�
	int		R_mask;
	int		R_Oncho_cnt;
	int		R_Oncho_def;
	int		RTL_vol;
	int		R_vol[6];
	int		R_pan[6];
	int		R_Loop_now;
	int		R_Sync_flg;
	int		R_State_flg;			// �C�l�[�u���t���O
	PARTS	_R;

	//------------------------------------------------------------------------
	//	���������f�[�^���[�N
	//------------------------------------------------------------------------
	int		TotalLoop;				// ���[�v�I���J�E���^
	int		Loop_cnt;				// ���[�v�I����
	int		Int_fcT;				// ���荞�݃t�F�[�h�J�E���^
	int		Int_fc;					// ���荞�݃t�F�[�h�J�E���^
	int		TimerA_cnt;				// �s���������f�B���C
	int		Ver;					// �ȃf�[�^�o�[�W����
	int		NowPPZmode;				// ���݂̂o�o�y�̍Đ����[�h
	int		MusicClockCnt;			// �Ȃ̃N���b�N�J�E���g(C??)
	int		ClockCnt;				// �N���b�N�J�E���g
	int		PcmHardVol;				// �o�b�l�n�[�h����
	int		ExtendKeyon;			// extend��Ԃ�3ch��keyon
	int		ExtendAlg;				// extend�`�����l���̃A���S���Y��

	//------------------------------------------------------------------------
	//	���������f�[�^���[�N
	//------------------------------------------------------------------------
	int		FM_effect_dat[4];		// ���ʉ����[�h���炵�l
	int		Play_flg;				// ���t���t���O
	int		Loop_flg;				// ���[�v�t���O
	int		Int_CX;					// ���荞�݃t�F�[�h�J�E���^
	
	//------------------------------------------------------------------------
	//	�`�����l���ʃ��[�N�A�h���X�e�[�u��
	//------------------------------------------------------------------------
	PARTS	*Chan_tbl_R;					// ���Y��
	PARTS	*Chan_tbl[FMP_NumOfFMPart+FMP_NumOfSSGPart+FMP_NumOfPPZ8Part+1+FMP_NumOfExtPart+1];

	//------------------------------------------------------------------------
	//	�r�r�f�G���x���[�v���[�N
	//------------------------------------------------------------------------
	ENVS	EnvAddr[FMP_NumOfSSGPart];
	
	//------------------------------------------------------------------------
	//	���t���̋Ȃ̃t�@�C����
	//------------------------------------------------------------------------
	char	Music_name[_MAX_PATH];	// ���t���̋Ȃ̃t�@�C����

	//------------------------------------------------------------------------
	//	���݂̂o�u�h�t�@�C����
	//------------------------------------------------------------------------
	char	PVI_name[_MAX_PATH];	// ���݂̂o�b�l�t�@�C����
	
	//------------------------------------------------------------------------
	//	�v�h�m�e�l�o��p�f�[�^
	//------------------------------------------------------------------------
	int		rate;				//	PCM �o�͎��g��(11k, 22k, 44k, 55k)
	int		ppzrate;			//	PPZ �o�͎��g��
	bool	fmcalc55k;							// FM �� 55kHz ���������邩�H
	bool	ppz8ip;								// PPZ8 �ŕ⊮���邩
	int		fadeout2_speed;						// fadeout(������)speed(>0�� fadeout)
	int		lastSyncExtTime;					// �Ō�� Sync_Ext �����s��������(ms/�J���I�P�p�j
	char	pcmdir[FMP_MAX_PCMDIR+1][_MAX_PATH];	//	PCM �����f�B���N�g��
} WORKS;

#define	S_WORKS			sizeof(WORKS)



//============================================================================
//	COM �� interface class
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
