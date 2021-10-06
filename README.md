# Installation
To install this module from source, you will need to install a suitable C++ compiler and CMake. Any other external dependencies of this project are included as git submodules and they must be initialized when this repository is cloned:

```sh
git clone https://github.com/abst0603/CosmicWeb.git
cd CosmicWeb
git submodule update --init
```

The project can then be installed as a Python module from the main directory of the project using pip:

```sh
pip install .
```

The module can now be imported and used in python:

```python
import CosmicWeb
pheromone = CosmicWeb.LAAT(...)
```

# Usage
An example of the usage of the CosmicWeb module can be found in the `examples` folder. After the CosmicWeb module for Python has been installed, the example can be run with:

```sh
cd examples
python example.py
```

To adapt this example to your data, fit the relevant functions into your data processing pipeline, or store your data in a csv file and load it into python as shown in the example. Tweak the hyper-parameters for both LAAT and MBMS to fit your dataset. Use the result of MBMS for further data processing, or write it straight to a csv file as shown in the example.

# Compiling from source
You can choose to build this module from source as a static C++ library for use in C++ projects. To do so, create a build directory and run CMake:

```sh
mkdir build
cd build
cmake ..
make
```

Now the build directory will create both the static C++ library file `libcosmicweblib.a` and the Python module binary file `cosmicweb.cpython`.

# How to cite this work
The manuscript associated with this software has been submitted and is under peer review at the moment. You can find a pre-print to the current version here on [arXiv](https://arxiv.org/abs/2009.08326).
