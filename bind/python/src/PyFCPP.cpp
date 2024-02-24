#include <pybind11/pybind11.h>

namespace py = pybind11;

void initCoreModule(py::module_&);
void initIOModule(py::module_&);

PYBIND11_MODULE(pyfcpp, m)
{
    m.doc() = "A FC/NES emulator for python";

    auto core = m.def_submodule("core", "fcpp core");
    auto io = m.def_submodule("io", "fcpp io");
    initCoreModule(core);
    initIOModule(io);
}
