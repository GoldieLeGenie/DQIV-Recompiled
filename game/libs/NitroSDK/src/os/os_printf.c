// Must be built with mwccarm 2.0/sp1p2 (2.0/base and 2.0/sp1 also match): with sp1p5/sp1p6 the hoisted
// zero of the 64-bit `val != 0` loop test cannot be spilled, which changes the register allocation of func_02077b48.
#include "os_internal.h"
#ifdef DQIV_NATIVE
#define SDK_SMALL_ARG(ap, type) ((type)va_arg(ap, int))
#else
#define SDK_SMALL_ARG(ap, type) va_arg(ap, type)
#endif

// Output buffer of the formatter
typedef struct UnkPrintfDst {
    /* 0x00 */ u32   len;
    /* 0x04 */ char* cur;
    /* 0x08 */ char* base;
} UnkPrintfDst;

enum {
    FLAG_BLANK    = 0x0001,
    FLAG_PLUS     = 0x0002,
    FLAG_SHARP    = 0x0004,
    FLAG_MINUS    = 0x0008,
    FLAG_ZERO     = 0x0010,
    FLAG_L        = 0x0020,
    FLAG_H        = 0x0040,
    FLAG_LL       = 0x0080,
    FLAG_HH       = 0x0100,
    FLAG_UNSIGNED = 0x1000
};

u64 func_02005fe4(u64 a, u64 b); // unsigned 64-bit division

// Writes one character
void func_02077a6c(UnkPrintfDst* p, char c) {
    if (p->len > 0) {
        *p->cur = c;
        --p->len;
    }
    ++p->cur;
}

// Writes a character n times
void func_02077a9c(UnkPrintfDst* p, char c, s32 n) {
    if (n > 0) {
        u32 i;
        u32 k = p->len;
        if (k > n) {
            k = n;
        }
        for (i = 0; i < k; ++i) {
            p->cur[i] = c;
        }
        p->len -= k;
        p->cur += n;
    }
}

// Writes n characters of a string
void func_02077af0(UnkPrintfDst* p, const char* s, s32 n) {
    if (n > 0) {
        u32 i;
        u32 k = p->len;
        if (k > n) {
            k = n;
        }
        for (i = 0; i < k; ++i) {
            p->cur[i] = s[i];
        }
        p->len -= k;
        p->cur += n;
    }
}

// vsnprintf
s32 func_02077b48(char* dst, u32 len, const char* fmt, va_list vlist) {
    char         buf[24];
    s32          n_buf;
    char         prefix[2];
    s32          n_prefix;
    const char*  s = fmt;
    UnkPrintfDst str;

    str.len = len;
    str.cur = str.base = dst;

    while (*s) {
        if ((u32)(((u8)*s ^ 0x20) - 0xa1) < 0x3c) {
            // Shift-JIS lead byte
            func_02077a6c(&str, *s);
            if (*++s) {
                func_02077a6c(&str, *s++);
            }
        } else if (*s != '%') {
            func_02077a6c(&str, *s++);
        } else {
            s32         flag      = 0;
            s32         width     = 0;
            s32         precision = -1;
            s32         radix     = 10;
            char        hex_char  = 'a' - 10;
            const char* p_start   = s;
            s32         c;

            for (;;) {
                c = *++s;
                switch (c) {
                    case '+':
                        if (s[-1] != ' ') {
                            goto post_padding;
                        }
                        flag |= FLAG_PLUS;
                        break;
                    case ' ':
                        flag |= FLAG_BLANK;
                        break;
                    case '-':
                        flag |= FLAG_MINUS;
                        break;
                    case '0':
                        flag |= FLAG_ZERO;
                        break;
                    default:
                        goto post_padding;
                }
            }
        post_padding:
            if (*s == '*') {
                ++s;
                width = va_arg(vlist, s32);
                if (width < 0) {
                    width = -width;
                    flag |= FLAG_MINUS;
                }
            } else {
                while (*s >= '0' && *s <= '9') {
                    width = width * 10 + *s++ - '0';
                }
            }

            if (*s == '.') {
                ++s;
                precision = 0;
                if (*s == '*') {
                    ++s;
                    precision = va_arg(vlist, s32);
                    if (precision < 0) {
                        precision = -1;
                    }
                } else {
                    while (*s >= '0' && *s <= '9') {
                        precision = precision * 10 + *s++ - '0';
                    }
                }
            }

            switch (*s) {
                case 'h':
                    if (*++s != 'h') {
                        flag |= FLAG_H;
                    } else {
                        ++s;
                        flag |= FLAG_HH;
                    }
                    break;
                case 'l':
                    if (*++s != 'l') {
                        flag |= FLAG_L;
                    } else {
                        ++s;
                        flag |= FLAG_LL;
                    }
                    break;
            }

            switch (*s) {
                case 'd':
                case 'i':
                    goto put_integer;
                case 'o':
                    flag |= FLAG_UNSIGNED;
                    radix = 8;
                    goto put_integer;
                case 'u':
                    flag |= FLAG_UNSIGNED;
                    goto put_integer;
                case 'X':
                    hex_char = 'A' - 10;
                    goto put_hexadecimal;
                case 'x':
                    goto put_hexadecimal;
                case 'p':
                    flag |= FLAG_SHARP;
                    precision = 8;
                    goto put_hexadecimal;
                case 'c':
                    if (precision >= 0) {
                        goto put_invalid;
                    }
                    {
                        c = va_arg(vlist, s32);
                        width -= 1;
                        if (flag & FLAG_MINUS) {
                            func_02077a6c(&str, (char)c);
                            func_02077a9c(&str, ' ', width);
                        } else {
                            char pad = (char)((flag & FLAG_ZERO) ? '0' : ' ');
                            func_02077a9c(&str, pad, width);
                            func_02077a6c(&str, (char)c);
                        }
                        ++s;
                    }
                    break;
                case 's': {
                    s32         n_bufs = 0;
                    const char* p_bufs = va_arg(vlist, const char*);
                    if (precision < 0) {
                        while (p_bufs[n_bufs]) {
                            ++n_bufs;
                        }
                    } else {
                        while (n_bufs < precision && p_bufs[n_bufs]) {
                            ++n_bufs;
                        }
                    }
                    width -= n_bufs;
                    if (flag & FLAG_MINUS) {
                        func_02077af0(&str, p_bufs, n_bufs);
                        func_02077a9c(&str, ' ', width);
                    } else {
                        char pad = (char)((flag & FLAG_ZERO) ? '0' : ' ');
                        func_02077a9c(&str, pad, width);
                        func_02077af0(&str, p_bufs, n_bufs);
                    }
                    ++s;
                } break;
                case 'n': {
                    s32 count = str.cur - str.base;
                    if (!(flag & FLAG_HH)) {
                        if (flag & FLAG_H) {
                            *va_arg(vlist, s16*) = (s16)count;
                        } else if (flag & FLAG_LL) {
                            *va_arg(vlist, s64*) = count;
                        } else {
                            *va_arg(vlist, s32*) = count;
                        }
                    }
                    ++s;
                } break;
                case '%':
                    if (p_start + 1 != s) {
                        goto put_invalid;
                    }
                    func_02077a6c(&str, *s++);
                    break;
                default:
                    goto put_invalid;

                put_invalid:
                    func_02077af0(&str, p_start, s - p_start);
                    break;

                put_hexadecimal:
                    flag |= FLAG_UNSIGNED;
                    radix = 16;

                put_integer : {
                    u64 val = 0;
                    n_prefix = 0;

                    if (flag & FLAG_MINUS) {
                        flag &= ~FLAG_ZERO;
                    }
                    if (precision < 0) {
                        precision = 1;
                    } else {
                        flag &= ~FLAG_ZERO;
                    }

                    if (flag & FLAG_UNSIGNED) {
                        if (flag & FLAG_HH) {
                            val = SDK_SMALL_ARG(vlist, u8);
                        } else if (flag & FLAG_H) {
                            val = SDK_SMALL_ARG(vlist, u16);
                        } else if (flag & FLAG_LL) {
                            val = va_arg(vlist, u64);
                        } else {
                            val = va_arg(vlist, u32);
                        }
                        flag &= ~(FLAG_PLUS | FLAG_BLANK);
                        if (flag & FLAG_SHARP) {
                            if (radix == 16) {
                                if (val != 0) {
                                    prefix[0] = (char)(hex_char + ('x' - 'a' + 10));
                                    prefix[1] = '0';
                                    n_prefix  = 2;
                                }
                            } else if (radix == 8) {
                                prefix[0] = '0';
                                n_prefix  = 1;
                            }
                        }
                    } else {
                        if (flag & FLAG_HH) {
                            val = SDK_SMALL_ARG(vlist, s8);
                        } else if (flag & FLAG_H) {
                            val = SDK_SMALL_ARG(vlist, s16);
                        } else if (flag & FLAG_LL) {
                            val = va_arg(vlist, u64);
                        } else {
                            val = va_arg(vlist, s32);
                        }
                        if ((val >> 32) & 0x80000000) {
                            val       = ~val + 1;
                            prefix[0] = '-';
                            n_prefix  = 1;
                        } else if (val || precision) {
                            if (flag & FLAG_PLUS) {
                                prefix[0] = '+';
                                n_prefix  = 1;
                            } else if (flag & FLAG_BLANK) {
                                prefix[0] = ' ';
                                n_prefix  = 1;
                            }
                        }
                    }

                    n_buf = 0;
                    switch (radix) {
                        case 8:
                            while (val != 0) {
                                s32 d = (s32)(val & 0x7);
                                val >>= 3;
                                buf[n_buf++] = (char)(d + '0');
                            }
                            break;
                        case 10:
                            if ((val >> 32) == 0) {
                                u32 v = (u32)val;
                                while (v != 0) {
                                    u32 r = v / 10;
                                    s32 d = (s32)(v - r * 10);
                                    v     = r;
                                    buf[n_buf++] = (char)(d + '0');
                                }
                            } else {
                                while (val != 0) {
                                    u64 r = val / 10;
                                    s32 d = (s32)(val - r * 10);
                                    val   = r;
                                    buf[n_buf++] = (char)(d + '0');
                                }
                            }
                            break;
                        case 16:
                            while (val != 0) {
                                s32 d = (s32)(val & 0xf);
                                val >>= 4;
                                buf[n_buf++] = (char)((d < 10) ? (d + '0') : (d + hex_char));
                            }
                            break;
                    }

                    if ((n_prefix > 0) && (prefix[0] == '0')) {
                        n_prefix     = 0;
                        buf[n_buf++] = '0';
                    }
                }
                    goto put_to_stream;

                put_to_stream:
                    precision -= n_buf;
                    if (flag & FLAG_ZERO) {
                        if (precision < width - n_buf - n_prefix) {
                            precision = width - n_buf - n_prefix;
                        }
                    }
                    if (precision > 0) {
                        width -= precision;
                    }
                    width -= n_prefix + n_buf;
                    if (!(flag & FLAG_MINUS)) {
                        func_02077a9c(&str, ' ', width);
                    }
                    while (n_prefix > 0) {
                        func_02077a6c(&str, prefix[--n_prefix]);
                    }
                    func_02077a9c(&str, '0', precision);
                    while (n_buf > 0) {
                        func_02077a6c(&str, buf[--n_buf]);
                    }
                    if (flag & FLAG_MINUS) {
                        func_02077a9c(&str, ' ', width);
                    }
                    ++s;
                    break;
            }
        }
    }

    if (str.len) {
        *str.cur = '\0';
    } else if (len) {
        *(str.base + len - 1) = '\0';
    }
    return str.cur - str.base;
}
