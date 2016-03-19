#ifndef	__KEYBOARD_HEADER
#define	__KEYBOARD_HEADER

#include <io.h>
#include <stdint.h>

#define	PORT_KB_ENC_INPUTBUF	    0x60
#define	PORT_KB_ENC_OUTPUTCMD	    0x60
#define	PORT_KB_CTRLR_INPUTSTATUS   0x64
#define	PORT_KB_CTRLR_OUTPUTCMD	    0x64

#define	KB_CTRLR_INPUTBUF_FULL	    0x02
#define	KB_CTRLR_OUTPUTBUF_FULL	    0x01

uint8_t kb_ctrlr_get_status()
{
    return inb(PORT_KB_CTRLR_INPUTSTATUS);
}

void kb_ctrlr_set_cmd(uint8_t __cmd)
{
    while((kb_ctrlr_get_status() & KB_CTRLR_INPUTBUF_FULL) != 0);

    outb(PORT_KB_CTRLR_OUTPUTCMD, __cmd);
}

uint8_t kb_enc_get_buf()
{
    return inb(PORT_KB_ENC_INPUTBUF);
}

void kb_enc_set_cmd(uint8_t __cmd)
{
    while((kb_enc_get_buf() & KB_CTRLR_INPUTNUF_FULL) != 0);

    outb(PORT_KB_ENC_OUTPUTCMD, __cmd);
}

#endif
