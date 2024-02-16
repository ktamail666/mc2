#include "gameos.hpp"
#include "toolos.hpp"
#include "mclib.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <filesystem>

UserHeapPtr systemHeap = NULL;
FastFile** fastFiles = NULL;
long numFastFiles = 0;
long maxFastFiles = 0;

void usage(char** argv) {
    printf("%s -rsp rsp_file -p <base_path> -w <wildcard> [ -w wildcard ... ] \n", argv[0]);
}

int gen_rsp(const char* prefix_path, std::queue<const char*>& wildcards, const char* rsp_file)
{
    if (!prefix_path || !rsp_file)
        return -1;

    size_t num_files2pack = 0;

    size_t prefix_path_len = strlen(prefix_path);
    size_t prefix_len = prefix_path_len + 1 + 1; // for '/' and '\0'

    std::unique_ptr<char[]> prefix = std::make_unique<char[]>(prefix_len);

    if (prefix_path[prefix_path_len - 1] != PATH_SEPARATOR_AS_CHAR) {
        snprintf(prefix.get(), prefix_len, "%s/", prefix_path);
    }
    else {
        strncpy(prefix.get(), prefix_path, prefix_len);
    }

    const char* cur_wildcard = nullptr;

    std::ofstream fh(rsp_file);
    if (!fh.is_open()) {
        std::cerr << "Cannot open file: \'" << rsp_file << "\'\n";
        return -1;
    }

    while (!wildcards.empty()) {

        cur_wildcard = wildcards.front();
        if (!cur_wildcard)
            continue;

        wildcards.pop();

        std::cout << "Processing wildcard: " << cur_wildcard << '\n';

        char wildcard_dir[1024] = { 0 };
        _splitpath(cur_wildcard, nullptr, wildcard_dir, nullptr, nullptr);

        std::unique_ptr<char[]> cur_search_path = std::make_unique<char[]>(strlen(prefix.get()) + strlen(PATH_SEPARATOR) + strlen(cur_wildcard) + 1);
        sprintf(cur_search_path.get(), "%s%s", prefix.get(), cur_wildcard);

        WIN32_FIND_DATA findResult;
        HANDLE searchHandle = FindFirstFile(cur_search_path.get(), &findResult);
        if (searchHandle != INVALID_HANDLE_VALUE)
        {
            do
            {
                if ((findResult.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
                {
                    // +1 for \n
                    std::unique_ptr<char[]> filename = std::make_unique<char[]>(strlen(findResult.cFileName) + strlen(wildcard_dir) + 1 + 1);
                    sprintf(filename.get(), "%s%s\n", wildcard_dir, findResult.cFileName);
                    std::cout << '\t' << filename.get() << '\n';
                    num_files2pack++;

                    fh.write(filename.get(), strlen(filename.get()));
                }
			} while (FindNextFile(searchHandle, &findResult));
            FindClose(searchHandle);
        }
    }

    if (num_files2pack == 0) {
        std::cout << "No files found to pack.\n";
        fh.close();
        return -1;
    }

    fh.close();
    return 0;
}

int main(int argc, char** argv)
{
    if(argc < 3) {
        usage(argv);
        return 1;
    }

    systemHeap = new UserHeap();
    if(!systemHeap) {
        STOP(("Failed to initialize system heap"));
        return -1;
    }
    systemHeap->init(32*1024*1024);

    std::queue<const char*> wildcards;
	const char* base_path = nullptr;
    const char* rsp_file = nullptr;

    for(int i=1;i<argc;++i)
	{
        if(0 == strcmp(argv[i], "-p") && i+1 < argc) {
           base_path = argv[i+1];
           ++i;
        }

        if(0 == strcmp(argv[i], "-w") && i+1 < argc) {
           wildcards.push(argv[i+1]);
           ++i;
        }

        if(0 == strcmp(argv[i], "-rsp") && i+1 < argc) {
           rsp_file = argv[i+1];
           ++i;
        }
    }

    return gen_rsp(base_path, wildcards, rsp_file);
}
