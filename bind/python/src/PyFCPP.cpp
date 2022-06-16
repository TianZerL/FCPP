#include <pybind11/pybind11.h>

namespace py = pybind11;

void initCoreModule(const py::module_&);
void initIOModule(const py::module_&);

PYBIND11_MODULE(pyfcpp, m)
{
    m.doc() = "fcpp for python";
    initCoreModule(m.def_submodule("core"));
    initIOModule(m.def_submodule("io"));
}
