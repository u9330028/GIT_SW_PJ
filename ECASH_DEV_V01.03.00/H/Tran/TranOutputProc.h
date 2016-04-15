/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranOutputProc_H_INCLUDED_)
#define _TranOutputProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
	int	A4PrintCnt;												// ����� A4Print Line Count(PMERecvData.y��)

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
////////////////////////////////////////////////////////////////////////////
//	������ó���Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_PSPProc(int PrintDevice, int WaitFlg = FALSE);// ��ǥ/���� ���																				// ������Ʈ
	int		fnAPP_IBK_PPPProc(int WaitFlg = FALSE);					// ���������Ʈ
	int		fnAPP_NONGHYUP_PPPProc(int WaitFlg = FALSE);					// ���������Ʈ
	int		fnAPP_KEB_PPPProc(int WaitFlg = FALSE);				// ���������Ʈ
	int		fnAPP_KJB_PPPProc(int WaitFlg = FALSE);				// ���������Ʈ #N0181
	int		fnAPP_HANA_PPPProc(int WaitFlg = FALSE);			// ���������Ʈ #N0214
	int		fnAPP_KB_PPPProc(int WaitFlg = FALSE);				// ���������Ʈ #N0226
	int		fnAPP_CTB_PPPProc(int WaitFlg = FALSE);				// ���������Ʈ	#N0252
	int		fnAPP_SC_PPPProc(int WaitFlg = FALSE);				// ���������Ʈ #N0245
	int		fnAPP_KNB_PPPProc(int WaitFlg = FALSE);				// ���������Ʈ #N0277

	int		fnAPP_TestPPPProc();								// ���������Ʈ�׽�Ʈ #N0211
	
	int		fnAPP_PPPProc(int WaitFlg = FALSE);					// ���������Ʈ
	int		fnAPP_PCWProc(int WaitFlg = FALSE);					// ī��M/S���
	int		fnAPP_PMDProc();									// �޼���ǥ��

	int		fnAPP_A4PrintProc(int nKind);						// A4 Print ����Procedure				2007.06.12(��)
	int		fnAPP_A4PrintTranList();							// �ŷ�������ȸ A4Print					2007.06.12(��)
	int		fnAPP_A4PrintDepSequreInquiry();					// ��������ȸ & ����������ȸ A4Print	2007.06.12(��)
	int		fnAPP_A4PrintInqStock();							// û����ȸ ����  A4Print	            2008.04.02(��)

	int		fnAPP_PMEAcceptInquiryTranSel();					// ��ȸ�ŷ� ���Ӱŷ�����
	int		fnAPP_PMEAcceptInquiryDisplay();					// ��ȸ���ǥ��(��������ȸ)
	int		fnAPP_PMEAcceptInquiryTranServiceConfirm();					// ��ȸ���ǥ��(��������ȸ)

	int		fnAPP_PMEAcceptTransConfirm();						// ��ü�ŷ� Ȯ��
	int		fnAPP_PMEAcceptRemitConfirm();						// �۱ݰŷ� Ȯ��
	int		fnAPP_PMEAcceptAutoConfirm();						// �ڵ���ü���� Ȯ��
	int		fnAPP_PMEAcceptTranListConfirm();					// �ŷ�������ȸ �ŷ� Ȯ��
	int		fnAPP_PMEAcceptSeqListConfirm();					// ���Ǹ���Ʈ �׸�Ȯ��
	int		fnAPP_PMEAcceptSeqTranConfirm();					// û��Ȯ����ȸ
	int		fnAPP_PMEAcceptCanSeqTranConfirm();					// �������Ȯ��
	int		fnAPP_PMEAcceptTranStocConfirm();					// �ڻ��üȮ��
	int		fnAPP_PMEAcceptRemitTranSel();						// ���װ��¼���
	int		fnAPP_PMEAcceptJiroTaxSel();						// ���ΰ����� �Ϲ����γ��� ���γ���Ȯ�� 1��
	int		fnAPP_PMEAcceptJiroTaxConfirm();					// ���ΰ����� �Ϲ����γ��� ���γ���Ȯ�� 2��
	int		fnAPP_PMEAcceptJiroFinConfirm();					// ���ΰ����� �������� ���γ���Ȯ��
	int		fnAPP_PMEAcceptJiroInqComfirm();					// ���γ��γ��� ��ȸȮ��
	int		fnAPP_PMEAcceptJiroFinInqComfirm();					// ���� ���γ��γ��� ��ȸȮ��
	int		fnAPP_PMEAcceptTaDepositConfirm();					// ����ü�Ա� ��ȸȮ��
	int		fnAPP_PMEAcceptDepConfirm();					// Ÿ���Ա���ȸ Ȯ��ȭ��
	int		fnAPP_PMEAcceptMuDepConfirm();					// Ÿ���Ա���ȸ Ȯ��ȭ��
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_ReadyProc();									// �غ��ó��
	int		fnAPP_ReadyPrintResult(int PrintDevice, int WaitFlg = FALSE);
																// �غ�ϰ������
	int		fnAPP_OpenProc();									// ����ó��
	int		fnAPP_OpenPrintResult(int PrintDevice, LPCTSTR szDate, LPCTSTR szTime, int WaitFlg = FALSE);
																// �����������
	int		fnAPP_OpenDesPrintResult(int PrintDevice, int WaitFlg = FALSE);
																// Des�����������
	int		fnAPP_SumProc(int InputID);							// �հ�ó��
	int		fnAPP_SumClear(int InputID = FALSE);						// �հ�ó��
	int		fnAPP_DepErrSumProc(int InputID);							// �Ա����
	int		fnAPP_WithErrSumProc(int InputID);							// ������

	int		fnAPP_SumPrintResult(int TranId, int PrintDevice, int WaitFlg = FALSE);
																// �հ�������
	int		fnAPP_ClosePrintResult(int TranId, int PrintDevice, int WaitFlg = FALSE);
																// �����������
	int		fnAPP_CheckResultPrint(int PrintDevice, int nFlag = FALSE, int WaitFlg = FALSE);
																// ��ǥ�������

/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_MiniATM_SumPrint(int PrintDevice, int WaitFlag = FALSE);	
	int		fnAPP_MiniATM_SumPrint1(int PrintDevice, int WaitFlag = FALSE);	
	int		fnAPP_MiniATM_SumPrint2(int PrintDevice, int WaitFlag = FALSE);	
	int		fnAPP_MiniATM_SumPrint3(int PrintDevice, int WaitFlag = FALSE);	
	int		fnAPP_MiniATM_CashPrint(int PrintDevice, int WaitFlag= FALSE);
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
	int		fnAPP_ErrorTranProc();								// �ŷ������������
	int		fnAPP_ErrorCancelProc();							// ��������������
	int		fnAPP_ErrorHostProc();								// ȣ��Ʈ�����������
	int		fnAPP_ErrorKCashProc();								// KCash �����ŷ���������

//////////////////////////////////////////////////////////////////////
	int		fnAPP_PrintError(int PrintDevice, int WaitFlg = FALSE);			
																// �������
	int		fnAPP_PrintWithCancelTran(int PrintDevice, int WaitFlg = FALSE);			
																// �����Ұŷ�����
	int		fnAPP_PrintDepCancelTran(int PrintDevice, int WaitFlg = FALSE);			
																// �Ա���Ұŷ�����
	int		fnAPP_PrintCancel(int PrintDevice, int WaitFlg = FALSE);
																// �������
	void	fvAPP_AnalyzeICError(char* ErrorData, int Param = FALSE);
																// K-CASH Module�� ������� �м�/����Ʈ
	int		fnAPP_PrintCheckDeposit(int PrintDevice, int WaitFlg = FALSE);
																// ��ǥ�Ա���������
/////////////////////////////////////////////////////////////////////////////
//	�����Լ�
/////////////////////////////////////////////////////////////////////////////
	char*	fchpAPP_EditBank();									// �����ڵ�����
	char*	fchpAPP_EditAcnt(int HiddenFlg,  int AcntOnlyFlg = FALSE);				
																// ���¹�ȣ����
	CString	fchpAPP_EditSrcAcnt(void *inbuff, int inLen, int outLen, int HiddenFlg = FALSE,  int AcntOnlyFlg = FALSE);				
																// ���� ���¹�ȣ����
	CString fchpAPP_EditDestAcnt(void *inbuff, int inLen, int outLen, int HiddenFlg = FALSE,  int AcntOnlyFlg = FALSE);				
																// ���� ���¹�ȣ����
	int		fnAPP_EditPrintLine(int PrintDevice, void* cData, int LineNum, int HiddenFlg = FALSE);
	int		fnAPP_EditPrintLine_Sub_U8100_1(int PrintDevice, void* cData, int LineNum, int HiddenFlg = FALSE);
	int		fnAPP_EditPrintLine_Sub_T3_1(int PrintDevice, void* cData, int LineNum, int HiddenFlg = FALSE);
	int		fnAPP_EditPrintLine_Sub_T3_2(int PrintDevice, void* cData, int LineNum, int HiddenFlg = FALSE);
	int		fnAPP_EditPrintLine_Sub_T3_3(int PrintDevice, void* cData, int LineNum, int HiddenFlg = FALSE);
																// ��ǥ����
	BOOL	fBAPP_EditAmount(void *inbuff,void *outbuff,int inLen,int outLen,int isminus);
																// ���ڿ� ��ȯ
	CString fchpAPP_Asc2Amt(void* inbuff, int inLen, int outLen, char MarkChar = ' ');

	// ���Ӱŷ� 
	int		fnAPP_TranGetContinueProc(int nTranTmpCode);
	int		fnAPP_TranGetContinueProc_Sub(int nTranTmpCode);

	int		fnAPP_POINTS_Sub01_OutputProc();

	int		fnAPP_UTILYBILLS_Sub01_A_OutputProc();
	int		fnAPP_UTILYBILLS_Sub01_B_OutputProc();

	int		fnAPP_UTILYBILLS_SR1_Sub01_OutputProc();
	
	//#GIRO
	int		fnAPP_LOCALTAX_Sub01_OutputProc();
	int		fnAPP_LOCALTAX_Sub02_OutputProc();
	int		fnAPP_LOCALTAX_Sub03_OutputProc();

	//#N0142
	int		fnAPP_E_TICKET_Sub01_OutputProc();
	int		fnAPP_E_TICKET_Sub02_OutputProc();
	int		fnAPP_E_TICKET_Sub03_OutputProc();
	int		fnAPP_E_TICKET_Sub04_OutputProc();

	int		fnAPP_TranCRMSProc();
	int		fnAPP_TranPOINTSProc();
	int		fnAPP_TranINTERADProc();
	int		fnAPP_TranMOBILEGAMEProc();

	//#N0174 ��ȯ ������  
	int		fnAPP_EASYONE_Sub01_OutputProc();
	int		fnAPP_EASYONE_Sub02_OutputProc();


/////////////////////////////////////////////////////////////////////////////
#endif
