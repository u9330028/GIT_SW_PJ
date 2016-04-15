/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranMainSubProc_H_INCLUDED_)
#define _TranMainSubProc_H_INCLUDED_


/////////////////////////////////////////////////////////////////////////////
//	Transaction Message Table
/////////////////////////////////////////////////////////////////////////////

//Ini
#define	ATM_TICKET_INI				_T("D:\\INI\\ISPORT.ini")


//Reg
#define REG_UNION_TICKET			_T("SOFTWARE\\ATM\\FORMAT\\UNION_TICKET")
#define REG_RXATM1160				_T("SOFTWARE\\ATM\\FORMAT\\RXATM1160")
#define REG_RXATM1171				_T("SOFTWARE\\ATM\\FORMAT\\RXATM1171")
#define REG_DEPOSIT					_T("SOFTWARE\\ATM\\FORMAT\\DEPOSIT")


//Ini [Section]
#define UNION_TICKET_TYPE_SPOR		_T("SPOR")
#define UNION_TICKET_TYPE_PLAY		_T("PLAY")
#define UNION_TICKET_TYPE_EXHI		_T("EXHI")
#define UNION_TICKET_TYPE_TICK		_T("TICK")
#define UNION_TICKET_TYPE_PIFF		_T("PIFF")
#define UNION_TICKET_TYPE_THTR		_T("THTR")

public:
	CStringArray strGetBufferArray; 
	CStringArray strGetBufferSubArray; 

public:
	//SubCmnLib
	int		fnAPP_AcceptCmnLibJuminNum();
	int		fnAPP_AcceptSmsNum();
	int		fnAPP_AcceptInstallmentPlan();
	int		fnAPP_AccpetPbConfirmMSG();							// ������ι�ȣ����ȳ�

	int		fnAPP_Accpet_KEB_PbConfirmMSG();
	int		fnAPP_Accpet_KEB_PbConfirmMSG1();
	int		fnAPP_Accpet_KEB_PbConfirmMSG2();

	int		fnAPP_Accpet_KEB_Fee_Confirm();
	int		fnAPP_Accpet_KIUP_Fee_Confirm();
	int		fnAPP_Accpet_NONGHYUP_Fee_Confirm();
	int		fnAPP_Accpet_KWANGJU_Fee_Confirm();
	int		fnAPP_Accpet_KYONGNAM_Fee_Confirm();				
	int		fnAPP_Accpet_HANA_Fee_Confirm();					
	int		fnAPP_Accpet_SHINHAN_Fee_Confirm();					
	int		fnAPP_Accpet_SC_Fee_Confirm();						
	int		fnAPP_Accpet_KB_Fee_Confirm();						
	int		fnAPP_Accpet_CITI_Fee_Confirm();					

	int		fnAPP_Accpet_MS_Block_Confirm();					

	int		fnAPP_Accpet_EMV_MS_Confirm();					

	int		fnAPP_Accpet_FOREGIN_INQ_Fee_Confirm();				

	int		fnAPP_Accpet_AntiSkimming_Confirm();			

	int		DeleteJnlFiles(LPCTSTR DirName, int DeleteDays = 30);


public:
/////////////////////////////////////////////////////////////////////////////
//	Transaction Ticket Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranTicketInitProc();						
	int		fnAPP_TranTicketInputProc();							
	int		fnAPP_TranTicketOutputProc();							
	int		fnAPP_TranTicketCheckDeviceActionProc();				
	int		fnAPP_TranTicketCheckMenuContinueProc();				

	CString fnAPP_TranTicket_SubStep_01_Proc();
	int		fnAPP_TranTicket_SubStep_02_Proc();
	int		fnAPP_TranTicket_SubStep_03_Proc();
	int		fnAPP_TranTicket_SubStep_04_Proc();
	int		fnAPP_TranTicket_SubStep_05_Proc();
	CString fnAPP_TranTicket_SubStep_06_Proc();
	int		fnAPP_TranTicket_SubStep_Accept_Card_Pwd_Proc();

	int		fnAPP_TranTicket_SubStep_07_Proc();
	int		fnAPP_TranTicket_SubStep_08_Proc();
	int		fnAPP_TranTicket_SubStep_09_Proc();
	int		fnAPP_TranTicket_SubStep_10_Proc();

/////////////////////////////////////////////////////////////////////////////
//	Transaction NoMedia Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranNoMediaInitProc();							
	int		fnAPP_TranNoMediaInputProc();						
	int		fnAPP_TranNoMediaOutputProc();							
	int		fnAPP_TranNoMediaCheckDeviceActionProc();			
	int		fnAPP_TranNoMediaCheckMenuContinueProc();

	//ȯ�����
	int		fnAPP_IBK_MANAGEMENT_InitProc();
	int		fnAPP_IBK_MANAGEMENT_InputProc();
	int		fnAPP_IBK_MANAGEMENT_OutputProc();
	int		fnAPP_IBK_MANAGEMENT_CheckDeviceActionProc();
	int		fnAPP_IBK_MANAGEMENT_CheckMenuContinueProc();
	
	int		fnAPP_SelectMenu();

/////////////////////////////////////////////////////////////////////////////
//	Transaction NoMedia Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranCommonInitProc();							
	int		fnAPP_TranCommonInputProc();						
	int		fnAPP_TranCommonOutputProc();							
	int		fnAPP_TranCommonCheckDeviceActionProc();			
	int		fnAPP_TranCommonCheckMenuContinueProc();
	int		fnAPP_SubStep_Accept_Card_Proc();
/////////////////////////////////////////////////////////////////////////////
//  Common Proc : Function Name rule = TranProc Define name + _ + Subname 
/////////////////////////////////////////////////////////////////////////////
	//���ΰ�����
	int		fnAPP_UTILYBILLS_InitProc();
	int		fnAPP_UTILYBILLS_InputProc();
	int		fnAPP_UTILYBILLS_OutputProc();
	int		fnAPP_UTILYBILLS_CheckDeviceActionProc();
	int		fnAPP_UTILYBILLS_CheckMenuContinueProc();

	//#GIRO ���漼
	int		fnAPP_LOCALTAX_InitProc();
	int		fnAPP_LOCALTAX_InputProc();
	int		fnAPP_LOCALTAX_OutputProc();
	int		fnAPP_LOCALTAX_CheckDeviceActionProc();
	int		fnAPP_LOCALTAX_CheckMenuContinueProc();

	//�ؿ�ī��    #N0171
	int		fnAPP_FOREGIN_InitProc();
	int		fnAPP_FOREGIN_InputProc();
	int		fnAPP_FOREGIN_OutputProc();
	int		fnAPP_FOREGIN_CheckDeviceActionProc();
	int		fnAPP_FOREGIN_CheckMenuContinueProc();

	// ���ֺ귣�� ���ݼ��� ��ü ���� ����
	int		fnAPP_AcceptKJBTransType();



	// �����н� ����		#N0266
	int		fnAPP_HIPASS_InitProc();
	int		fnAPP_HIPASS_InputProc();
	int		fnAPP_HIPASS_OutputProc();
	int		fnAPP_HIPASS_CheckDeviceActionProc();
	int		fnAPP_HIPASS_CheckMenuContinueProc();
	int		fnAPP_HIPASS_SendErrorComplet();
	int		fnAPP_HIPASS_SendComplet();
	int		fnAPP_HIPASS_AcceptPassword();

	int		fnAPP_HPSReadBalance();								// Hi-Pass �ܾ���ȸ
	int		fnAPP_HPSLoadS1();									// Hi-Pass ����S1
	int		fnAPP_HPSLoadS2();									// Hi-Pass ����S2
	int		fnAPP_HPSUnloadS1();								// Hi-Pass ��������S1
	int		fnAPP_HPSUnloadS2();								// Hi-Pass ��������S2
	int		fnAPP_HPSComplete();								// Hi-Pass �Ϸ�
	int		fnAPP_HPSBalanceConfirm();							// Hi-Pass �ܾ���ȸȮ��	
	int		fnAPP_HPSAcceptMoney();								// Hi-Pass ���� �ݾ��Է�
	int		fnAPP_HPSAcceptMoneyInConfirm();					// Hi-Pass �Աݱݾ�Ȯ��
	int		fnAPP_HPSLoadConfirm();								// Hi-Pass �����Ϸ�Ȯ��

/////////////////////////////////////////////////////////////////////////////
#endif