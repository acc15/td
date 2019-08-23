#pragma once

#include <vector>

#include <td/engine/engine.h>

namespace td {

// TODO make current detach() as "inner" detach. Implement new detach() which also removes object from parent|engine
class engine_object {
public:

    engine_object();
    virtual ~engine_object();

    virtual void on_attach();
    virtual void on_detach();

    /**
     * Called if current object is attached to engine.
     * Implementation MAY:
     * 1. attach all children
     * 2. call custom attach() function
     */
    void attach(engine& e, engine_object* parent);

    /**
     * Called when current object is goind to be detached from engine.
     * Implementation MUST:
     * 1. call custom detach() function
     * 2. remove all listeners from engine
     * 3. detach all children
     */
    void detach();

    /**
     * Checks whether current object is attached to engine hierarchy or not
     */
    bool is_attached() const;

    /**
     * Returns count of children
     * @return children count
     */
    size_t children() const;

    /**
     * Adds object as child object.
     * Automatically calls on_attach() if this is_attached()
     *
     * @param child child object
     * @return this reference to allow call chaining
     */
    engine_object& add(engine_object* child);

    /**
     * Removes and destroys child object (calls destructor)
     * @param index child index
     */
    void remove(size_t index);

    engine_object* child(size_t index);

    /**
     * Detaches child from current object (dont calls destructor).
     * Later object may be attached to another object.
     *
     * Note that after that call developer is responsible
     * to free memory claimed by detached object
     *
     * @param index child index
     * @return detached object pointer
     */
    engine_object* detach(size_t index);

    void listen(event_type type, event_handler handler, size_t priority = 0);
    void mute(event_type type);
    void mute_all();

    engine* get_engine();


private:
    std::vector<engine_object*> _children;
    engine_object* _parent;
    engine* _engine;

};

}