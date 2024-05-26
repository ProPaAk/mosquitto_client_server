#ifndef HELPER_FUNCS_H
#define HELPER_FUNCS_H

#include <include/mosquitto.h>

namespace helpers {

    void CleanUp(mosquitto *mosq) {
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
    }

    void CleanUp() {
        mosquitto_lib_cleanup();
    }

} // namespace helpers {
#endif // HELPER_FUNCS_H
