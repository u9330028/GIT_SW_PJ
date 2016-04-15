#if !defined(_TRANNICEDATAFIELDPROC_H_INCLUDED_)
#define _TRANNICEDATAFIELDPROC_H_INCLUDED_




public:
int		fnAPP_TxECash_SetDataField_SendProc(int nSec = FALSE);
int		fnAPP_RxECash_GetDataField_RecvProc(int nSec = FALSE);
int		fnAPP_RxECash_GetDataField_AnalProc(int nSec = FALSE);

int		fnAPP_ClearArrDataField(int nOption = FALSE);
int		fnAPP_Init_031X(int nOption = FALSE);
int		fnAPP_SetAddArrDataField(DataField *pArrDataField, int nTxRxOption = FALSE);
int		fnAPP_RXGetFindArryDataField(LPCTSTR szbyFCode, int nFLen, CString &strReturn, int nPackOption = PACK_F);
int		fnAPP_TXGetFindArryDataField(LPCTSTR szbyFCode, int nFLen, CString &strReturn, int nPackOption = PACK_F);

int		fnAPP_SetDataField_Item_081X(int nTranCode , int nTxRxOption = FALSE); //전문Format구분                                          
int		fnAPP_SetDataField_Item_001X(int nTranCode , int nTxRxOption = FALSE); //거래날짜                                          
int		fnAPP_SetDataField_Item_002X(int nTranCode , int nTxRxOption = FALSE); //거래시각                            
int		fnAPP_SetDataField_Item_003X(int nTranCode , int nTxRxOption = FALSE); //SST 일련번호                        
int		fnAPP_SetDataField_Item_004X(int nTranCode , int nTxRxOption = FALSE);            
int		fnAPP_SetDataField_Item_005X(int nTranCode , int nTxRxOption = FALSE); //전문종류(Msg-Type)                  
int		fnAPP_SetDataField_Item_006X(int nTranCode , int nTxRxOption = FALSE); //업무구분코드(Tran-ID)  
int		fnAPP_SetDataField_Item_007X(int nTranCode , int nTxRxOption = FALSE); //사용 MS Track 번호                
int		fnAPP_SetDataField_Item_008X(int nTranCode , int nTxRxOption = FALSE); //응답코드(RspCode)                   
int		fnAPP_SetDataField_Item_009X(int nTranCode , int nTxRxOption = FALSE); //마감구분                 
int		fnAPP_SetDataField_Item_010X(int nTranCode , int nTxRxOption = FALSE); //암호화구분 - 0:안함,1:기관암호화,2:중계망암호,3:기관&중계망     
int		fnAPP_SetDataField_Item_011X(int nTranCode , int nTxRxOption = FALSE); //SST기기번호(MecaNum)   
int		fnAPP_SetDataField_Item_0120(int nTranCode , int nTxRxOption = FALSE); //취급기관코드1  
   
//int		fnAPP_SetDataField_Item_071X(int nTranCode , int nTxRxOption = FALSE); //거래매체구분  
//int		fnAPP_SetDataField_Item_072X(int nTranCode , int nTxRxOption = FALSE); //암호화 종류 - 00:기타Or없음,'01':SEED,'02':소프트포럼,'03':이니텍
//int		fnAPP_SetDataField_Item_047X(int nTranCode , int nTxRxOption = FALSE); //SST Serial Number(SST-ID)   
//int		fnAPP_SetDataField_Item_088X(int nTranCode , int nTxRxOption = FALSE); //단말기의 IP Address(xxx.xxx.xxx.xxx)  
int		fnAPP_SetDataField_Item_464X(int nTranCode , int nTxRxOption = FALSE); //현금부족체크
                     
int		fnAPP_SetDataField_Item_048X(int nTranCode , int nTxRxOption = FALSE); //신용카드정보유무                    
int		fnAPP_SetDataField_Item_049X(int nTranCode , int nTxRxOption = FALSE); //SST기종구분  
int		fnAPP_SetDataField_Item_050X(int nTranCode , int nTxRxOption = FALSE); //원거래 단말기 일련번호 (050X)
int		fnAPP_SetDataField_Item_055X(int nTranCode , int nTxRxOption = FALSE); //현금장입금액
int		fnAPP_SetDataField_Item_056X(int nTranCode , int nTxRxOption = FALSE); //현금장입시각 
int		fnAPP_SetDataField_Item_059X(int nTranCode , int nTxRxOption = FALSE); //일반전문(출금가능 수표액), 시재전문(기기보유 수표액)
int		fnAPP_SetDataField_Item_061X(int nTranCode , int nTxRxOption = FALSE); //권종별 매수정보
int		fnAPP_SetDataField_Item_062X(int nTranCode , int nTxRxOption = FALSE); //업무세부구분코드 

int		fnAPP_SetDataField_Item_016X(int nTranCode , int nTxRxOption = FALSE); //취급기관코드1                       
int		fnAPP_SetDataField_Item_017X(int nTranCode , int nTxRxOption = FALSE); //거래계좌(카드)번호1    
int		fnAPP_SetDataField_Item_018X(int nTranCode , int nTxRxOption = FALSE); //출금매체구분    
int		fnAPP_SetDataField_Item_052X(int nTranCode , int nTxRxOption = FALSE); //거래계좌번호(기관용   
                 
int		fnAPP_SetDataField_Item_054X(int nTranCode , int nTxRxOption = FALSE); //암호화 블럭데이터                 

 
                   
int		fnAPP_SetDataField_Item_013X(int nTranCode , int nTxRxOption = FALSE); //ISO3 Data  
int		fnAPP_SetDataField_Item_0131(int nTranCode , int nTxRxOption = FALSE); //ISO3 Data - 암호화
int		fnAPP_SetDataField_Item_014X(int nTranCode , int nTxRxOption = FALSE); //ISO2 Data                           
int		fnAPP_SetDataField_Item_0141(int nTranCode , int nTxRxOption = FALSE); //ISO2 Data - 암호화                           
int		fnAPP_SetDataField_Item_015X(int nTranCode , int nTxRxOption = FALSE); //비밀번호                            
int		fnAPP_SetDataField_Item_0151(int nTranCode , int nTxRxOption = FALSE); //비밀번호  - 암호화                           
//int		fnAPP_SetDataField_Item_016X(int nTranCode , int nTxRxOption = FALSE); //통장 M/S Data                       
//int		fnAPP_SetDataField_Item_017X(int nTranCode , int nTxRxOption = FALSE); //거래계좌(카드)번호1                 
//int		fnAPP_SetDataField_Item_018X(int nTranCode , int nTxRxOption = FALSE); //거래계좌(카드)성명                  
int		fnAPP_SetDataField_Item_019X(int nTranCode , int nTxRxOption = FALSE); //수취기관코드                              
int		fnAPP_SetDataField_Item_0191(int nTranCode , int nTxRxOption = FALSE); //Filler                              
int		fnAPP_SetDataField_Item_020X(int nTranCode , int nTxRxOption = FALSE); //수취계좌번호                        
int		fnAPP_SetDataField_Item_021X(int nTranCode , int nTxRxOption = FALSE); //수취계좌성명                        
int		fnAPP_SetDataField_Item_022X(int nTranCode , int nTxRxOption = FALSE); //거래계좌(카드)잔액                  
int		fnAPP_SetDataField_Item_0221(int nTranCode , int nTxRxOption = FALSE); //거래계좌(카드)잔액                  
int		fnAPP_SetDataField_Item_023X(int nTranCode , int nTxRxOption = FALSE); //거래금액                            
int		fnAPP_SetDataField_Item_024X(int nTranCode , int nTxRxOption = FALSE); //거래수수료                          
int		fnAPP_SetDataField_Item_025X(int nTranCode , int nTxRxOption = FALSE); //미결제타점권 금액                   
int		fnAPP_SetDataField_Item_026X(int nTranCode , int nTxRxOption = FALSE); //해외송금 계좌정보                   
int		fnAPP_SetDataField_Item_027X(int nTranCode , int nTxRxOption = FALSE); //수표거래금액                        
int		fnAPP_SetDataField_Item_028X(int nTranCode , int nTxRxOption = FALSE); //수표매수                            
int		fnAPP_SetDataField_Item_029X(int nTranCode , int nTxRxOption = FALSE); //수표관련 정보  
int		fnAPP_SetDataField_Item_041X(int nTranCode , int nTxRxOption = FALSE); //암호화Key길이(삼성증권암호화)                     
int		fnAPP_SetDataField_Item_038X(int nTranCode , int nTxRxOption = FALSE); //보너스 매체 출금가능 정보                            
//int		fnAPP_SetDataField_Item_042X(int nTranCode , int nTxRxOption = FALSE); //일반전문(출금가능총액)    
int		fnAPP_SetDataField_Item_042X(int nTranCode , int nTxRxOption = FALSE); //암호화Key길이                      
int		fnAPP_SetDataField_Item_043X(int nTranCode , int nTxRxOption = FALSE); //주암호화Key                         
int		fnAPP_SetDataField_Item_044X(int nTranCode , int nTxRxOption = FALSE); //보조암호화Key                       
int		fnAPP_SetDataField_Item_0441(int nTranCode , int nTxRxOption = FALSE); //보조암호화Key                       
int		fnAPP_SetDataField_Item_045X(int nTranCode , int nTxRxOption = FALSE); //최종승인허가일자                    

int		fnAPP_SetDataField_Item_057X(int nTranCode , int nTxRxOption = FALSE); //IC카드 일련번호 (057X)                   
int		fnAPP_SetDataField_Item_058X(int nTranCode , int nTxRxOption = FALSE); //IC카드 암호화 비밀번호 (058X)                    

int		fnAPP_SetDataField_Item_063X(int nTranCode , int nTxRxOption = FALSE); //거래개시일시                        
int		fnAPP_SetDataField_Item_064X(int nTranCode , int nTxRxOption = FALSE); //HOST일련번호                        
int		fnAPP_SetDataField_Item_065X(int nTranCode , int nTxRxOption = FALSE); //전자화폐 충전 할증금액              
int		fnAPP_SetDataField_Item_066X(int nTranCode , int nTxRxOption = FALSE); //화폐종류                            
int		fnAPP_SetDataField_Item_067X(int nTranCode , int nTxRxOption = FALSE); //전자화폐 일련번호(IC카드일련번호)   
int		fnAPP_SetDataField_Item_068X(int nTranCode , int nTxRxOption = FALSE); //거래카운터                          
int		fnAPP_SetDataField_Item_0691(int nTranCode , int nTxRxOption = FALSE); //충전전카드잔액                      
int		fnAPP_SetDataField_Item_070X(int nTranCode , int nTxRxOption = FALSE); //난수                                
int		fnAPP_SetDataField_Item_071X(int nTranCode , int nTxRxOption = FALSE); //인증자                              
int		fnAPP_SetDataField_Item_072X(int nTranCode , int nTxRxOption = FALSE); //카드종류                            
int		fnAPP_SetDataField_Item_073X(int nTranCode , int nTxRxOption = FALSE); //알고리즘 ID                         
int		fnAPP_SetDataField_Item_074X(int nTranCode , int nTxRxOption = FALSE); //VK                                  
int		fnAPP_SetDataField_Item_075X(int nTranCode , int nTxRxOption = FALSE); //센터ID                              
int		fnAPP_SetDataField_Item_076X(int nTranCode , int nTxRxOption = FALSE); //발행기관ID                          
int		fnAPP_SetDataField_Item_077X(int nTranCode , int nTxRxOption = FALSE); //이용자카드ID                        
int		fnAPP_SetDataField_Item_078X(int nTranCode , int nTxRxOption = FALSE); //PAN                                 
int		fnAPP_SetDataField_Item_079X(int nTranCode , int nTxRxOption = FALSE); //발행자ID                            
int		fnAPP_SetDataField_Item_080X(int nTranCode , int nTxRxOption = FALSE); //승인 신용카드(계좌)번호             
int		fnAPP_SetDataField_Item_085X(int nTranCode , int nTxRxOption = FALSE); //통신사 구분                         
int		fnAPP_SetDataField_Item_086X(int nTranCode , int nTxRxOption = FALSE); //전화번호                            
int		fnAPP_SetDataField_Item_087X(int nTranCode , int nTxRxOption = FALSE); //장애세부코드                   
int		fnAPP_SetDataField_Item_088X(int nTranCode , int nTxRxOption = FALSE); //단말기의 IP Address(xxx.xxx.xxx.xxx)                  
int		fnAPP_SetDataField_Item_089X(int nTranCode , int nTxRxOption = FALSE); //통장거래관련 화면안내 메시지        
int		fnAPP_SetDataField_Item_090X(int nTranCode , int nTxRxOption = FALSE); //계좌잔액 인자여부                   
int		fnAPP_SetDataField_Item_091X(int nTranCode , int nTxRxOption = FALSE); //HOST작업 정보의 버전값                
int		fnAPP_SetDataField_Item_092X(int nTranCode , int nTxRxOption = FALSE); //통장정리 지정행                     
int		fnAPP_SetDataField_Item_093X(int nTranCode , int nTxRxOption = FALSE); //통장정리 대상건수                   
int		fnAPP_SetDataField_Item_094X(int nTranCode , int nTxRxOption = FALSE); //통장정리내역                        
int		fnAPP_SetDataField_Item_095X(int nTranCode , int nTxRxOption = FALSE); //통장정리자료 추가 존재 여부         
int		fnAPP_SetDataField_Item_096X(int nTranCode , int nTxRxOption = FALSE); //통장패턴번호                        
int		fnAPP_SetDataField_Item_097X(int nTranCode , int nTxRxOption = FALSE); //통장서브패턴                        
int		fnAPP_SetDataField_Item_098X(int nTranCode , int nTxRxOption = FALSE); //추가 기장용 KEY                     
int		fnAPP_SetDataField_Item_099X(int nTranCode , int nTxRxOption = FALSE); //다계좌 내용                         
int		fnAPP_SetDataField_Item_100X(int nTranCode , int nTxRxOption = FALSE); //전원OFF기간                            
int		fnAPP_SetDataField_Item_101X(int nTranCode , int nTxRxOption = FALSE); //조회구분                            
int		fnAPP_SetDataField_Item_104X(int nTranCode , int nTxRxOption = FALSE); //명세표 인자Data                                                       
int		fnAPP_SetDataField_Item_130X(int nTranCode , int nTxRxOption = FALSE); //거절응답사유                        
int		fnAPP_SetDataField_Item_131X(int nTranCode , int nTxRxOption = FALSE); //기사용금액                          
int		fnAPP_SetDataField_Item_166X(int nTranCode , int nTxRxOption = FALSE); //대출상품코드                        
int		fnAPP_SetDataField_Item_167X(int nTranCode , int nTxRxOption = FALSE); //대출요청개월수                      
int		fnAPP_SetDataField_Item_1680(int nTranCode , int nTxRxOption = FALSE); //대출이율                            
int		fnAPP_SetDataField_Item_1810(int nTranCode , int nTxRxOption = FALSE); //연체이율                            
int		fnAPP_SetDataField_Item_169X(int nTranCode , int nTxRxOption = FALSE); //원리금납입일                        
int		fnAPP_SetDataField_Item_170X(int nTranCode , int nTxRxOption = FALSE); //만기일                              
int		fnAPP_SetDataField_Item_171X(int nTranCode , int nTxRxOption = FALSE); //대출가능금액1                       
int		fnAPP_SetDataField_Item_172X(int nTranCode , int nTxRxOption = FALSE); //취급수수료                          
int		fnAPP_SetDataField_Item_189X(int nTranCode , int nTxRxOption = FALSE); //오천원권 매수                       
int		fnAPP_SetDataField_Item_190X(int nTranCode , int nTxRxOption = FALSE); //천원권 매수                         
int		fnAPP_SetDataField_Item_191X(int nTranCode , int nTxRxOption = FALSE); //지로번호                            
int		fnAPP_SetDataField_Item_192X(int nTranCode , int nTxRxOption = FALSE); //전자납부번호(일련번호)              
int		fnAPP_SetDataField_Item_193X(int nTranCode , int nTxRxOption = FALSE); //지로납부내역  
int		fnAPP_SetDataField_Item_1931(int nTranCode , int nTxRxOption = FALSE); //지방세 납부내역          
int		fnAPP_SetDataField_Item_194X(int nTranCode , int nTxRxOption = FALSE); //이용기관명(거래점 번호)             
int		fnAPP_SetDataField_Item_195X(int nTranCode , int nTxRxOption = FALSE); //납부결과조회내역                    
int		fnAPP_SetDataField_Item_196X(int nTranCode , int nTxRxOption = FALSE); //할부개월수                          
int		fnAPP_SetDataField_Item_201X(int nTranCode , int nTxRxOption = FALSE); //신용정보조회서비스 요청공통부       
int		fnAPP_SetDataField_Item_202X(int nTranCode , int nTxRxOption = FALSE); //휴대전화 SMS발송 인증키             
int		fnAPP_SetDataField_Item_204X(int nTranCode , int nTxRxOption = FALSE); //단지코드                            
int		fnAPP_SetDataField_Item_205X(int nTranCode , int nTxRxOption = FALSE); //아파트명                            
int		fnAPP_SetDataField_Item_206X(int nTranCode , int nTxRxOption = FALSE); //아파트주소                          
int		fnAPP_SetDataField_Item_207X(int nTranCode , int nTxRxOption = FALSE); //부동산 시세조회 상세내역            
int		fnAPP_SetDataField_Item_208X(int nTranCode , int nTxRxOption = FALSE); //추가 응답코드                       
int		fnAPP_SetDataField_Item_209X(int nTranCode , int nTxRxOption = FALSE); //운세 데이터                         
int		fnAPP_SetDataField_Item_210X(int nTranCode , int nTxRxOption = FALSE); //신용정보조회서비스 응답공통부       
int		fnAPP_SetDataField_Item_211X(int nTranCode , int nTxRxOption = FALSE); //신용정보조회 채무불이행(은연) 내역  
int		fnAPP_SetDataField_Item_811X(int nTranCode , int nTxRxOption = FALSE); //신용정보조회 채무불이행(은연) 2     
int		fnAPP_SetDataField_Item_212X(int nTranCode , int nTxRxOption = FALSE); //신용정보조회 채무불이행(신정) 내역  
int		fnAPP_SetDataField_Item_812X(int nTranCode , int nTxRxOption = FALSE); //신용정보조회 채무불이행(신정) 2     
int		fnAPP_SetDataField_Item_213X(int nTranCode , int nTxRxOption = FALSE); //신용정보조회 신용개설정보 내역      
int		fnAPP_SetDataField_Item_813X(int nTranCode , int nTxRxOption = FALSE); //신용정보조회 신용개설정보 2         
int		fnAPP_SetDataField_Item_214X(int nTranCode , int nTxRxOption = FALSE); //신용정보조회 조회기록정보 내역      
int		fnAPP_SetDataField_Item_215X(int nTranCode , int nTxRxOption = FALSE); //대출정보 내역                       
int		fnAPP_SetDataField_Item_815X(int nTranCode , int nTxRxOption = FALSE); //대출정보 내역 2                     
int		fnAPP_SetDataField_Item_216X(int nTranCode , int nTxRxOption = FALSE); //현금서비스 정보 내역                
int		fnAPP_SetDataField_Item_816X(int nTranCode , int nTxRxOption = FALSE); //현금서비스 정보 내역 2              
int		fnAPP_SetDataField_Item_217X(int nTranCode , int nTxRxOption = FALSE); //채무보증정보 내역                   
int		fnAPP_SetDataField_Item_817X(int nTranCode , int nTxRxOption = FALSE); //채무보증정보 내역 2                 
int		fnAPP_SetDataField_Item_218X(int nTranCode , int nTxRxOption = FALSE); //CB연체정보 내역                     
int		fnAPP_SetDataField_Item_818X(int nTranCode , int nTxRxOption = FALSE); //CB연체정보 내역 2                   
int		fnAPP_SetDataField_Item_828X(int nTranCode , int nTxRxOption = FALSE); //CB연체정보 내역 3                   
int		fnAPP_SetDataField_Item_219X(int nTranCode , int nTxRxOption = FALSE); //평점정보 내역                       
int		fnAPP_SetDataField_Item_220X(int nTranCode , int nTxRxOption = FALSE); //주민등록번호(농협:암호화해서)       
int		fnAPP_SetDataField_Item_221X(int nTranCode , int nTxRxOption = FALSE); //승인번호(주문번호)                  
int		fnAPP_SetDataField_Item_222X(int nTranCode , int nTxRxOption = FALSE); //전단계 거래고유번호                 
int		fnAPP_SetDataField_Item_223X(int nTranCode , int nTxRxOption = FALSE); //대출진단 송수신 공통부              
int		fnAPP_SetDataField_Item_224X(int nTranCode , int nTxRxOption = FALSE); //이전거래 SST일련번호                
int		fnAPP_SetDataField_Item_225X(int nTranCode , int nTxRxOption = FALSE); //운세구분 데이터                     
int		fnAPP_SetDataField_Item_230X(int nTranCode , int nTxRxOption = FALSE); //통장정리데이터 길이                 
int		fnAPP_SetDataField_Item_231X(int nTranCode , int nTxRxOption = FALSE); //통장정리내역1                       
int		fnAPP_SetDataField_Item_232X(int nTranCode , int nTxRxOption = FALSE); //통장정리내역2                       
int		fnAPP_SetDataField_Item_233X(int nTranCode , int nTxRxOption = FALSE); //통장정리내역3                       
int		fnAPP_SetDataField_Item_234X(int nTranCode , int nTxRxOption = FALSE); //통장정리내역4                       
int		fnAPP_SetDataField_Item_235X(int nTranCode , int nTxRxOption = FALSE); //통장정리내역5                       
int		fnAPP_SetDataField_Item_236X(int nTranCode , int nTxRxOption = FALSE); //통장정리내역6                       
int		fnAPP_SetDataField_Item_237X(int nTranCode , int nTxRxOption = FALSE); //통장정리내역7                       
int		fnAPP_SetDataField_Item_238X(int nTranCode , int nTxRxOption = FALSE); //통장정리내역8                       
int		fnAPP_SetDataField_Item_240X(int nTranCode , int nTxRxOption = FALSE); //판매처/일정 정보 내역(에버랜드티켓) 
int		fnAPP_SetDataField_Item_241X(int nTranCode , int nTxRxOption = FALSE); //권종 정보 응답내역(에버랜드티켓)    
int		fnAPP_SetDataField_Item_242X(int nTranCode , int nTxRxOption = FALSE); //신용승인요청내역(에버랜드티켓)      
int		fnAPP_SetDataField_Item_243X(int nTranCode , int nTxRxOption = FALSE); //신용승인응답내역(에버랜드티켓)      
int		fnAPP_SetDataField_Item_244X(int nTranCode , int nTxRxOption = FALSE); //안내메세지                          
int		fnAPP_SetDataField_Item_245X(int nTranCode , int nTxRxOption = FALSE); //공지사항1                           
int		fnAPP_SetDataField_Item_246X(int nTranCode , int nTxRxOption = FALSE); //공지사항2                           
int		fnAPP_SetDataField_Item_247X(int nTranCode , int nTxRxOption = FALSE); //티켓내역 응답(에버랜드티켓)         
int		fnAPP_SetDataField_Item_248X(int nTranCode , int nTxRxOption = FALSE); //판매처 응답내역(에버랜드티켓)       
int		fnAPP_SetDataField_Item_249X(int nTranCode , int nTxRxOption = FALSE); //일정/공지사항 응답내역(에버랜드티켓)
int		fnAPP_SetDataField_Item_250X(int nTranCode , int nTxRxOption = FALSE); //출발시간 응답내역                   
int		fnAPP_SetDataField_Item_251X(int nTranCode , int nTxRxOption = FALSE); //자동좌석 지정 요청 내역             
int		fnAPP_SetDataField_Item_252X(int nTranCode , int nTxRxOption = FALSE); //자동좌석 지정 응답 내역             
int		fnAPP_SetDataField_Item_253X(int nTranCode , int nTxRxOption = FALSE); //주문결제 응답 내역                  
int		fnAPP_SetDataField_Item_254X(int nTranCode , int nTxRxOption = FALSE); //출력매수보고 내역                   
int		fnAPP_SetDataField_Item_255X(int nTranCode , int nTxRxOption = FALSE); //발권 응답 내역 1(에버랜드티켓)      
int		fnAPP_SetDataField_Item_256X(int nTranCode , int nTxRxOption = FALSE); //발권 응답 내역 2(에버랜드티켓)      
int		fnAPP_SetDataField_Item_257X(int nTranCode , int nTxRxOption = FALSE); //발권 응답 내역 3(에버랜드티켓)      
int		fnAPP_SetDataField_Item_258X(int nTranCode , int nTxRxOption = FALSE); //발권 응답 내역 4(에버랜드티켓)      
int		fnAPP_SetDataField_Item_260X(int nTranCode , int nTxRxOption = FALSE); //TC/TE예매 조회/요청 공통내역        
int		fnAPP_SetDataField_Item_261X(int nTranCode , int nTxRxOption = FALSE); //TC/TE예매 요청 내역                 
int		fnAPP_SetDataField_Item_270X(int nTranCode , int nTxRxOption = FALSE); //지급가능 금액조회                   
int		fnAPP_SetDataField_Item_271X(int nTranCode , int nTxRxOption = FALSE); //예수금 조회                         
int		fnAPP_SetDataField_Item_272X(int nTranCode , int nTxRxOption = FALSE); //계좌관련 기타정보                   
int		fnAPP_SetDataField_Item_273X(int nTranCode , int nTxRxOption = FALSE); //상품, 종목관련 기타정보             
int		fnAPP_SetDataField_Item_280X(int nTranCode , int nTxRxOption = FALSE); //종목코드                            
int		fnAPP_SetDataField_Item_281X(int nTranCode , int nTxRxOption = FALSE); //종목명                              
int		fnAPP_SetDataField_Item_282X(int nTranCode , int nTxRxOption = FALSE); //수량                                
int		fnAPP_SetDataField_Item_283X(int nTranCode , int nTxRxOption = FALSE); //청약증거금                          
int		fnAPP_SetDataField_Item_284X(int nTranCode , int nTxRxOption = FALSE); //대출(가능)금액                      
int		fnAPP_SetDataField_Item_285X(int nTranCode , int nTxRxOption = FALSE); //청약(가능)금액                      
int		fnAPP_SetDataField_Item_286X(int nTranCode , int nTxRxOption = FALSE); //대출금액화면                        
int		fnAPP_SetDataField_Item_287X(int nTranCode , int nTxRxOption = FALSE); //대출상품유형분류                    
int		fnAPP_SetDataField_Item_288X(int nTranCode , int nTxRxOption = FALSE); //청구방법                            
int		fnAPP_SetDataField_Item_289X(int nTranCode , int nTxRxOption = FALSE); //상환방법                            
int		fnAPP_SetDataField_Item_290X(int nTranCode , int nTxRxOption = FALSE); //대출상품화면                        
int		fnAPP_SetDataField_Item_291X(int nTranCode , int nTxRxOption = FALSE); //신한카드론 상품상세                 
int		fnAPP_SetDataField_Item_299X(int nTranCode , int nTxRxOption = FALSE); //시스템 공지사항                     
int		fnAPP_SetDataField_Item_300X(int nTranCode , int nTxRxOption = FALSE); //마케팅 대상여부                     
int		fnAPP_SetDataField_Item_301X(int nTranCode , int nTxRxOption = FALSE); //한도상향 데이터                     
int		fnAPP_SetDataField_Item_302X(int nTranCode , int nTxRxOption = FALSE); //행사등록 데이터                     
int		fnAPP_SetDataField_Item_303X(int nTranCode , int nTxRxOption = FALSE); //신상보정 데이터                     
int		fnAPP_SetDataField_Item_304x(int nTranCode , int nTxRxOption = FALSE); //여부조회 데이터(현대)               
int		fnAPP_SetDataField_Item_305x(int nTranCode , int nTxRxOption = FALSE); //한도상향 데이터(현대)               
int		fnAPP_SetDataField_Item_306x(int nTranCode , int nTxRxOption = FALSE); //캠페인 데이터(현대)                 
int		fnAPP_SetDataField_Item_310X(int nTranCode , int nTxRxOption = FALSE); //인증매체구분                        
int		fnAPP_SetDataField_Item_311X(int nTranCode , int nTxRxOption = FALSE); //가상계좌은행코드                    
int		fnAPP_SetDataField_Item_312X(int nTranCode , int nTxRxOption = FALSE); //가상계좌번호                        
int		fnAPP_SetDataField_Item_313X(int nTranCode , int nTxRxOption = FALSE); //첫이자입금일                        
int		fnAPP_SetDataField_Item_314X(int nTranCode , int nTxRxOption = FALSE); //대출진단ID                          
int		fnAPP_SetDataField_Item_315X(int nTranCode , int nTxRxOption = FALSE); //할인율                              
int		fnAPP_SetDataField_Item_316X(int nTranCode , int nTxRxOption = FALSE); //판매상품권일련번호                  
int		fnAPP_SetDataField_Item_320X(int nTranCode , int nTxRxOption = FALSE); //거스름돈 방출정보                   
int		fnAPP_SetDataField_Item_915X(int nTranCode , int nTxRxOption = FALSE); //비밀번호(농협/하나/삼성증/광주)     
int		fnAPP_SetDataField_Item_9151(int nTranCode , int nTxRxOption = FALSE); //비밀번호(농협/하나/삼성증/광주)     
int		fnAPP_SetDataField_Item_034X(int nTranCode , int nTxRxOption = FALSE); //처리요구구분(장애구분)      
int		fnAPP_SetDataField_Item_035X(int nTranCode , int nTxRxOption = FALSE); //지폐함 및 용지 상태정보     
int		fnAPP_SetDataField_Item_036X(int nTranCode , int nTxRxOption = FALSE); //지폐함별 권종 설정정보      
int		fnAPP_SetDataField_Item_047X(int nTranCode , int nTxRxOption = FALSE); //SST Serial Number(SST-ID)   
int		fnAPP_SetDataField_Item_083X(int nTranCode , int nTxRxOption = FALSE); //통신망관리정보              
int		fnAPP_SetDataField_Item_084X(int nTranCode , int nTxRxOption = FALSE); //Shutdown시각                
int		fnAPP_SetDataField_Item_102X(int nTranCode , int nTxRxOption = FALSE); //H/W Module 상태정보         
int		fnAPP_SetDataField_Item_103X(int nTranCode , int nTxRxOption = FALSE); //자원 감시 상태정보          
int		fnAPP_SetDataField_Item_105X(int nTranCode , int nTxRxOption = FALSE); //총예금출금건수              
int		fnAPP_SetDataField_Item_106X(int nTranCode , int nTxRxOption = FALSE); //총예금출금금액              
int		fnAPP_SetDataField_Item_107X(int nTranCode , int nTxRxOption = FALSE); //기타취소 총건수              
int		fnAPP_SetDataField_Item_108X(int nTranCode , int nTxRxOption = FALSE); //기타취소 총금액              
int		fnAPP_SetDataField_Item_109X(int nTranCode , int nTxRxOption = FALSE); //총서비스출금건수            
int		fnAPP_SetDataField_Item_110X(int nTranCode , int nTxRxOption = FALSE); //총서비스출금금액
int		fnAPP_SetDataField_Item_111X(int nTranCode , int nTxRxOption = FALSE); //총서비스출금취소건수           
int		fnAPP_SetDataField_Item_112X(int nTranCode , int nTxRxOption = FALSE); //총서비스출금취소금액         

int		fnAPP_SetDataField_Item_113X(int nTranCode , int nTxRxOption = FALSE); //총이체건수           
int		fnAPP_SetDataField_Item_114X(int nTranCode , int nTxRxOption = FALSE); //총이체금액         
int		fnAPP_SetDataField_Item_117X(int nTranCode , int nTxRxOption = FALSE); //총타이체건수           
int		fnAPP_SetDataField_Item_118X(int nTranCode , int nTxRxOption = FALSE); //총타이체금액         

int		fnAPP_SetDataField_Item_122X(int nTranCode , int nTxRxOption = FALSE); //총서비스이체건수                  
int		fnAPP_SetDataField_Item_123X(int nTranCode , int nTxRxOption = FALSE); //총서비스이체금액                 
  
int		fnAPP_SetDataField_Item_128X(int nTranCode , int nTxRxOption = FALSE); //총입금건수                  
int		fnAPP_SetDataField_Item_129X(int nTranCode , int nTxRxOption = FALSE); //총입금금액  
                
int		fnAPP_SetDataField_Item_124X(int nTranCode , int nTxRxOption = FALSE); //수표입금건수                
int		fnAPP_SetDataField_Item_125X(int nTranCode , int nTxRxOption = FALSE); //수표입금금액                
int		fnAPP_SetDataField_Item_126X(int nTranCode , int nTxRxOption = FALSE); //권종설정정보                
int		fnAPP_SetDataField_Item_132X(int nTranCode , int nTxRxOption = FALSE); //이전시제합계일자            
int		fnAPP_SetDataField_Item_133X(int nTranCode , int nTxRxOption = FALSE); //이전시제합계시각            
int		fnAPP_SetDataField_Item_134X(int nTranCode , int nTxRxOption = FALSE); //현재시제합계일자            
int		fnAPP_SetDataField_Item_135X(int nTranCode , int nTxRxOption = FALSE); //현재시제합계시각            
int		fnAPP_SetDataField_Item_136X(int nTranCode , int nTxRxOption = FALSE); //Cassette1 총방출매수        
int		fnAPP_SetDataField_Item_137X(int nTranCode , int nTxRxOption = FALSE); //Cassette2 총방출매수        
     
int		fnAPP_SetDataField_Item_140X(int nTranCode , int nTxRxOption = FALSE); //Cassette1 총회수매수        
int		fnAPP_SetDataField_Item_141X(int nTranCode , int nTxRxOption = FALSE); //Cassette2 총회수매수        
      
int		fnAPP_SetDataField_Item_145X(int nTranCode , int nTxRxOption = FALSE); //해외/약관/카드론총거래건수  
int		fnAPP_SetDataField_Item_146X(int nTranCode , int nTxRxOption = FALSE); //해외/약관/카드론총거래금액  
int		fnAPP_SetDataField_Item_147X(int nTranCode , int nTxRxOption = FALSE); //현금미수취 총건수           
int		fnAPP_SetDataField_Item_148X(int nTranCode , int nTxRxOption = FALSE); //현금미수취 총금액           
int		fnAPP_SetDataField_Item_149X(int nTranCode , int nTxRxOption = FALSE); //출금셔터부 에러 총건수      
int		fnAPP_SetDataField_Item_150X(int nTranCode , int nTxRxOption = FALSE); //출금셔터부 에러 총금액      
int		fnAPP_SetDataField_Item_151X(int nTranCode , int nTxRxOption = FALSE); //통신장애 총건수             
int		fnAPP_SetDataField_Item_152X(int nTranCode , int nTxRxOption = FALSE); //통신장애 총금액             
int		fnAPP_SetDataField_Item_153X(int nTranCode , int nTxRxOption = FALSE); //출금계수장애 총건수         
int		fnAPP_SetDataField_Item_154X(int nTranCode , int nTxRxOption = FALSE); //출금계수장애 총금액         
int		fnAPP_SetDataField_Item_155X(int nTranCode , int nTxRxOption = FALSE); //미완료취소 총건수(삭제요망) 
int		fnAPP_SetDataField_Item_156X(int nTranCode , int nTxRxOption = FALSE); //미완료취소 총금액(삭제요망) 
int		fnAPP_SetDataField_Item_157X(int nTranCode , int nTxRxOption = FALSE); //지폐 방출 총 건수           
int		fnAPP_SetDataField_Item_158X(int nTranCode , int nTxRxOption = FALSE); //지폐 방출 총 금액           
int		fnAPP_SetDataField_Item_159X(int nTranCode , int nTxRxOption = FALSE); //T-Money비제휴충전 총건수    
int		fnAPP_SetDataField_Item_160X(int nTranCode , int nTxRxOption = FALSE); //T-Money비제휴충전 총금액    
int		fnAPP_SetDataField_Item_161X(int nTranCode , int nTxRxOption = FALSE); //기타거래 총건수             
int		fnAPP_SetDataField_Item_162X(int nTranCode , int nTxRxOption = FALSE); //기타거래 총금액             
// int		fnAPP_SetDataField_Item_163X(int nTranCode , int nTxRxOption = FALSE); //기타취소 총건수             
// int		fnAPP_SetDataField_Item_164X(int nTranCode , int nTxRxOption = FALSE); //기타취소 총금액             
int		fnAPP_SetDataField_Item_165X(int nTranCode , int nTxRxOption = FALSE); //기기프로그램버전   
int		fnAPP_SetDataField_Item_1721(int nTranCode , int nTxRxOption = FALSE); //하나은행 취급 수수료   #N0214    
int		fnAPP_SetDataField_Item_173X(int nTranCode , int nTxRxOption = FALSE); //입금통신장애 총건수         
int		fnAPP_SetDataField_Item_174X(int nTranCode , int nTxRxOption = FALSE); //입금통신장애 총금액         
int		fnAPP_SetDataField_Item_175X(int nTranCode , int nTxRxOption = FALSE); //입금계수장애 총건수         
int		fnAPP_SetDataField_Item_176X(int nTranCode , int nTxRxOption = FALSE); //입금계수장애 총금액         
int		fnAPP_SetDataField_Item_177X(int nTranCode , int nTxRxOption = FALSE); //입금수납장애 총건수         
int		fnAPP_SetDataField_Item_178X(int nTranCode , int nTxRxOption = FALSE); //입금수납장애 총금액         
int		fnAPP_SetDataField_Item_179X(int nTranCode , int nTxRxOption = FALSE); //입금미수취 총건수           
int		fnAPP_SetDataField_Item_180X(int nTranCode , int nTxRxOption = FALSE); //입금미수취 총금액           
int		fnAPP_SetDataField_Item_182X(int nTranCode , int nTxRxOption = FALSE); //만원권 총방출매수           
int		fnAPP_SetDataField_Item_183X(int nTranCode , int nTxRxOption = FALSE); //5만원권 총방출매수          
int		fnAPP_SetDataField_Item_184X(int nTranCode , int nTxRxOption = FALSE); //10만원권 총방출매수         
int		fnAPP_SetDataField_Item_185X(int nTranCode , int nTxRxOption = FALSE); //만원권 총입금매수           
int		fnAPP_SetDataField_Item_186X(int nTranCode , int nTxRxOption = FALSE); //5만원권 총입금매수          
int		fnAPP_SetDataField_Item_187X(int nTranCode , int nTxRxOption = FALSE); //10만원권 총입금매수         
int		fnAPP_SetDataField_Item_188X(int nTranCode , int nTxRxOption = FALSE); //기타권종 총입금금액         
int		fnAPP_SetDataField_Item_317X(int nTranCode , int nTxRxOption = FALSE); //5천원권 총방출매수          
int		fnAPP_SetDataField_Item_318X(int nTranCode , int nTxRxOption = FALSE); //1천원권 총방출매수          
int		fnAPP_SetDataField_Item_319X(int nTranCode , int nTxRxOption = FALSE); //주화 총방출금액             
int		fnAPP_SetDataField_Item_910x(int nTranCode , int nTxRxOption = FALSE); //브랜드제휴 구분             
int		fnAPP_SetDataField_Item_911x(int nTranCode , int nTxRxOption = FALSE); //기기설정정보                
int		fnAPP_SetDataField_Item_912X(int nTranCode , int nTxRxOption = FALSE); //러쉬앤캐쉬 거래선택건수 집계

int		fnAPP_SetDataField_Item_914x(int nTranCode , int nTxRxOption = FALSE); //RPC 설정 정보 #N0209

int		fnAPP_SetDataField_Item_916x(int nTranCode , int nTxRxOption = FALSE); //VPN IP #N0254

int		fnAPP_SetDataField_Item_337X(int nTranCode , int nTxRxOption = FALSE); //응답코드
int		fnAPP_SetDataField_Item_338X(int nTranCode , int nTxRxOption = FALSE); //단말기번호
int		fnAPP_SetDataField_Item_339X(int nTranCode , int nTxRxOption = FALSE); //기기종류

int		fnAPP_SetDataField_Item_387X(int nTranCode , int nTxRxOption = FALSE); //통장증서발행번호(회차)

//폐국 #N0278
int		fnAPP_SetDataField_Item_390X(int nTranCode , int nTxRxOption = FALSE); //Rebooting 사유정보
int		fnAPP_SetDataField_Item_388X(int nTranCode , int nTxRxOption = FALSE); //통신장애 내용 필드
int		fnAPP_SetDataField_Item_389X(int nTranCode , int nTxRxOption = FALSE); //기기모델정보 필드
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


int		fnAPP_SetDataField_Item_040X(int nTranCode , int nTxRxOption = FALSE); //전달정보 - UpLoad Or Down Load시 실제정보

int		fnAPP_SetDataField_Item_168X(int nTranCode , int nTxRxOption = FALSE); //대출이자율             //#N0161-1

int		fnAPP_SetDataField_Item_346X(int nTranCode , int nTxRxOption = FALSE); //해외카드              //#N0176

int		fnAPP_SetDataField_Item_345X(int nTranCode , int nTxRxOption = FALSE); //해외카드              //#N0174

int		fnAPP_SetDataField_Item_307X(int nTranCode , int nTxRxOption = FALSE); //신한카드 한도상향     //#N0184

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//하이패스 충전 
int		fnAPP_SetDataField_Item_353X(int nTranCode , int nTxRxOption = FALSE); //하이패스 sam 데이터    //#N0266
int		fnAPP_SetDataField_Item_354X(int nTranCode , int nTxRxOption = FALSE); //하이패스 거래로그데이터     //#N0266
int		fnAPP_SetDataField_Item_355X(int nTranCode , int nTxRxOption = FALSE); //하이패스 거래일련번호 로그     //#N0266
int		fnAPP_SetDataField_Item_356X(int nTranCode , int nTxRxOption = FALSE); //하이패스 sam 데이터 로그     //#N0266
int		fnAPP_SetDataField_Item_357X(int nTranCode , int nTxRxOption = FALSE); //하이패스 id service 데이터     //#N0266

int		fnAPP_SetDataField_Item_379X(int nTranCode , int nTxRxOption = FALSE); //휴대폰번호
int		fnAPP_SetDataField_Item_445X(int nTranCode , int nTxRxOption = FALSE); //출금취소사유	445X	STR	n	N	출금사유 분석 위한 출금취소사유 (전문사양서 참조)
    
int		fnAPP_SetDataField_Item_459X(int nTranCode , int nTxRxOption = FALSE); //이자율구분 
int		fnAPP_SetDataField_Item_470X(int nTranCode , int nTxRxOption = FALSE); //신용카드IC관련정보
int		fnAPP_SetDataField_Item_471X(int nTranCode , int nTxRxOption = FALSE); //ARPC
int		fnAPP_SetDataField_Item_4721(int nTranCode , int nTxRxOption = FALSE);
int		fnAPP_SetDataField_Item_4751(int nTranCode , int nTxRxOption = FALSE); 


#endif




























