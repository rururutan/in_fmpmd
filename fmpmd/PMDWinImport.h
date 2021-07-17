//=============================================================================
//	Professional Music Driver [P.M.D.] version 4.8
//			DLL Import Header
//			Programmed By M.Kajihara
//			Windows Converted by C60
//=============================================================================

#ifndef PMDWINIMPORT_H
#define PMDWINIMPORT_H

#include <stdlib.h>
//#include "types.h"
#include "PCMMusDriver.h"


#define	InterfaceVersion	117		// ��P���Fmajor, ���Q���Fminor version

//	DLL �� �߂�l
#define	PMDWIN_OK				 	  0		// ����I��
#define	ERR_OPEN_MUSIC_FILE			  1		// �� �f�[�^���J���Ȃ�����
#define	ERR_WRONG_MUSIC_FILE		  2		// PMD �̋ȃf�[�^�ł͂Ȃ�����
#define	ERR_OPEN_PPC_FILE		 	  3		// PPC ���J���Ȃ�����
#define	ERR_OPEN_P86_FILE		 	  4		// P86 ���J���Ȃ�����
#define	ERR_OPEN_PPS_FILE		 	  5		// PPS ���J���Ȃ�����
#define	ERR_OPEN_PPZ1_FILE		 	  6		// PPZ1 ���J���Ȃ�����
#define	ERR_OPEN_PPZ2_FILE		 	  7		// PPZ2 ���J���Ȃ�����
#define	ERR_WRONG_PPC_FILE		 	  8		// PPC/PVI �ł͂Ȃ�����c
#define	ERR_WRONG_P86_FILE		 	  9		// P86 �ł͂Ȃ�����
#define	ERR_WRONG_PPS_FILE		 	 10		// PPS �ł͂Ȃ�����
#define	ERR_WRONG_PPZ1_FILE		 	 11		// PVI/PZI �ł͂Ȃ�����(PPZ1)
#define	ERR_WRONG_PPZ2_FILE		 	 12		// PVI/PZI �ł͂Ȃ�����(PPZ2)
#define	WARNING_PPC_ALREADY_LOAD	 13		// PPC �͂��łɓǂݍ��܂�Ă���
#define	WARNING_P86_ALREADY_LOAD	 14		// P86 �͂��łɓǂݍ��܂�Ă���
#define	WARNING_PPS_ALREADY_LOAD	 15		// PPS �͂��łɓǂݍ��܂�Ă���
#define	WARNING_PPZ1_ALREADY_LOAD	 16		// PPZ1 �͂��łɓǂݍ��܂�Ă���
#define	WARNING_PPZ2_ALREADY_LOAD	 17		// PPZ2 �͂��łɓǂݍ��܂�Ă���

#define	ERR_WRONG_PARTNO			 30		// �p�[�g�ԍ����s�K
//#define	ERR_ALREADY_MASKED			 31		// �w��p�[�g�͂��łɃ}�X�N����Ă���
#define	ERR_NOT_MASKED				 32		// �w��p�[�g�̓}�X�N����Ă��Ȃ�
#define	ERR_MUSIC_STOPPED			 33		// �Ȃ��~�܂��Ă���̂Ƀ}�X�N���������
#define	ERR_EFFECT_USED				 34		// ���ʉ��Ŏg�p���Ȃ̂Ń}�X�N�𑀍�ł��Ȃ�

#define	ERR_OUT_OF_MEMORY			 99		// ���������m�ۂł��Ȃ�����
#define	ERR_OTHER					999		// ���̑��̃G���[


#define	SOUND_55K			  55555
#define	SOUND_48K			  48000
#define	SOUND_44K			  44100
#define	SOUND_22K			  22050
#define	SOUND_11K			  11025

#define	PPZ8_i0				  44100
#define	PPZ8_i1				  33080
#define	PPZ8_i2				  22050
#define	PPZ8_i3				  16540
#define	PPZ8_i4				  11025
#define	PPZ8_i5				   8270
#define	PPZ8_i6				   5513
#define	PPZ8_i7				   4135

#define	MAX_PCMDIR				 64
#define	MAX_MEMOBUF			   1024

#define	NumOfFMPart			      6
#define	NumOfSSGPart		      3
#define	NumOfADPCMPart		      1
#define	NumOFOPNARhythmPart	      1
#define	NumOfExtPart		      3
#define	NumOfRhythmPart		      1
#define	NumOfEffPart		      1
#define	NumOfPPZ8Part		      8
#define	NumOfAllPart		(NumOfFMPart+NumOfSSGPart+NumOfADPCMPart+NumOFOPNARhythmPart+NumOfExtPart+NumOfRhythmPart+NumOfEffPart+NumOfPPZ8Part)

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
#ifndef __cplusplus
typedef unsigned char bool;
#endif


/******************************************************************************
;	WORK AREA
******************************************************************************/


//	���t���̃f�[�^�G���A
typedef struct qqtag {
	uchar	*address;			//	2 �ݿ���� � ���ڽ
	uchar	*partloop;			//	2 �ݿ� �� �ܯ�ķ � ���ػ�
	int		leng;				//	1 ɺ� LENGTH
	int		qdat;				//	1 gatetime (q/Q�l���v�Z�����l)
	uint	fnum;				//	2 �ݿ���� � BLOCK/FNUM
	int		detune;				//	2 ������
	int		lfodat;				//	2 LFO DATA
	int		porta_num;			//	2 �|���^�����g�̉����l�i�S�́j
	int		porta_num2;			//	2 �|���^�����g�̉����l�i���j
	int		porta_num3;			//	2 �|���^�����g�̉����l�i�]��j
	int		volume;				//	1 VOLUME
	int		shift;				//	1 �ݶ� ��� � ���
	int		delay;				//	1 LFO	[DELAY] 
	int		speed;				//	1	[SPEED]
	int		step;				//	1	[STEP]
	int		time;				//	1	[TIME]
	int		delay2;				//	1	[DELAY_2]
	int		speed2;				//	1	[SPEED_2]
	int		step2;				//	1	[STEP_2]
	int		time2;				//	1	[TIME_2]
	int		lfoswi;				//	1 LFOSW. B0/tone B1/vol B2/���� B3/porta
								//	         B4/tone B5/vol B6/����
	int		volpush;			//	1 Volume PUSHarea
	int		mdepth;				//	1 M depth
	int		mdspd;				//	1 M speed
	int		mdspd2;				//	1 M speed_2
	int		envf;				//	1 PSG ENV. [START_FLAG] / -1��extend
	int		eenv_count;			//	1 ExtendPSGenv/No=0 AR=1 DR=2 SR=3 RR=4
	int		eenv_ar;			//	1 	/AR		/��pat
	int		eenv_dr;			//	1	/DR		/��pv2
	int		eenv_sr;			//	1	/SR		/��pr1
	int		eenv_rr;			//	1	/RR		/��pr2
	int		eenv_sl;			//	1	/SL
	int		eenv_al;			//	1	/AL
	int		eenv_arc;			//	1	/AR�̃J�E���^	/��patb
	int		eenv_drc;			//	1	/DR�̃J�E���^
	int		eenv_src;			//	1	/SR�̃J�E���^	/��pr1b
	int		eenv_rrc;			//	1	/RR�̃J�E���^	/��pr2b
	int		eenv_volume;		//	1	/Volume�l(0�`15)/��penv
	int		extendmode;			//	1 B1/Detune B2/LFO B3/Env Normal/Extend
	int		fmpan;				//	1 FM Panning + AMD + PMD
	int		psgpat;				//	1 PSG PATTERN [TONE/NOISE/MIX]
	int		voicenum;			//	1 ���F�ԍ�
	int		loopcheck;			//	1 ���[�v������P �I��������R
	int		carrier;			//	1 FM Carrier
	int		slot1;				//	1 SLOT 1 � TL
	int		slot3;				//	1 SLOT 3 � TL
	int		slot2;				//	1 SLOT 2 � TL
	int		slot4;				//	1 SLOT 4 � TL
	int		slotmask;			//	1 FM slotmask
	int		neiromask;			//	1 FM ���F��`�pmaskdata
	int		lfo_wave;			//	1 LFO�̔g�`
	int		partmask;			//	1 PartMask b0:�ʏ� b1:���ʉ� b2:NECPCM�p
								//	   b3:none b4:PPZ/ADE�p b5:s0�� b6:m b7:�ꎞ
	int		keyoff_flag;		//	1 Keyoff�������ǂ�����Flag
	int		volmask;			//	1 ����LFO�̃}�X�N
	int		qdata;				//	1 q�̒l
	int		qdatb;				//	1 Q�̒l
	int		hldelay;			//	1 HardLFO delay
	int		hldelay_c;			//	1 HardLFO delay Counter
	int		_lfodat;			//	2 LFO DATA
	int		_delay;				//	1 LFO	[DELAY] 
	int		_speed;				//	1		[SPEED]
	int		_step;				//	1		[STEP]
	int		_time;				//	1		[TIME]
	int		_delay2;			//	1		[DELAY_2]
	int		_speed2;			//	1		[SPEED_2]
	int		_step2;				//	1		[STEP_2]
	int		_time2;				//	1		[TIME_2]
	int		_mdepth;			//	1 M depth
	int		_mdspd;				//	1 M speed
	int		_mdspd2;			//	1 M speed_2
	int		_lfo_wave;			//	1 LFO�̔g�`
	int		_volmask;			//	1 ����LFO�̃}�X�N
	int		mdc;				//	1 M depth Counter (�ϓ��l)
	int		mdc2;				//	1 M depth Counter
	int		_mdc;				//	1 M depth Counter (�ϓ��l)
	int		_mdc2;				//	1 M depth Counter
	int		onkai;				//	1 ���t���̉��K�f�[�^ (0ffh:rest)
	int		sdelay;				//	1 Slot delay
	int		sdelay_c;			//	1 Slot delay counter
	int		sdelay_m;			//	1 Slot delay Mask
	int		alg_fb;				//	1 ���F��alg/fb
	int		keyon_flag;			//	1 �V���K/�x���f�[�^������������inc
	int		qdat2;				//	1 q �Œ�ۏؒl
	int		onkai_def;			//	1 ���t���̉��K�f�[�^ (�]�������O / ?fh:rest)
	int		shift_def;			//	1 �}�X�^�[�]���l
	int		qdat3;				//	1 q Random
} QQ;


typedef struct OpenWorktag {
	QQ *MusPart[NumOfAllPart];	// �p�[�g���[�N�̃|�C���^
	uchar	*mmlbuf;			//	Musicdata��address+1
	uchar	*tondat;			//	Voicedata��address
	uchar	*efcdat;			//	FM  Effecdata��address
	uchar	*prgdat_adr;		//	�ȃf�[�^�����F�f�[�^�擪�Ԓn
	ushort	*radtbl;			//	R part offset table �擪�Ԓn
	uchar	*rhyadr;			//	R part ���t���Ԓn
	int		rhythmmask;			//	Rhythm�����̃}�X�N x8c/10h��bit�ɑΉ�
	int		fm_voldown;			//	FM voldown ���l
	int		ssg_voldown;		//	PSG voldown ���l
	int		pcm_voldown;		//	ADPCM voldown ���l
	int		rhythm_voldown;		//	RHYTHM voldown ���l
	int		prg_flg;			//	�ȃf�[�^�ɉ��F���܂܂�Ă��邩flag
	int		x68_flg;			//	OPM flag
	int		status;				//	status1
	int		status2;			//	status2
	int		tempo_d;			//	tempo (TIMER-B)
	int		fadeout_speed;		//	Fadeout���x
	int		fadeout_volume;		//	Fadeout����
	int		tempo_d_push;		//	tempo (TIMER-B) / �ۑ��p
	int		syousetu_lng;		//	���߂̒���
	int		opncount;			//	�ŒZ�����J�E���^
	int		TimerAtime;			//	TimerA�J�E���^
	int		effflag;			//	PSG���ʉ�����on/off flag(���[�U�[�����)
	int		psnoi;				//	PSG noise���g��
	int		psnoi_last;			//	PSG noise���g��(�Ō�ɒ�`�������l)
	int		pcmstart;			//	PCM���F��start�l
	int		pcmstop;			//	PCM���F��stop�l
	int		rshot_dat;			//	���Y������ shot flag
	int		rdat[6];			//	���Y������ ����/�p���f�[�^
	int		rhyvol;				//	���Y���g�[�^�����x��
	int		kshot_dat;			//	�r�r�f���Y�� shot flag
	int		play_flag;			//	play flag
	int		fade_stop_flag;		//	Fadeout�� MSTOP���邩�ǂ����̃t���O
	bool	kp_rhythm_flag;		//	K/Rpart��Rhythm������炷��flag
	int		pcm_gs_flag;		//	ADPCM�g�p ���t���O (0�ŋ���)
	int		slot_detune1;		//	FM3 Slot Detune�l slot1
	int		slot_detune2;		//	FM3 Slot Detune�l slot2
	int		slot_detune3;		//	FM3 Slot Detune�l slot3
	int		slot_detune4;		//	FM3 Slot Detune�l slot4
	int		TimerB_speed;		//	TimerB�̌��ݒl(=ff_tempo�Ȃ�ff��)
	int		fadeout_flag;		//	��������fout���Ăяo������1
	int		revpan;				//	PCM86�t��flag
	int		pcm86_vol;			//	PCM86�̉��ʂ�SPB�ɍ��킹�邩?
	int		syousetu;			//	���߃J�E���^
	int		port22h;			//	OPN-PORT 22H �ɍŌ�ɏo�͂����l(hlfo)
	int		tempo_48;			//	���݂̃e���|(clock=48 t�̒l)
	int		tempo_48_push;		//	���݂̃e���|(����/�ۑ��p)
	int		_fm_voldown;		//	FM voldown ���l (�ۑ��p)
	int		_ssg_voldown;		//	PSG voldown ���l (�ۑ��p)
	int		_pcm_voldown;		//	PCM voldown ���l (�ۑ��p)
	int		_rhythm_voldown;	//	RHYTHM voldown ���l (�ۑ��p)
	int		_pcm86_vol;			//	PCM86�̉��ʂ�SPB�ɍ��킹�邩? (�ۑ��p)
	int		rshot_bd;			//	���Y������ shot inc flag (BD)
	int		rshot_sd;			//	���Y������ shot inc flag (SD)
	int		rshot_sym;			//	���Y������ shot inc flag (CYM)
	int		rshot_hh;			//	���Y������ shot inc flag (HH)
	int		rshot_tom;			//	���Y������ shot inc flag (TOM)
	int		rshot_rim;			//	���Y������ shot inc flag (RIM)
	int		rdump_bd;			//	���Y������ dump inc flag (BD)
	int		rdump_sd;			//	���Y������ dump inc flag (SD)
	int		rdump_sym;			//	���Y������ dump inc flag (CYM)
	int		rdump_hh;			//	���Y������ dump inc flag (HH)
	int		rdump_tom;			//	���Y������ dump inc flag (TOM)
	int		rdump_rim;			//	���Y������ dump inc flag (RIM)
	int		ch3mode;			//	ch3 Mode
	int		ppz_voldown;		//	PPZ8 voldown ���l
	int		_ppz_voldown;		//	PPZ8 voldown ���l (�ۑ��p)
	int		TimerAflag;			//	TimerA���荞�ݒ��H�t���O�i���s�v�H�j
	int		TimerBflag;			//	TimerB���荞�ݒ��H�t���O�i���s�v�H�j

	// for PMDWin
	int		rate;				//	PCM �o�͎��g��(11k, 22k, 44k, 55k)
	bool	ppz8ip;								//	PPZ8 �ŕ⊮���邩
	bool	ppsip;								//	PPS  �ŕ⊮���邩
	bool	p86ip;								//	P86  �ŕ⊮���邩
	bool	use_p86;							//	P86  ���g�p���Ă��邩
	int		fadeout2_speed;						//	fadeout(������)speed(>0�� fadeout)
	char	mus_filename[_MAX_PATH];			//	�Ȃ�FILE���o�b�t�@
	char	ppcfilename[_MAX_PATH];				//	PPC ��FILE���o�b�t�@
	char	pcmdir[MAX_PCMDIR+1][_MAX_PATH];	//	PCM �����f�B���N�g��
} OPEN_WORK;


//=============================================================================
//	COM �� interface class
//=============================================================================
interface IPMDWIN : public IFMPMD {
	virtual void WINAPI setppsuse(bool value) = 0;
	virtual void WINAPI setrhythmwithssgeffect(bool value) = 0;
	virtual void WINAPI setpmd86pcmmode(bool value) = 0;
	virtual bool WINAPI getpmd86pcmmode(void) = 0;
	virtual void WINAPI setppsinterpolation(bool ip) = 0;
	virtual void WINAPI setp86interpolation(bool ip) = 0;
	virtual int WINAPI maskon(int ch) = 0;
	virtual int WINAPI maskoff(int ch) = 0;
	virtual void WINAPI setfmvoldown(int voldown) = 0;
	virtual void WINAPI setssgvoldown(int voldown) = 0;
	virtual void WINAPI setrhythmvoldown(int voldown) = 0;
	virtual void WINAPI setadpcmvoldown(int voldown) = 0;
	virtual void WINAPI setppzvoldown(int voldown) = 0;
	virtual int WINAPI getfmvoldown(void) = 0;
	virtual int WINAPI getfmvoldown2(void) = 0;
	virtual int WINAPI getssgvoldown(void) = 0;
	virtual int WINAPI getssgvoldown2(void) = 0;
	virtual int WINAPI getrhythmvoldown(void) = 0;
	virtual int WINAPI getrhythmvoldown2(void) = 0;
	virtual int WINAPI getadpcmvoldown(void) = 0;
	virtual int WINAPI getadpcmvoldown2(void) = 0;
	virtual int WINAPI getppzvoldown(void) = 0;
	virtual int WINAPI getppzvoldown2(void) = 0;
	virtual char* WINAPI getmemo(char *dest, uchar *musdata, int size, int al) = 0;
	virtual char* WINAPI getmemo2(char *dest, uchar *musdata, int size, int al) = 0;
	virtual char* WINAPI getmemo3(char *dest, uchar *musdata, int size, int al) = 0;
	virtual int	WINAPI fgetmemo(char *dest, char *filename, int al) = 0;
	virtual int	WINAPI fgetmemo2(char *dest, char *filename, int al) = 0;
	virtual int	WINAPI fgetmemo3(char *dest, char *filename, int al) = 0;
	virtual char* WINAPI getppcfilename(char *dest) = 0;
	virtual char* WINAPI getppsfilename(char *dest) = 0;
	virtual char* WINAPI getp86filename(char *dest) = 0;
	virtual int WINAPI ppc_load(char *filename) = 0;
	virtual int WINAPI pps_load(char *filename) = 0;
	virtual int WINAPI p86_load(char *filename) = 0;
	virtual int WINAPI ppz_load(char *filename, int bufnum) = 0;
	virtual OPEN_WORK* WINAPI getopenwork(void) = 0;
	virtual QQ* WINAPI getpartwork(int ch) = 0;
};


//=============================================================================
//	Interface ID(IID) & Class ID(CLSID)
//=============================================================================

// GUID of IPMDWIN Interface ID
interface	__declspec(uuid("C07008F4-CAE0-421C-B08F-D8B319AFA4B4")) IPMDWIN;	

// GUID of PMDWIN Class ID
class		__declspec(uuid("97C7C3F0-35D8-4304-8C1B-AA926E7AEC5C")) PMDWIN;

const IID	IID_IPMDWIN		= _uuidof(IPMDWIN);	// IPMDWIN Interface ID
const CLSID	CLSID_PMDWIN	= _uuidof(PMDWIN);	// PMDWIN Class ID


//=============================================================================
//	DLL Export Functions
//=============================================================================

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllimport) int WINAPI getversion(void);
__declspec(dllimport) int WINAPI getinterfaceversion(void);
__declspec(dllimport) bool WINAPI pmdwininit(char *path);
__declspec(dllimport) bool WINAPI loadrhythmsample(char *path);
__declspec(dllimport) bool WINAPI setpcmdir(char **path);
__declspec(dllimport) void WINAPI setpcmrate(int rate);
__declspec(dllimport) void WINAPI setppzrate(int rate);
__declspec(dllimport) void WINAPI setppsuse(bool value);
__declspec(dllimport) void WINAPI setrhythmwithssgeffect(bool value);
__declspec(dllimport) void WINAPI setpmd86pcmmode(bool value);
__declspec(dllimport) bool WINAPI getpmd86pcmmode(void);
__declspec(dllimport) int WINAPI music_load(char *filename);
__declspec(dllimport) int WINAPI music_load2(uchar *musdata, int size);
__declspec(dllimport) void WINAPI music_start(void);
__declspec(dllimport) void WINAPI music_stop(void);
__declspec(dllimport) void WINAPI fadeout(int speed);
__declspec(dllimport) void WINAPI fadeout2(int speed);
__declspec(dllimport) void WINAPI getpcmdata(short *buf, int nsamples);
__declspec(dllimport) void WINAPI setfmcalc55k(bool flag);
__declspec(dllimport) void WINAPI setppsinterpolation(bool ip);
__declspec(dllimport) void WINAPI setp86interpolation(bool ip);
__declspec(dllimport) void WINAPI setppzinterpolation(bool ip);
__declspec(dllimport) char * WINAPI getmemo(char *dest, uchar *musdata, int size, int al);
__declspec(dllimport) char * WINAPI getmemo2(char *dest, uchar *musdata, int size, int al);
__declspec(dllimport) char * WINAPI getmemo3(char *dest, uchar *musdata, int size, int al);
__declspec(dllimport) char * WINAPI fgetmemo(char *dest, char *filename, int al);
__declspec(dllimport) char * WINAPI fgetmemo2(char *dest, char *filename, int al);
__declspec(dllimport) char * WINAPI fgetmemo3(char *dest, char *filename, int al);
__declspec(dllimport) char * WINAPI getmusicfilename(char *dest);
__declspec(dllimport) char * WINAPI getpcmfilename(char *dest);
__declspec(dllimport) char * WINAPI getppcfilename(char *dest);
__declspec(dllimport) char * WINAPI getppsfilename(char *dest);
__declspec(dllimport) char * WINAPI getp86filename(char *dest);
__declspec(dllimport) char * WINAPI getppzfilename(char *dest, int bufnum);
__declspec(dllimport) int WINAPI ppc_load(char *filename);
__declspec(dllimport) int WINAPI pps_load(char *filename);
__declspec(dllimport) int WINAPI p86_load(char *filename);
__declspec(dllimport) int WINAPI ppz_load(char *filename, int bufnum);
__declspec(dllimport) int WINAPI maskon(int ch);
__declspec(dllimport) int WINAPI maskoff(int ch);
__declspec(dllimport) void WINAPI setfmvoldown(int voldown);
__declspec(dllimport) void WINAPI setssgvoldown(int voldown);
__declspec(dllimport) void WINAPI setrhythmvoldown(int voldown);
__declspec(dllimport) void WINAPI setadpcmvoldown(int voldown);
__declspec(dllimport) void WINAPI setppzvoldown(int voldown);
__declspec(dllimport) int WINAPI getfmvoldown(void);
__declspec(dllimport) int WINAPI getfmvoldown2(void);
__declspec(dllimport) int WINAPI getssgvoldown(void);
__declspec(dllimport) int WINAPI getssgvoldown2(void);
__declspec(dllimport) int WINAPI getrhythmvoldown(void);
__declspec(dllimport) int WINAPI getrhythmvoldown2(void);
__declspec(dllimport) int WINAPI getadpcmvoldown(void);
__declspec(dllimport) int WINAPI getadpcmvoldown2(void);
__declspec(dllimport) int WINAPI getppzvoldown(void);
__declspec(dllimport) int WINAPI getppzvoldown2(void);
__declspec(dllimport) void WINAPI setpos(int pos);
__declspec(dllimport) void WINAPI setpos2(int pos);
__declspec(dllimport) int WINAPI getpos(void);
__declspec(dllimport) int WINAPI getpos2(void);
__declspec(dllimport) bool WINAPI getlength(char *filename, int *length, int *loop);
__declspec(dllimport) bool WINAPI getlength2(char *filename, int *length, int *loop);
__declspec(dllimport) int WINAPI getloopcount(void);
__declspec(dllimport) void WINAPI setfmwait(int nsec);
__declspec(dllimport) void WINAPI setssgwait(int nsec);
__declspec(dllimport) void WINAPI setrhythmwait(int nsec);
__declspec(dllimport) void WINAPI setadpcmwait(int nsec);
__declspec(dllimport) OPEN_WORK * WINAPI getopenwork(void);
__declspec(dllimport) QQ * WINAPI getpartwork(int ch);
__declspec(dllimport) HRESULT WINAPI pmd_CoCreateInstance(
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


#endif // PMDWINIMPORT_H
