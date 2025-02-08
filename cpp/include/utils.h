#pragma once

#include <format>
#include <unordered_map>
#include <vector>

template <typename T>
struct std::formatter<std::vector<T>> {
    bool curly = false;
    std::string value_format;

    template <class ParseContext>
    constexpr auto parse(ParseContext &ctx) {
        value_format = "{:";
        for (auto it = begin(ctx); it != end(ctx); ++it) {
            char c = *it;
            if (c == 'c')
                curly = true;
            else
                value_format += c;
            if (c == '}')
                return it;
        }
        return end(ctx);
    }

    template <typename FmtContext>
    auto format(const std::vector<T> &v, FmtContext &ctx) {
        bool first = true;
        for (const auto &elem : v) {
            if (!first) {
                ctx.out().put(',');
            }
            first = false;
            std::formatter<T>::format(elem, ctx);
        }
        return ctx.out();
        /*
                auto out = ctx.out();
                *out++ = curly ? '{' : '[';
                if (v.size() > 0) format_to(out, value_format, v[0]);
                for (int i = 1; i < v.size(); ++i)
                    format_to(out, ", " + value_format, v[i]);
                return format_to(out, curly ? "}}" : "]");
        */
    }
};

template <>
struct std::formatter<std::vector<bool>> {
    template <class ParseContext>
    constexpr auto parse(ParseContext &ctx) {
        for (auto it = begin(ctx); it != end(ctx); ++it) {
            char c = *it;
            if (c == '}')
                return it;
        }
        return end(ctx);
    }

    template <typename FmtContext>
    auto format(const std::vector<bool> &v, FmtContext &ctx) {
        auto out = ctx.out();
        if (v.size() == 0)
            format_to(out, "{}", "0");
        for (int i = 0; i < v.size(); ++i)
            format_to(out, "{}", v[i] ? '0' : '1');
        return out;
    }
};