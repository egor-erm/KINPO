﻿// pch.cpp: файл исходного кода, соответствующий предварительно скомпилированному заголовочному файлу

#include "pch.h"
#include <algorithm>

// При использовании предварительно скомпилированных заголовочных файлов необходим следующий файл исходного кода для выполнения сборки.

// Сравнение списков ошибок
bool compareErrors(const set<Error>& expected, const set<Error>& real, set<Error>& missing, set<Error>& excess) {
    set_difference(expected.begin(), expected.end(),
        real.begin(), real.end(),
        inserter(missing, missing.end()));

    
    set_difference(real.begin(), real.end(),
        expected.begin(), expected.end(),
        inserter(excess, excess.end()));

    return missing.empty() && excess.empty();
}

// Сравнение деревьев
bool compareTrees(Node* expected, Node* real, const string& path, set<string>& differences) {
    // Если один из узлов nullptr, а другой нет - добавляем различие
    if (expected == nullptr || real == nullptr) {
        if (expected != real) {
            stringstream ss;
            ss << "Difference at " << path << ": ";
            ss << (expected ? "expected exists" : "expected is null") << " vs ";
            ss << (real ? "real exists" : "real is null");
            differences.insert(ss.str());
        }
        return differences.empty();
    }

    // Проверяем тип узла
    if (expected->getType() != real->getType()) {
        stringstream ss;
        ss << "Difference at " << path << ": type differs (";
        ss << static_cast<int>(expected->getType()) << " vs " << static_cast<int>(real->getType()) << ")";
        differences.insert(ss.str());
    }

    // Проверяем значение узла
    if (expected->getValue() != real->getValue()) {
        stringstream ss;
        ss << "Difference at " << path << ": value differs ('";
        ss << expected->getValue() << "' vs '" << real->getValue() << "')";
        differences.insert(ss.str());
    }

    // Проверяем количество операндов
    if (expected->getOperands().size() != real->getOperands().size()) {
        stringstream ss;
        ss << "Difference at " << path << ": number of operands differs (";
        ss << expected->getOperands().size() << " vs " << real->getOperands().size() << ")";
        differences.insert(ss.str());
    }

    // Рекурсивно сравниваем операнды
    size_t maxOperands = max(expected->getOperands().size(), real->getOperands().size());
    for (size_t i = 0; i < maxOperands; ++i) {
        string newPath = path + "/operand[" + to_string(i) + "]";
        compareTrees(
            i < expected->getOperands().size() ? expected->getOperands()[i] : nullptr,
            i < real->getOperands().size() ? real->getOperands()[i] : nullptr,
            newPath, differences
        );
    }

    return differences.empty();
}

wstring stringToWstring(const string& s) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(s);
}