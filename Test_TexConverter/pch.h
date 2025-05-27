// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#ifndef PCH_H
#define PCH_H

#include <set>
#include <sstream>
#include <locale>
#include <codecvt>

#include "../TexConverter/Error.h"
#include "../TexConverter/Node.h"

using namespace std;

// Добавьте сюда заголовочные файлы для предварительной компиляции

bool compareErrors(const set<Error>& expected, const set<Error>& real, set<Error>& missing, set<Error>& excess);

bool compareTrees(Node* expected, Node* real, const string& path, set<string>& differences);

wstring stringToWstring(const string& s);

#endif //PCH_H
