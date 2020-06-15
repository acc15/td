#include <catch2/catch.hpp>
#include <td/engine/obj.h>

#include "../test/call_rec.h"

class test_obj: public td::obj {
public:
    static constexpr char TAG[] = "test_obj";

    call_rec* _cr;

    test_obj(call_rec* cr = nullptr): _cr(cr) {
    }

    virtual ~test_obj() {
        call_rec::opt_rec(_cr, call_rec::destructor(this));
    }

    void attach() override {
        call_rec::opt_rec(_cr, call_rec::obj(this, "attach"));
    }

    void detach() override {
        call_rec::opt_rec(_cr, call_rec::obj(this, "detach"));
    }

};


TEST_CASE("obj") {

    SECTION("setting parent calls attach") {

        call_rec cr;

        td::obj p;

        {
            test_obj c(&cr);
            c.parent(&p);
            REQUIRE(c.parent<td::obj>() == &p);
            REQUIRE(p.children() == std::vector<td::child_obj*>({&c}));
        }

        REQUIRE_THAT(cr, has_calls_in_order({ call_rec::any_obj("attach"), call_rec::destructor() }));
        REQUIRE(p.child_count() == 0);

    }

    SECTION("resetting parent removes child and calls detach") {

        call_rec cr;
        test_obj p;

        {

            test_obj c(&cr);
            c.parent(&p);
            REQUIRE(p.child_count() == 1);
            c.parent(nullptr);
            REQUIRE(p.child_count() == 0);

        }

        REQUIRE_THAT(cr, has_calls_in_order({
            call_rec::any_obj("attach"),
            call_rec::any_obj("detach"),
            call_rec::destructor()
        }));

    }

    SECTION("deleting parent object must also delete all children") {

        call_rec cr;

        test_obj* p = new test_obj();
        test_obj* c1 = new test_obj(&cr);
        test_obj* c2 = new test_obj(&cr);

        c1->parent(p);
        c2->parent(p);

        delete p;
        CHECK(cr.has_destructor_call(c1));
        if (!cr.has_destructor_call(c1)) {
            delete c1;
        }

        CHECK(cr.has_destructor_call(c2));
        if (!cr.has_destructor_call(c2)) {
            delete c2;
        }

        // destructor don't call detach()
        REQUIRE(!cr.has_call(call_rec::any_obj("detach")));
    }

    SECTION("deleting child objects must be in reverse order of add") {

        call_rec cr;
        test_obj* p = new test_obj();
        test_obj* c1 = new test_obj(&cr);
        test_obj* c2 = new test_obj(&cr);
        test_obj* c3 = new test_obj(&cr);

        p->add(c1);
        p->add(c2);
        p->add(c3);

        delete p;
        if (!cr.has_destructor_call(c1)) {
            delete c1;
        }
        if (!cr.has_destructor_call(c2)) {
            delete c2;
        }
        if (!cr.has_destructor_call(c3)) {
            delete c3;
        }

        REQUIRE_THAT(cr, has_calls_in_order({
            call_rec::obj(c1, "attach"),
            call_rec::obj(c2, "attach"),
            call_rec::obj(c3, "attach"),
            call_rec::destructor(c3),
            call_rec::destructor(c2),
            call_rec::destructor(c1)
        }));
    }


    SECTION("switching parent must move child to another parent") {

        call_rec cr;

        test_obj p1;
        test_obj p2;
        test_obj c(&cr);

        c.parent(&p1);
        REQUIRE(c.parent<td::obj>() == &p1);
        REQUIRE(p1.child_count() == 1);

        c.parent(&p2);
        REQUIRE(c.parent<td::obj>() == &p2);
        REQUIRE(p1.child_count() == 0);
        REQUIRE(p2.child_count() == 1);

        REQUIRE_THAT(cr, has_calls_in_order({
            call_rec::obj(&c, "attach"),
            call_rec::obj(&c, "detach"),
            call_rec::obj(&c, "attach")
        }));

    }

    SECTION("tagging object must return tagged object by name, and return null in case of destruction") {

        {
            test_obj obj;
            obj.tag("test");
            REQUIRE(td::obj::by_tag<test_obj>("test") == &obj);
        }

        REQUIRE_THROWS( td::obj::by_tag<test_obj>("test") );

    }

    SECTION("untagging object must remove tag") {

        test_obj obj;
        obj.tag("test");
        obj.untag();

        REQUIRE_THROWS( td::obj::by_tag<test_obj>("test") );

    }

    SECTION("retagging must remove old tag") {

        test_obj obj;
        obj.tag("test1");

        REQUIRE( td::obj::by_tag<test_obj>("test1") == &obj );

        obj.tag("test2");

        REQUIRE_THROWS( td::obj::by_tag<test_obj>("test1") );
        REQUIRE( td::obj::by_tag<test_obj>("test2") == &obj );

    }

    SECTION("can return tagged object using static TAG field") {

        test_obj obj;
        obj.tag(test_obj::TAG);

        REQUIRE( td::obj::by_tag<test_obj>() == &obj );

    }

    SECTION("children must be stored in order of addition") {

        test_obj p;
        test_obj c1;
        test_obj c2;
        test_obj c3;

        c1.parent(&p);
        c2.parent(&p);
        c3.parent(&p);

        c2.parent(nullptr);
        c2.parent(&p);

        REQUIRE(p.children() == std::vector<td::child_obj*>({ &c1, &c3, &c2 }));

    }

    SECTION("must avoid changing position if parent is same") {

        call_rec cr;

        test_obj p;
        test_obj c1(&cr);
        test_obj c2;

        c1.parent(&p);
        c2.parent(&p);
        c1.parent(&p);

        REQUIRE(p.child<test_obj>(0) == &c1);
        REQUIRE_THAT(cr, has_calls_in_order({
            call_rec::obj(&c1, "attach")
        }));

    }

}
