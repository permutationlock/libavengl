#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>

#include "config.h"

#define AVEN_IMPLEMENTATION

#include "libaven/include/aven.h"
#include "libaven/include/aven/arena.h"
#include "libaven/include/aven/arg.h"
#include "libaven/include/aven/build.h"
#include "libaven/include/aven/build/common.h"
#include "libaven/include/aven/path.h"

#include "build.h"
#include "libaven/build.h"

AvenArg custom_arg_data[] = {
    {
        .name = "-no-glfw",
        .description = "Don't build GLFW locally",
        .type = AVEN_ARG_TYPE_BOOL,
#if defined(BUILD_DEFAULT_NOGLFW)
        .value = {
            .type = AVEN_ARG_TYPE_BOOL,
            .data = { .arg_bool = BUILD_DEFAULT_NOGLFW },
        },
#endif
    },
    {
        .name = "-syslibs",
        .description = "System libraries to link",
        .type = AVEN_ARG_TYPE_STRING,
        .value = {
            .type = AVEN_ARG_TYPE_STRING,
#if defined(BUILD_DEFAULT_SYSLIBS)
            .data = { .arg_str = BUILD_DEFAULT_SYSLIBS },
#elif defined(_WIN32)
    #if defined(_MSC_VER) and !defined(__clang__)
            .data = { .arg_str = "kernel32.lib user32.lib gdi32.lib shell32.lib" },
    #else
            .data = { .arg_str = "kernel32 user32 gdi32 shell32" },
    #endif
#else
            .data = { .arg_str = "m dl" },
#endif
        },
    },
};
AvenArgSlice custom_args = {
    .ptr = custom_arg_data,
    .len = countof(custom_arg_data),
};

 
#define ARENA_SIZE (4096 * 2000)

int main(int argc, char **argv) {
    void *mem = malloc(ARENA_SIZE);
    assert(mem != NULL);

    AvenArena arena = aven_arena_init(mem, ARENA_SIZE);

    AvenArgSlice args = {
        .len = custom_args.len +
            aven_build_common_args.len +
            libavengl_build_args.len,
    };
    args.ptr = aven_arena_create_array(AvenArg, &arena, args.len);
    size_t args_end = 0;
    memcpy(
        args.ptr + args_end,
        custom_args.ptr,
        custom_args.len * sizeof(*custom_args.ptr)
    );
    args_end += custom_args.len;
    memcpy(
        args.ptr + args_end,
        aven_build_common_args.ptr,
        aven_build_common_args.len * sizeof(*aven_build_common_args.ptr)
    );
    args_end += aven_build_common_args.len;
    memcpy(
        args.ptr + args_end,
        libavengl_build_args.ptr,
        libavengl_build_args.len * sizeof(*libavengl_build_args.ptr)
    );
    args_end += libavengl_build_args.len;

#if defined(_WIN32) and \
    !defined(_MSC_VER) and \
    defined(__GNUC__) and \
    !defined(__clang__)
    {
        AvenStr ldflags = aven_str_cstr(aven_arg_get_str(args, "-ldflags"));
        AvenStr newldflags = aven_str_concat(ldflags, aven_str(" -mwindows"), &arena);
        aven_arg_set_str(args, "-ldflags", newldflags.ptr);
    }
#endif

    int error = aven_arg_parse(
        args,
        argv,
        argc,
        aven_build_common_overview.ptr,
        aven_build_common_usage.ptr
    );
    if (error != 0) {
        if (error != AVEN_ARG_ERROR_HELP) {
            fprintf(stderr, "ARG PARSE ERROR: %d\n", error);
            return error;
        }
        return 0;
    }

    AvenBuildCommonOpts opts = aven_build_common_opts(args, &arena);
    LibAvenGLBuildOpts agl_opts = libavengl_build_opts(args, &arena);
    bool opt_no_glfw = aven_arg_get_bool(args, "-no-glfw");
    AvenStrSlice opt_syslibs = aven_str_split(
        aven_str_cstr(aven_arg_get_str(args, "-syslibs")),
        ' ',
        &arena
    );

    AvenStr root_dir = aven_str(".");
    AvenStr libaven_dir = aven_str("libaven");
    AvenStr work_dir = aven_str("build_work");

    AvenBuildStep work_dir_step = aven_build_step_mkdir(work_dir);

    AvenBuildCommonOpts glfw_opts = opts;
    if (agl_opts.glfw.ccflags.valid) {
        glfw_opts.cc.flags = agl_opts.glfw.ccflags.value;
    }
    AvenBuildStep glfw_step = libavengl_build_step_glfw(
        &glfw_opts,
        root_dir,
        &work_dir_step,
        &arena
    );

    AvenStr include_paths[] = {
        libavengl_build_include_path(root_dir, &arena),
        libaven_build_include_path(libaven_dir, &arena),
        libavengl_build_include_glfw(root_dir, &arena),
        libavengl_build_include_gles2(root_dir, &arena),
    };
    AvenStrSlice includes = {
        .ptr = include_paths,
        .len = countof(include_paths),
    };

    AvenBuildStep *obj_refs[] = {
        &glfw_step,
    };
    AvenBuildStepPtrSlice objs = { 0 };
    if (!opt_no_glfw) {
        objs.ptr = obj_refs;
        objs.len = countof(obj_refs);
    }
    
    AvenBuildStep test_step = aven_build_common_step_cc_ld_run_exe_ex(
        &opts,
        includes,
        (AvenStrSlice){ 0 },
        opt_syslibs,
        objs,
        aven_str("test.c"),
        &work_dir_step,
        (AvenStrSlice){ 0 },
        &arena
    );
    AvenBuildStep root_step = aven_build_step_root();
    aven_build_step_add_dep(&root_step, &test_step, &arena);

    if (opts.clean) {
        aven_build_step_clean(&root_step);
    } else if (opts.test) {
        error = aven_build_step_run(&root_step, arena);
        if (error != 0) {
            fprintf(stderr, "BUILD FAILED: %d\n", error);
            return 1;
        }
    }

    return 0;
}

