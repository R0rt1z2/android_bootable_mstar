#ifndef  _R850_H_ 
#define _R850_H_
#define DEBUG_MODE  FALSE


#define VERSION   "R850_GUI_v1.2F_MSTAR_Format"
#define VER_NUM   0
#define CHIP_ID       0x98
//----------------------------------------------------------//
//                   Type Define                                    //
//----------------------------------------------------------//
#define TRUE   1
#define FALSE 0
//----------------------------------------------------------//
//                   Define                                            //
//----------------------------------------------------------//
//===========Clock out Setting=========//  
#define R850_CLOCK_OUT_ENABLE_AFTER_INIT  FALSE
#define R850_Xtal 24000                //Xtal clock rate.
#define R850_SHARE_XTAL    FALSE     //if share Xtal, xtal_gm = off (R32[4:3]=2'b11)


#define R850_REG_NUM         48  //R0~R7: read only
#define R850_RING_POWER_FREQ  115000
#define R850_IMR_IF              5300         
#define R850_IMR_TRIAL       9
#define R850_IMR_GAIN_REG     20
#define R850_IMR_PHASE_REG   21
#define R850_IMR_IQCAP_REG   21
#define R850_IMR_POINT_NUM  10
//----------------------------------------------------------//
//                   Internal Structure                            //
//----------------------------------------------------------//
typedef struct _R850_Sys_Info_Type
{
	MS_U8          BW;
	MS_U8		   HPF_COR;
	MS_U8          FILT_EXT_ENA;
	MS_U8          HPF_NOTCH;  
	MS_U8          AGC_CLK; //
	MS_U8          IMG_GAIN;  
	MS_U16		   FILT_COMP;
	MS_U16		   IF_KHz;            
	MS_U16		   FILT_CAL_IF;
    MS_U32         RF_KHz;
}R850_Sys_Info_Type;

typedef struct _R850_Freq_Info_Type
{
	MS_U8		RF_POLY;
	MS_U8		LNA_BAND;
	MS_U8		LPF_CAP;
	MS_U8		LPF_NOTCH;
	MS_U8		TF_DIPLEXER;
	MS_U8		TF_HPF_BPF;
	MS_U8		TF_HPF_CNR;
	MS_U8		IMR_MEM_NOR;
	MS_U8		IMR_MEM_REV;
	MS_U8       TEMP;    
}R850_Freq_Info_Type;

typedef struct _R850_SysFreq_Info_Type
{
	MS_U8	   LNA_TOP;
	MS_U8	   LNA_VTL_H;
	MS_U8      RF_TOP;
	MS_U8      RF_VTL_H;
	MS_U8      RF_GAIN_LIMIT;
	MS_U8      NRB_TOP;
	MS_U8      NRB_BW_HPF;
	MS_U8      NRB_BW_LPF;
	MS_U8	   MIXER_TOP;
	MS_U8	   MIXER_VTH;
	MS_U8	   MIXER_VTL;
	MS_U8      MIXER_GAIN_LIMIT;
	MS_U8      FILTER_TOP;
	MS_U8      FILTER_VTH;
	MS_U8      FILTER_VTL;
	MS_U8      LNA_RF_DIS_MODE;
	MS_U8      LNA_RF_DIS_CURR;
	MS_U8      LNA_DIS_SLOW_FAST;
	MS_U8      RF_DIS_SLOW_FAST;
	MS_U8      BB_DET_MODE;
	MS_U8      BB_DIS_CURR;
	MS_U8      MIXER_FILTER_DIS;
	MS_U8      IMG_NRB_ADDER;
	MS_U8      LNA_NRB_DET;
	MS_U8      ENB_POLY_GAIN;
	MS_U8	   MIXER_AMP_LPF;
	MS_U8	   NA_PWR_DET; 
	MS_U8	   FILT_3TH_LPF_CUR;
	MS_U8	   FILT_3TH_LPF_GAIN;
	MS_U8	   RF_LTE_PSG;
	MS_U8	   HPF_COMP;
	MS_U8	   FB_RES_1ST;
	MS_U8	   MIXER_DETBW_LPF;
	MS_U8	   LNA_RF_CHARGE_CUR;
	MS_U8      TEMP;
}R850_SysFreq_Info_Type;

typedef struct _R850_Cal_Info_Type
{
	MS_U8       TF_PATH;
	MS_U8		MIXER_AMP_GAIN;
	MS_U8		MIXER_BUFFER_GAIN;
	MS_U8		LNA_GAIN;
	MS_U8		LNA_POWER;
	MS_U8		RFBUF_POWER;
	MS_U8		RFBUF_OUT;
}R850_Cal_Info_Type;

typedef struct _R850_SectType
{
	MS_U8   Phase_Y;
	MS_U8   Gain_X;
	MS_U8   Iqcap;
	MS_U8   Value;
}R850_SectType;

typedef enum _R850_IMR_Type  
{
	R850_IMR_NOR=0,
	R850_IMR_REV,
}R850_IMR_Type;

typedef enum _R850_Cal_Type
{
	R850_IMR_CAL = 0,
	R850_IMR_LNA_CAL,
	R850_LPF_CAL,
	R850_LPF_LNA_CAL
}R850_Cal_Type;

typedef enum _R850_BW_Type
{
	BW_6M = 0,
	BW_7M,
	BW_8M,
	BW_1_7M,
	BW_10M,
	BW_200K
}R850_BW_Type;

typedef enum _R850_NRB_DET_Type  
{
	R850_NRB_DET_ON=0,
	R850_NRB_DET_OFF,
}R850_NRB_DET_Type;

enum XTAL_PWR_VALUE
{
	XTAL_LOWEST = 0,
    XTAL_LOW,
    XTAL_HIGH,
    XTAL_HIGHEST,
	XTAL_CHECK_SIZE
};

typedef enum _Rafael_Chip_Type  //Don't modify chip list
{
	R850_MP = 0,
	R850_MT1
}Rafael_Chip_Type;



//----------------------------------------------------------//
//                   R850 Public Parameter                     //
//----------------------------------------------------------//

typedef enum _R850_ErrCode
{
	RT_Success = TRUE,
	RT_Fail = FALSE
}R850_ErrCode;

typedef enum _TUNER_NUM
{
	R850_TUNER_1 = 0,   //master
	R850_TUNER_2,
	R850_TUNER_3,
	R850_TUNER_4,
	R850_MAX_TUNER_NUM
}R850_TUNER_NUM;

/*
typedef enum _R850_Multi_Tuner_Type
{
	R850_SINGLE_TUNER = 0,                     
	R850_MULTI_TUNER_ONE_MASTER,    
    R850_MULTI_TUNER_MULTI_MASTER,  
}R850_Multi_Tuner_Type;
*/

typedef enum _R850_Standard_Type  //Don't remove standand list!!
{
	//DTV
	R850_DVB_T_6M = 0,  
	R850_DVB_T_7M,
	R850_DVB_T_8M, 
    R850_DVB_T2_6M,       //IF=4.57M
	R850_DVB_T2_7M,       //IF=4.57M
	R850_DVB_T2_8M,       //IF=4.57M
	R850_DVB_T2_1_7M,
	R850_DVB_T2_10M,
	R850_DVB_C_8M,
	R850_DVB_C_6M, 
	R850_J83B,
	R850_ISDB_T_4063,           //IF=4.063M
	R850_ISDB_T_4570,           //IF=4.57M
	R850_DTMB_8M_4570,      //IF=4.57M
	R850_DTMB_6M_4500,      //IF=4.5M, BW=6M
	R850_ATSC,  
	//DTV IF=5M
	R850_DVB_T_6M_IF_5M,
	R850_DVB_T_7M_IF_5M,
	R850_DVB_T_8M_IF_5M,
	R850_DVB_T2_6M_IF_5M,
	R850_DVB_T2_7M_IF_5M,
	R850_DVB_T2_8M_IF_5M,
	R850_DVB_T2_1_7M_IF_5M,
	R850_DVB_C_8M_IF_5M,
	R850_DVB_C_6M_IF_5M, 
	R850_J83B_IF_5M,
	R850_ISDB_T_IF_5M,            
	R850_DTMB_8M_IF_5M,     
	R850_DTMB_6M_IF_5M,     
	R850_ATSC_IF_5M,  
	R850_FM,
	R850_STD_SIZE,
}R850_Standard_Type;


typedef enum _R850_LoopThrough_Type
{
	LT_ON = TRUE,
	LT_OFF= FALSE
}R850_LoopThrough_Type;

typedef enum _R850_ClkOutMode_Type
{
	CLK_OUT_OFF = 0,
	CLK_OUT_ON
}R850_ClkOutMode_Type;

typedef enum _R850_RF_Gain_TYPE
{
	RF_AUTO = 0,
	RF_MANUAL
}R850_RF_Gain_TYPE;

typedef enum _R850_TuningMode
{
	R850_AUTO_SCAN = 0,
	R850_CHANNEL_CHANGE
}R850_TuningMode;
typedef enum _R850_Xtal_Div_TYPE
{
	XTAL_DIV1 = 0,
	XTAL_DIV1_2,	//1st_div2=0(R34[0]), 2nd_div2=1(R34[1])  ; same AGC clock
	XTAL_DIV2_1,	//1st_div2=1(R34[0]), 2nd_div2=0(R34[1])  ; diff AGC clock
	XTAL_DIV4
}R850_Xtal_Div_TYPE;

typedef struct _R850_Set_Info
{
	MS_U32                   RF_KHz;
	R850_Standard_Type       R850_Standard;
	R850_LoopThrough_Type    R850_LT;
	R850_ClkOutMode_Type     R850_ClkOutMode;
}R850_Set_Info;

typedef struct _R850_RF_Gain_Info
{
	MS_U16  RF_gain_comb;
	MS_U8   RF_gain1;
	MS_U8   RF_gain2;
	MS_U8   RF_gain3;
	MS_U8   RF_gain4;
}R850_RF_Gain_Info;

typedef enum _R850_Vga_Mode_TYPE
{
	VGA_AUTO = 0,
	VGA_MANUAL
}R850_Vga_Mode_TYPE;

typedef struct _I2C_LEN_TYPE
{
	MS_U8 RegAddr;
	MS_U8 Data[50];
	MS_U8 Len;
}I2C_LEN_TYPE;

typedef struct _I2C_TYPE
{
	MS_U8 RegAddr;
	MS_U8 Data;
}I2C_TYPE;

//----------------------------------------------------------//
//                   R850 Public Function                       //
//----------------------------------------------------------//
#define R850_Delay_MS    MsOS_DelayTask

R850_ErrCode R850_Init(MS_U8 u8TunerIndex, R850_Standard_Type R850_Standard);
R850_ErrCode R850_SetPllData(MS_U8 u8TunerIndex, R850_Set_Info R850_INFO);
R850_ErrCode R850_SetPllData_Mode(MS_U8 u8TunerIndex, R850_Set_Info R850_INFO, R850_TuningMode R850_Tune_Mode);
R850_ErrCode R850_Standby(MS_U8 u8TunerIndex, R850_LoopThrough_Type R850_STANDBY_LT);
R850_ErrCode R850_WakeUp(MS_U8 u8TunerIndex);
R850_ErrCode R850_SetLoop_Through(MS_U8 u8TunerIndex, R850_LoopThrough_Type R850_LT);
R850_ErrCode R850_GetRfGain(MS_U8 u8TunerIndex, R850_RF_Gain_Info *pR850_rf_gain);
R850_ErrCode R850_RfGainMode(MS_U8 u8TunerIndex, R850_RF_Gain_TYPE R850_RfGainType);
R850_ErrCode R850_GetRfRssi(MS_U8 u8TunerIndex, MS_U32 RF_Freq_Khz, R850_Standard_Type RT_Standard, MS_S32 *RfLevelDbm, MS_U8 *fgRfMaxGain);
MS_U8 R850_PLL_Lock(MS_U8 u8TunerIndex);
R850_ErrCode R850_AGC_Slow(MS_U8 u8TunerIndex);

typedef enum _R850_HpfNotch_Type
{
	HPF_NOTCH_HIGH = 0,
	HPF_NOTCH_LOW
}R850_HpfNotch_Type;

typedef enum _R850_ClkSwing_Type
{
	CLK_SWING_LOW = 0,
	CLK_SWING_HIGH
}R850_ClkSwing_Type;

R850_ErrCode R850_SetHpfNotch(MS_U8 u8TunerIndex, R850_HpfNotch_Type R850_HpfNotch);
R850_ErrCode R850_GetIfRssi(MS_U8 u8TunerIndex, MS_S32 *VgaGain);
R850_ErrCode R850_GetTotalRssi(MS_U8 u8TunerIndex, MS_U32 RF_Freq_Khz, R850_Standard_Type RT_Standard, MS_S32 *RssiDbm);
R850_ErrCode R850_SetXtalCap(MS_U8 u8TunerIndex, MS_U8 u8XtalCap);
R850_ErrCode R850_GetRfRssi(MS_U8 u8TunerIndex, MS_U32 RF_Freq_Khz, R850_Standard_Type RT_Standard, MS_S32 *RfLevelDbm, MS_U8 *fgRfMaxGain);

//-------------for GUI test-------------//
R850_ErrCode R850_SetIfFreq(MS_U8 u8TunerIndex, MS_U16 IF_KHz, MS_U32 RF_KHz, R850_Standard_Type u1SystemStd);
R850_ErrCode R850_SetLpfBw(MS_U8 u8TunerIndex, MS_U8 LPF_BW);
R850_ErrCode R850_SetLpfOffset(MS_U8 u8TunerIndex, MS_U8 LPF_Offset);
R850_ErrCode R850_SetHpfOffset(MS_U8 u8TunerIndex, MS_U8 HPF_Offset);
R850_ErrCode R850_SetIfLpf(MS_U8 u8TunerIndex, MS_U32 LPF_Cor_Freq);

#endif

