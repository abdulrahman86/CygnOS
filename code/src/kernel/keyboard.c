#include <keyboard.h>

int kb_self_test()
{
    kb_ctrlr_send_cmd(0xAA);

    if(kb_get_data() == 0x55)
    {
	kb_ctrlr_send_cmd(0xAB);

	if(kb_get_data() == 0x0)
	    return 0;
	else
	    return -1;
    }
    else
	return -1;
}

void kb_enable()
{
    kb_ctrlr_send_cmd(0xAD);
}

void kb_disable()
{
    kb_ctrlr_send_cmd(0xAE);
}
