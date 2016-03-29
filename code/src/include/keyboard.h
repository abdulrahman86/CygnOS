#ifndef	__KEYBOARD_HEADER
#define	__KEYBOARD_HEADER

#include <io.h>
#include <stdint.h>

#define	PORT_KB_DATA		    	0x60
#define	PORT_KB_CTRLR_STATUS	    0x64
#define	PORT_KB_CTRLR_CMD	    	0x64

#define	KB_CTRLR_INPUTBUF_FULL	    0x02
#define	KB_CTRLR_OUTPUTBUF_FULL	    0x01

#define	KB_CMD_CONFIG_READ			0x20
#define	KB_CMD_CONFIG_WRITE			0x60
#define	KB_CMD_DISABLE				0xAD
#define	KB_CMD_SELF_TEST			0xAA
#define	KB_CMD_IFACE_TEST			0xAB
#define	KB_CMD_ENABLE				0xAE
#define	KB_CMD_RESET				0xFF

#define	KB_RESP_ACK					0xFA
#define	KB_RESP_RESET_SUCCESS		0xAA

static uint8_t kb_ctrlr_get_status()
{
    return inb(PORT_KB_CTRLR_STATUS);
}

static void kb_ctrlr_send_cmd(uint8_t __cmd)
{
    while((kb_ctrlr_get_status() & KB_CTRLR_INPUTBUF_FULL) != 0);

    outb(PORT_KB_CTRLR_CMD, __cmd);
}

static uint8_t kb_get_data()
{
    while((kb_ctrlr_get_status() & KB_CTRLR_OUTPUTBUF_FULL) == 0);

    return inb(PORT_KB_DATA);
}

static void kb_send_data(uint8_t __data)
{
    while((kb_ctrlr_get_status() & KB_CTRLR_INPUTBUF_FULL) != 0);

    outb(PORT_KB_DATA, __data);
}

//make the keyboard run the self test and return 0 is success, -1 if not.
extern int kb_init();

#endif
