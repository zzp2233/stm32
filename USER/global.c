#include "includes.h"

/* ������ */
void xor_encryption(uint8_t key,char *src,char *dest,uint32_t len)
{
	char *psrc=src;
	char *pdest=dest;

	while(len--)
	{
		*pdest = *psrc ^ key;
		psrc++;
		pdest++;
	}
}

/* ������ */
void xor_decryption(uint8_t key,char *src,char *dest,uint32_t len)
{

	char *psrc=src;
	char *pdest=dest;

	while(len--)
	{
		*pdest = *psrc ^ key;
		psrc++;
		pdest++;
	}

}

/* ��׽��������쳣�жϣ���ӡ��Ӧ���жϺ� */
void exceptions_catch(void)
{
	uint32_t icsr=SCB->ICSR;
	uint32_t ipsr=__get_IPSR();
	
	
	printf("exceptions catch:\r\n");
	printf("IPSR=0x%08X\r\n",ipsr);	
	printf("ICSR=0x%08X\r\n",icsr);

	taskDISABLE_INTERRUPTS();
}


