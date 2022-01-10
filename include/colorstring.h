// This file defines compile-time constant strings containing standard ansi escape codes
// code : \033 = 'escape' : octal representation of ascii character value

constexpr const char * reset   = "\033[0m";

constexpr const char * black   = "\033[30m";
constexpr const char * red     = "\033[31m";
constexpr const char * green   = "\033[32m";
constexpr const char * yellow  = "\033[33m";
constexpr const char * blue    = "\033[34m";
constexpr const char * magenta = "\033[35m";
constexpr const char * cyan    = "\033[36m";
constexpr const char * white   = "\033[37m";

constexpr const char * black1  = "\033[30;1m";
constexpr const char * red1    = "\033[31;1m"; // FAILURE, error
constexpr const char * green1  = "\033[32;1m"; // SUCCESS
constexpr const char * yellow1 = "\033[33;1m";
constexpr const char * blue1   = "\033[34;1m";
constexpr const char * pink    = "\033[35;1m";
constexpr const char * cyan1   = "\033[36;1m";
constexpr const char * white1  = "\033[37;1m";

constexpr const char * redb    = "\033[37;41;1m";
constexpr const char * greenb  = "\033[37;42;1;2m";


// yellow alloc and dealloc message strings
constexpr const char * alloc   = "\033[33malloc:\033[0m";
constexpr const char * dealloc = "\033[33mdealloc:\033[0m";