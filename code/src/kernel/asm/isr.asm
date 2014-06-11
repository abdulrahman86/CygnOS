EXTERN	default_isr_handler

__ISR_IRQ0:

__ISR_DEFAULT:
	cli
	call default_isr_handler
	hlt
	
