#ifndef LIBAVENGL_BUILD_H
#define LIBAVENGL_BUILD_H

AvenArg libavengl_build_arg_data[] = {
    {
        .name = "-glfw-external",
        .description = "Don't build GLFW, link system library",
        .type = AVEN_ARG_TYPE_BOOL,
#if defined(LIBAVENGL_DEFAULT_GLFW_EXTERNAL)
        .value = {
            .type = AVEN_ARG_TYPE_BOOL,
            .data = { .arg_bool = LIBAVENGL_DEFAULT_GLFW_EXTERNAL },
        },
#endif
    },
    {
        .name = "-glfw-ccflags",
        .description = "C compiler flags for GLFW",
        .type = AVEN_ARG_TYPE_STRING,
#if defined(LIBAVENGL_DEFAULT_GLFW_CCFLAGS)
        .value = {
            .type = AVEN_ARG_TYPE_STRING,
            .data = { .arg_str = LIBAVENGL_DEFAULT_GLFW_CCFLAGS },
        },
#else
        .optional = true,
#endif
    },
};
AvenArgSlice libavengl_build_args = {
    .ptr = libavengl_build_arg_data,
    .len = countof(libavengl_build_arg_data),
};

typedef struct {
    Optional(AvenStrSlice) ccflags;
    bool external;
} LibAvenGLBuildGLFWOpts;

typedef struct {
    LibAvenGLBuildGLFWOpts glfw;
} LibAvenGLBuildOpts;

static inline LibAvenGLBuildOpts libavengl_build_opts(
    AvenArgSlice args,
    AvenArena *arena
) {
    LibAvenGLBuildOpts opts = { 0 };
    opts.glfw.external = aven_arg_get_bool(args, "-glfw-external");
    if (aven_arg_has_arg(args, "-glfw-ccflags")) {
        opts.glfw.ccflags.valid = true;
        opts.glfw.ccflags.value = aven_str_split(
            aven_str_cstr(aven_arg_get_str(args, "-glfw-ccflags")),
            ' ',
            arena
        );
    }
    return opts;
}

static inline AvenStr libavengl_build_include_path(
    AvenStr root_path,
    AvenArena *arena
) {
    return aven_path(arena, root_path.ptr, "include", NULL);
}

static inline AvenStr libavengl_build_include_libaven(
    AvenStr root_path,
    AvenArena *arena
) {
    return aven_path(arena, root_path.ptr, "deps", "libaven", "include", NULL);
}

static inline AvenStr libavengl_build_include_gles2(
    AvenStr root_path,
    AvenArena *arena
) {
    return aven_path(arena, root_path.ptr, "deps", "gles2", "include", NULL);
}

static inline AvenStr libavengl_build_include_glfw(
    AvenStr root_path,
    AvenArena *arena
) {
    return aven_path(arena, root_path.ptr, "deps", "glfw", "include", NULL);
}

static inline AvenStr libavengl_build_include_wayland(
    AvenStr root_path,
    AvenArena *arena
) {
    return aven_path(arena, root_path.ptr, "deps", "wayland", "include", NULL);
}

static inline AvenStr libavengl_build_include_winpthreads(
    AvenStr root_path,
    AvenArena *arena
) {
    return aven_path(
        arena,
        root_path.ptr,
        "deps",
        "winpthreads",
        "include",
        NULL
    );
}

static inline AvenStr libavengl_build_include_x11(
    AvenStr root_path,
    AvenArena *arena
) {
    return aven_path(arena, root_path.ptr, "deps", "X11", "include", NULL);
}

static inline AvenStr libavengl_build_include_xkbcommon(
    AvenStr root_path,
    AvenArena *arena
) {
    return aven_path(
        arena,
        root_path.ptr,
        "deps",
        "xkbcommon",
        "include",
        NULL
    );
}

static inline AvenBuildStep libavengl_build_step_glfw(
    AvenBuildCommonOpts *opts,
    AvenStr root_path,
    AvenBuildStep *out_dir_step,
    AvenArena *arena
) {
    AvenStr include_paths[] = {
        libavengl_build_include_gles2(root_path, arena),
        libavengl_build_include_glfw(root_path, arena),
        libavengl_build_include_wayland(root_path, arena),
        libavengl_build_include_x11(root_path, arena),
        libavengl_build_include_xkbcommon(root_path, arena),
    };
    AvenStrSlice includes = {
        .ptr = include_paths,
        .len = countof(include_paths),
    };
    return aven_build_common_step_cc_ex(
        opts,
        includes,
        (AvenStrSlice){ 0 },
        aven_path(arena, root_path.ptr, "deps", "glfw", "glfw.c", NULL),
        out_dir_step,
        arena
    );
}

static inline AvenBuildStep libavengl_build_step_winpthreads(
    AvenBuildCommonOpts *opts,
    AvenStr root_path,
    AvenBuildStep *out_dir_step,
    AvenArena *arena
) {
    AvenStr include_paths[] = {
        libavengl_build_include_winpthreads(root_path, arena),
    };
    AvenStrSlice includes = {
        .ptr = include_paths,
        .len = countof(include_paths),
    };
    return aven_build_common_step_cc_ex(
        opts,
        includes,
        (AvenStrSlice){ 0 },
        aven_path(arena, root_path.ptr, "winpthreads", "winpthreads.c", NULL),
        out_dir_step,
        arena
    );
}

#endif // LIBAVENGL_BUILD_H
