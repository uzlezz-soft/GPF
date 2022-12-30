#ifdef _WIN32
/* Windows x64/x86 */
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define GARBAGE_PLATFORM_WINDOWS
#elif defined(__linux__)
#define GARBAGE_PLATFORM_LINUX
#else
/* Unknown compiler/platform */
#error "Unknown platform!"
#endif