#ifndef _DBGINFO_H
#define _DBGINFO_H
#include <stdint.h>

void dbginfo_load(const char *dbgpak);
const char *dbginfo_resolve(uint32_t checksum);
#endif //_DBGINFO_H