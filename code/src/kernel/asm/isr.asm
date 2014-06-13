__ISR_IRQ0:

GLOBAL __ISR_DEFAULT
EXTERN	default_isr_handler

__ISR_DEFAULT:
	cli
	call default_isr_handler
	hlt
