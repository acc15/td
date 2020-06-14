#include <catch2/catch.hpp>
#include <td/engine/obj.h>

class test_obj: public td::obj {
public:
    size_t attach_count = 0;
    size_t detach_count = 0;
    bool *destroyed;

    test_obj(bool *destroyed = nullptr): destroyed(destroyed) {
    }

    virtual ~test_obj() {
        if (destroyed != nullptr) {
            *destroyed = true;
        }
    }

    void attach() override {
        ++attach_count;
    }

    void detach() override {
        ++detach_count;
    }
};

TEST_CASE("obj") {


    SECTION("setting parent calls attach") {

        td::obj p;

        {
            test_obj c;
            c.parent(&p);

            REQUIRE(c.attach_count == 1);
            REQUIRE(c.parent() == &p);
            REQUIRE(p.children() == std::vector<td::child_obj*>({&c}));
        }

        REQUIRE(p.children().empty());

    }

    SECTION("resetting parent calls detach") {

        td::obj p;

        {
            test_obj c;
            c.parent(&p);
            REQUIRE(c.attach_count == 1);
            c.parent(nullptr);
            REQUIRE(c.detach_count == 1);
        }

        REQUIRE(p.children().empty());

    }

    SECTION("deleting parent object must also delete all children") {

        bool c1_d = false;
        bool c2_d = false;

        test_obj* p = new test_obj();
        test_obj* c1 = new test_obj(&c1_d);
        test_obj* c2 = new test_obj(&c2_d);

        c1->parent(p);
        c2->parent(p);

        delete p;
        CHECK(c1_d);
        CHECK(c2_d);
        if (!c1_d) {
            delete c1;
        }
        if (!c2_d) {
            delete c2;
        }
    }

}
