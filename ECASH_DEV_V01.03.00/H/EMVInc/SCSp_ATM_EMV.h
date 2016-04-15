/***************************************************************************
                    SCSp_ATM_EMV_Crypt.h  -  description
                         -------------------
begin                : May 06 2010
copyright            : (C) 2010 by SCSpro
***************************************************************************/
#include "SCSp_ATM_EMV_V5.h"
#include "SCSp_ATM_EMV_Crypt.h"

// EMVL2Kernel2, EMVL2Kernel5에서 중복된 항목은 "SCSp_ATM_EMV_V5.h"로 이동함

#define		EMV_TYPE_APPROVAL	1			// 인증 시에만 사용

//---------------------------  DEFINE RESULT VALUE ------------------------//
/************************************************************************/

#define		EMV_TRUE_VAL			1
#define		EMV_FALSE_VAL			0


//------------------------------ DEFINE DEBUG TYPE----------------------------//
#define     INTEGER     0
#define     STR     	1
#define     HEX     	2
#define		INTSTR  	4
#define		INTHEX  	5
#define		STRHEX  	6
  
//----------------------------------------------------------------------------        	
//------------------------ Description about Structure -----------------------//
//----------------------------------------------------------------------------        	

typedef struct {
	int		aidLen;
	unsigned char	aid[16];
}EMVst_AIDLIST;

typedef  struct {
	int		aidCNT;
	EMVst_AIDLIST	aidI[ICEMV_APP_CNT];
}EMVst_LIST_OF_AID;

typedef  struct
{
    unsigned char	AppLabelLen;
    unsigned char	AppLabel[32];
    unsigned char	AidLen;
    unsigned char	AidName[16];
    unsigned char	priority;
    unsigned char	IsrCntryCodeLen;
    unsigned char	IsrCntryCode[3];
    unsigned char	ApSelCodeLen;
    unsigned char	ApSelCode[32];
} ICC_APP_DESC;

typedef  struct
{
    int			CardHolderConfirm;
    int			AppCnt;
    ICC_APP_DESC	IccAppDesc[ICEMV_APP_CNT];
} EMV_ICC_APPS_INFORM;

typedef  struct
{
    unsigned char	AppLabelLen;
    unsigned char	AppLabel[32];
    unsigned char	AidLen;
    unsigned char	AidName[16];
    unsigned char	priority;
    unsigned char	IsrCntryCodeLen;
    unsigned char	IsrCntryCode[3];
    unsigned char	ApSelCodeLen;
    unsigned char	ApSelCode[32];
	unsigned char   IsrCodeTableIndex;
	unsigned char   Reserved[63];
} ICC_APP_DESC_EX;

typedef  struct
{
    int					CardHolderConfirm;
    int					AppCnt;
    ICC_APP_DESC_EX		IccAppDesc[ICEMV_APP_CNT];
} EMV_ICC_APPS_INFORM_EX;

//------------------------ Description about Extern Functions -----------------------//
#define DLL_IMPORT		__declspec(dllimport)
//-------------------------------------------------------------------
// EMV Application Selection
DLL_IMPORT int emvkrnl_Sel_ApplicationSelection(unsigned char SupportPSE, EMVst_LIST_OF_AID *AIDList, int *CandidateCnt);
DLL_IMPORT int emvkrnl_Sel_MakeDisplayAppList(unsigned char SupportUserConfirm, 
								   unsigned short CandidateCnt, 
								   EMV_ICC_APPS_INFORM *IccAppInform);
DLL_IMPORT int emvkrnl_Sel_MakeDisplayAppListEx(unsigned char SupportUserConfirm, 
								   unsigned short CandidateCnt, 
								   EMV_ICC_APPS_INFORM_EX *IccAppInformEx);								   
