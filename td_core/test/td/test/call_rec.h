#pragma once

#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <functional>

#include <catch2/catch.hpp>

template <typename T>
struct s_join {
    std::string sep;
    const std::vector<T>& vals;

    s_join(const std::string& sep, const std::vector<T>& vals): sep(sep), vals(vals) {
    }
};

class call_rec {
public:
    static constexpr intptr_t ANY_PTR = -1;
    static constexpr const char* DESTRUCTOR = "destructor";

    typedef std::pair<intptr_t, std::string> call;

    static void opt_rec(call_rec* rec, const call& call);
    static bool match(const call& c, const call& o);

    static inline call_rec::call obj(void* ptr, const std::string& name) {
        return std::make_pair(reinterpret_cast<intptr_t>(ptr), name);
    }

    static inline call_rec::call any_obj(const std::string& name) {
        return std::make_pair(call_rec::ANY_PTR, name);
    }

    static inline call_rec::call destructor() {
        return std::make_pair(ANY_PTR, DESTRUCTOR);
    }

    static inline call_rec::call destructor(void* ptr) {
        return std::make_pair(reinterpret_cast<intptr_t>(ptr), DESTRUCTOR);
    }

    bool has_destructor_call(void* ptr) const;
    bool has_call(const call& c) const;
    void rec(const call& call);

    std::vector<call> calls;

};

std::ostream& operator<<(std::ostream& s, const call_rec::call& call);

template <typename T>
std::ostream& operator<<(std::ostream& s, const s_join<T>& op) {
    if (!op.vals.empty()) {
        s << op.vals[0];
    }
    for (size_t i = 1; i < op.vals.size(); ++i) {
        s << op.sep << op.vals[i];
    }
    return s;
}

namespace Catch {
    template<>
    struct StringMaker<call_rec> {
        static std::string convert( call_rec const& value ) {
            std::ostringstream ss;
            ss << "call_recorder("
                << s_join(", ", value.calls)
                << ")";
            return ss.str();
        }
    };
}

class has_calls_in_order_matcher : public Catch::MatcherBase<call_rec> {
    std::vector<call_rec::call> _calls;
public:
    has_calls_in_order_matcher(const std::vector<call_rec::call>& calls);
    bool match(call_rec const& cr) const override;
    std::string describe() const override;
};

// The builder function
inline has_calls_in_order_matcher has_calls_in_order(const std::vector<call_rec::call>& calls) {
    return has_calls_in_order_matcher( calls );
}



