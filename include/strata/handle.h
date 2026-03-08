#ifndef __STRATA_HANDLE_H__
#define __STRATA_HANDLE_H__

#include <stddef.h>

#include <strata/compiler.h>
#include <strata/status.h>
#include <strata/uuid.h>

#define STHANDLE_INVALID NULL

struct StHandleInner;
typedef struct StHandleInner *StHandle;

StStatus StHandle_Open(const uint8_t *path __in, uint32_t flags __in, StHandle *handle __out);
StStatus StHandle_Close(StHandle handle __in);
StStatus StHandle_Query(
    StHandle handle __in,
    const struct StUuid *if_uuid __in,
    uint32_t request_groupid __in,
    uint32_t request_abiver __in,
    uint32_t *funcid_base __out,
    uint32_t *result_abiver __out
);

#ifdef __x86_64__
StStatus StHandle_Call0(
    StHandle handle __in,
    uint32_t funcid __in
);
StStatus StHandle_Call1(
    StHandle handle __in,
    uint32_t funcid __in,
    unsigned long arg0 __in
);
StStatus StHandle_Call2(
    StHandle handle __in,
    uint32_t funcid __in,
    unsigned long arg0 __in,
    unsigned long arg1 __in
);
StStatus StHandle_Call3(
    StHandle handle __in,
    uint32_t funcid __in,
    unsigned long arg0 __in,
    unsigned long arg1 __in,
    unsigned long arg2 __in
);
StStatus StHandle_Call4(
    StHandle handle __in,
    uint32_t funcid __in,
    unsigned long arg0 __in,
    unsigned long arg1 __in,
    unsigned long arg2 __in,
    unsigned long arg3 __in
);
StStatus StHandle_CallN(
    StHandle handle __in,
    uint32_t funcid __in,
    const void *args __buf,
    void *result __buf,
    unsigned long arg0 __in,
    unsigned long arg1 __in
);
#endif

#endif  // __STRATA_HANDLE_H__
