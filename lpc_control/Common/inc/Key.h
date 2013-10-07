/*******************************Copyright (c)**********************************
**--------------�ļ���Ϣ-------------------------------------------------------
** ��   ��   ��: key.h
** ��        ��: ���������������ͷ�ļ�.
**
-------------------------------------------------------------------------------
** �޸���: ��ȫ��
** ��  ��: 0.0.2
** �ա���: 2010.09.30
******************************************************************************/

#define KeyBufSize		0x20
#define KEY_NONE		0xFF

enum {
KEY_PROCESS_PRESS_DETECT,
KEY_PROCESS_PRESS_CONFIRM,
KEY_PROCESS_PRESS_SURE,
KEY_PROCESS_RELEASE_CONFIRM,
KEY_PROCESS_RELEASE_SURE
};

unsigned char display(int);
void KeyInit(void);
unsigned char ReadKeyValue(void);
void KeyScan(void);
void KeyValuePush(unsigned char keyValue);
unsigned char KeyValuePop(void);
