#include "file_utils.h"

#include <sys/stat.h>
#include <filesystem>

#ifdef LINUX_BUILD
const char filesystem::kPathSeparatorAsChar  = '/';
const char* const filesystem::kPathSeparator = "/";
#else
const char filesystem::kPathSeparatorAsChar  = '\\';
const char* const filesystem::kPathSeparator = "\\";
#endif

uint64_t filesystem::get_file_mod_time_ms(const char* filename)
{
#if PLATFORM_WINDOWS
    struct _stat fi = { 0 };
    _stat(filename, &fi);
    return fi.st_mtime * 1000;
#else
    struct stat fi = { 0 };
    stat(filename, &fi);
    return fi.st_mtim.tv_sec * 1e+3 + fi.st_mtim.tv_nsec / 1e+6;
#endif
}

std::string filesystem::get_path(const char* filename)
{
    auto path = std::filesystem::path(filename).parent_path();
    return path.generic_string();
}
