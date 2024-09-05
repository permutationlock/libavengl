#ifndef AVEN_ARENA_H
#define AVEN_ARENA_H

#include "../aven.h"

#if __STDC_VERSION__ >= 201112L
    #include <stdalign.h>
    #define aven_arena_alignof(t) alignof(t)
#elif __STDC_VERSION__ >= 199901L
    #ifndef __BIGGEST_ALIGNMENT__
        #error "__BIGGEST_ALIGNMENT__ must be the max required alignment"
    #endif
    #define aven_arena_alignof(t) __BIGGEST_ALIGNMENT__
#else
    #error "C99 or later is required"
#endif

typedef struct {
    unsigned char *base;
    unsigned char *top;
} AvenArena;

static inline AvenArena aven_arena_init(void *mem, size_t size) {
    return (AvenArena){ .base = mem, .top = (unsigned char *)mem + size };
}


#if __has_attribute(malloc)
    __attribute__((malloc))
#endif
#if !defined(AVEN_ARENA_IMPLEMENTATION) and !defined(AVEN_HIMPLEMENTATION)
    // These attributes cause issues when compiling as one translation unit
    #if __has_attribute(alloc_size)
        __attribute__((alloc_size(2)))
    #endif
    #if __has_attribute(alloc_align)
        __attribute__((alloc_align(3)))
    #endif
#endif
AVEN_FN void *aven_arena_alloc(AvenArena *arena, size_t size, size_t align);

#define aven_arena_create(t, a) (t *)aven_arena_alloc( \
        a, \
        sizeof(t), \
        aven_arena_alignof(t) \
    )
#define aven_arena_create_array(t, a, n) (t *)aven_arena_alloc( \
        a, \
        n * sizeof(t), \
        aven_arena_alignof(t) \
    )

#ifdef AVEN_IMPLEMENTATION

AVEN_FN void *aven_arena_alloc(AvenArena *arena, size_t size, size_t align) {
    assert(
        align == 1 ||
        align == 2 ||
        align == 4 ||
        align == 8 ||
        align == 16 ||
        align == 32
    );
    unsigned char *mem = arena->top - size;
    size_t padding = (size_t)((uintptr_t)mem & (align - 1));
    assert((arena->top - arena->base) >= (ptrdiff_t)(size + padding));

    arena->top = mem - padding;
    return arena->top;
}

#endif // AVEN_IMPLEMENTATION

#endif // AVEN_ARENA_H
