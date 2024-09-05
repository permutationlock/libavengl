#ifndef AVEN_H
#define AVEN_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Inspired by and/or copied from Chris Wellons (https://nullprogram.com)

#define or ||
#define and &&

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#ifndef __has_attribute
    #define __has_attribute(unused) 0
#endif

#ifndef __has_builtin
    #define __has_builtin(unused) 0
#endif

#if defined(AVEN_UNREACHABLE_ASSERT) and __has_builtin(__builtin_unreachable)
    #ifndef NDEBUG
        #define assert(c) while (!(c)) { __builtin_unreachable(); }
    #else
        #define assert(unused) ((void)0)
    #endif
#else
    #include <assert.h>
#endif

#define countof(array) (sizeof(array) / sizeof(*array))

#define Optional(t) struct { t value; bool valid; }
#define Result(t) struct { t payload; int error; }
#define Slice(t) struct { t *ptr; size_t len; }
#define List(t, n) struct { size_t len; t array[n]; }

typedef Slice(unsigned char) ByteSlice;

#ifndef NDEBUG
    static inline size_t aven_assert_lt_internal_fn(size_t a, size_t b) {
        assert(a < b);
        return a;
    }

    #define aven_assert_lt_internal(a, b) aven_assert_lt_internal_fn(a, b)
#else
    #define aven_assert_lt_internal(a, b) a
#endif

#define slice_get(s, i) s.ptr[aven_assert_lt_internal(i, s.len)]
#define list_get(l, i) l.array[aven_assert_lt_internal(i, l.len)]

#define as_bytes(ptr) (ByteSlice){ \
        .ptr = (unsigned char *)ptr, \
        .len = sizeof(*ptr) \
    }
#define array_as_bytes(ptr) (ByteSlice){ \
        .ptr = (unsigned char *)ptr, \
        .len = sizeof(ptr)\
    }
#define slice_as_bytes(s) (ByteSlice){ \
        .ptr = (unsigned char *)s.ptr, \
        .len = s.len * sizeof(*s.ptr), \
    }

#if defined(_WIN32) and defined(_MSC_VER)
    void *memcpy(void *s1, const void *s2, size_t n);
#else
    void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
#endif

#define slice_copy(d, s) memcpy( \
        d.ptr, \
        s.ptr, \
        aven_assert_lt_internal( \
            s.len * sizeof(*s.ptr), \
            d.len * sizeof(*d.ptr) + 1 \
        ) \
    )

#if defined(AVEN_IMPLEMENTATION) and !defined(AVEN_IMPLEMENTATION_STU)
    #define AVEN_FN static inline
#else
    #define AVEN_FN
#endif

#ifdef _WIN32
    #define AVEN_WIN32_FN(t) __declspec(dllimport) t __stdcall
#endif

#endif // AVEN_H
