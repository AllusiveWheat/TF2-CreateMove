#include "hooks.h"
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <Psapi.h>
#include <vector>
#include <fstream>
#include <sstream>



inline void**& getvtable(void* inst, size_t offset = 0)
{
    return *reinterpret_cast<void***>((size_t)inst + offset);
}
inline const void** getvtable(const void* inst, size_t offset = 0)
{
    return *reinterpret_cast<const void***>((size_t)inst + offset);
}
template< typename Fn >
inline Fn vFunc(const void* inst, size_t index, size_t offset = 0)
{
    return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
}