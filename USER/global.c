#include "includes.h"

/* 异或加密 */
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

/* 异或解密 */
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

/* 捕捉有问题的异常中断，打印相应的中断号 */
void exceptions_catch(void)
{
	uint32_t icsr=SCB->ICSR;
	uint32_t ipsr=__get_IPSR();
	
	
	printf("exceptions catch:\r\n");
	printf("IPSR=0x%08X\r\n",ipsr);	
	printf("ICSR=0x%08X\r\n",icsr);

	taskDISABLE_INTERRUPTS();
}


