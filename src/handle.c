#include <strata/handle.h>

#include <strata/status.h>
#include <strata/uuid.h>
#include <strata/macros.h>

#include "handle_internal.h"

static struct krt_entries *__krt_entries;
static struct StHandleInner __early_handle_buf[32];
static int __early_handle_buf_use_count = 0;


static StStatus allocate_handle(StHandle *handle __out)
{
    if (__early_handle_buf_use_count >= ARRAY_SIZE(__early_handle_buf)) return STATUS_NOT_IMPLEMENTED;

    if (handle) *handle = &__early_handle_buf[__early_handle_buf_use_count++];

    return STATUS_SUCCESS;
}

static StStatus free_handle(StHandle handle)
{
    return STATUS_SUCCESS;
}

StStatus __StHandle_Init(struct krt_entries *krt_entries __in)
{
    __krt_entries = krt_entries;

    return STATUS_SUCCESS;
}

StStatus StHandle_Open(const uint8_t *path __in, uint32_t flags __in, StHandle *handle __out)
{
    StStatus status;
    uint32_t handle_num;
    StHandle new_handle;

    status = __krt_entries->node_open(path, flags, &handle_num);
    if (!CHECK_SUCCESS(status)) return status;

    status = allocate_handle(&new_handle);
    if (!CHECK_SUCCESS(status)) return status;

    if (handle) *handle = new_handle;

    return STATUS_SUCCESS;
}

StStatus StHandle_Close(StHandle handle __in)
{
    return __krt_entries->node_close(handle->handle_num);
}

StStatus StHandle_Query(
    StHandle handle __in,
    struct StUuid *if_uuid __in,
    uint32_t request_groupid __in,
    uint32_t request_abiver __in,
    uint32_t *funcid_base __out,
    uint32_t *result_abiver __out
)
{
    return __krt_entries->node_query(
        handle->handle_num,
        if_uuid,
        request_groupid,
        request_abiver,
        funcid_base,
        result_abiver
    );
}

#ifdef __x86_64__
StStatus StHandle_Call0(
    StHandle handle __in,
    uint32_t funcid __in
)
{
    return __krt_entries->node_call0(handle->handle_num, funcid);
}

StStatus StHandle_Call1(
    StHandle handle __in,
    uint32_t funcid __in,
    unsigned long arg0 __in
)
{
    return __krt_entries->node_call1(handle->handle_num, funcid, arg0);
}

StStatus StHandle_Call2(
    StHandle handle __in,
    uint32_t funcid __in,
    unsigned long arg0 __in,
    unsigned long arg1 __in
)
{
    return __krt_entries->node_call2(handle->handle_num, funcid, arg0, arg1);
}

StStatus StHandle_Call3(
    StHandle handle __in,
    uint32_t funcid __in,
    unsigned long arg0 __in,
    unsigned long arg1 __in,
    unsigned long arg2 __in
)
{
    return __krt_entries->node_call3(handle->handle_num, funcid, arg0, arg1, arg2);
}

StStatus StHandle_Call4(
    StHandle handle __in,
    uint32_t funcid __in,
    unsigned long arg0 __in,
    unsigned long arg1 __in,
    unsigned long arg2 __in,
    unsigned long arg3 __in
)
{
    return __krt_entries->node_call4(handle->handle_num, funcid, arg0, arg1, arg2, arg3);
}

StStatus StHandle_CallN(
    StHandle handle __in,
    uint32_t funcid __in,
    const void *args __buf,
    void *result __buf,
    unsigned long arg0 __in,
    unsigned long arg1 __in
)
{
    return __krt_entries->node_call_n(handle->handle_num, funcid, args, result, arg0, arg1);
}

#endif
