#ifndef _RUNNING_STR_H_
#define _RUNNING_STR_H_

#include <stdint.h>

#define RS_DISPLAY_WIDTH 8
#define RS_DISPLAY_HEIGHT 8

#define RS_DISPLAY_PADDING_VERTICAL 1

int   RS_StringSet(char *str);
void *RS_StringProc();
void  RS_StringStop();




#endif /* _RUNNING_STR_H_ */
