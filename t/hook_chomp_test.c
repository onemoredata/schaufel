#include "schaufel.h"
#include "test/test.h"
#include "hooks.h"
#include "queue.h"
#include <string.h>

int main(void)
{
    int res = 0;
    config_t root;
    bool rv = true;
    size_t len = 0;
    config_setting_t *hook_conf = NULL;
    config_init(&root);
    res = config_read_string(&root, "hooks = ({ type = \"chomp\"; });");
    pretty_assert(res == CONFIG_TRUE);
    hook_conf = config_lookup(&root,"hooks");

    // test finding a hook
    hooks_register();
    pretty_assert(hooks_validate(hook_conf) == true);

    Hooklist test = hook_init();
    hooks_add(test,hook_conf);

    Message msg = message_init();
    message_set_data(msg,strdup("hurz\n"));
    message_set_len(msg,5);
    pretty_assert((rv = hooklist_run(test,msg)) == true);
    pretty_assert((res = strcmp(message_get_data(msg),"hurz")) == 0);
    pretty_assert((len = message_get_len(msg)) == 4);
    res |= (len != 4) || !rv;

    message_set_data(msg,strdup("hurz\n\n\v\r\n"));
    message_set_len(msg,9);
    pretty_assert((rv = hooklist_run(test,msg)) == true);
    pretty_assert((res |= strcmp(message_get_data(msg),"hurz")) == 0);
    pretty_assert((len = message_get_len(msg)) == 4);
    res |= (len != 4) | !rv;

    message_set_data(msg,strdup(""));
    message_set_len(msg,0);
    pretty_assert((rv = hooklist_run(test,msg)) == true);
    pretty_assert((res |= strcmp(message_get_data(msg),"")) == 0);
    pretty_assert((len = message_get_len(msg)) == 0);
    res |= (len != 0) || !rv;

    message_set_data(msg,strdup("       "));
    message_set_len(msg,7);
    pretty_assert((rv = hooklist_run(test,msg)) == true);
    pretty_assert((res |= strcmp(message_get_data(msg),"")) == 0);
    pretty_assert((len = message_get_len(msg)) == 0);
    res |= (len != 0) || !rv;

    free(msg);
    hook_free(test);
    hooks_deregister();

    config_destroy(&root);

    return res != 0;
}
