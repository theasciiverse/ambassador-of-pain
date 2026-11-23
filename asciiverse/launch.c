/* The Asciiverse front door for Ambassador of Pain.
 *
 * aop.c stays exactly as Raffael Himmelreich and Claire Wolf left it, 64
 * lines. It is compiled with -Dmain=aop_main and this file supplies the
 * real main(), so nothing has to change inside those 64 lines:
 *
 *   - Started with no arguments (as av starts it), it finds the eleven
 *     level files beside the program, in levels/ (the av archive), or in
 *     ../share/ambassador-of-pain/levels (the system packages), and hands
 *     them to the game in order. Upstream would look in
 *     /usr/local/share/aop instead.
 *   - Started with arguments, it passes them on untouched, so
 *     `aop my-level.txt` plays your own levels as before.
 *   - The game prints its last word and the score after leaving the
 *     screen ("Well done! (1234 points)"); when it was started with no
 *     arguments from a terminal, this waits for Enter so the score stays
 *     readable before av takes the screen back.
 *
 * No saves, no settings, nothing written anywhere.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#define SEP '\\'
#else
#include <termios.h>
#include <unistd.h>
#define SEP '/'
#endif
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

#define LEVELS 11

int aop_main(int argc, char *argv[]);

/* the folder the running program is in, with a trailing separator */
static int exe_dir(char *buf, size_t size)
{
    char *slash;
#if defined(_WIN32)
    DWORD n = GetModuleFileNameA(NULL, buf, (DWORD)size);
    if (n == 0 || n >= size) return 0;
    slash = strrchr(buf, '\\');
    if (!slash) slash = strrchr(buf, '/');
#elif defined(__APPLE__)
    char tmp[4096];
    uint32_t n = sizeof tmp;
    if (_NSGetExecutablePath(tmp, &n) != 0) return 0;
    if (!realpath(tmp, buf)) return 0;
    slash = strrchr(buf, '/');
#else
    ssize_t n = readlink("/proc/self/exe", buf, size - 1);
    if (n <= 0) return 0;
    buf[n] = '\0';
    slash = strrchr(buf, '/');
#endif
    if (!slash) return 0;
    slash[1] = '\0';
    return 1;
}

static int readable(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    fclose(f);
    return 1;
}

int main(int argc, char *argv[])
{
    static const char *dirs[] = {
        "levels",
        "../share/ambassador-of-pain/levels",
    };
    static char paths[LEVELS][4096 + 128];
    char base[4096], dir[4096 + 40];
    char *args[LEVELS + 2];
    size_t d;
    int i, result;

    if (argc > 1) return aop_main(argc, argv);

    if (!exe_dir(base, sizeof base)) base[0] = '\0';
    for (d = 0; d < sizeof dirs / sizeof dirs[0]; d++) {
        char *p;
        snprintf(dir, sizeof dir, "%s%s", base, dirs[d]);
        for (p = dir; *p; p++) if (*p == '/') *p = SEP;
        snprintf(paths[0], sizeof paths[0], "%s%caop-level-01.txt", dir, SEP);
        if (readable(paths[0])) break;
    }
    if (d == sizeof dirs / sizeof dirs[0]) {
        fprintf(stderr, "aop: can't find the level files beside the program "
                "(levels%caop-level-01.txt)\n", SEP);
        return 1;
    }

    args[0] = argv[0];
    for (i = 0; i < LEVELS; i++) {
        snprintf(paths[i], sizeof paths[i], "%s%caop-level-%02d.txt", dir, SEP, i + 1);
        args[i + 1] = paths[i];
    }
    args[LEVELS + 1] = NULL;

    result = aop_main(LEVELS + 1, args);

    if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
        int c;
#if !defined(_WIN32)
        tcflush(fileno(stdin), TCIFLUSH);   /* keys still queued from play */
#endif
        printf("Press Enter.\n");
        fflush(stdout);
        while ((c = getchar()) != EOF && c != '\n') {}
    }
    return result;
}
