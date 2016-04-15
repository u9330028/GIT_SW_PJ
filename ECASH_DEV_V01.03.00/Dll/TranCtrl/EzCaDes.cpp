// EzCaDes.cpp : implementation file
//

#include "stdafx.h"
#include "EzCaDes.h"
#include "ezdesmain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEzCaDes

/*#F*************************************************************************
* @설    명 : 카드사/옥외CD 암호화/복호화 처리를 한다.
* @리 턴 값 : void
* @주의 사항 :
*****************************************************************************/
void CEzCaDes::EzCaDes(SUCARDDES *ptDes)
{
	//ComLog (ZZDBG, PGMLIN, "************** 암호화/복호화 모듈 *************");
	//ComLog (ZZDBG, PGMLIN, "    sType      [%.1s]", ptDes->sType);
	//ComLog (ZZDBG, PGMLIN, "    sOrgData   [%.16s]", ptDes->sInpData);
	//ComLog (ZZDBG, PGMLIN, "    sKeyData   [%.16s]", ptDes->sKeyData);

	if (ptDes->sType[0] == 'E')			/* 암호화	*/
		EzDes_Encryption(ptDes);
	else if (ptDes->sType[0] == 'D')	/* 복호화	*/
		EzDes_Decryption(ptDes);

	//ComLog (ZZDBG, PGMLIN, "    sOutData : [%.16s]", ptDes->sOutData);
	//ComLog (ZZDBG, PGMLIN, "************** 암호화/복호화 모듈 *************");

	return;

} /* End EzDes() */

/*#F*************************************************************************
* @설    명 : 암호화 처리를 한다.
* @리 턴 값 : void
* @주의 사항 :
*****************************************************************************/
void CEzCaDes::EzDes_Encryption (SUCARDDES *ptDes)
{
	BYTE lsInpData[17], lsKeyData[17], lsOutData[17];

	lsInpData[16] = lsKeyData[16] = lsOutData[16] = 0;
	memcpy (lsInpData, ptDes->sInpData, 16);

		/* 입력데이터 검증 */
	EzDes_Hex2Bit((char *)lsInpData, 16);

	memcpy (lsKeyData, ptDes->sKeyData, 16);

	EzDes_Hex2Bit((char *)lsKeyData, 16);

	EzDes_Rtn(lsInpData, lsKeyData, 1);

	EzDes_Bit2Hex((char *)lsInpData, (char *)lsOutData);

	memcpy (ptDes->sOutData, lsOutData, 16);

	return;

} /* End EzDes_Encryption() */

/*#F*************************************************************************
* @설    명 : 복호화 처리를 한다.
* @리 턴 값 : void
* @주의 사항 :
*****************************************************************************/
void CEzCaDes::EzDes_Decryption(SUCARDDES *ptDes)
{
	BYTE lsInpData[17], lsKeyData[17], lsOutData[17];

	lsInpData[16] = lsKeyData[16] = lsOutData[16] = 0;
	memcpy (lsInpData, ptDes->sInpData, 16);

	EzDes_Hex2Bit((char *)lsInpData, 16);

	memcpy (lsKeyData, ptDes->sKeyData, 16);

	EzDes_Hex2Bit((char *)lsKeyData, 16);

	EzDes_Rtn(lsInpData, lsKeyData, 0);

	EzDes_Bit2Hex((char *)lsInpData, (char *)lsOutData);

	memcpy (ptDes->sOutData, lsOutData, 16);

	return;

} /* End EzDes_Decription() */

/*#F*************************************************************************
* @설    명 : A string -> B string변환
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_StringCopy(BYTE psDest[], BYTE psSrce[], int piMax)
{
	int i;
	for (i = 0; i < piMax; i++) psDest[i] = psSrce[i];
	return;
} /* End EzDes_StringCopy() */

/*#F*************************************************************************
* @설    명 : 직접 전치를 수행하는 함수
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_Straight_Permutation (BYTE psLookup[], BYTE psDest[], int 
piMax)
{
	int i, liContent, liCount = 0;
	BYTE *lpSrce;

	lpSrce = (BYTE*) malloc(piMax);
	EzDes_StringCopy(lpSrce, psDest, piMax);

	for (i = 0; i < piMax; i++)
	{
		liContent = psLookup[liCount];
		psDest[liCount++] = lpSrce[liContent-1];
	} /* End for */

	free(lpSrce);
	return;
} /* End EzDes_Straight_Permutation() */

/*#F*************************************************************************
* @설    명 : IP TABLE의 역함수 형태의 전치를 수행하는 함수
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_InvertPermutation(BYTE psLookup[], BYTE psDest[], int piMax)
{
	int  i, liContent, liCount = 0;
	BYTE *lpSrce;

	lpSrce = (BYTE*) malloc(piMax);
	EzDes_StringCopy(lpSrce, psDest, piMax);

	for (i = 0; i < piMax; i++)
	{
		liContent = psLookup[liCount];
		psDest[liContent-1] = lpSrce[liCount++];
	} /* End for */

	free(lpSrce);
	return;
} /* End EzDes_InvertPermutation() */

/*#F*************************************************************************
* @설    명 : 확장된 전치를 수행한다
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_ExpansionPermutation(BYTE psLookup[],BYTE psSrce[],BYTE 
psDest[])
{
	int i, liCount, liContent;

	for (i = 0, liCount = 0; i < EP_SIZE; i++)
	{
		liContent = psLookup[liCount];
		psDest[liCount++] = psSrce[liContent-1];
	} /* End for */
	return;
} /* End EzDes_ExpansionPermutation() */

/*#F*************************************************************************
* @설    명 : 두 문자열을 Exclisive OR 시킨다
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_Xor(BYTE psDest[], BYTE psSrce[], int piMax)
{
	int i;
	for (i = 0; i < piMax; i++) psDest[i] ^= psSrce[i];
	return;
} /* End EzDes_Xor() */

/*#F*************************************************************************
* @설    명 : S-Box 연산을 수행, 입력 : 6비트, 출력 : 4비트
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_Confusion(BYTE psDest[], BYTE psSrce[], int piNo)
{
	int i, liRow, liColumn = 0, liResult;

	liRow = (psSrce[0] << 1) + psSrce[5];

	for (i = 1; i < 5; i++)
	{
		liColumn <<= 1;
		liColumn += psSrce[i];
	} /* End for */

	liResult = gsSboxTable[piNo][liRow][liColumn];

	for (i = 0; i < QUARTET; i++)
	{
		psDest[i] = (liResult & 8) >> 3;
		liResult <<= 1;
	} /* End for */
	return;
} /* End EzDes_Confusion() */

/*#F*************************************************************************
* @설    명 : 직접 전치를 MAX SIZE만큼 수행한다
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_PermutedChoice
	 (BYTE psLookup[], BYTE psDest[], BYTE psSrce[], int piMax)
{
	int i, liContent;

	for (i = 0; i < piMax; i++)
	{
		liContent = psLookup[i];
		psDest[i] = psSrce[liContent-1];
	} /* End for */
	return;
} /* End EzDes_PermutedChoice() */

/*#F*************************************************************************
* @설    명 : C, D 레지스터에 대해 좌측 쉬프트를 수행한다
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_ShiftLeftRegs(BYTE psCreg[], BYTE psDreg[], int piNo)
{
	int i, j, liShiftCnt, liMsb;
	BYTE lsTempC, lsTempD;

	liMsb = HALF_PC1 - 1;

	liShiftCnt = gsShiftLeft[piNo];

	for (i = 0; i < liShiftCnt; i++)
	{
		lsTempC = psCreg[0];
		lsTempD = psDreg[0];
		for (j = 0; j < liMsb; j++)
		{
			psCreg[j] = psCreg[j + 1];
			psDreg[j] = psDreg[j + 1];
		} /* End for */

		psCreg[liMsb] = lsTempC;
		psDreg[liMsb] = lsTempD;

	} /* End for */
	return;
} /* End EzDes_ShiftLeftRegs() */

/*#F*************************************************************************
* @설    명 : C, D 레지스터에 대해 우측 쉬프트를 수행한다
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_ShiftRightRegs(BYTE psCreg[], BYTE psDreg[], int piNo)
{
	int i, j, liShiftCnt, liLsb;
	BYTE lsTempC, lsTempD;

	liLsb = HALF_PC1 - 1;

	liShiftCnt = gsShiftRight[piNo];
	for (i = 0; i < liShiftCnt; i++)
	{
		lsTempC = psCreg[liLsb];
		lsTempD = psDreg[liLsb];
		for (j = 0; j < liLsb; j++)
		{
			psCreg[liLsb - j] = psCreg[liLsb - j - 1];
			psDreg[liLsb - j] = psDreg[liLsb - j - 1];
		} /* End for */

		psCreg[0] = lsTempC;
		psDreg[0] = lsTempD;

	} /* End for */
	return;
} /* End EzDes_ShiftRightRegs() */

/*#F*************************************************************************
* @설    명 : Hex Bit 를 이진배열로 변환
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_Hex2Bin(BYTE psBin[], BYTE psHex[], int piMax)
{
	int i, j, liCount = 0;
	BYTE *lpHexa;

	lpHexa = (BYTE *) malloc(piMax);
	EzDes_StringCopy(lpHexa, psHex, piMax);

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			psBin[liCount++] = (lpHexa[i] & 0x80) >> 7;
			lpHexa[i] <<= 1;
		} /* End for */

	} /* End for */

	free(lpHexa);
	return;
} /* End EzDes_Hex2Bin() */

/*#F*************************************************************************
* @설    명 : 이진배열을 Hex 값으로 변환한다
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_Bin2Hex(BYTE psHex[], BYTE psBin[])
{
	int i, j, liCount = 0;

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
			psHex[i] = (psHex[i] << 1) + psBin[liCount++];
	} /* End for */
	return;
} /* End EzDes_Bin2Hex() */

/*#F*************************************************************************
* @설    명 : DES 알고리즘의 MAIN ROUTINE
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_Rtn(BYTE psPlainText[], BYTE psOriginKey[], int piMode)
{
	int i, j;

	union
	{
		BYTE full[BLOCK_SIZE];
		BYTE half[2][HALF_BLOCK];
	} Info;

	union
	{
		BYTE full[HALF_BLOCK];
		BYTE sbox[OCTET][QUARTET];
	} Temp;

	union
	{
		BYTE full[PC1_SIZE];
		BYTE half[2][HALF_PC1];
	} Key;

	union
	{
		BYTE right[EP_SIZE];
		BYTE sbox[OCTET][SEXTET];
	} Exp;

	BYTE gsChosenKey[EP_SIZE];

	EzDes_Hex2Bin(Info.full, psOriginKey, BLOCK_SIZE);
	EzDes_PermutedChoice(gsPc1Table, Key.full, Info.full, PC1_SIZE);

	EzDes_Hex2Bin(Info.full, psPlainText, BLOCK_SIZE);
	EzDes_Straight_Permutation(gsIpTable, Info.full, BLOCK_SIZE);

	/*****  main loop	*****/

	for (i = 0; i < LOOPCOUNT; i++)
	{
		EzDes_ExpansionPermutation(gsEpTable, Info.half[RIGHT], Exp.right);

		/****  Key seletion  ****/

		if (piMode == ENCIPHER)
			EzDes_ShiftLeftRegs(Key.half[0], Key.half[1], i);
		else
			EzDes_ShiftRightRegs(Key.half[0], Key.half[1], i);

		EzDes_PermutedChoice(gsPc2Table, gsChosenKey, Key.full, PC2_SIZE);
		EzDes_Xor(Exp.right, gsChosenKey, EP_SIZE);

		for (j = 0; j < OCTET; j++)
			EzDes_Confusion(Temp.sbox[j], Exp.sbox[j], j);

		EzDes_Straight_Permutation(gsPTable, Temp.full, HALF_BLOCK);
		EzDes_Xor(Temp.full, Info.half[LEFT], HALF_BLOCK);
		EzDes_StringCopy(Info.half[LEFT],  Info.half[RIGHT], HALF_BLOCK);
		EzDes_StringCopy(Info.half[RIGHT], Temp.full, HALF_BLOCK);
		EzDes_Bin2Hex(psPlainText, Info.full);
	} /* End for */

	EzDes_StringCopy(Temp.full,	 Info.half[LEFT], HALF_BLOCK);
	EzDes_StringCopy(Info.half[LEFT],  Info.half[RIGHT], HALF_BLOCK);
	EzDes_StringCopy(Info.half[RIGHT], Temp.full, HALF_BLOCK);
	EzDes_InvertPermutation(gsIpTable, Info.full, BLOCK_SIZE);
	EzDes_Bin2Hex(psPlainText, Info.full);

	return;
} /* End EzDes_Rtn() */

/*#F*************************************************************************
* @설    명 : 이진 Bit 배열을 Hex값으로  변환
* @리 턴 값 : void
*****************************************************************************/
void CEzCaDes::EzDes_Bit2Hex(char *psBuf, char *psRetBuf)
{
	int  i;
	char lsBof[17];

	for (i = 0; i < 8; i++)
	{
		lsBof[2*i+1] = (psBuf[i] & 0x0f) < 10?
				(psBuf[i] & 0x0f) + '0' : (psBuf[i] & 0x0f) - 10 + 'A';
		psBuf[i] >>= 4;
		lsBof[2*i] = (psBuf[i] & 0x0f) < 10?
			(psBuf[i] & 0x0f) + '0' : (psBuf[i] & 0x0f) - 10 + 'A';
	} /* End for */

	strncpy(psRetBuf, lsBof, 16);
	psRetBuf[16] = 0;

	return;

} /* End EzDes_Bit2Hex() */

/*#F*************************************************************************
* @설    명 : Hex값을 Bit배열로 변환
* @리 턴 값 : void
* @주의사항 : piLen은 반드시 짝수여야 한다.
*****************************************************************************/
void CEzCaDes::EzDes_Hex2Bit(char *psRetBuf, int piLen)
{
	int  i, liSet, liTet;
	char lsBuf[22], *lpRet;

	if (piLen % 2 == 1)	return;
	lpRet = psRetBuf;

	memcpy (lsBuf, psRetBuf, piLen);

	for (i = 0; lsBuf[i] && i < piLen; i += 2)
	{
		liSet = liTet = 0;
		if (isdigit(lsBuf[i]))
			liSet = lsBuf[i] - '0';
		else
			liSet = toupper(lsBuf[i]) - '@' + 9;

		liSet = liSet << 4;

		if (isdigit(lsBuf[i + 1]))
			liTet = lsBuf[i + 1] - '0';
		else
			liTet = toupper(lsBuf[i + 1]) - '@' + 9;

		*lpRet++ = liSet + liTet;
	} /* End for */

	*lpRet = 0;

	return;
} /* End int EzDes_Hex2Bit() */
