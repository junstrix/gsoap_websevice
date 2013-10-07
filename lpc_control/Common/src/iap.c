						   
/*********************************************************************************************************
  变量与宏定义
*********************************************************************************************************/

#define IAP_ENTER_ADR   0x1FFF1FF1                                      /* IAP入口地址定义              */

/* 
 *  定义IAP命令字
 */                                     
#define IAP_Prepare             50                                      /* 选择扇区                     */
                                                                        /* 【起始扇区号、结束扇区号】   */                    
#define IAP_RAMTOFLASH          51                                      /* 拷贝数据 FLASH目标地址       */
                                                                        /* RAM源地址    【、写入字节数  */
                                                                        /* 系统时钟频率】               */
#define IAP_ERASESECTOR         52                                      /* 擦除扇区    【起始扇区号     */
                                                                        /* 结束扇区号、系统时钟频率】   */
#define IAP_BLANKCHK            53                                      /* 查空扇区    【起始扇区号、   */
                                                                        /* 结束扇区号】                 */
#define IAP_READPARTID          54                                      /* 读器件ID    【无】           */
#define IAP_BOOTCODEID          55                                      /* 读Boot版本号【无】           */
#define IAP_COMPARE             56                                      /* 比较命令    【Flash起始地址  */
                                                                        /* RAM起始地址、需要比较的      */
                                                                        /* 字节数】                     */

/*
 *  定义IAP返回状态字
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
 *  定义CCLK值大小，单位为KHz 
 */
#define IAP_FCCLK            (25000)

/*
 *  定义函数指针  
 */
void (*IAP_Entry) (unsigned int ulParam_tab[], unsigned int ulPesult_tab[]);

unsigned int  GulParamin[8];                                                  /* IAP入口参数缓冲区            */
unsigned int  GulParamout[8];                                                 /* IAP出口参数缓冲区            */
/*********************************************************************************************************
** Function name:       sectorPrepare
** Descriptions:        IAP操作扇区选择，命令代码50
** input parameters:    ucSec1:           起始扇区
**                      ucSec2:           终止扇区
** output parameters:   GulParamout[0]:    IAP操作状态码,IAP返回值     
** Returned value:      GulParamout[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
unsigned int  sectorPrepare (unsigned char ucSec1, unsigned char ucSec2)
{  
    GulParamin[0] = IAP_Prepare;                                        /* 设置命令字                   */
    GulParamin[1] = ucSec1;                                             /* 设置参数                     */
    GulParamin[2] = ucSec2;                            
    (*IAP_Entry)(GulParamin, GulParamout);                              /* 调用IAP服务程序              */
   
    return (GulParamout[0]);                                            /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       ramCopy
** Descriptions:        复制RAM的数据到FLASH，命令代码51
** input parameters:    ulDst:             目标地址，即FLASH起始地址。以512字节为分界
**                      ulSrc:             源地址，即RAM地址。地址必须字对齐
**                      ulNo:              复制字节个数，为512/1024/4096/8192
** output parameters:   GulParamout[0]:    IAP操作状态码,IAP返回值     
** Returned value:      GulParamout[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
unsigned int  ramCopy (unsigned int ulDst, unsigned int ulSrc, unsigned int ulNo)
{  
    GulParamin[0] = IAP_RAMTOFLASH;                                     /* 设置命令字                   */
    GulParamin[1] = ulDst;                                              /* 设置参数                     */
    GulParamin[2] = ulSrc;
    GulParamin[3] = ulNo;
    GulParamin[4] = IAP_FCCLK;
    (*IAP_Entry)(GulParamin, GulParamout);                              /* 调用IAP服务程序              */
    
    return (GulParamout[0]);                                            /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       sectorErase
** Descriptions:        扇区擦除，命令代码52
** input parameters:    ucSec1             起始扇区
**                      ucSec2             终止扇区92
** output parameters:   GulParamout[0]:    IAP操作状态码,IAP返回值
** Returned value:      GulParamout[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
unsigned int  sectorErase (unsigned char ucSec1, unsigned char ucSec2)
{  
    GulParamin[0] = IAP_ERASESECTOR;                                    /* 设置命令字                   */
    GulParamin[1] = ucSec1;                                             /* 设置参数                     */
    GulParamin[2] = ucSec2;
    GulParamin[3] = IAP_FCCLK;
    (*IAP_Entry)(GulParamin, GulParamout);                              /* 调用IAP服务程序              */
   
    return (GulParamout[0]);                                            /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       blankChk
** Descriptions:        扇区查空，命令代码53
** input parameters:    ucSec1:              起始扇区
**                      ucSec2:              终止扇区92
** output parameters:   GulParamout[0]:    IAP操作状态码,IAP返回值
** Returned value:      GulParamout[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
unsigned int  blankChk (unsigned char ucSec1, unsigned char ucSec2)
{  
    GulParamin[0] = IAP_BLANKCHK;                                       /* 设置命令字                   */
    GulParamin[1] = ucSec1;                                             /* 设置参数                     */
    GulParamin[2] = ucSec2;
    (*IAP_Entry)(GulParamin, GulParamout);                              /* 调用IAP服务程序              */

    return (GulParamout[0]);                                            /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       parIdRead
** Descriptions:        read part id，命令代码54
** input parameters:    无
** output parameters:   GulParamout[0]:    IAP操作状态码,IAP返回值
** Returned value:      GulParamout[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
unsigned int  parIdRead (void)
{  
    GulParamin[0] = IAP_READPARTID;                                     /* 设置命令字                   */
    (*IAP_Entry)(GulParamin, GulParamout);                              /* 调用IAP服务程序              */

    return (GulParamout[0]);                                            /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       codeIdBoot
** Descriptions:        read boot code version，命令代码55
** input parameters:    无
** output parameters:   GulParamout[0]:    IAP操作状态码,IAP返回值
** Returned value:      GulParamout[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
unsigned int  codeIdBoot (void)
{  
    GulParamin[0] = IAP_BOOTCODEID;                                     /* 设置命令字                   */
    (*IAP_Entry)(GulParamin, GulParamout);                              /* 调用IAP服务程序              */

    return (GulParamout[0]);                                            /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       dataCompare
** Descriptions:        校验数据，命令代码56
** input parameters:    ulDst:             目标地址，即RAM/FLASH起始地址。地址必须字对齐
**                      ulSrc:             源地址，即FLASH/RAM地址。地址必须字对齐
**                      ulNo:              复制字节个数，必须能被4整除
** output parameters:   GulParamout[0]:    IAP操作状态码,IAP返回值
** Returned value:      GulParamout[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
unsigned int  dataCompare (unsigned int ulDst, unsigned int ulSrc, unsigned int ulNo)
{  
    GulParamin[0] = IAP_COMPARE;                                        /* 设置命令字                   */
    GulParamin[1] = ulDst;                                              /* 设置参数                     */
    GulParamin[2] = ulSrc;
    GulParamin[3] = ulNo;
    (*IAP_Entry)(GulParamin, GulParamout);                              /* 调用IAP服务程序              */

    return (GulParamout[0]);                                            /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       main
** Descriptions:        通过RTC中断将芯片从深度掉电中唤醒，深度掉电唤醒后复位。短接JP17，全速运行以后，
**                      蜂鸣器会响一声
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
#define IAP_SECTOR_NUMBER	7
#define IAP_ADDRESS			(0x1000*IAP_SECTOR_NUMBER)
int iap_entry (unsigned char *data,unsigned int length)
{   
    unsigned char ucErr;
	unsigned int i;
   	unsigned char ramData[256]; // 定义变量区 
	if(length > 256) length = 256;
	for(i = 0;i < length;i ++)
	{
		ramData[i]  = data[i];
	}
    IAP_Entry = (void(*)())IAP_ENTER_ADR;                               /* 初始化函数指针IAP_Entry      */

   // __disable_irq();                                                   /* 在IAP操作前必须关闭所有中断  */
    
    parIdRead();                                                        /* 读器件ID                     */
	codeIdBoot();                                                       /* 读Boot版本号                 */

    sectorPrepare(IAP_SECTOR_NUMBER,IAP_SECTOR_NUMBER);                                                 /* 准备扇区1                    */
    sectorErase(IAP_SECTOR_NUMBER,IAP_SECTOR_NUMBER);                                                   /* 擦除扇区1                    */
    blankChk(IAP_SECTOR_NUMBER,IAP_SECTOR_NUMBER) ;                                                     /* 查空扇区1                    */
   
    sectorPrepare(IAP_SECTOR_NUMBER,IAP_SECTOR_NUMBER);                                                 /* 选择扇区1                    */
    ramCopy(IAP_ADDRESS, (unsigned int)ramData, 256);                               /* 写数据到扇区1                */
	ucErr = dataCompare(IAP_ADDRESS, (unsigned int)ramData, 256);                   /* 比较数据                     */
   
   // __enable_irq();
   
    if (ucErr == CMD_SUCCESS) {                                         /* 成功 */    	

    }    
//    while (1);
}


/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
