#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <iostream>
#include "LAAT/LAAT.h"
#include "MBMS/MBMS.h"

namespace py = pybind11;
using namespace py::literals;

py::array LAAT(py::array_t<float> in,
	       size_t numberOfAntsX,
	       size_t numberOfAntsY,
	       size_t numberOfAntsZ,
	       size_t numberOfIterations,
	       size_t numberOfSteps,
	       size_t threshold,
	       float radius,
	       float beta,
	       float kappa,
	       float p_release,
	       float evapRate,
	       float lowerlimit,
	       float upperlimit)
{
  auto buf = in.request();
  float *npData = static_cast<float *>(buf.ptr);
  size_t size = buf.shape[0];
  std::vector<std::vector<float>> data(size, std::vector<float>(3));

  for (size_t i = 0; i < size; ++i)
    for (size_t j = 0; j < 3; ++j)
      data[i][j] = npData[i + j * size];

  py::array ret = py::cast(LocallyAlignedAntTechnique(data,
						      numberOfAntsX,
						      numberOfAntsY,
						      numberOfAntsZ,
						      numberOfIterations,
						      numberOfSteps,
						      threshold,
						      radius,
						      beta,
						      kappa,
						      p_release,
						      evapRate,
						      lowerlimit,
						      upperlimit));
  return ret;
}

py::array MBMS(py::array_t<float> in,
	       size_t iter,
	       float radius,
	       float sigma,
	       size_t k)
{
  auto buf = in.request();
  float *npData = static_cast<float *>(buf.ptr);
  size_t size = buf.shape[0];
  std::vector<std::vector<float>> data(size, std::vector<float>(3));

  for (size_t i = 0; i < size; ++i)
    for (size_t j = 0; j < 3; ++j)
      data[i][j] = npData[i + j * size];

  modifiedBlurringMeanShift(data, iter, radius, sigma, k);

  return py::array(py::cast(data));
}

PYBIND11_MODULE(LAAT, m)
{
  m.doc() = "Cosmic Web module for Python";

  m.def("LAAT", &LAAT, "Locally Aligned Ant Technique algorithm",
	"in"_a,
	"numberOfAntsX"_a = 8,
	"numberOfAntsY"_a = 5,
	"numberOfAntsZ"_a = 5,
	"numberOfIterations"_a = 100,
	"numberOfSteps"_a = 12000,
	"threshold"_a = 3,
	"radius"_a = 0.5,
	"beta"_a = 10.0,
	"kappa"_a = 0.8,
	"p_release"_a = 0.05,
	"evapRate"_a = 0.05,
	"lowerlimit"_a = 0.0001,
	"upperlimit"_a = 10);

  m.def("MBMS", &MBMS, "Modified Blurring Mean Shift algorithm",
	"in"_a,
	"iter"_a = 10,
	"radius"_a = 3,
	"sigma"_a = 1.5,
	"k"_a = 10);
}
