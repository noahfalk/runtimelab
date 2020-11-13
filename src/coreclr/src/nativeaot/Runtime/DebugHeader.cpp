//
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
#include "common.h"
#include "CommonTypes.h"
#include "CommonMacros.h"
#include "daccess.h"
#include "PalRedhawkCommon.h"
#include "PalRedhawk.h"


struct NetNativeRuntimeDebugHeader
{
    // The cookie serves as a sanity check against process corruption or being requested
    // to treat some other non-.Net module as though it did contain the coreRT runtime.
    // It can also be changed if we want to make a breaking change so drastic that
    // earlier debuggers should treat the module as if it had no .Net runtime at all.
    // If the cookie is valid a debugger is safe to assume the Major/Minor version fields
    // will follow, but any contents beyond that depends on the version values.
    // The cookie value is currently set to 0x6e, 0x66, 0x31, 0x36 (NF16 in ascii)
    UInt8 Cookie[4];

    // This counter can be incremented to indicate breaking changes
    // This field must be encoded little endian, regardless of the typical endianess of
    // the machine
    UInt16 MajorVersion;

    // This counter can be incremented to indicate back-compatible changes
    // This field must be encoded little endian, regardless of the typical endianess of
    // the machine
    UInt16 MinorVersion;

    // These flags must be encoded little endian, regardless of the typical endianess of
    // the machine. Ie Bit 0 is the least significant bit of the first byte.
    // Bit 0 - Set if the pointer size is 8 bytes, otherwise pointer size is 4 bytes
    // Bit 1 - Set if the machine is big endian
    // The high 30 bits are reserved. Changes to these bits will be considered a
    // back-compatible change.
    UInt32 Flags;

    // Reserved - Currently it only serves as alignment padding for the pointers which 
    // follow but future usage will be considered a back-compatible change.
    UInt32 ReservedPadding;

};

extern "C"
const NetNativeRuntimeDebugHeader g_NetNativeRuntimeDebugHeader =
{
    { 0x6e, 0x66, 0x31, 0x36 },          //Cookie
    1,                                   //MajorVersion
    0,                                   //MinorVersion
    (sizeof(void*) == 8 ? 0x1 : 0x0) |   //Flags - PointerSize
    0x0,                                 //Flags - Endianess
    0,                                   //ReservedPadding
};
