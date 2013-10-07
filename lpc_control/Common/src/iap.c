						   
/*********************************************************************************************************
  ������궨��
*********************************************************************************************************/

#define IAP_ENTER_ADR   0x1FFF1FF1                                      /* IAP��ڵ�ַ����              */

/* 
 *  ����IAP������
 */                                     
#define IAP_Prepare             50                                      /* ѡ������                     */
                                                                        /* ����ʼ�����š����������š�   */                    
#define IAP_RAMTOFLASH          51                                      /* �������� FLASHĿ���ַ       */
                                                                        /* RAMԴ��ַ    ����д���ֽ���  */
                                                                        /* ϵͳʱ��Ƶ�ʡ�               */
#define IAP_ERASESECTOR         52                                      /* ��������    ����ʼ������     */
                                                                        /* ���������š�ϵͳʱ��Ƶ�ʡ�   */
#define IAP_BLANKCHK            53                                      /* �������    ����ʼ�����š�   */
                                                                        /* ���������š�                 */
#define IAP_READPARTID          54                                      /* ������ID    ���ޡ�           */
#define IAP_BOOTCODEID          55                                      /* ��Boot�汾�š��ޡ�           */
#define IAP_COMPARE             56                                      /* �Ƚ�����    ��Flash��ʼ��ַ  */
                                                                        /* RAM��ʼ��ַ����Ҫ�Ƚϵ�      */
                                                                        /* �ֽ�����                     */

/*
 *  ����IAP����״̬��
 */
#define CMD_SUCCESS                                0
#define INVALID_COMMAND                            1
#define SRC_ADDR_ERROR                             2 
#define DST_ADDR_ERROR                             3
#define SRC_ADDR_NOT_MAPPED                        4
#define DST_ADDR_NOT_MAPPED                        5
#define COUNT_ERROR                                6
#define INVALID_SECTOR                             7
#define SECTOR_NOT_BLANK                           8
#define SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION    9
#define COMPARE_ERROR                              10
#define BUSY                                       11


/* 
 *  ����CCLKֵ��С����λΪKHz 
 */
#define IAP_FCCLK            (25000)

/*
 *  ���庯��ָ��  
 */
void (*IAP_Entry) (unsigned int ulParam_tab[], unsigned int ulPesult_tab[]);

unsigned int  GulParamin[8];                                                  /* IAP��ڲ���������            */
unsigned int  GulParamout[8];                                                 /* IAP���ڲ���������            */
/*********************************************************************************************************
** Function name:       sectorPrepare
** Descriptions:        IAP��������ѡ���������50
** input parameters:    ucSec1:           ��ʼ����
**                      ucSec2:           ��ֹ����
** output parameters:   GulParamout[0]:    IAP����״̬��,IAP����ֵ     
** Returned value:      GulParamout[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
unsigned int  sectorPrepare (unsigned char ucSec1, unsigned char ucSec2)
{  
    GulParamin[0] = IAP_Prepare;                                        /* ����������                   */
    GulParamin[1] = ucSec1;                                             /* ���ò���                     */
    GulParamin[2] = ucSec2;                            
    (*IAP_Entry)(GulParamin, GulParamout);                              /* ����IAP�������              */
   
    return (GulParamout[0]);                                            /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       ramCopy
** Descriptions:        ����RAM�����ݵ�FLASH���������51
** input parameters:    ulDst:             Ŀ���ַ����FLASH��ʼ��ַ����512�ֽ�Ϊ�ֽ�
**                      ulSrc:             Դ��ַ����RAM��ַ����ַ�����ֶ���
**                      ulNo:              �����ֽڸ�����Ϊ512/1024/4096/8192
** output parameters:   GulParamout[0]:    IAP����״̬��,IAP����ֵ     
** Returned value:      GulParamout[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
unsigned int  ramCopy (unsigned int ulDst, unsigned int ulSrc, unsigned int ulNo)
{  
    GulParamin[0] = IAP_RAMTOFLASH;                                     /* ����������                   */
    GulParamin[1] = ulDst;                                              /* ���ò���                     */
    GulParamin[2] = ulSrc;
    GulParamin[3] = ulNo;
    GulParamin[4] = IAP_FCCLK;
    (*IAP_Entry)(GulParamin, GulParamout);                              /* ����IAP�������              */
    
    return (GulParamout[0]);                                            /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       sectorErase
** Descriptions:        �����������������52
** input parameters:    ucSec1             ��ʼ����
**                      ucSec2             ��ֹ����92
** output parameters:   GulParamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      GulParamout[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
unsigned int  sectorErase (unsigned char ucSec1, unsigned char ucSec2)
{  
    GulParamin[0] = IAP_ERASESECTOR;                                    /* ����������                   */
    GulParamin[1] = ucSec1;                                             /* ���ò���                     */
    GulParamin[2] = ucSec2;
    GulParamin[3] = IAP_FCCLK;
    (*IAP_Entry)(GulParamin, GulParamout);                              /* ����IAP�������              */
   
    return (GulParamout[0]);                                            /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       blankChk
** Descriptions:        ������գ��������53
** input parameters:    ucSec1:              ��ʼ����
**                      ucSec2:              ��ֹ����92
** output parameters:   GulParamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      GulParamout[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
unsigned int  blankChk (unsigned char ucSec1, unsigned char ucSec2)
{  
    GulParamin[0] = IAP_BLANKCHK;                                       /* ����������                   */
    GulParamin[1] = ucSec1;                                             /* ���ò���                     */
    GulParamin[2] = ucSec2;
    (*IAP_Entry)(GulParamin, GulParamout);                              /* ����IAP�������              */

    return (GulParamout[0]);                                            /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       parIdRead
** Descriptions:        read part id���������54
** input parameters:    ��
** output parameters:   GulParamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      GulParamout[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
unsigned int  parIdRead (void)
{  
    GulParamin[0] = IAP_READPARTID;                                     /* ����������                   */
    (*IAP_Entry)(GulParamin, GulParamout);                              /* ����IAP�������              */

    return (GulParamout[0]);                                            /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       codeIdBoot
** Descriptions:        read boot code version���������55
** input parameters:    ��
** output parameters:   GulParamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      GulParamout[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
unsigned int  codeIdBoot (void)
{  
    GulParamin[0] = IAP_BOOTCODEID;                                     /* ����������                   */
    (*IAP_Entry)(GulParamin, GulParamout);                              /* ����IAP�������              */

    return (GulParamout[0]);                                            /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       dataCompare
** Descriptions:        У�����ݣ��������56
** input parameters:    ulDst:             Ŀ���ַ����RAM/FLASH��ʼ��ַ����ַ�����ֶ���
**                      ulSrc:             Դ��ַ����FLASH/RAM��ַ����ַ�����ֶ���
**                      ulNo:              �����ֽڸ����������ܱ�4����
** output parameters:   GulParamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      GulParamout[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
unsigned int  dataCompare (unsigned int ulDst, unsigned int ulSrc, unsigned int ulNo)
{  
    GulParamin[0] = IAP_COMPARE;                                        /* ����������                   */
    GulParamin[1] = ulDst;                                              /* ���ò���                     */
    GulParamin[2] = ulSrc;
    GulParamin[3] = ulNo;
    (*IAP_Entry)(GulParamin, GulParamout);                              /* ����IAP�������              */

    return (GulParamout[0]);                                            /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       main
** Descriptions:        ͨ��RTC�жϽ�оƬ����ȵ����л��ѣ���ȵ��绽�Ѻ�λ���̽�JP17��ȫ�������Ժ�
**                      ����������һ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
#define IAP_SECTOR_NUMBER	7
#define IAP_ADDRESS			(0x1000*IAP_SECTOR_NUMBER)
int iap_entry (unsigned char *data,unsigned int length)
{   
    unsigned char ucErr;
	unsigned int i;
   	unsigned char ramData[256]; // ��������� 
	if(length > 256) length = 256;
	for(i = 0;i < length;i ++)
	{
		ramData[i]  = data[i];
	}
    IAP_Entry = (void(*)())IAP_ENTER_ADR;                               /* ��ʼ������ָ��IAP_Entry      */

   // __disable_irq();                                                   /* ��IAP����ǰ����ر������ж�  */
    
    parIdRead();                                                        /* ������ID                     */
	codeIdBoot();                                                       /* ��Boot�汾��                 */

    sectorPrepare(IAP_SECTOR_NUMBER,IAP_SECTOR_NUMBER);                                                 /* ׼������1                    */
    sectorErase(IAP_SECTOR_NUMBER,IAP_SECTOR_NUMBER);                                                   /* ��������1                    */
    blankChk(IAP_SECTOR_NUMBER,IAP_SECTOR_NUMBER) ;                                                     /* �������1                    */
   
    sectorPrepare(IAP_SECTOR_NUMBER,IAP_SECTOR_NUMBER);                                                 /* ѡ������1                    */
    ramCopy(IAP_ADDRESS, (unsigned int)ramData, 256);                               /* д���ݵ�����1                */
	ucErr = dataCompare(IAP_ADDRESS, (unsigned int)ramData, 256);                   /* �Ƚ�����                     */
   
   // __enable_irq();
   
    if (ucErr == CMD_SUCCESS) {                                         /* �ɹ� */    	

    }    
//    while (1);
}


/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
