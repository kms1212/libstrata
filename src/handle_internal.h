#ifndef __HANDLE_INTERNAL_H__
#define __HANDLE_INTERNAL_H__

#include <stdint.h>

#include <strata/handle.h>

struct krt_entries {
    StStatus (*node_open)(const uint8_t *path __in, uint32_t flags __in, uint32_t *handle_num __out);
    StStatus (*node_close)(uint32_t handle_num __in);
    StStatus (*node_query)(
        uint32_t handle_num __in,
        const struct StUuid *if_uuid __in,
        uint32_t request_groupid __in,
        uint32_t request_abiver __in,
        uint32_t *funcid_base __out,
        uint32_t *result_abiver __out
    );
    StStatus (*node_call0)(
        uint32_t handle_num __in,
        uint32_t funcid __in
    );
    StStatus (*node_call1)(
        uint32_t handle_num __in,
        uint32_t funcid __in,
        unsigned long arg0 __in
    );
    StStatus (*node_call2)(
        uint32_t handle_num __in,
        uint32_t funcid __in,
        unsigned long arg0 __in,
        unsigned long arg1 __in
    );
    StStatus (*node_call3)(
        uint32_t handle_num __in,
        uint32_t funcid __in,
        unsigned long arg0 __in,
        unsigned long arg1 __in,
        unsigned long arg2 __in
    );
    StStatus (*node_call4)(
        uint32_t handle_num __in,
        uint32_t funcid __in,
        unsigned long arg0 __in,
        unsigned long arg1 __in,
        unsigned long arg2 __in,
        unsigned long arg3 __in
    );
    StStatus (*node_call_n)(
        uint32_t handle_num __in,
        uint32_t funcid __in,
        const void *args __in,
        void *result __buf,
        unsigned long arg0 __in,
        unsigned long arg1 __in
    );
};

struct StHandleInner {
    uint32_t handle_num;
};

#endif  // __HANDLE_INTERNAL_H__
