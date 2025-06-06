#ifndef __BLE_H__
#define __BLE_H__


#define BLE_DISCONNECT	0
#define BLE_CONNECT		1

extern void 	ble_init(uint32_t baud);
extern uint32_t ble_connect_sta_get(void);

#endif


