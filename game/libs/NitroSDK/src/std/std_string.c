#include "../sdk_internal.h"

char* STD_CopyString(char* dst, const char* src) {
    char* ret = dst;

    while (*src != '\0') {
        *dst++ = *src++;
    }
    *dst = '\0';
    return ret;
}

int STD_GetStringLength(const char* str);

int STD_CopyLString(char* dst, const char* src, int len) {
    int i;
    const char* p = src;

    for (i = 0; i < len - 1; i++, p++) {
        dst[i] = *p;
        if (*p == '\0') {
            break;
        }
    }
    if (i >= len - 1 && len != 0) {
        dst[i] = '\0';
    }
    return STD_GetStringLength(src);
}

char* func_0207c2d0(const char* str, const char* pattern) {
    int i;
    int j;

    for (i = 0; str[i] != '\0'; i++) {
        for (j = 0; pattern[j] != '\0' && str[i + j] == pattern[j]; j++) {
        }
        if (pattern[j] == '\0') {
            return (char*)str + i;
        }
    }
    return NULL;
}

int STD_GetStringLength(const char* str) {
    int i = 0;

    while (str[i] != '\0') {
        i++;
    }
    return i;
}

char* STD_ConcatenateString(char* dst, const char* src) {
    int len = STD_GetStringLength(dst);
    STD_CopyString(dst + len, src);
    return dst;
}

int STD_CompareString(const char* s1, const char* s2) {
    while (*s1 == *s2 && *s1 != '\0') {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

int STD_CompareNString(const char* s1, const char* s2, int n) {
    const u8* p1 = (const u8*)s1;
    const u8* p2 = (const u8*)s2;

    if (n != 0) {
        int i;
        for (i = 0; i < n; i++) {
            if (p1[i] != p2[i]) {
                return p1[i] - p2[i];
            }
        }
    }
    return 0;
}
