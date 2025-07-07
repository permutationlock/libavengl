#ifndef LIBAVENGL_BUILD_H
    #define LIBAVENGL_BUILD_H

    AvenArg libavengl_build_arg_data[] = {
        {
            .name = aven_str_init("--glfw-ccflags"),
            .description = aven_str_init("C compiler flags for GLFW"),
            .type = AVEN_ARG_TYPE_STRING,
    #if defined(LIBAVENGL_DEFAULT_GLFW_CCFLAGS)
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
                .data = {
                    .arg_str = aven_str_init(LIBAVENGL_DEFAULT_GLFW_CCFLAGS),
                },
            },
    #else
            .optional = true,
    #endif
        },
        {
            .name = aven_str_init("--stb-ccflags"),
            .description = aven_str_init("C compiler flags for STB"),
            .type = AVEN_ARG_TYPE_STRING,
    #if defined(LIBAVENGL_DEFAULT_STB_CCFLAGS)
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
                .data = {
                    .arg_str = aven_str_init(LIBAVENGL_DEFAULT_STB_CCFLAGS),
                },
            },
    #else
            .optional = true,
    #endif
        },
        {
            .name = aven_str_init("--no-glfw"),
            .description = aven_str_init("Don't build GLFW locally"),
            .type = AVEN_ARG_TYPE_BOOL,
        },
        {
            .name = aven_str_init("--android-so"),
            .description = aven_str_init(
                "Build a shared object for the Adroid NDK"
            ),
            .type = AVEN_ARG_TYPE_BOOL,
        },
        {
            .name = aven_str_init("--syslibs"),
            .description = aven_str_init("System libraries to link"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(LIBAVENGL_DEFAULT_SYSLIBS)
                .data = { .arg_str = aven_str_init(LIBAVENGL_DEFAULT_SYSLIBS) },
    #elif defined(_WIN32)
    #if defined(_MSC_VER) and !defined(__clang__)
                .data = {
                    .arg_str = aven_str_init(
                        "kernel32.lib user32.lib gdi32.lib shell32.lib"
                    ),
                },
    #else
                .data = {
                    .arg_str = aven_str_init("kernel32 user32 gdi32 shell32"),
                },
    #endif
    #else
                .data = { .arg_str = aven_str_init("m dl") },
    #endif
            },
        },
    };

    static inline AvenArgSlice libavengl_build_args(void) {
        AvenArgSlice args = slice_array(libavengl_build_arg_data);
        return args;
    }

    typedef struct {
        Optional(AvenStrSlice) ccflags;
        bool external;
    } LibAvenGlBuildGLFWOpts;

    typedef struct {
        Optional(AvenStrSlice) ccflags;
    } LibAvenGlBuildSTBOpts;

    typedef struct {
        LibAvenGlBuildGLFWOpts glfw;
        LibAvenGlBuildSTBOpts stb;
        AvenStrSlice syslibs;
        bool no_glfw;
        bool android_so;
    } LibAvenGlBuildOpts;

    static inline LibAvenGlBuildOpts libavengl_build_opts(
        AvenArgSlice args,
        AvenArena *arena
    ) {
        LibAvenGlBuildOpts opts = { 0 };

        if (aven_arg_has_arg(args, "--glfw-ccflags")) {
            opts.glfw.ccflags.valid = true;
            opts.glfw.ccflags.value = aven_str_split(
                aven_arg_get_str(args, "--glfw-ccflags"),
                ' ',
                arena
            );
        }

        if (aven_arg_has_arg(args, "--stb-ccflags")) {
            opts.stb.ccflags.valid = true;
            opts.stb.ccflags.value = aven_str_split(
                aven_arg_get_str(args, "--stb-ccflags"),
                ' ',
                arena
            );
        }

        opts.syslibs = aven_str_split(
            aven_arg_get_str(args, "--syslibs"),
            ' ',
            arena
        );
        opts.no_glfw = aven_arg_get_bool(args, "--no-glfw");
        opts.android_so = aven_arg_get_bool(args, "--android-so");

        return opts;
    }

    static inline AvenStr libavengl_build_include_path(
        AvenStr root_path,
        AvenArena *arena
    ) {
        return aven_path(arena, root_path, aven_str("include"));
    }

    static inline AvenStr libavengl_build_include_gles2(
        AvenStr root_path,
        AvenArena *arena
    ) {
        return aven_path(
            arena,
            root_path,
            aven_str("deps"),
            aven_str("gles2"),
            aven_str("include")
        );
    }

    static inline AvenStr libavengl_build_include_glfw(
        AvenStr root_path,
        AvenArena *arena
    ) {
        return aven_path(
            arena,
            root_path,
            aven_str("deps"),
            aven_str("glfw"),
            aven_str("include")
        );
    }

    static inline AvenStr libavengl_build_include_wayland(
        AvenStr root_path,
        AvenArena *arena
    ) {
        return aven_path(
            arena,
            root_path,
            aven_str("deps"),
            aven_str("wayland"),
            aven_str("include")
        );
    }

    static inline AvenStr libavengl_build_include_x11(
        AvenStr root_path,
        AvenArena *arena
    ) {
        return aven_path(
            arena,
            root_path,
            aven_str("deps"),
            aven_str("X11"),
            aven_str("include")
        );
    }

    static inline AvenStr libavengl_build_include_xkbcommon(
        AvenStr root_path,
        AvenArena *arena
    ) {
        return aven_path(
            arena,
            root_path,
            aven_str("deps"),
            aven_str("xkbcommon"),
            aven_str("include")
        );
    }

    static inline AvenStr libavengl_build_include_android(
        AvenStr root_path,
        AvenArena *arena
    ) {
        return aven_path(
            arena,
            root_path,
            aven_str("deps"),
            aven_str("android"),
            aven_str("include")
        );
    }

    static inline AvenBuildStep libavengl_build_step_stb(
        AvenBuildCommonOpts *opts,
        LibAvenGlBuildOpts *libavengl_opts,
        AvenStr libaven_include_path,
        AvenStr root_path,
        AvenBuildStep *out_dir_step,
        bool pic,
        AvenArena *arena
    ) {
        AvenBuildCommonOpts stb_opts = *opts;
        if (libavengl_opts->stb.ccflags.valid) {
            stb_opts.cc.flags = libavengl_opts->stb.ccflags.value;
        }

        AvenStr include_paths[] = { libaven_include_path };
        AvenStrSlice includes = slice_array(include_paths);

        return aven_build_common_step_cc_ex(
            &stb_opts,
            includes,
            (AvenStrSlice){ 0 },
            aven_path(
                arena,
                root_path,
                aven_str("deps"),
                aven_str("stb"),
                aven_str("stb.c")
            ),
            out_dir_step,
            pic,
            arena
        );
    }

    static inline AvenBuildStep libavengl_build_step_glfw(
        AvenBuildCommonOpts *opts,
        LibAvenGlBuildOpts *libavengl_opts,
        AvenStr root_path,
        AvenBuildStep *out_dir_step,
        bool pic,
        AvenArena *arena
    ) {
        AvenBuildCommonOpts glfw_opts = *opts;
        if (libavengl_opts->glfw.ccflags.valid) {
            glfw_opts.cc.flags = libavengl_opts->glfw.ccflags.value;
        }

        AvenStr include_paths[6];
        List(AvenStr) include_list = list_array(include_paths);
        list_push(include_list) = libavengl_build_include_gles2(
            root_path,
            arena
        );
        list_push(include_list) = libavengl_build_include_glfw(root_path, arena);
        list_push(include_list) = libavengl_build_include_wayland(
            root_path,
            arena
        );
        list_push(include_list) = libavengl_build_include_x11(root_path, arena);
        list_push(include_list) = libavengl_build_include_xkbcommon(
            root_path,
            arena
        );
        if (libavengl_opts->android_so) {
            list_push(include_list) = libavengl_build_include_android(
                root_path,
                arena
            );
        }

        AvenStrSlice includes = slice_list(include_list);

        return aven_build_common_step_cc_ex(
            &glfw_opts,
            includes,
            (AvenStrSlice){ 0 },
            aven_path(
                arena,
                root_path,
                aven_str("deps"),
                aven_str("glfw"),
                aven_str("glfw.c")
            ),
            out_dir_step,
            pic,
            arena
        );
    }

    static inline AvenBuildStep libavengl_build_step_android(
        AvenBuildCommonOpts *opts,
        LibAvenGlBuildOpts *libavengl_opts,
        AvenStr root_path,
        AvenBuildStep *out_dir_step,
        bool pic,
        AvenArena *arena
    ) {
        AvenBuildCommonOpts glfw_opts = *opts;
        if (libavengl_opts->glfw.ccflags.valid) {
            glfw_opts.cc.flags = libavengl_opts->glfw.ccflags.value;
        }

        AvenStr include_paths[] = {
            libavengl_build_include_android(root_path, arena),
        };

        AvenStrSlice includes = slice_array(include_paths);

        return aven_build_common_step_cc_ex(
            &glfw_opts,
            includes,
            (AvenStrSlice){ 0 },
            aven_path(
                arena,
                root_path,
                aven_str("deps"),
                aven_str("android"),
                aven_str("android.c")
            ),
            out_dir_step,
            pic,
            arena
        );
    }

    static inline AvenBuildStep libavengl_build_step(
        AvenBuildCommonOpts *opts,
        LibAvenGlBuildOpts *libavengl_opts,
        AvenStr libaven_include_path,
        AvenStr root_path,
        AvenBuildStep *work_dir_step,
        AvenBuildStep *out_dir_step,
        bool pic,
        AvenArena *arena
    ) {
        AvenBuildStep *obj_steps[3];
        List(AvenBuildStep *) obj_list = list_array(obj_steps);

        AvenBuildStep *stb_step = aven_arena_create(AvenBuildStep, arena);
        *stb_step = libavengl_build_step_stb(
            opts,
            libavengl_opts,
            libaven_include_path,
            root_path,
            work_dir_step,
            pic,
            arena
        );
        list_push(obj_list) = stb_step;

        if (!libavengl_opts->no_glfw) {
            AvenBuildStep *glfw_step = aven_arena_create(AvenBuildStep, arena);
            *glfw_step = libavengl_build_step_glfw(
                opts,
                libavengl_opts,
                root_path,
                work_dir_step,
                pic,
                arena
            );
            list_push(obj_list) = glfw_step;
        }

        if (libavengl_opts->android_so) {
            AvenBuildStep *android_step = aven_arena_create(
                AvenBuildStep,
                arena
            );
            *android_step = libavengl_build_step_android(
                opts,
                libavengl_opts,
                root_path,
                work_dir_step,
                pic,
                arena
            );
            list_push(obj_list) = android_step;
        }

        AvenBuildStepPtrSlice obj_slice = slice_list(obj_list);
        return aven_build_common_step_ar(
            opts,
            obj_slice,
            out_dir_step,
            aven_str("libavengl"),
            arena
        );
    }

    static inline AvenBuildStep libavengl_build_step_ld(
        AvenBuildCommonOpts *opts,
        LibAvenGlBuildOpts *libavengl_opts,
        AvenStr libaven_include_path,
        AvenStr root_path,
        AvenBuildStepPtrSlice obj_steps,
        AvenBuildStep *work_dir_step,
        AvenBuildStep *out_dir_step,
        AvenStr out_fname,
        AvenArena *arena
    ) {
        AvenBuildStep *lib_step = aven_arena_create(AvenBuildStep, arena);
        *lib_step = libavengl_build_step(
            opts,
            libavengl_opts,
            libaven_include_path,
            root_path,
            work_dir_step,
            work_dir_step,
            libavengl_opts->android_so,
            arena
        );

        AvenBuildStepPtrSlice objs = aven_arena_create_slice(
            AvenBuildStep *,
            arena,
            obj_steps.len + 1
        );
        {
            size_t i = 0;
            for (; i < obj_steps.len; i += 1) {
                get(objs, i) = get(obj_steps, i);
            }
            get(objs, i) = lib_step;
            i += 1;
        }

        if (!libavengl_opts->android_so) {
            return aven_build_common_step_ld_exe_ex(
                opts,
                libavengl_opts->syslibs,
                objs,
                out_dir_step,
                out_fname,
                true,
                arena
            );
        }

        AvenStrSlice ldflags = aven_arena_create_slice(
            AvenStr,
            arena,
            opts->ld.flags.len + 1
        );
        {
            size_t i = 0;
            for (; i < opts->ld.flags.len; i += 1) {
                get(ldflags, i) = get(opts->ld.flags, i);
            }
            get(ldflags, i) = aven_str("-uANativeActivity_onCreate");
            i += 1;
        }

        AvenBuildCommonOpts opts_cpy = *opts;
        opts_cpy.ld.flags = ldflags;

        AvenStrSlice syslibs = aven_arena_create_slice(
            AvenStr,
            arena,
            libavengl_opts->syslibs.len + 4
        );
        {
            size_t i = 0;
            for (; i < libavengl_opts->syslibs.len; i += 1) {
                get(syslibs, i) = get(libavengl_opts->syslibs, i);
            }
            get(syslibs, i) = aven_str("m");
            i += 1;
            get(syslibs, i) = aven_str("dl");
            i += 1;
            get(syslibs, i) = aven_str("android");
            i += 1;
            get(syslibs, i) = aven_str("log");
            i += 1;
        }

        return aven_build_common_step_ld_so_ex(
            &opts_cpy,
            syslibs,
            objs,
            out_dir_step,
            out_fname,
            arena
        );
    }
#endif // LIBAVENGL_BUILD_H
