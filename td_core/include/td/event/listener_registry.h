#pragma once

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "event.h"

namespace td {

class listener;
class emitter;

class listener_registry {
public:
    typedef std::function<void(const event&)> handler_fn;
    typedef std::function<void(bool)> activation_fn;

private:
    listener_registry() = default;

    typedef std::pair<listener*, handler_fn> listener_pair;
    typedef std::multimap<size_t, listener_pair, std::greater<> > priority_map;
    typedef std::unordered_map<event_type, priority_map> event_type_map;
    typedef std::unordered_map<emitter*, event_type_map> emitter_map;

    typedef std::unordered_multimap<event_type, priority_map::iterator> listener_iterator_map;
    typedef std::unordered_map<emitter*, listener_iterator_map> listener_emitter_map;
    typedef std::unordered_map<listener*, listener_emitter_map> listener_ref_map;

    typedef std::unordered_map<emitter*,
        std::unordered_multimap<event_type,
            std::pair<activation_fn, std::unordered_set<event_type>>
        >
    > activation_map;

    emitter_map _e;
    listener_ref_map _l;
    activation_map _a;

    template <typename Col>
    static bool erase_if_nested_empty(const typename Col::iterator& i, Col& c) {
        if (!i->second.empty()) {
            return false;
        }
        c.erase(i);
        return true;
    }

    bool erase_et_entry(emitter* e, event_type t, const event_type_map::iterator& et_iter, event_type_map& et_map);

    void unlink_by_iterators(const std::pair<emitter*, const listener_iterator_map&>& le_entry);

    bool check_activation_deps(emitter* e, const std::unordered_set<event_type>& deps) const;
    void call_activations(emitter* e, event_type t, bool active) const;

public:
    struct listener_tag {
        emitter* e;
        listener* l;
        event_type t;
        priority_map::iterator i;
    };

    static listener_registry& get();

    /**
     * Links emitter, listener together supplying event type and handling function
     * @param e emitter
     * @param l listener
     * @param t event type
     * @param h handling function
     * @param p listener priority
     * @return listener tag - by using this tag listener can be deleted later (by using mute(tag))
     */
    listener_tag link(emitter* e, listener* l, event_type t, const handler_fn& h, size_t p);

    /**
     * Emits single event from emitter
     * @param e emitter
     * @param ev event
     */
    void emit(emitter* e, const event& ev);

    /**
     * Removes all links to emitter
     * @param e emitter
     */
    void unlink(emitter* e);

    /**
     * Removes all links to listener
     * @param l listener
     */
    void unlink(listener* l);

    /**
     * Removes all links of listener to emitter
     * @param l listener
     * @param e emitter
     */
    void unlink(listener* l, emitter* e);

    /**
     * Removed link to listener_tag (single listener removal)
     * @param t tag
     */
    void unlink(const listener_tag& t);

    /**
     * Returns all emitters for specific listener
     * @param l listener
     * @return set of currently active emitters
     */
    std::unordered_set<emitter*> emitters_of(const listener* l) const;

    /**
     * Returns all listeners of emitter and event type
     * @param e emitter
     * @param t event type
     * @return set of currently active listeners
     */
    std::unordered_set<listener*> listeners_of(const emitter* e, event_type t) const;

    /**
     * Returns all listeners of emitter
     * @param e emitter
     * @return set of currently active listeners
     */
    std::unordered_set<listener*> listeners_of(const emitter* e) const;

    /**
     * Checks whether registry is empty or not
     * @return true if registry doesn't contains any refs to listener and emitter
     */
    bool empty() const;

    /**
     * Allows to monitor when listener attached and detached for specific emitter and event_type.
     * Useful for optimizing performance and listen for MOUSE_MOVE, RENDER
     * and other intensive events
     * only when it's actually needed - when at least one listener linked
     * @param e emitter
     * @param t event type
     * @param fn activation function
     */
    void activation(emitter* e, event_type t, const activation_fn& fn);

    /**
     * Allows to monitor when
     *  [*] at least one listener attached to one of provided event_types
     *  [*] all listeners detached for specific emitter and provided event_types
     * Useful for optimizing performance and listen for MOUSE_MOVE, RENDER
     * and other intensive events
     * only when it's actually needed - when at least one listener linked
     * @param e emitter
     * @param t set of event type
     * @param fn activation function
     */
    void activation(emitter* e, const std::unordered_set<event_type>& t, const activation_fn& fn);

    /**
     * Checks whether specified emitter has at least one activation function
     * @param e emitter
     * @return true if e has at least one activation function
     */
    bool has_activations(emitter* e) const;

    /**
     * Removes activation functions for specific emitter
     * @param e emitter
     */
    void remove_activations(emitter* e);

};

typedef listener_registry::listener_tag listener_tag;

}

