#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include <nitro/std.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

ARM int dss::sprintf(char* dst, const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int result = ::vsprintf(dst, fmt, va);
    va_end(va);
    return result;
}

ARM unsigned int dss::strlen(const char* str)
{
    return STD_GetStringLength(str);
}

ARM char* dss::strcpy(char* dst, const char* src)
{
    return STD_CopyString(dst, src);
}

ARM char* dss::strcat(char* dst, const char* src)
{
    return STD_ConcatenateString(dst, src);
}

ARM char* dss::strstr(const char* str, const char* sub)
{
    return func_0207c2d0(str, sub);
}

ARM int dss::strcmp(const char* str1, const char* str2)
{
    return STD_CompareString(str1, str2);
}

ARM int dss::strncmp(const char* str1, const char* str2, unsigned int n)
{
    return STD_CompareNString(str1, str2, n);
}

ARM char* dss::strchr(const char* str, int c)
{
    return const_cast<char*>(::strchr(str, c));
}

ARM void* dss::memset(void* dst, int c, int n)
{
    MI_CpuSet(dst, c, n);
    return dst;
}

ARM void* dss::memcpy(void* dst, void* src, int n)
{
    MI_CpuCopyU8(src, dst, n);
    return dst;
}

ARM int dss::sprintf_s(char* dst, size_t size, const char* fmt, ...)
{
    va_list va;
    int ret;

    va_start(va, fmt);
    ret = ::vsnprintf(dst, size, fmt, va);
    va_end(va);
    if (ret >= (int)size || ret == -1) {
        *dst = '\0';
        return -1;
    }
    return ret;
}

ARM int dss::strcpy_s(char* dst, size_t size, const char* src)
{
    if (dst == 0) return -1;
    if (size == 0) return -1;
    if (src == 0) return -1;

    size = size - 1;
    char* kp = dst;

    while (*src != '\0') {
        if (size == 0) {
            *kp = '\0';
            return -1;
        }

        size--;
        *dst++ = *src++;
    }

    *dst = '\0';
    return 0;
}

ARM int dss::strcat_s(char* dest, size_t size, const char* src)
{
    char* start;

    if (dest == NULL)
        return -1;
    if (size == 0)
        return -1;
    if (src == NULL)
        return -1;

    size--;
    start = dest;

    while (*dest != '\0') {
        if (size == 0) {
            *dest = '\0';
            return -1;
        }
        dest++;
        size--;
    }

    while (*src != '\0') {
        if (size == 0) {
            *start = '\0';
            return -1;
        }
        size--;
        *dest++ = *src++;
    }

    *dest = '\0';
    return 0;
}
