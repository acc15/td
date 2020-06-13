#include <td/event/listener_registry.h>

#include <iostream>

namespace td {

bool listener_registry::erase_et_entry(emitter* e, event_type t, const event_type_map::iterator& et_iter, event_type_map& et_map) {
    if (erase_if_nested_empty(et_iter, et_map)) {
        call_activations(e, t, false);
        return true;
    }
    return false;
}

void listener_registry::unlink_by_iterators(const std::pair<emitter*, const listener_iterator_map&>& le_entry) {
    const auto e_iter = _e.find(le_entry.first);
    if (e_iter == _e.end()) {
        return;
    }
    for (const auto& event_ref: le_entry.second) {
        const auto et_iter = e_iter->second.find(event_ref.first);
        if (et_iter == e_iter->second.end()) {
            continue;
        }
        et_iter->second.erase(event_ref.second);
        erase_et_entry(le_entry.first, event_ref.first, et_iter, e_iter->second);
    }
    erase_if_nested_empty(e_iter, _e);
}

bool listener_registry::check_activation_deps(emitter* e, const std::unordered_set<event_type>& deps) const {
    const auto e_iter = _e.find(e);
    if (e_iter == _e.end()) {
        return deps.empty();
    }

    const event_type_map& et_map = e_iter->second;
    for (const event_type& t: deps) {
        if (et_map.count(t) > 0) {
            return false;
        }
    }
    return true;
}

void listener_registry::call_activations(emitter* e, event_type t, bool active) const {

    auto a_iter = _a.find(e);
    if (a_iter == _a.end()) {
        return;
    }

    auto at_range = a_iter->second.equal_range(t);
    for (auto at_iter = at_range.first; at_iter != at_range.second; ++at_iter) {
        if (!check_activation_deps(e, at_iter->second.second)) {
            continue;
        }
        at_iter->second.first(active);
    }

}

listener_registry& listener_registry::get() {
    static listener_registry i;
    return i;
}

listener_tag listener_registry::link(emitter* e, listener* l, event_type t, const handler_fn& h, size_t p) {

    priority_map& p_map = _e[e][t];
    if (p_map.empty()) {
        call_activations(e, t, true);
    }
    
    const auto iter = p_map.insert(std::make_pair(p, std::make_pair(l, h)));
    _l[l][e].insert(std::make_pair(t, iter));
    return { e, l, t, iter };
}

void listener_registry::emit(emitter* e, const event& ev) {
    const auto e_iter = _e.find(e);
    if (e_iter == _e.end()) {
        return;
    }

    const event_type t = ev.type();
    const auto et_iter = e_iter->second.find(t);
    if (et_iter == e_iter->second.end()) {
        return;
    }
    for (const auto& p_entry: et_iter->second) {
        p_entry.second.second(ev);
    }
}

void listener_registry::unlink(emitter* e) {

    const auto e_iter = _e.find(e);
    if (e_iter == _e.end()) {
        return;
    }

    for (const auto& et_entry: e_iter->second) {
        for (const auto& etp_entry: et_entry.second) {
            listener* l = etp_entry.second.first;

            const auto l_iter = _l.find(l);
            if (l_iter == _l.end()) {
                continue;
            }

            l_iter->second.erase(e);
            erase_if_nested_empty(l_iter, _l);
        }

        // calling activation for each registered event type - later we will remove whole emitter map
        if (!et_entry.second.empty()) {
            call_activations(e, et_entry.first, false);
        }

    }
    _e.erase(e);

}

void listener_registry::unlink(listener* l) {
    const auto l_iter = _l.find(l);
    if (l_iter == _l.end()) {
        return;
    }
    for (const auto& le_entry: l_iter->second) {
        unlink_by_iterators(le_entry);
    }
    _l.erase(l);
}


void listener_registry::unlink(listener* l, emitter* e) {

    const auto l_iter = _l.find(l);
    if (l_iter == _l.end()) {
        return;
    }

    const auto le_iter = l_iter->second.find(e);
    if (le_iter == l_iter->second.end()) {
        return;
    }

    unlink_by_iterators(*le_iter);

    l_iter->second.erase(le_iter);
    erase_if_nested_empty(l_iter, _l);

}

void listener_registry::unlink(const listener_tag& t) {

    const auto e_iter = _e.find(t.e);
    if (e_iter != _e.end()) {
        const auto et_iter = e_iter->second.find(t.t);
        if (et_iter != e_iter->second.end()) {
            et_iter->second.erase(t.i);
            if (erase_et_entry(t.e, t.t, et_iter, e_iter->second)) {
                erase_if_nested_empty(e_iter, _e);
            }
        }
    }

    const auto l_iter = _l.find(t.l);
    if (l_iter != _l.end()) {
        const auto le_iter = l_iter->second.find(t.e);
        if (le_iter != l_iter->second.end()) {
            const auto let_range = le_iter->second.equal_range(t.t);
            for (auto iter = let_range.first; iter != let_range.second; ++iter) {
                if (iter->second != t.i) {
                    continue;
                }
                iter = le_iter->second.erase(iter);
                if (erase_if_nested_empty(le_iter, l_iter->second)) {
                    erase_if_nested_empty(l_iter, _l);
                }
                break;
            }
        }
    }
}

std::unordered_set<emitter*> listener_registry::emitters_of(const listener* l) const {
    std::unordered_set<emitter*> res;
    const auto i = _l.find(const_cast<listener*>(l));
    if (i == _l.end()) {
        return res;
    }
    for (const auto& emitter_entry : i->second) {
        res.insert(emitter_entry.first);
    }
    return res;
}

std::unordered_set<listener*> listener_registry::listeners_of(const emitter* e, event_type t) const {
    std::unordered_set<listener*> res;
    const auto i = _e.find(const_cast<emitter*>(e));
    if (i == _e.end()) {
        return res;
    }
    const auto j = i->second.find(t);
    if (j == i->second.end()) {
        return res;
    }
    for (const auto& priority_entry : j->second) {
        res.insert(priority_entry.second.first);
    }
    return res;
}

std::unordered_set<listener*> listener_registry::listeners_of(const emitter* e) const {
    std::unordered_set<listener*> res;
    const auto i = _e.find(const_cast<emitter*>(e));
    if (i == _e.end()) {
        return res;
    }
    for (const auto& event_entry : i->second) {
        for (const auto& priority_entry : event_entry.second) {
            res.insert(priority_entry.second.first);
        }
    }
    return res;
}

bool listener_registry::empty() const {
    return _l.empty() && _e.empty() && _a.empty();
}

void listener_registry::activation(emitter* e, event_type t, const activation_fn& fn) {
    _a[e].insert(std::make_pair(t, std::make_pair( fn, std::unordered_set<event_type>() )));
}

void listener_registry::activation(emitter* e, const std::unordered_set<event_type>& ts, const activation_fn& fn) {
    for (const event_type& t: ts) {
        std::unordered_set deps(ts);
        deps.erase(t);
        _a[e].insert(std::make_pair(t, std::make_pair( fn, deps )));
    }
}

bool listener_registry::has_activations(emitter *e) const {
    return _a.count(e) > 0;
}

void listener_registry::remove_activations(emitter* e) {
    _a.erase(e);
}

}