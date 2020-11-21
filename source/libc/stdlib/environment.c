#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *getenv(const char *name) {
    size_t name_len = strlen(name);
	for (size_t i = 0; environ[i] != NULL; i++) {
        if (strncmp(name, environ[i], name_len) == 0) {
            // Assume vars have the format VAR=VALUE.
            return environ[i] + name_len + 1;
        }
	}

    return NULL;
}
