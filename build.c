#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
    #define _POSIX_C_SOURCE 200112L
#endif

#include <stdlib.h>

#include "config.h"

#define AVEN_IMPLEMENTATION

#include "deps/libaven/include/aven.h"
#include "deps/libaven/include/aven/arena.h"
#include "deps/libaven/include/aven/arg.h"
#include "deps/libaven/include/aven/build.h"
#include "deps/libaven/include/aven/build/common.h"
#include "deps/libaven/include/aven/path.h"

#include "build.h"
#include "deps/libaven/build.h"
 
#define ARENA_SIZE (4096 * 2000)

int main(int argc, char **argv) {
    void *mem = malloc(ARENA_SIZE);
    assert(mem != NULL);

    AvenArena arena = aven_arena_init(mem, ARENA_SIZE);

    AvenArgSlice common_args = aven_build_common_args();
    AvenArgSlice libaven_args = libaven_build_args();
    AvenArgSlice libavengl_args = libavengl_build_args();

    AvenArgSlice args = {
        .len = common_args.len + libaven_args.len + libavengl_args.len,
    };
    args.ptr = aven_arena_create_array(AvenArg, &arena, args.len);
    size_t arg_index = 0;
    for (size_t i = 0; i < common_args.len; i += 1) {
        get(args, arg_index) = get(common_args, i);
        arg_index += 1;
    }
    for (size_t i = 0; i < libaven_args.len; i += 1) {
        get(args, arg_index) = get(libaven_args, i);
        arg_index += 1;
    }
    for (size_t i = 0; i < libavengl_args.len; i += 1) {
        get(args, arg_index) = get(libavengl_args, i);
        arg_index += 1;
    }

    int error = aven_arg_parse(
        args,
        argv,
        argc,
        aven_build_common_overview().ptr,
        aven_build_common_usage().ptr
    );
    if (error != 0) {
        if (error != AVEN_ARG_ERROR_HELP) {
            fprintf(stderr, "ARG PARSE ERROR: %d\n", error);
            return error;
        }
        return 0;
    }

    AvenBuildCommonOpts opts = aven_build_common_opts(args, &arena);
    LibAvenBuildOpts libaven_opts = libaven_build_opts(args, &arena);
    LibAvenGlBuildOpts libavengl_opts = libavengl_build_opts(args, &arena);

    AvenStr root_dir = aven_str(".");
    AvenStr libaven_dir = aven_path(&arena, "deps", "libaven", NULL);
    AvenStr work_dir = aven_str("build_work");

    AvenBuildStep work_dir_step = aven_build_step_mkdir(work_dir);

    Optional(AvenBuildStep) winutf8_obj_step = { 0 };
    if (libaven_opts.winutf8) {
        winutf8_obj_step.value = libaven_build_step_windres_manifest(
            &opts,
            libaven_dir,
            &work_dir_step,
            &arena
        );
        winutf8_obj_step.valid = true;
    }

    Optional(AvenBuildStep) glfw_obj_step = { 0 };
    if (!libavengl_opts.no_glfw) {
        glfw_obj_step.value = libavengl_build_step_glfw(
            &opts,
            &libavengl_opts,
            root_dir,
            &work_dir_step,
            &arena
        );
        glfw_obj_step.valid = true;
    }

    AvenStr include_data[4];
    List(AvenStr) include_list = list_array(include_data);
    list_push(include_list) = libaven_build_include_path(libaven_dir, &arena);
    list_push(include_list) = libavengl_build_include_path(root_dir, &arena);
    list_push(include_list) = libavengl_build_include_gles2(root_dir, &arena);
    list_push(include_list) = libavengl_build_include_glfw(root_dir, &arena);
    AvenStrSlice includes = slice_list(include_list);

    AvenBuildStep *obj_data[2];
    List(AvenBuildStep *) obj_list = list_array(obj_data);
    if (winutf8_obj_step.valid) {
        list_push(obj_list) = &winutf8_obj_step.value;
    }
    if (glfw_obj_step.valid) {
        list_push(obj_list) = &glfw_obj_step.value;
    }

    AvenBuildStepPtrSlice objs = slice_list(obj_list);
    
    AvenBuildStep test_step = aven_build_common_step_cc_ld_run_exe_ex(
        &opts,
        includes,
        (AvenStrSlice){ 0 },
        libavengl_opts.syslibs,
        objs,
        aven_str("test.c"),
        &work_dir_step,
        false,
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

