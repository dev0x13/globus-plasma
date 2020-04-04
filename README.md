Globus Plasma Diagnostics Toolkit
=================================

This repository contains a set of tools and recipes for various plasma diagnostics methods such as:
* Plasma crash detection

Documentation notes
-------------------

TBD

Terminology used within the project is according to [the one used in ITER CODAC team](https://static.iter.org/codac/pcdh7/Folder%201/19-ITER_CODAC_Abbreviations_and_Acronyms_2LT73V_v3_0.pdf).

Software specific notes
-----------------------

Globus employs a small part of [Kaldi](https://github.com/kaldi-asr/kaldi) toolkit with some minor
changes (removed typedefs for numbers, changed headers paths) for logging, errors handling and
program arguments parsing. The decision to use part of Kaldi code was made due to its high quality
and simplicity. Also the Globus toolkit structure is similar to Kaldi one, so if you are familiar 
with Kaldi, you won't face any problems diving into Globus. **Please note that Globus is licensed under
MIT license, but Kaldi code used in the project is licensed under Apache 2.0.**

Globus provides a Python wrapper created with [pybind11](https://github.com/pybind/pybind11). Wrapper
source code and basic examples can be found at `src/python` folder.

Building
--------

Available CMake options:

- `use_python` - specifying building Python wrapper or not
- `use_debug` - specifying outputing debug messages or not
- `-DPYTHON_EXECTUALBE=<path to Python executable>` - specifying for which Python version pyglobus is built (please see more details on [pybind11 docs](https://pybind11.readthedocs.io/en/stable/faq.html#cmake-doesn-t-detect-the-right-python-version))

### Linux

- Globus is expected to work out of the box in Ubuntu >= 16.04 with installed CMake >= 3.12 and Python >= 2.7
  (only if you are going to use Python wrapper).
- The easiest way to install stable CMake version is via pip: `pip install cmake`
- Build process is the following:
    * Create build directory: `mkdir _build && cd _build`
    * Run CMake: `cmake [cmake options] ..`
    * Build: `cmake --build . --target install` or `make && make install`

### Windows

- Globus build and running were teset on Windows 10 with Visual Studio 2017, but most likely at this stage of
  delopment it will work on any Visual Studio versions higher than 2017.
- It is recommended to use [chocolatey](https://chocolatey.org/) to install building dependencies.
- The easiest way to install stable CMake version is via pip: `pip install cmake`
- If you are using chocolatey, then just install Visual Studio requirements: `choco install visualstudio2017community visualstudio2017-workload-vctools`

- Build process is the following:
    * Create build directory: `mkdir _build ; cd _build`
    * Run CMake: `cmake [cmake options] ..`. If you are on x64 platform, you should specify x64 architecture explicitly for cmake: `cmake -A x64 [cmake options] ..`
    * Build: `cmake --build . --target install`
