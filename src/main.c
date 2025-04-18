/**
* @file main.c
 * @brief Утилита mangen — генератор манифеста файлов каталога.
 *
 * mangen рекурсивно обходит директорию, выводит список файлов и может вычислять
 * контрольную сумму всего манифеста. Поддерживаются исключения файлов и директорий,
 * режим проверки контрольной суммы, а также вывод версии и справки.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "walker.h"
#include "hash.h"
#include "exclude.h"
#include "version.h"
#include "errors.h"


/// Размер буфера манифеста
#define BUFSIZE 1000000

/// Флаг режима генерации контрольной суммы
static bool check_mode = false;

/// Буфер для хранения манифеста
static char manifest_buf[BUFSIZE];

/// Текущая длина манифеста
static size_t manifest_len = 0;

/**
 * @brief Выводит справку по использованию утилиты.
 *
 * Печатает информацию о возможных параметрах командной строки.
 */
void print_help() {
    printf("Usage: ./mangen [DIR_PATH] [OPTIONS]\n");
    printf("Options:\n");
    printf("  -h            Show this help message\n");
    printf("  -v            Show version info\n");
    printf("  -e [PATTERN]  Exclude files/directories by name\n");
    printf("  --check      Print manifest and checksum\n");
}

/**
 * @brief Печатает строку в стандартный вывод либо накапливает в буфере.
 *
 * Если активен режим `--check`, строки сохраняются в буфере для вычисления контрольной суммы.
 * Иначе строка сразу выводится на экран.
 *
 * @param line Строка для вывода.
 * @return SUCCESS при успешной обработке, FAILURE при переполнении буфера.
 */
int print_line(const char *line) {
    if (check_mode) {
        const size_t len = strlen(line);
        if (manifest_len + len + 1 < sizeof(manifest_buf)) {
            strcpy(&manifest_buf[manifest_len], line);
            manifest_len += len;
            manifest_buf[manifest_len++] = '\n';
        }
        else {
            fprintf(stderr, "Manifest buffer overflow\n");
            return FAILURE;
        }
    } else {
        puts(line);
    }
    return SUCCESS;
}

/**
 * @brief Точка входа в программу mangen.
 *
 * Обрабатывает аргументы командной строки, вызывает обход директории и, при необходимости,
 * печатает контрольную сумму по содержимому манифеста.
 *
 * @param argc Количество аргументов.
 * @param argv Массив аргументов.
 * @return SUCCESS при нормальном завершении, либо код ошибки.
 */
int main(int argc, char *argv[]) {
    const char *path = ".";

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0) {
            print_help();
            return 0;
        } else if (strcmp(argv[i], "-v") == 0) {
            printf("mangen version: %s\n", VERSION);
            return 0;
        } else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            set_exclude_pattern(argv[++i]);
        } else if (strcmp(argv[i], "--check") == 0) {
            check_mode = true;
        } else {
            path = argv[i];
        }
    }

    int rc = walk_directory(path, path);
    if (rc) return rc;

    if (check_mode) {
        manifest_buf[manifest_len] = 0;
        fputs(manifest_buf, stdout);
        const unsigned int checksum = compute_crc32(manifest_buf, manifest_len);
        printf("\nMANIFEST_CHECKSUM : %08X\n", checksum);
    }

    return SUCCESS;
}
