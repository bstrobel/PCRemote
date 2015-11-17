/*
 * MCEMapper.h
 *
 * Created: 17.11.2015 08:02:34
 *  Author: Bernd
 */ 


#ifndef MCEMAPPER_H_
#define MCEMAPPER_H_

typedef enum { MAKE, BREAK } MK_OR_BK;

void send_kbcode_for_ir(unsigned long ircode, MK_OR_BK mk_or_bk);

#endif /* MCEMAPPER_H_ */