#define FMT_HEADER_ONLY
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <cctype>
#include <regex>

#include <fmt/core.h>
#include <fmt/args.h>

#include "Functions.h"
#include "Error.h"

int main(int argc, char* argv[])
{
    // Устанваливаем кодировку консоли
    system("chcp 65001>nul");

    // Проверка количества аргументов командной строки
    // Программа ожидает 2 аргумента (входной и выходной файлы)
    if (argc != 3) {
        cout << "Check arguments count!";
        return 1;
    }

    // Объявление переменной для хранения возможной ошибки
    Error* err = nullptr;

    // Чтение входного файла (первый аргумент командной строки)
    string tree = readInputFile(argv[1], err);
    if (err != nullptr) { // Если возникла ошибка чтения файла
        cout << err->generateErrorMessage() << endl; // Вывести сообщение об ошибке
        return 1;
    }

    // Множество для хранения ошибок парсинга
    set<Error> errors;

    // Парсинг выражения из обратной польской записи в дерево узлов
    Node* node = parseExtensionParseTreeToNodeTree(tree, errors);
    if (!errors.empty()) { // Если список ошибок не пуст
        for (Error error : errors) { // Для каждой возникшей ошибке
            cout << error.generateErrorMessage() << endl; // Вывести сообщение об ошибке
        }

        return 1;
    }

    // Подготовка дерева разбора
    prepareExtensionParseTree(node);

    // Форматирование порядка умножения операндов
    formatMultiplicationOrder(node);

    // Конвертация дерева узлов в TeX-формат
    string tex = convertNodeToTex(node, NULL, true);

    // Добавление необходимой обёртки для TeX-документа
    tex = "\\documentclass{article}\n\\begin{document}\n$ " + tex + " $\n\\end{document}";

    // Запись результата в выходной файл (второй аргумент командной строки)
    err = writeOutputFile(argv[2], tex);
    if (err != NULL) { // Если возникла ошибка записи файла
        cout << err->generateErrorMessage() << endl; // Вывести сообщение об ошибке
        return 1;
    }

    cout << "OK";
    return 0;
}

string readInputFile(string path, Error*& error) {
    ifstream file(path); // Пытаемся открыть файл

    if (!file.is_open()) { // Если файл открыть не удалось
        // Возвращаем ошибку, что доступа к входному файлу нет
        error = new Error(ErrorType::NoAccessToInputFile);
        error->setErrorInputFileWay(path);
        return "";
    }
    
    stringstream buffer; // буфер для чтения содержимого файла
    buffer << file.rdbuf(); // Читаем содержимое файла в буфер

    // Проверяем, не установлены ли флаги ошибок после чтения
    if (!file) {
        // Возвращаем ошибку, что доступа к входному файлу нет
        error = new Error(ErrorType::NoAccessToInputFile);
        error->setErrorInputFileWay(path);
        return "";
    }

    string line = buffer.str(); // Читаем содержимое в строку
    if (line.empty()) { // Если строка пустая
        // Возвращаем ошибку, что входной файл пуст
        error = new Error(ErrorType::InputFileIsEmpty);
        return "";
    }

    if (line.find("\n") != string::npos) { // Если строка имеет символ перевода строки
        // Возвращаем ошибку, что входной файл имеет несколько строк
        error = new Error(ErrorType::MultipleInputLines);
        error->setErrorInputFileWay(path);
        return "";
    }

    // Если всё успешно - возвращаем содержимое
    return line;
}

Error* writeOutputFile(string path, string text) {
    ofstream file(path); // Инициализация нового файла

    if (!file.is_open()) { // Если файл не был создан
        // Возвращаем ошибку, что файл создать не удалось
        Error *error = new Error(ErrorType::ImpossibleToCreateOutputFile);
        error->setErrorOutputFileWay(path);
        return error;
    }

    file << text; // Пытаемся записать строку в файл

    if (!file) { // Если произошла ошибка при записи
        // Возвращаем ошибку, что файл создать не удалось
        Error* error = new Error(ErrorType::ImpossibleToCreateOutputFile);
        error->setErrorOutputFileWay(path);
        return error;
    }

    // Закрываем файл и проверяем, не было ли ошибки
    file.close();
    if (!file) { // Если файл закрылся с ошибками
        // Возвращаем ошибку, что файл создать не удалось
        Error* error = new Error(ErrorType::ImpossibleToCreateOutputFile);
        error->setErrorOutputFileWay(path);
        return error;
    }

    // Если всё успешно - возвращаем NULL
    return NULL;
}

Node* parseExtensionParseTreeToNodeTree(string extensionParseTree, set<Error>& errors) {
    // Стек для построения дерева
    stack<Node*> nodeStack;
    // Поток для чтения строки по токенам
    istringstream stringStream(extensionParseTree);

    string token;  // текущий обрабатываемый токен
    int operandsCount, // количество операндов для текущего оператора
        index = 0; // индекс текущего токена в строке (для сообщений об ошибках)

    // Мапы для хранения дополнительной информации о узлах (для сообщений об ошибках)
    map<Node*, int> nodesIndex;  // Соответствие узла и его индекса в строке
    map<Node*, string> nodesToken; // Соответствие узла и его исходного токена

    // Пока получается обнаружить новый токен
    while (stringStream >> token) {
        NodeType type;  // тип узла
        Node* node;     // создаваемый узел

        if (isOperator(token, &type)) { // Если токен является оператором
            // Получаем количество операндов для данного оператора
            operandsCount = Node::operatorTypeToOperandCount.at(type);

            // Проверяем, достаточно ли операндов в стеке
            if (nodeStack.size() < operandsCount) {
                // Создаем ошибку о недостающем операнде
                Error error(ErrorType::MissingOperand);
                error.setErrorIndex(index);
                error.setErrorValue(token);
                errors.insert(error);
                return nullptr;  // Прерываем выполнение при ошибке
            }

            // Извлекаем необходимое количество операндов из стека
            vector<Node*> operands;
            for (int i = 0; i < operandsCount; i++) {
                // Вставляем в начало, чтобы сохранить порядок операндов
                operands.insert(operands.begin(), nodeStack.top());
                nodeStack.pop();
            }

            // Создаем новый узел-оператор с извлеченными операндами
            node = new Node(type, operands);
            // Сохраняем дополнительную информацию о узле
            nodesIndex[node] = index;
            nodesToken[node] = token;
        }
        else if (isOperand(token, &type)) { // Если токен является операндом
            // Создаем новый узел-операнд
            node = new Node(type, token);
            // Сохраняем дополнительную информацию о узле
            nodesIndex[node] = index;
            nodesToken[node] = token;
        }
        // Если токен не оператор и не операнд
        else {
            // Возвращаем ошибку, что дерево разбора выражения содержит недопустимый элемент
            Error error(ErrorType::InvalidExpressionParseTreeElement);
            error.setErrorIndex(index);
            error.setErrorValue(token);
            errors.insert(error);
            return nullptr;  // Прерываем выполнение при ошибке
        }

        // Помещаем созданный узел в стек
        nodeStack.push(node);
        nodesIndex.insert({ node, index });
        index++;  // Увеличиваем индекс для следующего токена
    }

    // Возвращаем ошибку
    if (nodeStack.empty()) {
        errors.insert(Error(ErrorType::InputFileIsEmpty));
        return nullptr;
    }

    // Проверка на лишние операнды
    if (nodeStack.size() > 1) { // Если в стеке осталось больше 1 элемента
        nodeStack.pop();  // Оставляем один элемент для сообщения об ошибке

        // Создаем ошибки для всех лишних операндов
        while (!nodeStack.empty()) {
            Node* node = nodeStack.top();

            Error error(ErrorType::ExtraOperand);
            error.setErrorIndex(nodesIndex.at(node));
            error.setErrorValue(nodesToken.at(node));
            errors.insert(error);

            nodeStack.pop();
        }

        return nullptr;
    }

    // Возвращаем корневой узел дерева (единственный оставшийся в стеке)
    return nodeStack.top();
}

bool isOperator(string value, NodeType* type) {
    // Определяем и возвращаем тип узла, если значение является операндом
    if (Node::operatorToNodeType.count(value)) {
        *type = Node::operatorToNodeType.at(value);
        return true;
    }

    return false;
}

bool isOperand(const string value, NodeType* type) {
    // Регулярное выражение для переменной
    regex variable_regex("^[a-zA-Z_][a-zA-Z0-9_]*$");
    if (regex_match(value, variable_regex)) {
        if (type) *type = NodeType::Variable;
        return true;
    }

    // Регулярные выражения для целых чисел
    regex integer_regex("^(0b[01]+|0[0-7]+|0x[0-9a-fA-F]+|\\d+)[LlUu]?$");
    if (regex_match(value, integer_regex)) {
        if (type) *type = NodeType::Integer;
        return true;
    }

    // Регулярные выражения для вещественных чисел
    regex float_regex("^(\\d+\\.\\d*|\\.\\d+|\\d+)([eE][-+]?\\d+)?[FfLl]?$");
    if (regex_match(value, float_regex)) {
        if (type) *type = NodeType::Float;
        return true;
    }

    return false;
}

void prepareExtensionParseTree(Node* startNode) {
    // Обработка не требуется, если узел является операндом
    if (startNode->isOperand()) return;

    // Словарь соответствия составных операторов присваивания простым операторам
    static map<NodeType, NodeType> operators = {
        {NodeType::AddAssign, NodeType::Plus},       // += → +
        {NodeType::SubtractAssign, NodeType::Minus}, // -= → -
        {NodeType::MultiplyAssign, NodeType::Multiply}, // *= → *
        {NodeType::DivideAssign, NodeType::Divide},  // /= → /
        {NodeType::ModAssign, NodeType::Mod},        // %= → %
    };

    // Если текущий узел - составной оператор присваивания
    if (operators.count(startNode->getType())) {
        // Получаем соответствующий простой оператор
        NodeType operatorType = operators.at(startNode->getType());
        // Создаем новый узел для операции
        Node* operationNode = new Node(operatorType, startNode->getOperands());

        // Удаляем последний операнд
        startNode->getOperands().pop_back();
        // Добавляем созданный узел операции как новый последний операнд
        startNode->getOperands().push_back(operationNode);

        // Преобразуем текущий узел в простое присваивание
        startNode->setType(NodeType::Assign);
    }

    // Рекурсивная обработка всех операндов текущего узла
    Node* operand;
    for (int i = 0; i < startNode->getOperands().size(); i++) {
        operand = startNode->getOperands().at(i);
        prepareExtensionParseTree(operand);

        // Оптимизация вложенных операций:
        // 1. Объединение множителей (a*(b*c) → a*b*c)
        // 2. Обьединение при работе с многомерными массивами (A[i][j] → A[i,j])
        if ((startNode->getType() == NodeType::Multiply && operand->getType() == NodeType::Multiply)
            || (i == 0 && startNode->getType() == NodeType::Indexing && operand->getType() == NodeType::Indexing)) {
            // Удаляем вложенный узел
            startNode->getOperands().erase(startNode->getOperands().begin() + i);

            // Вставляем его операнды на место удаленного узла
            startNode->getOperands().insert(startNode->getOperands().begin() + i,
                operand->getOperands().begin(),
                operand->getOperands().end());

            // Корректируем индекс цикла с учетом добавленных операндов
            i += operand->getOperands().size() - 1;
            // Освобождаем память удаленного узла
            delete operand;
        }
    }
}

void formatMultiplicationOrder(Node* startNode) {
    // Если текущий узел представляет операцию умножения
    if (startNode->getType() == NodeType::Multiply) {
        // Сортируем операнды умножения с использованием функции сравнения compareNodes
        sort(startNode->getOperands().begin(),
            startNode->getOperands().end(),
            compareNodes);
    }

    // Рекурсивно обрабатываем все операнды текущего узла
    for (Node* operand : startNode->getOperands()) {
        formatMultiplicationOrder(operand);
    }
}

bool compareNodes(const Node* leftNode, const Node* rightNode) {
    if (leftNode->getMultiplierPrecedence() == rightNode->getMultiplierPrecedence()) {
        if (leftNode->getMultiplierPrecedence() == 3)
            return leftNode->getType() < rightNode->getType();
        
        return leftNode->getValue() < rightNode->getValue();
    }

    return leftNode->getMultiplierPrecedence() < rightNode->getMultiplierPrecedence();
}

string convertNodeToTex(Node* node, Node* degreeNode, const bool isFirstOperand) {
    // Вернуть отформатированное значение, если узел является операндом
    if (node->isOperand()) return node->getTexFormatedValue();
  
    // Вектор операндов текущего узла 
    vector<Node*> operands = node->getOperands();

    // Регулярное выражение для проверки значения "1" в разных форматах
    static const regex one_regex("^1[LlUu]?$|^1\.0[FfLl]?$");

    // Обработка операции умножения
    if (node->getType() == NodeType::Multiply) {
        bool leftInParentheses, rightInParentheses;
        Node* leftOperator, * rightOperator;

        // Обрабатываем первый операнд
        leftOperator = operands[0];
        leftInParentheses = leftOperator->needsParentheses(node, isFirstOperand);

        string tex = putInParenthesesIfNeeded(
            convertNodeToTex(leftOperator, NULL, isFirstOperand || leftInParentheses),
            leftInParentheses
        );

        // Обрабатываем остальные операнды
        for (int i = 1; i < operands.size(); i++) {
            rightOperator = operands[i];
            rightInParentheses = rightOperator->needsParentheses(node, isFirstOperand);

            // Определяем, нужно ли опускать символ умножения
            bool omitBullet = ((leftOperator->getType() == NodeType::Integer ||
                leftOperator->getType() == NodeType::Float ||
                leftOperator->getType() == NodeType::Divide)
                && (rightOperator->getType() == NodeType::Variable ||
                    rightOperator->isLogOrTrigonometricFunction()))
                || leftInParentheses || rightInParentheses;

            if (!omitBullet) {
                tex += Node::operatorTypeToTexValue.at(node->getType());
            }

            tex += putInParenthesesIfNeeded(
                convertNodeToTex(rightOperator, NULL, rightInParentheses),
                rightInParentheses
            );

            leftInParentheses = rightInParentheses;
            leftOperator = rightOperator;
        }

        return tex;
    }
    // Обработка операции возведения в степень
    else if (node->getType() == NodeType::Pow) {
        if (operands[1]->getType() == NodeType::Divide) { // Если значение возводится в дробную степень
            string tex; // строка с результатом

            // Если первый операнд является логарифмической или тригонометрической функцией
            if (operands[0]->isLogOrTrigonometricFunction()) { 
                tex = convertNodeToTex(operands[0], operands[1]->getOperands().at(0), true);
            }
            else { // Иначе
                string operand = putInParenthesesIfNeeded(
                    convertNodeToTex(operands[0], NULL, true),
                    operands[0]->needsParentheses(node, isFirstOperand)
                );
                string degree = convertNodeToTex(operands[1]->getOperands().at(0), NULL, true);

                tex = fmt::vformat(
                    Node::operatorTypeToTexValue.at(node->getType()),
                    fmt::make_format_args(operand, degree)
                );
            }

            // Добавляем корень, если знаменатель не является единицей
            if (!regex_match(operands[1]->getOperands().at(1)->getValue(), one_regex)) {
                tex = "\\sqrt[" + convertNodeToTex(operands[1]->getOperands().at(1), NULL, true) + "]{" + tex + "}";
            }

            return tex;
        }
        else { // Обычное возведение в степень
            // Если первый операнд является логарифмической или тригонометрической функцией
            if (operands[0]->isLogOrTrigonometricFunction()) {
                return convertNodeToTex(operands[0], operands[1], true);
            }
            else { // Иначе
                string operand = putInParenthesesIfNeeded(
                    convertNodeToTex(operands[0], NULL, true),
                    operands[0]->needsParentheses(node, isFirstOperand)
                );
                string degree = convertNodeToTex(operands[1], NULL, true);

                return fmt::vformat(
                    Node::operatorTypeToTexValue.at(node->getType()),
                    fmt::make_format_args(operand, degree)
                );
            }
        }
    }
    // Обработка логарифмических и тригонометрических функций
    else if (node->isLogOrTrigonometricFunction()) {
        string operand = putInParenthesesIfNeeded(
            convertNodeToTex(operands[0], NULL, true),
            operands[0]->needsParentheses(node, isFirstOperand)
        );

        string degree = "";
        // Добавление степени функции
        if (degreeNode != NULL && !regex_match(degreeNode->getValue(), one_regex)) {
            degree = "^{" + convertNodeToTex(degreeNode, NULL, true) + "}";
        }

        return fmt::vformat(
            Node::operatorTypeToTexValue.at(node->getType()),
            fmt::make_format_args(degree, operand)
        );
    }
    // Обработка операции обращения к элементу массива
    else if (node->getType() == NodeType::Indexing) {
        string operand = putInParenthesesIfNeeded(
            convertNodeToTex(operands[0], NULL, true),
            operands[0]->isOperator()
        );
        string indexes = convertNodeToTex(operands[1], NULL, true);

        // Добавление дополнительных индексов через запятую
        for (int i = 2; i < operands.size(); i++) {
            indexes += ", " + convertNodeToTex(operands[i], NULL, true);
        }

        return fmt::vformat(
            Node::operatorTypeToTexValue.at(node->getType()),
            fmt::make_format_args(operand, indexes)
        );
    }
    // Обработка тернарного оператора
    else if (node->getType() == NodeType::Ternary) {
        // Таблица противоположных логических операторов
        static map<NodeType, NodeType> logicalAntonims = {
            {NodeType::GreaterThan, NodeType::LessOrEqual},
            {NodeType::LessThan, NodeType::GreaterOrEqual},
            {NodeType::EqualTo, NodeType::NotEqualTo},
            {NodeType::NotEqualTo, NodeType::EqualTo},
            {NodeType::GreaterOrEqual, NodeType::LessThan},
            {NodeType::LessOrEqual, NodeType::GreaterThan},
        };

        // Строка с условием
        string condition = convertNodeToTex(operands[0], NULL, true);
        // Если условие не имеет явного противоположного оператора
        if (!logicalAntonims.count(operands[0]->getType())) {
            return "\\begin{cases}" + convertNodeToTex(operands[1], NULL, true) + ", " + condition + " = true\\\\"
                + convertNodeToTex(operands[2], NULL, true) + ", " + condition + " = false\\end{cases}";
        }

        // Создаем противоположное условие
        Node oppositeNode = Node(logicalAntonims.at(operands[0]->getType()), operands[0]->getOperands());
        
        // Строка с противоположным условием
        string oppositeCondition = convertNodeToTex(&oppositeNode, NULL, true);

        return "\\begin{cases}" + convertNodeToTex(operands[1], NULL, true) + ", " + condition + "\\\\"
            + convertNodeToTex(operands[2], NULL, true) + ", " + oppositeCondition + "\\end{cases}";
    }
    // Обработка оператора, который содержит 2 операнда
    else if (operands.size() == 2) {
        bool firstOpearandInParentheses = operands[0]->needsParentheses(node, isFirstOperand);
        bool secondOpearandInParentheses = operands[1]->needsParentheses(node, isFirstOperand);
        string operand1 = putInParenthesesIfNeeded(
            convertNodeToTex(operands[0], NULL, isFirstOperand || node->isSeparatingOperator() || firstOpearandInParentheses),
            firstOpearandInParentheses
        );
        string operand2 = putInParenthesesIfNeeded(
            convertNodeToTex(operands[1], NULL, node->isSeparatingOperator() || secondOpearandInParentheses),
            secondOpearandInParentheses
        );

        return fmt::vformat(
            Node::operatorTypeToTexValue.at(node->getType()),
            fmt::make_format_args(operand1, operand2)
        );
    }
    // Обработка оператора, который содержит 1 операнд 
    else {
        bool opearandInParentheses = operands[0]->needsParentheses(node, isFirstOperand);
        string operand = putInParenthesesIfNeeded(
            convertNodeToTex(operands[0], NULL, isFirstOperand || node->isSeparatingOperator() || opearandInParentheses),
            opearandInParentheses
        );
        return fmt::vformat(
            Node::operatorTypeToTexValue.at(node->getType()),
            fmt::make_format_args(operand)
        );
    }
}

string putInParenthesesIfNeeded(string str, const bool putInParentheses) {
    // Вернуть строку в круглых скобках, если требуется
    if (putInParentheses) return "(" + str + ")";

    return str;
}