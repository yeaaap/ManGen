/**
 * @file walker.c
 * @brief Реализация рекурсивного обхода директорий с выводом хешей файлов.
 *
 * Этот модуль реализует обход директорий, исключение по имени, а также формирование
 * строк вида `<относительный_путь> : <HASH>` для каждого обычного файла.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "errors.h"
#include "hash.h"
#include "exclude.h"
#include "walker.h"

/// Внешняя функция для печати строки или накопления её в буфере
extern int print_line(const char *);

/**
 * @brief Печатает или сохраняет строку с данными об одном файле.
 *
 * Вычисляет хеш файла по `full_path`, формирует строку вида
 * `<relative_path> : <HASH>` и передает её в `print_line()`.
 *
 * @param relative_path Относительный путь файла (от корня обхода).
 * @param full_path Полный путь до файла на диске.
 * @return SUCCESS при успехе, либо код ошибки.
 */
static int print_file_entry(const char *relative_path, const char *full_path) {
    int rc = SUCCESS;
    char line[LENLINEMAX];

    const unsigned int hash = compute_file_hash(full_path, &rc);
    if (rc) return rc;

    snprintf(line, sizeof(line), "%s : %08X", relative_path + 1, hash);

    return print_line(line);
}

/**
 * @brief Рекурсивно обходит директорию и обрабатывает каждый файл.
 *
 * Пропускает `.` и `..`, исключает файлы по `should_exclude()`, вычисляет хеш
 * всех обычных файлов и вызывает `print_file_entry` для них.
 *
 * @param base_path Текущий путь при рекурсии.
 * @param root_path Путь, от которого начинается относительный вывод.
 * @return SUCCESS при успешном завершении, либо код ошибки.
 */
int walk_directory(const char *base_path, const char *root_path) {
    DIR *dir = opendir(base_path);
    if (!dir) {
        perror(base_path);
        return FAILURE;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) continue;
        if (should_exclude(name)) continue;

        char full_path[PATHLENMAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, name);

        char relative_path[PATHLENMAX];
        snprintf(relative_path, sizeof(relative_path), "%s/%s", base_path + strlen(root_path), name);

        struct stat st;
        if (stat(full_path, &st) == -1) {
            perror(full_path);
            continue;
        }

        int rc = SUCCESS;
        if (S_ISDIR(st.st_mode)) {
            rc = walk_directory(full_path, root_path);
        } else if (S_ISREG(st.st_mode)) {
            rc = print_file_entry(relative_path, full_path);
        }

        if (rc) {
            closedir(dir);
            return rc;
        }
    }

    closedir(dir);
    return SUCCESS;
}
