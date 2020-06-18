#include <catch2/catch.hpp>

#include <td/gl/program.h>

using namespace Catch::Matchers;
using namespace td;

enum class ref_type {
    CLV,
    LV,
    RV
};

template <typename T>
class move_semantics_tester {
public:
    move_semantics_tester(const T& /*unused*/): _type(ref_type::CLV) {
    }

    move_semantics_tester(T& /*unused*/): _type(ref_type::LV) {
    }

    move_semantics_tester(T&& /*unused*/): _type(ref_type::RV) {
    }

    ref_type type() {
        return _type;
    }

private:
    enum ref_type _type;

};


class test_program: public program {
public:
    test_program() = default;
    test_program(test_program&& other) = default;
    ~test_program() {
        _id = 0;
    }

    void set_id(GLuint v) {
        _id = v;
    }

    GLuint get_id() const {
        return _id;
    }

};


TEST_CASE("program") {


    SECTION("add by lv") {
        shader lv(GL_VERTEX_SHADER, "test");

        program p;
        p.add(lv);

        CHECK(p.internals().empty());
        REQUIRE(!p.externals().empty());
        REQUIRE(&p.externals()[0].get() == &lv);
    }

    SECTION("add by rv") {

        program p;
        p.add(shader(GL_VERTEX_SHADER, "internal"));

        CHECK(p.externals().empty());
        REQUIRE(!p.internals().empty());
        REQUIRE_THAT(p.internals()[0].src(), Equals( "internal" ));

    }

    SECTION("move constructor") {

        shader s(GL_FRAGMENT_SHADER, "frag");

        test_program p1;
        p1.set_id(5);
        p1.vertex("test");
        p1.fragment("s");
        p1.add(s);

        test_program p2( std::move(p1) );

        CHECK(p1.get_id() == 0);
        CHECK(p1.internals().empty());
        CHECK(p1.externals().empty());

    }

    SECTION("move semantics") {

        program cp;
        cp.add(shader(GL_VERTEX_SHADER, "vertex"));

        CHECK( move_semantics_tester<td::program>(td::program()).type() == ref_type::RV );
        CHECK( move_semantics_tester<td::program>(td::program().add(td::shader(GL_FRAGMENT_SHADER, "fragment"))).type() == ref_type::RV );
        CHECK( move_semantics_tester<td::program>(cp).type() == ref_type::LV );
        CHECK( move_semantics_tester<td::program>(cp.add(td::shader(GL_FRAGMENT_SHADER, "fragment"))).type() == ref_type::RV );

    }

}
