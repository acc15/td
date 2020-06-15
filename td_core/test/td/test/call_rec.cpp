#include "call_rec.h"

std::ostream& operator<<(std::ostream& s, const call_rec::call& call) {
    s << std::endl << "{[";
    if (call.first == call_rec::ANY_PTR) {
        s << "<any ptr>";
    } else {
        s << "0x" << std::hex << call.first;
    }
    s << "] " << call.second << "}";
    return s;
}

void call_rec::rec(const call& call) {
    calls.push_back(call);
}

void call_rec::opt_rec(call_rec* rec, const call& call) {
    if (rec != nullptr) {
        rec->rec(call);
    }
}

bool call_rec::match(const call& c, const call& o) {
    if (c.second != o.second) {
        return false;
    }
    if (c.first == ANY_PTR || o.first == ANY_PTR) {
        return true;
    }
    return c.first == o.first;
}

bool call_rec::has_destructor_call(void* ptr) const {
    return has_call(destructor(ptr));
}

bool call_rec::has_call(const call& c) const {
    for (const call& t: calls) {
        if (match(t, c)) {
            return true;
        }
    }
    return false;
}

has_calls_in_order_matcher::has_calls_in_order_matcher(const std::vector<call_rec::call>& calls) : _calls(calls) {
}

bool has_calls_in_order_matcher::match( call_rec const& cr ) const {
    if (cr.calls.size() != _calls.size()) {
        return false;
    }
    for (size_t i = 0; i < _calls.size(); i++) {
        if (!call_rec::match(cr.calls[i], _calls[i])) {
            return false;
        }
    }
    return true;
}

std::string has_calls_in_order_matcher::describe() const {
    std::ostringstream ss;
    ss << "has calls in order (" << s_join(", ", _calls) << ")";
    return ss.str();
}