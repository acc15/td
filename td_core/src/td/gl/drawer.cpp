#include <td/gl/drawer.h>

namespace td {

drawer::drawer(td::program& p): _p(p) {
    glUseProgram(_p.id());
}

}