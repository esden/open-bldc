#ifndef __SPINUP_H__
#define __SPINUP_H__

#include "control_process.h"

void spinup_reset(void); 
enum control_process_cb_state control_process_spinup_cb(struct control_process * cps);

#endif
