# ManGen

**ManGen** — это утилита командной строки на C для генерации манифеста файлов директории с контрольной суммой.  
Позволяет исключать отдельные файлы по имени, сравнивать целостность данных и генерировать документацию по исходному коду.

---

## Сборка:
```bash
 cmake -B build/release
 cmake --build build/release
 cd build/release
 ./mangen
```

## Документация:
```bash
 doxygen Doxyfile
```

## Тесты:
```bash
 cmake -B build/debug
 cmake --build build/debug
 cd build/debug
 ctest --output-on-failure
```
