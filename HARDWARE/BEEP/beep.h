#ifndef __BEEP_H__
#define __BEEP_H__

extern void beep_init(void);//��ʼ��

#define BEEP(x)		PFout(8)=(x)


#endif
