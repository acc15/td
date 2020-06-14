#include <catch2/catch.hpp>
#include <td/engine/obj.h>

struct test_struct {
    size_t attach_count = 0;
    size_t detach_count = 0;
    bool destroyed = false;
};

class test_obj: public td::obj {
public:
    test_struct* _ts;

    test_obj(test_struct* ts = nullptr): _ts(ts) {
    }

    virtual ~test_obj() {
        if (_ts != nullptr) {
            _ts->destroyed = true;
        }
    }

    void attach() override {
        if (_ts != nullptr) {
            ++_ts->attach_count;
        }
    }

    void detach() override {
        if (_ts != nullptr) {
            ++_ts->detach_count;
        }
    }
};

TEST_CASE("obj") {

    SECTION("setting parent calls attach, destructor calls detach") {

        test_struct ts;

        td::obj p;

        {
            test_obj c(&ts);
            c.parent(&p);

            REQUIRE(ts.attach_count == 1);
            REQUIRE(c.parent() == &p);
            REQUIRE(p.children() == std::vector<td::child_obj*>({&c}));
        }

        REQUIRE(ts.destroyed);
        REQUIRE(ts.detach_count == 1);

        REQUIRE(p.child_count() == 0);

    }

    SECTION("resetting parent calls detach") {

        test_struct ts;
        td::obj p;

        {

            test_obj c(&ts);
            c.parent(&p);
            REQUIRE(ts.attach_count == 1);
            REQUIRE(p.child_count() == 1);
            c.parent(nullptr);
            REQUIRE(ts.detach_count == 1);

        }

        REQUIRE(p.child_count() == 0);

    }

    SECTION("deleting parent object must also delete all children") {

        test_struct ts1;
        test_struct ts2;

        test_obj* p = new test_obj();
        test_obj* c1 = new test_obj(&ts1);
        test_obj* c2 = new test_obj(&ts2);

        c1->parent(p);
        c2->parent(p);

        delete p;
        CHECK(ts1.destroyed);
        if (!ts1.destroyed) {
            delete c1;
        }

        CHECK(ts2.destroyed);
        if (!ts2.destroyed) {
            delete c2;
        }
    }

    SECTION("switching parent must call detach and attach") {

        test_struct ts;

        test_obj p1;
        test_obj p2;
        test_obj c(&ts);

        c.parent(&p1);
        REQUIRE(c.parent() == &p1);
        REQUIRE(ts.attach_count == 1);
        REQUIRE(ts.detach_count == 0);
        REQUIRE(p1.child_count() == 1);

        c.parent(&p2);
        REQUIRE(c.parent() == &p2);
        REQUIRE(ts.attach_count == 2);
        REQUIRE(ts.detach_count == 1);
        REQUIRE(p1.child_count() == 0);
        REQUIRE(p2.child_count() == 0);

    }

    SECTION("tagging object must return tagged object by name, and return null in case of destruction") {

        {
            test_obj obj;
            obj.tag("test");
            REQUIRE(test_obj::by_tag("test") == &obj);
        }
        REQUIRE(test_obj::by_tag("test") == nullptr);

    }

    SECTION("untagging object must remove tag") {

        test_obj obj;
        obj.tag("test");
        obj.untag();
        REQUIRE( test_obj::by_tag("test") == nullptr );

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

}
