// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#ifndef PCH_H
#define PCH_H
#define _CRT_SECURE_NO_WARNINGS

// Добавьте сюда заголовочные файлы для предварительной компиляции
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <sstream>
#include <AccCtrl.h>
#include <Aclapi.h>
#include <sddl.h>
#define TRACEPATH L"D:/_УЧЁБА_/5сем/СП/Лабораторные/SP_Lab06/storage/HT.trace"

#endif //PCH_H
