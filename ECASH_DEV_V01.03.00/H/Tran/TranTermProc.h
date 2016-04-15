/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranTermProc_H_INCLUDED_)
#define _TranTermProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	종료변수
/////////////////////////////////////////////////////////////////////////////
	int		CancelProcNum;				// 취소처리N번실행변수
	char	CancelProcCount;			// 취소처리전진행번호 
	int		CancelDepRejectFlg;			// 입금반환Flg
	int		CancelDepRejectProcFlg;		// 입금반환처리Flg
	int		CancelCheckClearFlg;		// 수표정보초기화Flg
	int		CancelPrintFlg;				// 취소인자Flg
	int		CancelDepPrintFlg;			// 취소입금인자Flg(초기:실행무)
	int		CancelWithSendFlg;			// 취소출금전송Flg(초기:실행무)
	int		CancelWithProcFlg;			// 취소출금처리Flg
	int		CancelDepSendFlg;			// 취소입금전송Flg(초기:실행무)
	int		CancelDepProcFlg;			// 취소입금처리Flg
	int		CancelKCashSendFlg;			// K-CASH취소출금전송Flg(초기:실행무)
	int		CancelKCashProcFlg;			// K-CASH취소출금처리Flg

	int		CancelEMVFlg;				// 인증취소FLG

	int		CancelHIPassLoadFlg;		// 하이패스 거래실패시 Complete 처리Flag	#N0266


	int		CancelHipassPrintFlg;			// 취소하이패스인자Flg(초기:실행무) #N0266
 
/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	종료함수(TranCmn.cpp)
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_CancelDepRejectProc(int CancelCode, char *pCancelMsg = NULL, char *pCancelMsg2 = NULL, char *pCancelMsg3 = NULL, int CancelScrNum = 0);
																// 취소입금반환함수(정상업무처리)
	int		fnAPP_CancelProc(int CancelCode, char *pCancelMsg = NULL, char *pCancelMsg2 = NULL, char *pCancelMsg3 = NULL, int CancelScrNum = 0);
																// 취소함수
	int		fnAPP_CancelProc2(int CancelCode, char *pCancelMsg = NULL, char *pCancelMsg2 = NULL, char *pCancelMsg3 = NULL, int CancelScrNum = 0);
																// 취소함수2
	int		fnAPP_CancelProc3(int CancelCode, char *pCancelMsg = NULL, char *pCancelMsg2 = NULL, char *pCancelMsg3 = NULL, int CancelScrNum = 0);
																// 취소함수3
	int		fnAPP_CancelProc4(int CancelCode, char *pCancelMsg = NULL, char *pCancelMsg2 = NULL, char *pCancelMsg3 = NULL, int CancelScrNum = 0);
																// 취소함수4
	int		fnAPP_TerminateProc(int TerminateCode);				// 종료함수
	int		fnAPP_TerminateDummyProc(int TerminateCode);		// 종료함수

/////////////////////////////////////////////////////////////////////////////
#endif
