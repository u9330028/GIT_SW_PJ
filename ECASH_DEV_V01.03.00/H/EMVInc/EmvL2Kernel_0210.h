
#define DLL_IMPORT		__declspec(dllimport)


//-------------------------------------------------------------------
// EMV Initialize
DLL_IMPORT void  Term_Loading_ListofAID(void);
DLL_IMPORT int   Load_EMVTerminalData(char *DefaultPath);
DLL_IMPORT int   RegistIFMCommFunc(int (CALLBACK* SMCSENDAPDU)(BYTE dCode, BYTE *apdu, UINT apduLen, BYTE *resp,int *rlen));

//-------------------------------------------------------------------
// EMV Processing
DLL_IMPORT int   Sel_BuildCandidateApp(int *candidate);
DLL_IMPORT int   Sel_MakeApplist(BYTE *pBuff, int candidate);
DLL_IMPORT void  Sel_RemakeCandidateList(int count, int df);
DLL_IMPORT int   Sel_FinalAppSelection(int dfIndex, int *app, int *aid, BYTE *SW);
DLL_IMPORT int   EMVProc_InitApplication(BYTE ProcCode, BYTE *TrAmt, BYTE *TrDTime, int iTermCount);
DLL_IMPORT int   EMVProc_ReadAppData(int app);
DLL_IMPORT int   EMVProc_OfflineDataAuth(void);
DLL_IMPORT int   EMVProc_ProcessRestrict(void);
DLL_IMPORT int   EMVProc_CardholderVerify(BYTE *EncPin);
DLL_IMPORT BYTE  EMVProc_TerminalActionAnalysis(void);
DLL_IMPORT int   EMVProc_CardActionAnalysis(BYTE term_crypt, BYTE *ResponseCode);
DLL_IMPORT int   EMVProc_OnlApproveProcess(int app, BYTE *ResCd, int iARPCLen, BYTE *ARPCVal, int ISLen, BYTE *IScriptVal);
DLL_IMPORT int   EMVProc_DefaultActionAnalysis(int aid);
DLL_IMPORT int   EMVProc_Completion(int aid);
DLL_IMPORT int   Tlv_GetVal(int tid, int	*len, BYTE *val);

//-------------------------------------------------------------------
// EMV TLV
DLL_IMPORT void  Tlv_TableClear(void);
DLL_IMPORT int   Tlv_StoreVal(int tid, int len, BYTE *val);
DLL_IMPORT int   Tlv_GetTid(BYTE *tag, int len);
DLL_IMPORT void  Tlv_GetTidTag(int tid, BYTE *tag);
DLL_IMPORT int   Tlv_GetTag(BYTE *buf, BYTE *tag);
DLL_IMPORT int   Tlv_GetTidLen(int tid);
DLL_IMPORT int   EMVIso_GetData(int bit, int tid, BYTE *ptr);

//-------------------------------------------------------------------
// ISO8583
DLL_IMPORT short EMVIso_BuildISO8583Msg(int app, char *msgType, BYTE *data);
DLL_IMPORT void  EMVIso_SetAtmData(int iTransTypeLen, char *cpTransType,
									int iAdditionalDataLen, char *cpAdditionalData,
									int iPriAccountLen, char *cpPriAccount,
									int iTransAccountLen, char *cpTransAccount);
DLL_IMPORT int   EMVIso_SetRecvMsg(int iApp, int iLen, char *cpRecvMsg);
DLL_IMPORT int   EMVIso_Approval_AnalysysReplyMsg(int app, BYTE *resData, 
												  WORD resLen);
DLL_IMPORT WORD  GetCRC(BYTE *data, int Length);
DLL_IMPORT short EMVIso_ReversalMsg(int app, BYTE *data);
DLL_IMPORT short EMVIso_MsrMsg(int app, int iPanLen, char* cpPan, int iAmtLen, char* cpAmt, BYTE *data);
DLL_IMPORT short EMVIso_SettlementMsg(int app, BYTE *data);
DLL_IMPORT short EMVIso_KeyDownloadMsg(int app, BYTE *data);
DLL_IMPORT short EMVIso_ConfirmMsg(BYTE *cpTerminal, BYTE *data);

