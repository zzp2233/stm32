#ifndef __MENU_H__
#define __MENU_H__

typedef struct __menu_t
{
    uint8_t *item;				//��ǰ��ĿҪ��ʾ���ַ�
    void(*fun)(void *);			//ѡ��ĳһ�˵���ִ�еĹ��ܺ���
    struct __menu_t *same_left;	//��ǰ��Ŀ��ͬ�����˵�
    struct __menu_t *same_right;//��ǰ��Ŀ��ͬ���Ҳ�˵�	
    struct __menu_t *parent;	//��ǰ��Ŀ�ĸ��˵�	
    struct __menu_t *child;		//��ǰ��Ŀ���Ӳ˵�
}menu_t;


 typedef struct __menu_ext_t
{
	menu_t 	*menu;
	uint32_t item_cursor;
	uint32_t item_total;
	key_fn_t key_fn;	
}menu_ext_t;

extern menu_t  *menu_main_tbl[];
extern menu_t 	menu_main_1[];
extern menu_t 	menu_main_2[];
extern menu_t   menu_main_3[];


extern uint32_t menu_item_total(menu_t *menu);
extern void 	item_cursor_show(uint8_t x,uint8_t y,uint8_t *cursor);
extern void 	menu_show(menu_ext_t *menu_ext);






#endif
