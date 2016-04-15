/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranLibProc_H_INCLUDED_)
#define _TranLibProc_H_INCLUDED_

typedef struct tagDetailTbl
{	
	char	AccountNo[19];										// Account No
	char	TranType[2];										// TranType: only 01
	char	TranAmount[12];										// Tran Amount
	char	SerialNo[6];										// Serial No
	char	CurrencyCode[3];									// Currency Code: 156

	char	DetailBatchNo[14];									// Detail Batch No: 2005.08.22
} DetailTbl;


/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Library Variable
/////////////////////////////////////////////////////////////////////////////
	DetailTbl	DetailData;
	CString		strDetailData;
	int			DetailBeginPSetFlag;
	
/////////////////////////////////////////////////////////////////////////////
// Operations 
public:
/////////////////////////////////////////////////////////////////////////////
//	Library Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPLLib_FormatMacData(LPCTSTR MacData, int& Length, LPSTR DeData);

	CString	fnAPLLib_GetCenterPrtData(CString PrtData, int MaxColumn);
	int		fnAPLLib_CombineKeyInSound(LPCTSTR szKeyIn);							// Combine Inserted PIN and play sound
	CString	fnAPLLib_MakePackCardTrack(LPSTR szCardData);							// YHJ 20080306A
	CString fnAPLLib_MakeUnPackCardTrack(LPSTR szPackedCardData);					// YHJ 20080306A

	
	CString	fstrAPL_MakeRandomMac();											// Make Random Time Variant Number
/////////////////////////////////////////////////////////////////////////////

// [#156] US Justin 2012.04.40 Add EMV Transaction
	/////////////////////////////////////////////////////////////////////////////
	//	EMV SUPPORT
	/////////////////////////////////////////////////////////////////////////////
	int		fnAPLLib_EmvTranDataMake(int nEmvTrType);							// Emv before Processing
	int		fnAPLLib_EmvFallBackProc(BOOL bGeneratePinBlock = FALSE);							// Emv -> MS Fallback Processing
	CString fnAPLLib_EmvMakeCardPanProc();
	int		fnAPLLib_EmvOnlineApproval(int nHostResponse, int nOnlineMode);
////////////////////////////////////////////////////////////////
// End of [#156]

#endif
