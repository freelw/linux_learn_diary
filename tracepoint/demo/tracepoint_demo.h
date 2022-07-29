#undef TRACE_SYSTEM
#define TRACE_SYSTEM wangli

#if !defined(_TRACE_WANGLI_TEST_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_WANGLI_TEST_H

#include <linux/tracepoint.h>

TRACE_EVENT(
    wangli_test,
    TP_PROTO(int num),
    TP_ARGS(num),
    TP_STRUCT__entry(
        __field(int, count)
    ),
    TP_fast_assign(
        __entry->count = num;
    ),
    TP_printk("count=%d", __entry->count)
);
#endif

/* This part must be outside protection */

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#define TRACE_INCLUDE_FILE tracepoint_demo
#include <trace/define_trace.h>