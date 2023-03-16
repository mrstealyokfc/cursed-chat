#ifndef __SETTINGS_H
#define __SETTINGS_H

/*
**
** Edit config.h to change settings.
*/

#include "config.h"

#ifndef PORT
#define PORT 1982
#endif

#ifndef MESSAGE_LENGTH
#define MESSAGE_LENGTH 512
#endif

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 32
#endif

#ifndef SERVER_CLOSE_MESSAGE
#define SERVER_CLOSE_MESSAGE "Default Server Close Message\n"
#endif

#ifndef SERVER_FULL_MESSAGE
#define SERVER_FULL_MESSAGE "Default Server Full Message\n"
#endif

#ifndef DEFAULT_USERNAME
#define DEFAULT_USERNAME "DEFAULT"
#endif

#endif // __SETTINGS_H
