#ifndef __STRATA_KRT_H__
#define __STRATA_KRT_H__

#include <strata/status.h>
#include <strata/handle.h>
#include <strata/compiler.h>
#include <strata/uuid.h>

StStatus StKrt_Open(const char *path __in, uint32_t flags __in, StHandle *handle __out);
StStatus StKrt_Close(StHandle handle __in);
StStatus StKrt_Query(StHandle handle __in, struct StUuid *if_uuid __in, uint32_t request_groupid __in, uint32_t request_abiver __in, uint32_t *funcid_base __out, uint32_t *result_abiver __out);
StStatus StKrt_Call(StHandle handle __in, uint32_t funcid __in, const void *args __buf, size_t args_size __in, void *result __buf, size_t result_size __in);

#endif // __STRATA_KRT_H__
