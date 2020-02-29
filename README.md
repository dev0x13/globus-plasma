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
with Kaldi, you won't face any problems diving into Globus.

Globus provides a Python wrapper created with [pybind11](https://github.com/pybind/pybind11). Wrapper
source code and basic examples can be found at `src/python` folder.
