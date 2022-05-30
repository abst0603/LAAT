# Installation
To install this module from source, you will need to install a suitable C++ compiler and CMake. Any other external dependencies of this project are included as git submodules and they must be initialized when this repository is cloned:

```sh
git clone https://github.com/abst0603/LAAT.git
cd LAAT
git submodule update --init
```

The project can then be installed as a Python module from the main directory of the project using pip:

```sh
pip install .
```

The module can now be imported and used in python:

```python
import laat
pheromone = laat.LAAT(...)
```

# Usage
An example of the usage of the LAAT module can be found in the `examples` folder. After the LAAT module for Python has been installed, the example can be run with:

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

Now the build directory will create both the static C++ library file `liblaatlib.a` and the Python module binary file `laat.cpython`.

# Matlab
You don't need to install anything to use Matlab functions. The code written in Matlab is standalone, however, it is also much slower than python and C++ code. We don't recommend to use Matlab code for a dataset larger than 100,000 samples. You first need to run the "preprocessing" and then use that as an input for "LAAT".
```Matlab
pstruct = PreprocessLAAT(Data,'Radius',0.5,'Threshold',3)
pheromone = LAAT(pstruct,'Option','option.mat')
```

# Citing
If you use LAAT in your research, please cite [LAAT: Locally Aligned Ant Technique for discovering multiple faint low dimensional structures of varying density](https://ieeexplore.ieee.org/abstract/document/9780217)
```
@ARTICLE{9780217,
author={Taghribi, Abolfazl and Bunte, Kerstin and Smith, Rory and Shin, Jihye and Mastropietro, Michele and Peletier, Reynier F. and Tiňo, Peter},
journal={IEEE Transactions on Knowledge and Data Engineering},
title={LAAT: Locally Aligned Ant Technique for discovering multiple faint low dimensional structures of varying density},
year={2022},
volume={},
number={},
pages={1-1},
doi={10.1109/TKDE.2022.3177368}
}
```
