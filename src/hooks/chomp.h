#ifndef _SCHAUFEL_HOOK_CHOMP_H_
#define _SCHAUFEL_HOOK_CHOMP_H_

#include "hooks.h"
#include "utils/config.h"

bool    h_chomp(Context ctx, Message msg);
Context h_chomp_init(config_setting_t *config);
bool    h_chomp_validate(config_setting_t *config);
void    h_chomp_free(Context ctx);

#endif
