/**
* @file hash.c
 * @brief Реализация функций хеширования для файлов и строк.
 *
 * Содержит простую функцию хеширования содержимого файлов, а также реализацию CRC32.
 * Используется для генерации контрольных сумм файлов в манифесте.
 */

#include <stdio.h>
#include "hash.h"

/**
 * @brief Вычисляет хеш-функцию для содержимого файла.
 *
 * Использует простое побитное XOR-смешивание и ротацию битов.
 * Это не криптографический хеш, но достаточно для контроля целостности.
 *
 * @param file_path Путь к файлу.
 * @param error Указатель на переменную ошибки: устанавливается в `FAILURE` при ошибке открытия.
 * @return Хеш-значение файла. При ошибке — 0.
 */
unsigned int compute_file_hash(const char *file_path, int *error) {
    FILE *f = fopen(file_path, "rb");
    if (!f) {
        perror(file_path);
        *error = FAILURE;
        return 0;
    }

    unsigned int hash = 0;
    int ch;
    while ((ch = fgetc(f)) != EOF) {
        hash ^= ch;
        hash = (hash << 5) | (hash >> 27);  ///< Ротация влево на 5 бит
    }

    fclose(f);
    return hash;
}

/**
 * @brief Вычисляет CRC32-хеш строки.
 *
 * Использует стандартный полином 0xEDB88320 для подсчета контрольной суммы.
 *
 * @param data Указатель на буфер данных.
 * @param len Длина буфера.
 * @return CRC32-хеш в виде 32-битного беззнакового числа.
 */
unsigned int compute_crc32(const char *data, const size_t len) {
    unsigned int crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; ++i) {
        crc ^= (unsigned char)data[i];
        for (int j = 0; j < 8; ++j)
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
    }
    return ~crc;
}
