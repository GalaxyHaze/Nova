//
// Created by al24254 on 07/11/2025.
//

#ifndef NOVA_TOKENIZER_H
#define NOVA_TOKENIZER_H
#include <string>
#include <string_view>

typedef unsigned long count_t;
typedef void(*lambda_t)();

struct Info {
    count_t index{1};
    count_t line{1};

    auto operator++(int) {
        const auto tmp = index++;
        return tmp;
    }

    auto& operator+=(const count_t offset) {
        index += offset;
        return *this;
    }

    explicit operator count_t() const {
        return index;
    }

    void newLine() {
        ++line;
        index = 1;
    }


};

class Tokenizer {

    static bool has(const Info& info, const std::string_view src) {
        return static_cast<count_t>(info) - 1 < src.size();
    }

    static char peek(const Info& info, const std::string_view src, const count_t offset = 0) {
        if (info.index + offset - 1  >= src.size()) {
            return '\0';
        }

        return src[info.index + offset - 1];
    }

    static void consume(Info& info, const count_t offset = 1) {
        info.index += offset;
    }

    static void skipMultiLine(Info& info, std::stringstream& src, std::string line) {

        auto logic = [&] {
            while (has(info, line)) {

                if (peek(info, line) == '*' && peek(info, line, 1) == '/') {
                    consume(info, 2);
                    return;
                }
                consume(info);
            }
            info.newLine();
        };

        do {
            logic();
        }   while (std::getline(src, line));

        throw std::runtime_error("Unterminated multi-line comment at line " + std::to_string(info.line));
    }

public:

    static auto tokenize(std::stringstream& src) {
        std::string line;
        Info info{};
        while (std::getline(src, line)) {
            while (has(info, line)) {

                if (peek(info,line) == '/' && peek(info,line,1) == '/') {
                    consume(info, 2);
                    break; // Skip single-line comments
                }

                if (peek(info,line) == '/' && peek(info,line,1) == '*') {
                    consume(info, 2);
                    skipMultiLine(info, src, line);
                    continue;
                }

                consume(info);
            }
            info.newLine();
        }
    }

};

#endif //NOVA_TOKENIZER_H