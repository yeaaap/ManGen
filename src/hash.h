/**
* @file hash.h
 * @brief Интерфейс функций хеширования файлов и строк.
 *
 * Содержит декларации функций для вычисления простого хеша содержимого файла
 * и контрольной суммы по алгоритму CRC32.
 */

#ifndef HASH_H
#define HASH_H

#include "errors.h"

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
unsigned int compute_file_hash(const char *file_path, int *error);

/**
 * @brief Вычисляет CRC32-хеш строки.
 *
 * Использует стандартный полином 0xEDB88320 для подсчета контрольной суммы.
 *
 * @param data Указатель на буфер данных.
 * @param len Длина буфера.
 * @return CRC32-хеш в виде 32-битного беззнакового числа.
 */
unsigned int compute_crc32(const char *data, size_t len);

#endif
