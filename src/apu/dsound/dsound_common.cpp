/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 12/01/2023 10:53
 */

#include "dsound_common.hpp"

const char* DsoundErrorString(HRESULT Result)
{
    if (Result == DSERR_ALLOCATED)
        return "DSERR_ALLOCATED";
    if (Result == DSERR_BADFORMAT)
        return "DSERR_BADFORMAT";
    if (Result == DSERR_BUFFERTOOSMALL)
        return "DSERR_BUFFERTOOSMALL";
    if (Result == DSERR_CONTROLUNAVAIL)
        return "DSERR_CONTROLUNAVAIL";
    if (Result == DSERR_DS8_REQUIRED)
        return "DSERR_DS8_REQUIRED";
    if (Result == DSERR_INVALIDCALL)
        return "DSERR_INVALIDCALL";
    if (Result == DSERR_INVALIDPARAM)
        return "DSERR_INVALIDPARAM";
    if (Result == DSERR_NOAGGREGATION)
        return "DSERR_NOAGGREGATION";
    if (Result == DSERR_OUTOFMEMORY)
        return "DSERR_OUTOFMEMORY";
    if (Result == DSERR_UNINITIALIZED)
        return "DSERR_UNINITIALIZED";
    if (Result == DSERR_UNSUPPORTED)
        return "DSERR_UNSUPPORTED";
    if (Result == DSERR_BUFFERLOST)
        return "DSERR_BUFFERLOST";
    if (Result == DSERR_PRIOLEVELNEEDED)
        return "DSERR_PRIOLEVELNEEDED";
    
    return "DSERR_UNKNOWN";
}
