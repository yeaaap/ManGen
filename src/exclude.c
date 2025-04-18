/**
* @file exclude.c
 * @brief Простая система исключения файлов по имени.
 *
 * Реализует механизм исключения отдельных файлов или директорий
 * по строковому совпадению. Поддерживается только одно исключение.
 */

#include "exclude.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/// Строка с шаблоном имени, который следует исключать
static char *raw_pattern = NULL;

/**
 * @brief Устанавливает шаблон для исключения файлов.
 *
 * Шаблон сохраняется как строка и будет сравниваться с именами файлов.
 *
 * @param pattern Строка, которую нужно исключить из обработки.
 */
void set_exclude_pattern(const char *pattern) {
    raw_pattern = strdup(pattern);
}

/**
 * @brief Проверяет, следует ли исключить файл по его имени.
 *
 * Возвращает `true`, если имя файла точно совпадает с установленным шаблоном.
 *
 * @param filename Имя файла.
 * @return `true` если файл нужно исключить, иначе `false`.
 */
bool should_exclude(const char *filename) {
    if (!raw_pattern) return false;
    return strcmp(filename, raw_pattern) == 0;
}
