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

int		fnAPP_SetDataField_Item_081X(int nTranCode , int nTxRxOption = FALSE); //����Format����                                          
int		fnAPP_SetDataField_Item_001X(int nTranCode , int nTxRxOption = FALSE); //�ŷ���¥                                          
int		fnAPP_SetDataField_Item_002X(int nTranCode , int nTxRxOption = FALSE); //�ŷ��ð�                            
int		fnAPP_SetDataField_Item_003X(int nTranCode , int nTxRxOption = FALSE); //SST �Ϸù�ȣ                        
int		fnAPP_SetDataField_Item_004X(int nTranCode , int nTxRxOption = FALSE);            
int		fnAPP_SetDataField_Item_005X(int nTranCode , int nTxRxOption = FALSE); //��������(Msg-Type)                  
int		fnAPP_SetDataField_Item_006X(int nTranCode , int nTxRxOption = FALSE); //���������ڵ�(Tran-ID)  
int		fnAPP_SetDataField_Item_007X(int nTranCode , int nTxRxOption = FALSE); //��� MS Track ��ȣ                
int		fnAPP_SetDataField_Item_008X(int nTranCode , int nTxRxOption = FALSE); //�����ڵ�(RspCode)                   
int		fnAPP_SetDataField_Item_009X(int nTranCode , int nTxRxOption = FALSE); //��������                 
int		fnAPP_SetDataField_Item_010X(int nTranCode , int nTxRxOption = FALSE); //��ȣȭ���� - 0:����,1:�����ȣȭ,2:�߰����ȣ,3:���&�߰��     
int		fnAPP_SetDataField_Item_011X(int nTranCode , int nTxRxOption = FALSE); //SST����ȣ(MecaNum)   
int		fnAPP_SetDataField_Item_0120(int nTranCode , int nTxRxOption = FALSE); //��ޱ���ڵ�1  
   
//int		fnAPP_SetDataField_Item_071X(int nTranCode , int nTxRxOption = FALSE); //�ŷ���ü����  
//int		fnAPP_SetDataField_Item_072X(int nTranCode , int nTxRxOption = FALSE); //��ȣȭ ���� - 00:��ŸOr����,'01':SEED,'02':����Ʈ����,'03':�̴���
//int		fnAPP_SetDataField_Item_047X(int nTranCode , int nTxRxOption = FALSE); //SST Serial Number(SST-ID)   
//int		fnAPP_SetDataField_Item_088X(int nTranCode , int nTxRxOption = FALSE); //�ܸ����� IP Address(xxx.xxx.xxx.xxx)  
int		fnAPP_SetDataField_Item_464X(int nTranCode , int nTxRxOption = FALSE); //���ݺ���üũ
                     
int		fnAPP_SetDataField_Item_048X(int nTranCode , int nTxRxOption = FALSE); //�ſ�ī����������                    
int		fnAPP_SetDataField_Item_049X(int nTranCode , int nTxRxOption = FALSE); //SST��������  
int		fnAPP_SetDataField_Item_050X(int nTranCode , int nTxRxOption = FALSE); //���ŷ� �ܸ��� �Ϸù�ȣ (050X)
int		fnAPP_SetDataField_Item_055X(int nTranCode , int nTxRxOption = FALSE); //�������Աݾ�
int		fnAPP_SetDataField_Item_056X(int nTranCode , int nTxRxOption = FALSE); //�������Խð� 
int		fnAPP_SetDataField_Item_059X(int nTranCode , int nTxRxOption = FALSE); //�Ϲ�����(��ݰ��� ��ǥ��), ��������(��⺸�� ��ǥ��)
int		fnAPP_SetDataField_Item_061X(int nTranCode , int nTxRxOption = FALSE); //������ �ż�����
int		fnAPP_SetDataField_Item_062X(int nTranCode , int nTxRxOption = FALSE); //�������α����ڵ� 

int		fnAPP_SetDataField_Item_016X(int nTranCode , int nTxRxOption = FALSE); //��ޱ���ڵ�1                       
int		fnAPP_SetDataField_Item_017X(int nTranCode , int nTxRxOption = FALSE); //�ŷ�����(ī��)��ȣ1    
int		fnAPP_SetDataField_Item_018X(int nTranCode , int nTxRxOption = FALSE); //��ݸ�ü����    
int		fnAPP_SetDataField_Item_052X(int nTranCode , int nTxRxOption = FALSE); //�ŷ����¹�ȣ(�����   
                 
int		fnAPP_SetDataField_Item_054X(int nTranCode , int nTxRxOption = FALSE); //��ȣȭ ��������                 

 
                   
int		fnAPP_SetDataField_Item_013X(int nTranCode , int nTxRxOption = FALSE); //ISO3 Data  
int		fnAPP_SetDataField_Item_0131(int nTranCode , int nTxRxOption = FALSE); //ISO3 Data - ��ȣȭ
int		fnAPP_SetDataField_Item_014X(int nTranCode , int nTxRxOption = FALSE); //ISO2 Data                           
int		fnAPP_SetDataField_Item_0141(int nTranCode , int nTxRxOption = FALSE); //ISO2 Data - ��ȣȭ                           
int		fnAPP_SetDataField_Item_015X(int nTranCode , int nTxRxOption = FALSE); //��й�ȣ                            
int		fnAPP_SetDataField_Item_0151(int nTranCode , int nTxRxOption = FALSE); //��й�ȣ  - ��ȣȭ                           
//int		fnAPP_SetDataField_Item_016X(int nTranCode , int nTxRxOption = FALSE); //���� M/S Data                       
//int		fnAPP_SetDataField_Item_017X(int nTranCode , int nTxRxOption = FALSE); //�ŷ�����(ī��)��ȣ1                 
//int		fnAPP_SetDataField_Item_018X(int nTranCode , int nTxRxOption = FALSE); //�ŷ�����(ī��)����                  
int		fnAPP_SetDataField_Item_019X(int nTranCode , int nTxRxOption = FALSE); //�������ڵ�                              
int		fnAPP_SetDataField_Item_0191(int nTranCode , int nTxRxOption = FALSE); //Filler                              
int		fnAPP_SetDataField_Item_020X(int nTranCode , int nTxRxOption = FALSE); //������¹�ȣ                        
int		fnAPP_SetDataField_Item_021X(int nTranCode , int nTxRxOption = FALSE); //������¼���                        
int		fnAPP_SetDataField_Item_022X(int nTranCode , int nTxRxOption = FALSE); //�ŷ�����(ī��)�ܾ�                  
int		fnAPP_SetDataField_Item_0221(int nTranCode , int nTxRxOption = FALSE); //�ŷ�����(ī��)�ܾ�                  
int		fnAPP_SetDataField_Item_023X(int nTranCode , int nTxRxOption = FALSE); //�ŷ��ݾ�                            
int		fnAPP_SetDataField_Item_024X(int nTranCode , int nTxRxOption = FALSE); //�ŷ�������                          
int		fnAPP_SetDataField_Item_025X(int nTranCode , int nTxRxOption = FALSE); //�̰���Ÿ���� �ݾ�                   
int		fnAPP_SetDataField_Item_026X(int nTranCode , int nTxRxOption = FALSE); //�ؿܼ۱� ��������                   
int		fnAPP_SetDataField_Item_027X(int nTranCode , int nTxRxOption = FALSE); //��ǥ�ŷ��ݾ�                        
int		fnAPP_SetDataField_Item_028X(int nTranCode , int nTxRxOption = FALSE); //��ǥ�ż�                            
int		fnAPP_SetDataField_Item_029X(int nTranCode , int nTxRxOption = FALSE); //��ǥ���� ����  
int		fnAPP_SetDataField_Item_041X(int nTranCode , int nTxRxOption = FALSE); //��ȣȭKey����(�Ｚ���Ǿ�ȣȭ)                     
int		fnAPP_SetDataField_Item_038X(int nTranCode , int nTxRxOption = FALSE); //���ʽ� ��ü ��ݰ��� ����                            
//int		fnAPP_SetDataField_Item_042X(int nTranCode , int nTxRxOption = FALSE); //�Ϲ�����(��ݰ����Ѿ�)    
int		fnAPP_SetDataField_Item_042X(int nTranCode , int nTxRxOption = FALSE); //��ȣȭKey����                      
int		fnAPP_SetDataField_Item_043X(int nTranCode , int nTxRxOption = FALSE); //�־�ȣȭKey                         
int		fnAPP_SetDataField_Item_044X(int nTranCode , int nTxRxOption = FALSE); //������ȣȭKey                       
int		fnAPP_SetDataField_Item_0441(int nTranCode , int nTxRxOption = FALSE); //������ȣȭKey                       
int		fnAPP_SetDataField_Item_045X(int nTranCode , int nTxRxOption = FALSE); //���������㰡����                    

int		fnAPP_SetDataField_Item_057X(int nTranCode , int nTxRxOption = FALSE); //ICī�� �Ϸù�ȣ (057X)                   
int		fnAPP_SetDataField_Item_058X(int nTranCode , int nTxRxOption = FALSE); //ICī�� ��ȣȭ ��й�ȣ (058X)                    

int		fnAPP_SetDataField_Item_063X(int nTranCode , int nTxRxOption = FALSE); //�ŷ������Ͻ�                        
int		fnAPP_SetDataField_Item_064X(int nTranCode , int nTxRxOption = FALSE); //HOST�Ϸù�ȣ                        
int		fnAPP_SetDataField_Item_065X(int nTranCode , int nTxRxOption = FALSE); //����ȭ�� ���� �����ݾ�              
int		fnAPP_SetDataField_Item_066X(int nTranCode , int nTxRxOption = FALSE); //ȭ������                            
int		fnAPP_SetDataField_Item_067X(int nTranCode , int nTxRxOption = FALSE); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)   
int		fnAPP_SetDataField_Item_068X(int nTranCode , int nTxRxOption = FALSE); //�ŷ�ī����                          
int		fnAPP_SetDataField_Item_0691(int nTranCode , int nTxRxOption = FALSE); //������ī���ܾ�                      
int		fnAPP_SetDataField_Item_070X(int nTranCode , int nTxRxOption = FALSE); //����                                
int		fnAPP_SetDataField_Item_071X(int nTranCode , int nTxRxOption = FALSE); //������                              
int		fnAPP_SetDataField_Item_072X(int nTranCode , int nTxRxOption = FALSE); //ī������                            
int		fnAPP_SetDataField_Item_073X(int nTranCode , int nTxRxOption = FALSE); //�˰��� ID                         
int		fnAPP_SetDataField_Item_074X(int nTranCode , int nTxRxOption = FALSE); //VK                                  
int		fnAPP_SetDataField_Item_075X(int nTranCode , int nTxRxOption = FALSE); //����ID                              
int		fnAPP_SetDataField_Item_076X(int nTranCode , int nTxRxOption = FALSE); //������ID                          
int		fnAPP_SetDataField_Item_077X(int nTranCode , int nTxRxOption = FALSE); //�̿���ī��ID                        
int		fnAPP_SetDataField_Item_078X(int nTranCode , int nTxRxOption = FALSE); //PAN                                 
int		fnAPP_SetDataField_Item_079X(int nTranCode , int nTxRxOption = FALSE); //������ID                            
int		fnAPP_SetDataField_Item_080X(int nTranCode , int nTxRxOption = FALSE); //���� �ſ�ī��(����)��ȣ             
int		fnAPP_SetDataField_Item_085X(int nTranCode , int nTxRxOption = FALSE); //��Ż� ����                         
int		fnAPP_SetDataField_Item_086X(int nTranCode , int nTxRxOption = FALSE); //��ȭ��ȣ                            
int		fnAPP_SetDataField_Item_087X(int nTranCode , int nTxRxOption = FALSE); //��ּ����ڵ�                   
int		fnAPP_SetDataField_Item_088X(int nTranCode , int nTxRxOption = FALSE); //�ܸ����� IP Address(xxx.xxx.xxx.xxx)                  
int		fnAPP_SetDataField_Item_089X(int nTranCode , int nTxRxOption = FALSE); //����ŷ����� ȭ��ȳ� �޽���        
int		fnAPP_SetDataField_Item_090X(int nTranCode , int nTxRxOption = FALSE); //�����ܾ� ���ڿ���                   
int		fnAPP_SetDataField_Item_091X(int nTranCode , int nTxRxOption = FALSE); //HOST�۾� ������ ������                
int		fnAPP_SetDataField_Item_092X(int nTranCode , int nTxRxOption = FALSE); //�������� ������                     
int		fnAPP_SetDataField_Item_093X(int nTranCode , int nTxRxOption = FALSE); //�������� ���Ǽ�                   
int		fnAPP_SetDataField_Item_094X(int nTranCode , int nTxRxOption = FALSE); //������������                        
int		fnAPP_SetDataField_Item_095X(int nTranCode , int nTxRxOption = FALSE); //���������ڷ� �߰� ���� ����         
int		fnAPP_SetDataField_Item_096X(int nTranCode , int nTxRxOption = FALSE); //�������Ϲ�ȣ                        
int		fnAPP_SetDataField_Item_097X(int nTranCode , int nTxRxOption = FALSE); //���弭������                        
int		fnAPP_SetDataField_Item_098X(int nTranCode , int nTxRxOption = FALSE); //�߰� ����� KEY                     
int		fnAPP_SetDataField_Item_099X(int nTranCode , int nTxRxOption = FALSE); //�ٰ��� ����                         
int		fnAPP_SetDataField_Item_100X(int nTranCode , int nTxRxOption = FALSE); //����OFF�Ⱓ                            
int		fnAPP_SetDataField_Item_101X(int nTranCode , int nTxRxOption = FALSE); //��ȸ����                            
int		fnAPP_SetDataField_Item_104X(int nTranCode , int nTxRxOption = FALSE); //��ǥ ����Data                                                       
int		fnAPP_SetDataField_Item_130X(int nTranCode , int nTxRxOption = FALSE); //�����������                        
int		fnAPP_SetDataField_Item_131X(int nTranCode , int nTxRxOption = FALSE); //����ݾ�                          
int		fnAPP_SetDataField_Item_166X(int nTranCode , int nTxRxOption = FALSE); //�����ǰ�ڵ�                        
int		fnAPP_SetDataField_Item_167X(int nTranCode , int nTxRxOption = FALSE); //�����û������                      
int		fnAPP_SetDataField_Item_1680(int nTranCode , int nTxRxOption = FALSE); //��������                            
int		fnAPP_SetDataField_Item_1810(int nTranCode , int nTxRxOption = FALSE); //��ü����                            
int		fnAPP_SetDataField_Item_169X(int nTranCode , int nTxRxOption = FALSE); //�����ݳ�����                        
int		fnAPP_SetDataField_Item_170X(int nTranCode , int nTxRxOption = FALSE); //������                              
int		fnAPP_SetDataField_Item_171X(int nTranCode , int nTxRxOption = FALSE); //���Ⱑ�ɱݾ�1                       
int		fnAPP_SetDataField_Item_172X(int nTranCode , int nTxRxOption = FALSE); //��޼�����                          
int		fnAPP_SetDataField_Item_189X(int nTranCode , int nTxRxOption = FALSE); //��õ���� �ż�                       
int		fnAPP_SetDataField_Item_190X(int nTranCode , int nTxRxOption = FALSE); //õ���� �ż�                         
int		fnAPP_SetDataField_Item_191X(int nTranCode , int nTxRxOption = FALSE); //���ι�ȣ                            
int		fnAPP_SetDataField_Item_192X(int nTranCode , int nTxRxOption = FALSE); //���ڳ��ι�ȣ(�Ϸù�ȣ)              
int		fnAPP_SetDataField_Item_193X(int nTranCode , int nTxRxOption = FALSE); //���γ��γ���  
int		fnAPP_SetDataField_Item_1931(int nTranCode , int nTxRxOption = FALSE); //���漼 ���γ���          
int		fnAPP_SetDataField_Item_194X(int nTranCode , int nTxRxOption = FALSE); //�̿�����(�ŷ��� ��ȣ)             
int		fnAPP_SetDataField_Item_195X(int nTranCode , int nTxRxOption = FALSE); //���ΰ����ȸ����                    
int		fnAPP_SetDataField_Item_196X(int nTranCode , int nTxRxOption = FALSE); //�Һΰ�����                          
int		fnAPP_SetDataField_Item_201X(int nTranCode , int nTxRxOption = FALSE); //�ſ�������ȸ���� ��û�����       
int		fnAPP_SetDataField_Item_202X(int nTranCode , int nTxRxOption = FALSE); //�޴���ȭ SMS�߼� ����Ű             
int		fnAPP_SetDataField_Item_204X(int nTranCode , int nTxRxOption = FALSE); //�����ڵ�                            
int		fnAPP_SetDataField_Item_205X(int nTranCode , int nTxRxOption = FALSE); //����Ʈ��                            
int		fnAPP_SetDataField_Item_206X(int nTranCode , int nTxRxOption = FALSE); //����Ʈ�ּ�                          
int		fnAPP_SetDataField_Item_207X(int nTranCode , int nTxRxOption = FALSE); //�ε��� �ü���ȸ �󼼳���            
int		fnAPP_SetDataField_Item_208X(int nTranCode , int nTxRxOption = FALSE); //�߰� �����ڵ�                       
int		fnAPP_SetDataField_Item_209X(int nTranCode , int nTxRxOption = FALSE); //� ������                         
int		fnAPP_SetDataField_Item_210X(int nTranCode , int nTxRxOption = FALSE); //�ſ�������ȸ���� ��������       
int		fnAPP_SetDataField_Item_211X(int nTranCode , int nTxRxOption = FALSE); //�ſ�������ȸ ä��������(����) ����  
int		fnAPP_SetDataField_Item_811X(int nTranCode , int nTxRxOption = FALSE); //�ſ�������ȸ ä��������(����) 2     
int		fnAPP_SetDataField_Item_212X(int nTranCode , int nTxRxOption = FALSE); //�ſ�������ȸ ä��������(����) ����  
int		fnAPP_SetDataField_Item_812X(int nTranCode , int nTxRxOption = FALSE); //�ſ�������ȸ ä��������(����) 2     
int		fnAPP_SetDataField_Item_213X(int nTranCode , int nTxRxOption = FALSE); //�ſ�������ȸ �ſ밳������ ����      
int		fnAPP_SetDataField_Item_813X(int nTranCode , int nTxRxOption = FALSE); //�ſ�������ȸ �ſ밳������ 2         
int		fnAPP_SetDataField_Item_214X(int nTranCode , int nTxRxOption = FALSE); //�ſ�������ȸ ��ȸ������� ����      
int		fnAPP_SetDataField_Item_215X(int nTranCode , int nTxRxOption = FALSE); //�������� ����                       
int		fnAPP_SetDataField_Item_815X(int nTranCode , int nTxRxOption = FALSE); //�������� ���� 2                     
int		fnAPP_SetDataField_Item_216X(int nTranCode , int nTxRxOption = FALSE); //���ݼ��� ���� ����                
int		fnAPP_SetDataField_Item_816X(int nTranCode , int nTxRxOption = FALSE); //���ݼ��� ���� ���� 2              
int		fnAPP_SetDataField_Item_217X(int nTranCode , int nTxRxOption = FALSE); //ä���������� ����                   
int		fnAPP_SetDataField_Item_817X(int nTranCode , int nTxRxOption = FALSE); //ä���������� ���� 2                 
int		fnAPP_SetDataField_Item_218X(int nTranCode , int nTxRxOption = FALSE); //CB��ü���� ����                     
int		fnAPP_SetDataField_Item_818X(int nTranCode , int nTxRxOption = FALSE); //CB��ü���� ���� 2                   
int		fnAPP_SetDataField_Item_828X(int nTranCode , int nTxRxOption = FALSE); //CB��ü���� ���� 3                   
int		fnAPP_SetDataField_Item_219X(int nTranCode , int nTxRxOption = FALSE); //�������� ����                       
int		fnAPP_SetDataField_Item_220X(int nTranCode , int nTxRxOption = FALSE); //�ֹε�Ϲ�ȣ(����:��ȣȭ�ؼ�)       
int		fnAPP_SetDataField_Item_221X(int nTranCode , int nTxRxOption = FALSE); //���ι�ȣ(�ֹ���ȣ)                  
int		fnAPP_SetDataField_Item_222X(int nTranCode , int nTxRxOption = FALSE); //���ܰ� �ŷ�������ȣ                 
int		fnAPP_SetDataField_Item_223X(int nTranCode , int nTxRxOption = FALSE); //�������� �ۼ��� �����              
int		fnAPP_SetDataField_Item_224X(int nTranCode , int nTxRxOption = FALSE); //�����ŷ� SST�Ϸù�ȣ                
int		fnAPP_SetDataField_Item_225X(int nTranCode , int nTxRxOption = FALSE); //����� ������                     
int		fnAPP_SetDataField_Item_230X(int nTranCode , int nTxRxOption = FALSE); //�������������� ����                 
int		fnAPP_SetDataField_Item_231X(int nTranCode , int nTxRxOption = FALSE); //������������1                       
int		fnAPP_SetDataField_Item_232X(int nTranCode , int nTxRxOption = FALSE); //������������2                       
int		fnAPP_SetDataField_Item_233X(int nTranCode , int nTxRxOption = FALSE); //������������3                       
int		fnAPP_SetDataField_Item_234X(int nTranCode , int nTxRxOption = FALSE); //������������4                       
int		fnAPP_SetDataField_Item_235X(int nTranCode , int nTxRxOption = FALSE); //������������5                       
int		fnAPP_SetDataField_Item_236X(int nTranCode , int nTxRxOption = FALSE); //������������6                       
int		fnAPP_SetDataField_Item_237X(int nTranCode , int nTxRxOption = FALSE); //������������7                       
int		fnAPP_SetDataField_Item_238X(int nTranCode , int nTxRxOption = FALSE); //������������8                       
int		fnAPP_SetDataField_Item_240X(int nTranCode , int nTxRxOption = FALSE); //�Ǹ�ó/���� ���� ����(��������Ƽ��) 
int		fnAPP_SetDataField_Item_241X(int nTranCode , int nTxRxOption = FALSE); //���� ���� ���䳻��(��������Ƽ��)    
int		fnAPP_SetDataField_Item_242X(int nTranCode , int nTxRxOption = FALSE); //�ſ���ο�û����(��������Ƽ��)      
int		fnAPP_SetDataField_Item_243X(int nTranCode , int nTxRxOption = FALSE); //�ſ�������䳻��(��������Ƽ��)      
int		fnAPP_SetDataField_Item_244X(int nTranCode , int nTxRxOption = FALSE); //�ȳ��޼���                          
int		fnAPP_SetDataField_Item_245X(int nTranCode , int nTxRxOption = FALSE); //��������1                           
int		fnAPP_SetDataField_Item_246X(int nTranCode , int nTxRxOption = FALSE); //��������2                           
int		fnAPP_SetDataField_Item_247X(int nTranCode , int nTxRxOption = FALSE); //Ƽ�ϳ��� ����(��������Ƽ��)         
int		fnAPP_SetDataField_Item_248X(int nTranCode , int nTxRxOption = FALSE); //�Ǹ�ó ���䳻��(��������Ƽ��)       
int		fnAPP_SetDataField_Item_249X(int nTranCode , int nTxRxOption = FALSE); //����/�������� ���䳻��(��������Ƽ��)
int		fnAPP_SetDataField_Item_250X(int nTranCode , int nTxRxOption = FALSE); //��߽ð� ���䳻��                   
int		fnAPP_SetDataField_Item_251X(int nTranCode , int nTxRxOption = FALSE); //�ڵ��¼� ���� ��û ����             
int		fnAPP_SetDataField_Item_252X(int nTranCode , int nTxRxOption = FALSE); //�ڵ��¼� ���� ���� ����             
int		fnAPP_SetDataField_Item_253X(int nTranCode , int nTxRxOption = FALSE); //�ֹ����� ���� ����                  
int		fnAPP_SetDataField_Item_254X(int nTranCode , int nTxRxOption = FALSE); //��¸ż����� ����                   
int		fnAPP_SetDataField_Item_255X(int nTranCode , int nTxRxOption = FALSE); //�߱� ���� ���� 1(��������Ƽ��)      
int		fnAPP_SetDataField_Item_256X(int nTranCode , int nTxRxOption = FALSE); //�߱� ���� ���� 2(��������Ƽ��)      
int		fnAPP_SetDataField_Item_257X(int nTranCode , int nTxRxOption = FALSE); //�߱� ���� ���� 3(��������Ƽ��)      
int		fnAPP_SetDataField_Item_258X(int nTranCode , int nTxRxOption = FALSE); //�߱� ���� ���� 4(��������Ƽ��)      
int		fnAPP_SetDataField_Item_260X(int nTranCode , int nTxRxOption = FALSE); //TC/TE���� ��ȸ/��û ���볻��        
int		fnAPP_SetDataField_Item_261X(int nTranCode , int nTxRxOption = FALSE); //TC/TE���� ��û ����                 
int		fnAPP_SetDataField_Item_270X(int nTranCode , int nTxRxOption = FALSE); //���ް��� �ݾ���ȸ                   
int		fnAPP_SetDataField_Item_271X(int nTranCode , int nTxRxOption = FALSE); //������ ��ȸ                         
int		fnAPP_SetDataField_Item_272X(int nTranCode , int nTxRxOption = FALSE); //���°��� ��Ÿ����                   
int		fnAPP_SetDataField_Item_273X(int nTranCode , int nTxRxOption = FALSE); //��ǰ, ������� ��Ÿ����             
int		fnAPP_SetDataField_Item_280X(int nTranCode , int nTxRxOption = FALSE); //�����ڵ�                            
int		fnAPP_SetDataField_Item_281X(int nTranCode , int nTxRxOption = FALSE); //�����                              
int		fnAPP_SetDataField_Item_282X(int nTranCode , int nTxRxOption = FALSE); //����                                
int		fnAPP_SetDataField_Item_283X(int nTranCode , int nTxRxOption = FALSE); //û�����ű�                          
int		fnAPP_SetDataField_Item_284X(int nTranCode , int nTxRxOption = FALSE); //����(����)�ݾ�                      
int		fnAPP_SetDataField_Item_285X(int nTranCode , int nTxRxOption = FALSE); //û��(����)�ݾ�                      
int		fnAPP_SetDataField_Item_286X(int nTranCode , int nTxRxOption = FALSE); //����ݾ�ȭ��                        
int		fnAPP_SetDataField_Item_287X(int nTranCode , int nTxRxOption = FALSE); //�����ǰ�����з�                    
int		fnAPP_SetDataField_Item_288X(int nTranCode , int nTxRxOption = FALSE); //û�����                            
int		fnAPP_SetDataField_Item_289X(int nTranCode , int nTxRxOption = FALSE); //��ȯ���                            
int		fnAPP_SetDataField_Item_290X(int nTranCode , int nTxRxOption = FALSE); //�����ǰȭ��                        
int		fnAPP_SetDataField_Item_291X(int nTranCode , int nTxRxOption = FALSE); //����ī��� ��ǰ��                 
int		fnAPP_SetDataField_Item_299X(int nTranCode , int nTxRxOption = FALSE); //�ý��� ��������                     
int		fnAPP_SetDataField_Item_300X(int nTranCode , int nTxRxOption = FALSE); //������ ��󿩺�                     
int		fnAPP_SetDataField_Item_301X(int nTranCode , int nTxRxOption = FALSE); //�ѵ����� ������                     
int		fnAPP_SetDataField_Item_302X(int nTranCode , int nTxRxOption = FALSE); //����� ������                     
int		fnAPP_SetDataField_Item_303X(int nTranCode , int nTxRxOption = FALSE); //�Ż��� ������                     
int		fnAPP_SetDataField_Item_304x(int nTranCode , int nTxRxOption = FALSE); //������ȸ ������(����)               
int		fnAPP_SetDataField_Item_305x(int nTranCode , int nTxRxOption = FALSE); //�ѵ����� ������(����)               
int		fnAPP_SetDataField_Item_306x(int nTranCode , int nTxRxOption = FALSE); //ķ���� ������(����)                 
int		fnAPP_SetDataField_Item_310X(int nTranCode , int nTxRxOption = FALSE); //������ü����                        
int		fnAPP_SetDataField_Item_311X(int nTranCode , int nTxRxOption = FALSE); //������������ڵ�                    
int		fnAPP_SetDataField_Item_312X(int nTranCode , int nTxRxOption = FALSE); //������¹�ȣ                        
int		fnAPP_SetDataField_Item_313X(int nTranCode , int nTxRxOption = FALSE); //ù�����Ա���                        
int		fnAPP_SetDataField_Item_314X(int nTranCode , int nTxRxOption = FALSE); //��������ID                          
int		fnAPP_SetDataField_Item_315X(int nTranCode , int nTxRxOption = FALSE); //������                              
int		fnAPP_SetDataField_Item_316X(int nTranCode , int nTxRxOption = FALSE); //�ǸŻ�ǰ���Ϸù�ȣ                  
int		fnAPP_SetDataField_Item_320X(int nTranCode , int nTxRxOption = FALSE); //�Ž����� ��������                   
int		fnAPP_SetDataField_Item_915X(int nTranCode , int nTxRxOption = FALSE); //��й�ȣ(����/�ϳ�/�Ｚ��/����)     
int		fnAPP_SetDataField_Item_9151(int nTranCode , int nTxRxOption = FALSE); //��й�ȣ(����/�ϳ�/�Ｚ��/����)     
int		fnAPP_SetDataField_Item_034X(int nTranCode , int nTxRxOption = FALSE); //ó���䱸����(��ֱ���)      
int		fnAPP_SetDataField_Item_035X(int nTranCode , int nTxRxOption = FALSE); //������ �� ���� ��������     
int		fnAPP_SetDataField_Item_036X(int nTranCode , int nTxRxOption = FALSE); //�����Ժ� ���� ��������      
int		fnAPP_SetDataField_Item_047X(int nTranCode , int nTxRxOption = FALSE); //SST Serial Number(SST-ID)   
int		fnAPP_SetDataField_Item_083X(int nTranCode , int nTxRxOption = FALSE); //��Ÿ���������              
int		fnAPP_SetDataField_Item_084X(int nTranCode , int nTxRxOption = FALSE); //Shutdown�ð�                
int		fnAPP_SetDataField_Item_102X(int nTranCode , int nTxRxOption = FALSE); //H/W Module ��������         
int		fnAPP_SetDataField_Item_103X(int nTranCode , int nTxRxOption = FALSE); //�ڿ� ���� ��������          
int		fnAPP_SetDataField_Item_105X(int nTranCode , int nTxRxOption = FALSE); //�ѿ�����ݰǼ�              
int		fnAPP_SetDataField_Item_106X(int nTranCode , int nTxRxOption = FALSE); //�ѿ�����ݱݾ�              
int		fnAPP_SetDataField_Item_107X(int nTranCode , int nTxRxOption = FALSE); //��Ÿ��� �ѰǼ�              
int		fnAPP_SetDataField_Item_108X(int nTranCode , int nTxRxOption = FALSE); //��Ÿ��� �ѱݾ�              
int		fnAPP_SetDataField_Item_109X(int nTranCode , int nTxRxOption = FALSE); //�Ѽ�����ݰǼ�            
int		fnAPP_SetDataField_Item_110X(int nTranCode , int nTxRxOption = FALSE); //�Ѽ�����ݱݾ�
int		fnAPP_SetDataField_Item_111X(int nTranCode , int nTxRxOption = FALSE); //�Ѽ��������ҰǼ�           
int		fnAPP_SetDataField_Item_112X(int nTranCode , int nTxRxOption = FALSE); //�Ѽ��������ұݾ�         

int		fnAPP_SetDataField_Item_113X(int nTranCode , int nTxRxOption = FALSE); //����ü�Ǽ�           
int		fnAPP_SetDataField_Item_114X(int nTranCode , int nTxRxOption = FALSE); //����ü�ݾ�         
int		fnAPP_SetDataField_Item_117X(int nTranCode , int nTxRxOption = FALSE); //��Ÿ��ü�Ǽ�           
int		fnAPP_SetDataField_Item_118X(int nTranCode , int nTxRxOption = FALSE); //��Ÿ��ü�ݾ�         

int		fnAPP_SetDataField_Item_122X(int nTranCode , int nTxRxOption = FALSE); //�Ѽ�����ü�Ǽ�                  
int		fnAPP_SetDataField_Item_123X(int nTranCode , int nTxRxOption = FALSE); //�Ѽ�����ü�ݾ�                 
  
int		fnAPP_SetDataField_Item_128X(int nTranCode , int nTxRxOption = FALSE); //���ԱݰǼ�                  
int		fnAPP_SetDataField_Item_129X(int nTranCode , int nTxRxOption = FALSE); //���Աݱݾ�  
                
int		fnAPP_SetDataField_Item_124X(int nTranCode , int nTxRxOption = FALSE); //��ǥ�ԱݰǼ�                
int		fnAPP_SetDataField_Item_125X(int nTranCode , int nTxRxOption = FALSE); //��ǥ�Աݱݾ�                
int		fnAPP_SetDataField_Item_126X(int nTranCode , int nTxRxOption = FALSE); //������������                
int		fnAPP_SetDataField_Item_132X(int nTranCode , int nTxRxOption = FALSE); //���������հ�����            
int		fnAPP_SetDataField_Item_133X(int nTranCode , int nTxRxOption = FALSE); //���������հ�ð�            
int		fnAPP_SetDataField_Item_134X(int nTranCode , int nTxRxOption = FALSE); //��������հ�����            
int		fnAPP_SetDataField_Item_135X(int nTranCode , int nTxRxOption = FALSE); //��������հ�ð�            
int		fnAPP_SetDataField_Item_136X(int nTranCode , int nTxRxOption = FALSE); //Cassette1 �ѹ���ż�        
int		fnAPP_SetDataField_Item_137X(int nTranCode , int nTxRxOption = FALSE); //Cassette2 �ѹ���ż�        
     
int		fnAPP_SetDataField_Item_140X(int nTranCode , int nTxRxOption = FALSE); //Cassette1 ��ȸ���ż�        
int		fnAPP_SetDataField_Item_141X(int nTranCode , int nTxRxOption = FALSE); //Cassette2 ��ȸ���ż�        
      
int		fnAPP_SetDataField_Item_145X(int nTranCode , int nTxRxOption = FALSE); //�ؿ�/���/ī����Ѱŷ��Ǽ�  
int		fnAPP_SetDataField_Item_146X(int nTranCode , int nTxRxOption = FALSE); //�ؿ�/���/ī����Ѱŷ��ݾ�  
int		fnAPP_SetDataField_Item_147X(int nTranCode , int nTxRxOption = FALSE); //���ݹ̼��� �ѰǼ�           
int		fnAPP_SetDataField_Item_148X(int nTranCode , int nTxRxOption = FALSE); //���ݹ̼��� �ѱݾ�           
int		fnAPP_SetDataField_Item_149X(int nTranCode , int nTxRxOption = FALSE); //��ݼ��ͺ� ���� �ѰǼ�      
int		fnAPP_SetDataField_Item_150X(int nTranCode , int nTxRxOption = FALSE); //��ݼ��ͺ� ���� �ѱݾ�      
int		fnAPP_SetDataField_Item_151X(int nTranCode , int nTxRxOption = FALSE); //������ �ѰǼ�             
int		fnAPP_SetDataField_Item_152X(int nTranCode , int nTxRxOption = FALSE); //������ �ѱݾ�             
int		fnAPP_SetDataField_Item_153X(int nTranCode , int nTxRxOption = FALSE); //��ݰ����� �ѰǼ�         
int		fnAPP_SetDataField_Item_154X(int nTranCode , int nTxRxOption = FALSE); //��ݰ����� �ѱݾ�         
int		fnAPP_SetDataField_Item_155X(int nTranCode , int nTxRxOption = FALSE); //�̿Ϸ���� �ѰǼ�(�������) 
int		fnAPP_SetDataField_Item_156X(int nTranCode , int nTxRxOption = FALSE); //�̿Ϸ���� �ѱݾ�(�������) 
int		fnAPP_SetDataField_Item_157X(int nTranCode , int nTxRxOption = FALSE); //���� ���� �� �Ǽ�           
int		fnAPP_SetDataField_Item_158X(int nTranCode , int nTxRxOption = FALSE); //���� ���� �� �ݾ�           
int		fnAPP_SetDataField_Item_159X(int nTranCode , int nTxRxOption = FALSE); //T-Money���������� �ѰǼ�    
int		fnAPP_SetDataField_Item_160X(int nTranCode , int nTxRxOption = FALSE); //T-Money���������� �ѱݾ�    
int		fnAPP_SetDataField_Item_161X(int nTranCode , int nTxRxOption = FALSE); //��Ÿ�ŷ� �ѰǼ�             
int		fnAPP_SetDataField_Item_162X(int nTranCode , int nTxRxOption = FALSE); //��Ÿ�ŷ� �ѱݾ�             
// int		fnAPP_SetDataField_Item_163X(int nTranCode , int nTxRxOption = FALSE); //��Ÿ��� �ѰǼ�             
// int		fnAPP_SetDataField_Item_164X(int nTranCode , int nTxRxOption = FALSE); //��Ÿ��� �ѱݾ�             
int		fnAPP_SetDataField_Item_165X(int nTranCode , int nTxRxOption = FALSE); //������α׷�����   
int		fnAPP_SetDataField_Item_1721(int nTranCode , int nTxRxOption = FALSE); //�ϳ����� ��� ������   #N0214    
int		fnAPP_SetDataField_Item_173X(int nTranCode , int nTxRxOption = FALSE); //�Ա������� �ѰǼ�         
int		fnAPP_SetDataField_Item_174X(int nTranCode , int nTxRxOption = FALSE); //�Ա������� �ѱݾ�         
int		fnAPP_SetDataField_Item_175X(int nTranCode , int nTxRxOption = FALSE); //�Աݰ����� �ѰǼ�         
int		fnAPP_SetDataField_Item_176X(int nTranCode , int nTxRxOption = FALSE); //�Աݰ����� �ѱݾ�         
int		fnAPP_SetDataField_Item_177X(int nTranCode , int nTxRxOption = FALSE); //�Աݼ������ �ѰǼ�         
int		fnAPP_SetDataField_Item_178X(int nTranCode , int nTxRxOption = FALSE); //�Աݼ������ �ѱݾ�         
int		fnAPP_SetDataField_Item_179X(int nTranCode , int nTxRxOption = FALSE); //�Աݹ̼��� �ѰǼ�           
int		fnAPP_SetDataField_Item_180X(int nTranCode , int nTxRxOption = FALSE); //�Աݹ̼��� �ѱݾ�           
int		fnAPP_SetDataField_Item_182X(int nTranCode , int nTxRxOption = FALSE); //������ �ѹ���ż�           
int		fnAPP_SetDataField_Item_183X(int nTranCode , int nTxRxOption = FALSE); //5������ �ѹ���ż�          
int		fnAPP_SetDataField_Item_184X(int nTranCode , int nTxRxOption = FALSE); //10������ �ѹ���ż�         
int		fnAPP_SetDataField_Item_185X(int nTranCode , int nTxRxOption = FALSE); //������ ���Աݸż�           
int		fnAPP_SetDataField_Item_186X(int nTranCode , int nTxRxOption = FALSE); //5������ ���Աݸż�          
int		fnAPP_SetDataField_Item_187X(int nTranCode , int nTxRxOption = FALSE); //10������ ���Աݸż�         
int		fnAPP_SetDataField_Item_188X(int nTranCode , int nTxRxOption = FALSE); //��Ÿ���� ���Աݱݾ�         
int		fnAPP_SetDataField_Item_317X(int nTranCode , int nTxRxOption = FALSE); //5õ���� �ѹ���ż�          
int		fnAPP_SetDataField_Item_318X(int nTranCode , int nTxRxOption = FALSE); //1õ���� �ѹ���ż�          
int		fnAPP_SetDataField_Item_319X(int nTranCode , int nTxRxOption = FALSE); //��ȭ �ѹ���ݾ�             
int		fnAPP_SetDataField_Item_910x(int nTranCode , int nTxRxOption = FALSE); //�귣������ ����             
int		fnAPP_SetDataField_Item_911x(int nTranCode , int nTxRxOption = FALSE); //��⼳������                
int		fnAPP_SetDataField_Item_912X(int nTranCode , int nTxRxOption = FALSE); //������ĳ�� �ŷ����ðǼ� ����

int		fnAPP_SetDataField_Item_914x(int nTranCode , int nTxRxOption = FALSE); //RPC ���� ���� #N0209

int		fnAPP_SetDataField_Item_916x(int nTranCode , int nTxRxOption = FALSE); //VPN IP #N0254

int		fnAPP_SetDataField_Item_337X(int nTranCode , int nTxRxOption = FALSE); //�����ڵ�
int		fnAPP_SetDataField_Item_338X(int nTranCode , int nTxRxOption = FALSE); //�ܸ����ȣ
int		fnAPP_SetDataField_Item_339X(int nTranCode , int nTxRxOption = FALSE); //�������

int		fnAPP_SetDataField_Item_387X(int nTranCode , int nTxRxOption = FALSE); //�������������ȣ(ȸ��)

//�� #N0278
int		fnAPP_SetDataField_Item_390X(int nTranCode , int nTxRxOption = FALSE); //Rebooting ��������
int		fnAPP_SetDataField_Item_388X(int nTranCode , int nTxRxOption = FALSE); //������ ���� �ʵ�
int		fnAPP_SetDataField_Item_389X(int nTranCode , int nTxRxOption = FALSE); //�������� �ʵ�
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


int		fnAPP_SetDataField_Item_040X(int nTranCode , int nTxRxOption = FALSE); //�������� - UpLoad Or Down Load�� ��������

int		fnAPP_SetDataField_Item_168X(int nTranCode , int nTxRxOption = FALSE); //����������             //#N0161-1

int		fnAPP_SetDataField_Item_346X(int nTranCode , int nTxRxOption = FALSE); //�ؿ�ī��              //#N0176

int		fnAPP_SetDataField_Item_345X(int nTranCode , int nTxRxOption = FALSE); //�ؿ�ī��              //#N0174

int		fnAPP_SetDataField_Item_307X(int nTranCode , int nTxRxOption = FALSE); //����ī�� �ѵ�����     //#N0184

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����н� ���� 
int		fnAPP_SetDataField_Item_353X(int nTranCode , int nTxRxOption = FALSE); //�����н� sam ������    //#N0266
int		fnAPP_SetDataField_Item_354X(int nTranCode , int nTxRxOption = FALSE); //�����н� �ŷ��α׵�����     //#N0266
int		fnAPP_SetDataField_Item_355X(int nTranCode , int nTxRxOption = FALSE); //�����н� �ŷ��Ϸù�ȣ �α�     //#N0266
int		fnAPP_SetDataField_Item_356X(int nTranCode , int nTxRxOption = FALSE); //�����н� sam ������ �α�     //#N0266
int		fnAPP_SetDataField_Item_357X(int nTranCode , int nTxRxOption = FALSE); //�����н� id service ������     //#N0266

int		fnAPP_SetDataField_Item_379X(int nTranCode , int nTxRxOption = FALSE); //�޴�����ȣ
int		fnAPP_SetDataField_Item_445X(int nTranCode , int nTxRxOption = FALSE); //�����һ���	445X	STR	n	N	��ݻ��� �м� ���� �����һ��� (������缭 ����)
    
int		fnAPP_SetDataField_Item_459X(int nTranCode , int nTxRxOption = FALSE); //���������� 
int		fnAPP_SetDataField_Item_470X(int nTranCode , int nTxRxOption = FALSE); //�ſ�ī��IC��������
int		fnAPP_SetDataField_Item_471X(int nTranCode , int nTxRxOption = FALSE); //ARPC
int		fnAPP_SetDataField_Item_4721(int nTranCode , int nTxRxOption = FALSE);
int		fnAPP_SetDataField_Item_4751(int nTranCode , int nTxRxOption = FALSE); 


#endif




























