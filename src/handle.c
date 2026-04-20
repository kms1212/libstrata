#include <strata/handle.h>

#include <strata/status.h>
#include <strata/uuid.h>
#include <strata/macros.h>

#include "handle_internal.h"

static struct krt_entries *__krt_entries;
static struct StHandleInner __handle_pool[1024];
static volatile int __handle_pool_lock = 0;


static void lock_handle_pool(void)
{
    while (__sync_lock_test_and_set(&__handle_pool_lock, 1)) {
        while (__handle_pool_lock) {
        }
    }
}

static void unlock_handle_pool(void)
{
    __sync_lock_release(&__handle_pool_lock);
}


static StStatus allocate_handle(StHandle *handle __out)
{
    size_t i;
    StHandle free_handle = NULL;

    lock_handle_pool();
    for (i = 0; i < ARRAY_SIZE(__handle_pool); ++i) {
        if (__handle_pool[i].in_use) continue;
        __handle_pool[i].in_use = 1;
        free_handle = &__handle_pool[i];
        break;
    }
    unlock_handle_pool();

    if (!free_handle) return STATUS_TOO_MANY_OPEN_FILES;
    if (handle) *handle = free_handle;
    return STATUS_SUCCESS;
}

static int lookup_query_cache(
    StHandle handle __in,
    const struct StUuid *if_uuid __in,
    uint32_t request_abiver __in,
    uint32_t *funcid_base __out,
    uint32_t *result_abiver __out,
    StStatus *status_out
)
{
    size_t i;

    for (i = 0; i < ARRAY_SIZE(handle->query_cache); ++i) {
        struct StHandleQueryCacheEntry *entry = &handle->query_cache[i];

        if (!entry->valid) continue;
        if (entry->request_abiver != request_abiver) continue;
        if (__builtin_memcmp(&entry->if_uuid, if_uuid, sizeof(*if_uuid)) != 0) continue;

        if (funcid_base) *funcid_base = entry->funcid_base;
        if (result_abiver) *result_abiver = entry->result_abiver;
        if (status_out) *status_out = entry->status;
        return 1;
    }

    return 0;
}

static void store_query_cache(
    StHandle handle __in,
    const struct StUuid *if_uuid __in,
    uint32_t request_abiver __in,
    uint32_t funcid_base __in,
    uint32_t result_abiver __in,
    StStatus status __in
)
{
    size_t slot = handle->next_query_cache_slot++ % ARRAY_SIZE(handle->query_cache);
    struct StHandleQueryCacheEntry *entry = &handle->query_cache[slot];

    entry->if_uuid = *if_uuid;
    entry->request_abiver = request_abiver;
    entry->funcid_base = funcid_base;
    entry->result_abiver = result_abiver;
    entry->status = status;
    entry->valid = 1;
}

static StStatus free_handle(StHandle handle)
{
    if (!handle) return STATUS_INVALID_HANDLE;

    lock_handle_pool();
    __builtin_memset(handle, 0, sizeof(*handle));
    unlock_handle_pool();

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
    if (!CHECK_SUCCESS(status)) {
        __krt_entries->node_close(handle_num);
        return status;
    }

    __builtin_memset(new_handle, 0, sizeof(*new_handle));
    new_handle->in_use = 1;
    new_handle->handle_num = handle_num;

    if (handle) *handle = new_handle;

    return STATUS_SUCCESS;
}

StStatus StHandle_Close(StHandle handle __in)
{
    StStatus status;

    if (!handle || !handle->in_use) return STATUS_INVALID_HANDLE;

    status = __krt_entries->node_close(handle->handle_num);
    free_handle(handle);
    return status;
}

StStatus StHandle_Query(
    StHandle handle __in,
    const struct StUuid *if_uuid __in,
    uint32_t request_abiver __in,
    uint32_t *funcid_base __out,
    uint32_t *result_abiver __out
)
{
    StStatus status;
    uint32_t cached_funcid_base = 0;
    uint32_t cached_result_abiver = 0;

    if (!handle || !handle->in_use) return STATUS_INVALID_HANDLE;

    if (lookup_query_cache(handle, if_uuid, request_abiver, &cached_funcid_base,
                           &cached_result_abiver, &status)) {
        if (funcid_base) *funcid_base = cached_funcid_base;
        if (result_abiver) *result_abiver = cached_result_abiver;
        return status;
    }

    status = __krt_entries->node_query(
        handle->handle_num,
        if_uuid,
        request_abiver,
        funcid_base,
        result_abiver
    );

    store_query_cache(
        handle,
        if_uuid,
        request_abiver,
        funcid_base ? *funcid_base : 0,
        result_abiver ? *result_abiver : 0,
        status
    );

    return status;
}

#ifdef __x86_64__
StStatus StHandle_Call0(
    StHandle handle __in,
    uint32_t funcid __in
)
{
    if (!handle || !handle->in_use) return STATUS_INVALID_HANDLE;
    return __krt_entries->node_call0(handle->handle_num, funcid);
}

StStatus StHandle_Call1(
    StHandle handle __in,
    uint32_t funcid __in,
    unsigned long arg0 __in
)
{
    if (!handle || !handle->in_use) return STATUS_INVALID_HANDLE;
    return __krt_entries->node_call1(handle->handle_num, funcid, arg0);
}

StStatus StHandle_Call2(
    StHandle handle __in,
    uint32_t funcid __in,
    unsigned long arg0 __in,
    unsigned long arg1 __in
)
{
    if (!handle || !handle->in_use) return STATUS_INVALID_HANDLE;
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
    if (!handle || !handle->in_use) return STATUS_INVALID_HANDLE;
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
    if (!handle || !handle->in_use) return STATUS_INVALID_HANDLE;
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
    if (!handle || !handle->in_use) return STATUS_INVALID_HANDLE;
    return __krt_entries->node_call_n(handle->handle_num, funcid, args, result, arg0, arg1);
}

#endif
