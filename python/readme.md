# Python wrapper of mdlib library

This python program uses functions from the mdlib library written in C. All calculations such as velocity verlet and foce computations are done in C.

To use this python program with serial version, set `_MPI` to `False`. Set `_MPI = True` to run with MPI parallelization.

For serial code, run with the command

```Python
python ljmd-python.py
```

To use with MPI, run with the command
```Python
mpiexec -n 6 python ljmd-python.py
```