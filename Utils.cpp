#include "Utils.h"
VOID huge* operator new(IN unsigned long int size) noexcept
{
    if (!size)
    {
        return nullptr;
    }
    else
    {
        VOID *RetVal;
        if (::gBS->AllocatePool(EfiLoaderData, static_cast<UINTN>(size), &RetVal))
            RetVal = nullptr;
        return RetVal;
    }
}
VOID huge* operator new[](IN unsigned long int cb) noexcept
{
    return operator new(cb);
}
VOID operator delete(IN VOID *address) noexcept
{
    if (address)
    {
        gBS->FreePool(address);
    }
}
VOID operator delete[](IN VOID *address) noexcept
{
    operator delete(address);
}