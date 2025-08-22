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
#include "deps/libaven/include/aven/fs.h"
#include "deps/libaven/include/aven/io.h"
#include "deps/libaven/include/aven/path.h"

#include "build.h"
#include "deps/libaven/build.h"

#define ARENA_SIZE (4096 * 2000)

int main(int argc, char **argv) {
    aven_fs_utf8_mode();

    void *mem = malloc(ARENA_SIZE);
    if (mem == NULL) {
        aven_panic("malloc failed");
    }

    AvenArena arena = aven_arena_init(mem, ARENA_SIZE);

    AvenArgSlice common_args = aven_build_common_args();
    AvenArgSlice libaven_args = libaven_build_args();
    AvenArgSlice libavengl_args = libavengl_build_args();

    List(AvenArg) arg_list = aven_arena_create_list(
        AvenArg,
        &arena,
        common_args.len + libaven_args.len + libavengl_args.len
    );
    for (size_t i = 0; i < common_args.len; i += 1) {
        list_push(arg_list) = get(common_args, i);
    }
    for (size_t i = 0; i < libaven_args.len; i += 1) {
        list_push(arg_list) = get(libaven_args, i);
    }
    for (size_t i = 0; i < libavengl_args.len; i += 1) {
        list_push(arg_list) = get(libavengl_args, i);
    }

    AvenArgSlice args = slice_list(arg_list);

    AvenArgError arg_error = aven_arg_parse(
        args,
        argv,
        argc,
        aven_build_common_overview(),
        aven_build_common_usage()
    );
    if (arg_error != 0) {
        if (arg_error != AVEN_ARG_ERROR_HELP) {
            aven_io_perrf("ARG PARSE ERROR: {}\n", aven_fmt_int(arg_error));
            return 1;
        }
        return 0;
    }

    AvenBuildCommonOpts opts = aven_build_common_opts(args, &arena);
    LibAvenBuildOpts libaven_opts = libaven_build_opts(args, &arena);
    LibAvenGlBuildOpts libavengl_opts = libavengl_build_opts(args, &arena);

    AvenStr root_dir = aven_str(".");
    AvenStr libaven_dir = aven_path(
        &arena,
        aven_str("deps"),
        aven_str("libaven")
    );
    AvenStr work_dir = aven_str("build_test");

    AvenStr libaven_include_path = libaven_build_include_path(
        libaven_dir,
        &arena
    );

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

    AvenStr include_data[4];
    List(AvenStr) include_list = list_array(include_data);
    list_push(include_list) = libaven_include_path;
    list_push(include_list) = libavengl_build_include_path(root_dir, &arena);
    list_push(include_list) = libavengl_build_include_gles2(root_dir, &arena);
    list_push(include_list) = libavengl_build_include_glfw(root_dir, &arena);
    AvenStrSlice includes = slice_list(include_list);

    AvenBuildStep test_obj_step = aven_build_common_step_cc_ex(
        &opts,
        includes,
        (AvenStrSlice){ 0 },
        aven_str("test.c"),
        &work_dir_step,
        libavengl_opts.android.enabled,
        &arena
    );

    AvenBuildStep *obj_data[2];
    List(AvenBuildStep *) obj_list = list_array(obj_data);
    if (winutf8_obj_step.valid) {
        list_push(obj_list) = &winutf8_obj_step.value;
    }
    list_push(obj_list) = &test_obj_step;

    AvenBuildStepPtrSlice objs = slice_list(obj_list);

    AvenBuildStep build_step = libavengl_build_step_ld(
        &opts,
        &libavengl_opts,
        libaven_include_path,
        root_dir,
        objs,
        &work_dir_step,
        &work_dir_step,
        aven_str("test"),
        &arena
    );

    AvenBuildStep root_step = aven_build_step_root();
    aven_build_step_add_dep(&root_step, &build_step, &arena);

    AvenBuildStep run_step = aven_build_common_step_run_exe(
        &build_step,
        (AvenStrSlice){ 0 },
        &arena
    );

    AvenBuildStep test_root_step = aven_build_step_root();
    aven_build_step_add_dep(&test_root_step, &run_step, &arena);

    if (opts.clean) {
        aven_build_step_clean(&root_step, arena);
    } else if (opts.test) {
        if (opts.dry_run) {
            aven_build_step_dry_run(&test_root_step, arena);
        } else {
            AvenBuildStepRunError run_error = aven_build_step_run(
                &test_root_step,
                arena
            );
            if (run_error != 0) {
                aven_io_perrf("BUILD FAILED: {}\n", aven_fmt_int(run_error));
                return 1;
            }
        }
    } else {
        if (opts.dry_run) {
            aven_build_step_dry_run(&root_step, arena);
        } else {
            AvenBuildStepRunError run_error = aven_build_step_run(
                &root_step,
                arena
            );
            if (run_error != 0) {
                aven_io_perrf("BUILD FAILED: {}\n", aven_fmt_int(run_error));
                return 1;
            }
        }
    }

    return 0;
}
