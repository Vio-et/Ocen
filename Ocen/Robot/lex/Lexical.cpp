#include "Lexical.h"

TokenStream* lexical(string rslPath)
{
    bool isLexError = false, isWhite = false;   // 记录是否存在词法错误、记录记号是否为空白符
    int  lines = 0, mark;                       // 保存当前处理的行号、保存记号的类型
    string word, attribute;                     // 保存脚本中的单词、保存记号的属性
    auto* tokens = new TokenStream;  // 保存通过脚本生成的记号流  
    ifstream wordStream(rslPath);               // 用脚本打开的文件流

    // 若脚本文件正常打开，则生成记号流；否则，进行对应报错
    if (wordStream.is_open()) {
        // 从文件流读入单词，分析单词的类型，将其转化为记号
        for (int i = 0; wordStream >> word; i++) {
            if (word == "\n") {
                isWhite = true;
                lines++;
            }
            else if (word == "State")
                mark = KEYWORD, attribute = "State";
            else if (word == "Write")
                mark = KEYWORD, attribute = "Write";
            else if (word == "Read")
                mark = KEYWORD, attribute = "Read";
            else if (word == "Rouse")
                mark = KEYWORD, attribute = "Rouse";
            else if (word == "Default")
                mark = KEYWORD, attribute = "Default";
            else if (word == "Exit")
                mark = KEYWORD, attribute = "Exit";
            else if (word == ">>")
                mark = IO, attribute = "IN";
            else if (word == "<<")
                mark = IO, attribute = "OUT";
            else if (word[word.size() - 1] == ':' && isAlpha(word.substr(0, word.size() - 1)))
                mark = STATE, attribute = word.substr(0, word.size() - 1);
            else if (word[0] == '{' && word[word.size() - 1] == '}')
                mark = IDENTIFY, attribute = word;
            else if (word[0] == '\"' && word[word.size() - 1] == '\"')
                mark = STRING, attribute = word.substr(1, word.size() - 2);
            else if (isNumber(word))
                mark = NUMBER, attribute = word;
            else if (isAlpha(word))
                mark = PROCESS, attribute = word;
            else {
                isLexError = true;
                cout << "Lexical error, line " << lines << ", text is " << word << endl;
            }

            // 如果不存在词法错误，则将记号添加至记号流中
            if (!isLexError && !isWhite)
                tokens->append(mark, attribute);
            else {
                isLexError = false;
                isWhite = false;
            }
        }

        // 词法分析结束，关闭文件流
        wordStream.close();
    }
    else cout << "Error, Can't open script file." << endl;

    // 返回生成的记号流
    return tokens;
}

/**
 * @brief 记号Token类的构造函数
 * @param mark 记号的类型，默认为-1
 * @param attr 记号的属性，默认为空
*/
Token::Token(int mark, string attr)
{
    this->mark = mark;
    this->attribute = attr;
}

/**
 * @brief 记号Token类的拷贝构造函数
 * @param token 被拷贝的记号
*/
Token::Token(const Token& token)
{
    this->mark = token.mark;
    this->attribute = token.attribute;
}

/**
 * @brief 记号Token类的默认析构函数
*/
Token::~Token() = default;

/**
 * @brief 设置记号的类型
 * @param i 保存记号类型的整数
*/
void Token::setMark(int i)
{
    this->mark = i;
}

/**
 * @brief 设置记号的属性
 * @param attr 保存记号属性的字符串
*/
void Token::setAttribute(string attr)
{
    this->attribute = attr;
}

/**
 * @brief 获取记号的类型
 * @return 保存记号类型的整型
*/
int Token::getMark() const
{
    return this->mark;
}

/**
 * @brief 获取记号的属性
 * @return 保存记号属性的字符串
*/
string Token::getAttribute()
{
    return this->attribute;
}

/**
 * @brief 格式化输出记号的信息
*/
void Token::output() {
    cout << this->mark << " ";
    cout << this->attribute << endl;
}

/**
 * @brief 记号流TokenStream类的构造函数
 * @param size 保存记号流大小的整型
*/
TokenStream::TokenStream(int size)
{
    this->size = size;
    this->length = 0;
    this->tokens = new Token[size];
}

/**
 * @brief 记号流TokenStream的拷贝构造函数
 * @param tokenStream 被拷贝的记号流
*/
TokenStream::TokenStream(TokenStream& tokenStream)
{
    this->length = tokenStream.length;
    this->size = tokenStream.size;
    this->tokens = new Token[size];

    for (int i = 0; i < length; i++)
        this->tokens[i] = tokenStream.tokens[i];
}

/**
 * @brief 记号流TokenStream的析构函数
*/
TokenStream::~TokenStream()
{
    delete[] this->tokens;
}

/**
 * @brief 获取记号流的长度
 * @return 保存记号流长度的整型
*/
int TokenStream::getLength() const
{
    return this->length;
}

/**
 * @brief 按索引获取记号流中的记号
 * @param index 目标记号的索引
 * @return 返回记号流中索引对应的记号
*/
Token TokenStream::getToken(int index)
{
    return this->tokens[index];
}

/**
 * @brief 向记号流中增加记号
 * @param mark 记号的类型
 * @param attribute 记号的属性
*/
void TokenStream::append(int mark, string attribute)
{
    // 若记号流的长度等于最大空间，则先扩增空间再添加新记号
    if (this->length == this->size) {
        auto* extendTokens = new Token[this->size + 30];

        for (int i = 0; i < this->size; i++) {
            extendTokens[i].setMark(this->tokens[i].getMark());
            extendTokens[i].setAttribute(this->tokens[i].getAttribute());
        }

        delete[] this->tokens;

        this->tokens = extendTokens;
        this->size += 30;
    }

    this->tokens[length].setMark(mark);
    this->tokens[length].setAttribute(attribute);
    this->length++;
}

/**
 * @brief 格式化输出记号流
*/
void TokenStream::output()
{
    for (int i = 0; i < length; i++)
        this->tokens[i].output();
}

/**
 * @brief 检查字符串是否全部由字母组成
 * @param uncheck 待检查的字符串
 * @return 若待查字符串全由字母组成，返回true；否则，返回false；
*/
bool isAlpha(string uncheck)
{
    bool isAlpha = true;

    for (int i = 0; i < uncheck.size() - 1 && isAlpha; i++) {
        if (!isalpha(uncheck[i]))
            isAlpha = false;
    }

    return isAlpha;
}

/**
* @brief  检查字符串是否全部由数字组成
* @param uncheck   待检查的字符串
* @return 若待查字符串全由数字组成，返回true；否则，返回false；
*/
bool isNumber(string uncheck)
{
    bool isNumber = true;

    for (int i = 0; i < uncheck.size() - 1 && isNumber; i++) {
        if (uncheck[i] < '0' || uncheck[i] > '9')
            isNumber = false;
    }

    return isNumber;
}