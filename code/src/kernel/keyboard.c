#include <keyboard.h>

int kb_init()
{
	//disable keyboard
	kb_ctrlr_send_cmd(KB_CMD_DISABLE);
	
	//flush output buffer
	kb_get_data();
	
	//set configuration byte
	kb_ctrlr_send_cmd(KB_CMD_CONFIG_READ);
	uint8_t config_byte = kb_get_data();
	config_byte &= 0xBC;					//clear bits 0, 1 and 6 (both port interrupts and first port translation
	kb_ctrlr_send_cmd(KB_CMD_CONFIG_WRITE);
	kb_send_data(config_byte);
	
	//keyboard controller self test
	kb_ctrlr_send_cmd(KB_CMD_SELF_TEST);
	if(kb_get_data() != 0x55)
		return -1;
	
	//interface test
	kb_ctrlr_send_cmd(KB_CMD_IFACE_TEST);
	if(kb_get_data() != 0x0)
		return -1;
	
	//enable keyboard
	kb_ctrlr_send_cmd(KB_CMD_ENABLE);
	
	//reset keyboard
	kb_send_data(KB_CMD_RESET);
	if(kb_get_data() != KB_RESP_ACK)
		return -1;
	if(kb_get_data() != KB_RESP_RESET_SUCCESS)
		return -1;
	
	return 0;
}
