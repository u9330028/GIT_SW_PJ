#if !defined(AFX_EZCADES_H__EBA28BDE_F79E_417E_874C_D6FF0621125A__INCLUDED_)
#define AFX_EZCADES_H__EBA28BDE_F79E_417E_874C_D6FF0621125A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EzCaDes.h : header file
//


/*#H*************************************************************************
* @시 스 템  명 : 대외계/대외계/대외공통
* @프로그램  ID : ezcarddes.h
* @프로그램  명 : 카드사/옥외CD 암호화/복호화 헤더
* @작   성   자 : 응용개발1팀 대외계 장기석
* @작   성   일 : 2003.07.29
* @수 정  내 역 :
*****************************************************************************/
typedef struct {
	BYTE sType[1];
	BYTE sInpData[16];
	BYTE sKeyData[16];
	BYTE sOutData[16];
} SUCARDDES;


/////////////////////////////////////////////////////////////////////////////
// CEzCaDes window

class CEzCaDes
{
public:
	//CEzCaDes();           // protected constructor used by dynamic creation
	//virtual ~CEzCaDes();

// Operations
public:
	void EzCaDes(SUCARDDES *ptDes);
	void EzDes_StringCopy(BYTE psDest[], BYTE psSrce[], int piMax);
	void EzDes_Straight_Permutation (BYTE psLookup[], BYTE psDest[], int piMax);
	void EzDes_InvertPermutation(BYTE psLookup[], BYTE psDest[], int piMax);
	void EzDes_ExpansionPermutation(BYTE psLookup[],BYTE psSrce[],BYTE psDest[]);
	void EzDes_Xor(BYTE psDest[], BYTE psSrce[], int piMax);
	void EzDes_Confusion(BYTE psDest[], BYTE psSrce[], int piNo);
	void EzDes_PermutedChoice (BYTE psLookup[], BYTE psDest[], BYTE psSrce[], int piMax);
	void EzDes_ShiftLeftRegs(BYTE psCreg[], BYTE psDreg[], int piNo);
	void EzDes_ShiftRightRegs(BYTE psCreg[], BYTE psDreg[], int piNo);
	void EzDes_Hex2Bin(BYTE psBin[], BYTE psHex[], int piMax);
	void EzDes_Bin2Hex(BYTE psHex[], BYTE psBin[]);
	void EzDes_Rtn(BYTE psPlainText[], BYTE psOriginKey[], int piMode);
	void EzDes_Bit2Hex(char *psBuf, char *psRetBuf);
	void EzDes_Hex2Bit(char *psRetBuf, int piLen);
	void EzDes_Encryption(SUCARDDES *);
	void EzDes_Decryption(SUCARDDES *);


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EZCADES_H__EBA28BDE_F79E_417E_874C_D6FF0621125A__INCLUDED_)
