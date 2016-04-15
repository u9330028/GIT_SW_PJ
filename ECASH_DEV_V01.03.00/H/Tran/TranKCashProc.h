/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranKCashProc_H_INCLUDED_)
#define _TranKCashProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	K-CASH함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_KCashCheckPin();								// 전자화폐 비밀번호검사
	int		fnAPP_KCashReadPan();								// 전자화폐 PAN파일읽기
	int		fnAPP_KCashCheckMainAccount();						// 전자화폐 계좌정보분석(PAN정보)
	int		fnAPP_KCashReadBAmount();							// 전자화폐 JAN파일읽기
	int		fnAPP_KCashReadTrafficInfo();						// 전자화폐 교통정보읽기
	int		fnAPP_KCashPrepareUnloadRcvIEP();					// 전자화폐 환불복구전처리

	int		fnAPP_KCashVerifyTranBit();							// 전자화폐 거래중지Bit확인
	int		fnAPP_KCashRemoveTranBit();							// 전자화폐 거래중지Bit제거

	int		fnAPP_KCashLoad();									// K-CASH충전
	int		fnAPP_KCashUnload();								// K-CASH환불
	int		fnAPP_KCashChangePin();								// 비밀번호변경
	
	int		fnAPP_KCashInquiry();								// K-CASH잔액조회
	int		fnAPP_KCashLoadUnloadInquiry();						// 충전/환불내역
	int		fnAPP_KCashBuyListInquiry();						// 구매내역조회

	int		fnAPP_KCashUpdate();								// 파라미터갱신

	int		fnAPP_KCashReadEID();								// 전자화폐번호 읽기

/////////////////////////////////////////////////////////////////////////////
// 입력 검증함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_KCashCheckChangePin();						// PIN변경시 변경할PIN Check	2004.02.27

/////////////////////////////////////////////////////////////////////////////
#endif
