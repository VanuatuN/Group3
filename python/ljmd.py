from ctypes import *
try:
    from mpi4py import MPI
    _MPI = True
except ImportError:
    _MPI = False

_MPI = False

class MDSYS(Structure):
  pass

MDSYS._fields_ = [
  ("natoms", c_int), ("nfi", c_int), ("nsteps", c_int),
  ("dt", c_double), ("mass", c_double), ("epsilon", c_double), ("sigma", c_double), ("box", c_double), ("rcut", c_double),
  ("rx", POINTER(c_double)), ("ry", POINTER(c_double)), ("rz", POINTER(c_double)),
  ("vx", POINTER(c_double)), ("vy", POINTER(c_double)), ("vz", POINTER(c_double)),
  ("fx", POINTER(c_double)), ("fy", POINTER(c_double)), ("fz", POINTER(c_double))
]

if _MPI:
  MDSYS._fields_ += [
    ("cx", POINTER(c_double)), ("cy", POINTER(c_double)), ("cz", POINTER(c_double)),
    ("rank", c_int),
    ("npes", c_int),
  ]

if __name__ == "__main__":
  mdlib = CDLL('../serial_build/libmd_lib.dylib')
  md = MDSYS()
  BLEN = 200
  LJMD_VERSION = 1.0

  if _MPI:
    comm = MPI.COMM_WORLD
    md.rank = comm.Get_rank()
    md.npes = comm.Get_size()

  print("LJMD_VERSION ", LJMD_VERSION)
  t_start = mdlib.wallclock()
  md.natoms = 108
  md.rx = (c_double * md.natoms)()
  md.rx[0] = 1.0

  if _MPI:
    md.cx = (c_double * md.natoms)()

  print("md.rx[0] = ", md.rx[0])

