#include "utils/config.h"
#include "utils/scalloc.h"
#include "utils/helper.h"
#include "queue.h"
// This header contains the public api used to initialise this hook
#include "hooks/chomp.h"


// h_chomp removes trailing field separators (see perldoc chomp)
bool h_chomp(UNUSED Context ctx, Message msg)
{
    // TODO: Consider marking messages as strings/blobs.
    // TODO: this is utf-8/ASCII. Consider storing encodings.
    size_t len = message_get_len(msg);
    char *data = message_get_data(msg);

    // all ascii chars < 16 are control characters
    while (len > 0 && ((data[len-1] | 0xf) == 0xf || data[len-1] == ' '))
        len--;

    data[len] = '\0';
    message_set_len(msg, len);

    // Technically message data is a pointer, so we don't have to set it,
    // but if we ever not zero-copy, this would be correct.
    message_set_data(msg, data);

    end:
    return true;
}

Context h_chomp_init(UNUSED config_setting_t *config)
{
    Context ctx = SCALLOC(0,sizeof(*ctx));
    return ctx;
}

// chomp takes no configurables
bool h_chomp_validate(UNUSED config_setting_t *config)
{
    return true;
}

void h_chomp_free(Context ctx)
{
    free(ctx);
    return;
}

