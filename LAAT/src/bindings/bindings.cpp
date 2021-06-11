#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "../LAAT.h"
#include <iostream>

namespace py = pybind11;
using namespace py::literals;

void example(py::array_t<float> in)
{
  auto buf = in.request();
  float *npData = static_cast<float *>(buf.ptr);
  size_t size = buf.shape[0];
  std::vector<DataPoint> data(size);

  for (size_t i = 0; i < size; ++i)
    for (size_t j = 0; j < 3; ++j)
      data[i][j] = npData[i * size + j * 3];
}

PYBIND11_MODULE(laat, m)
{
  m.doc() = "LAAT module for Python";

  m.def("example", &example, "this is an example function", "in"_a);
}
