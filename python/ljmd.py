from ctypes import *
try:
    from mpi4py import MPI
    _MPI = True
except ImportError:
    _MPI = False

mdlib = CDLL('../buildserial/libmd_lib.dylib')

class MDSYS(Structure):
    _fields_ = (
        ("natoms", c_int), ("nfi", c_int), ("nsteps", c_int),
        ("dt", c_double), ("mass", c_double), ("epsilon", c_double), ("sigma", c_double), ("box", c_double), ("rcut", c_double),
        ("rx", POINTER(c_double)), ("ry", POINTER(c_double)), ("rz", POINTER(c_double)),
        ("vx", POINTER(c_double)), ("vy", POINTER(c_double)), ("vz", POINTER(c_double)),
        ("fx", POINTER(c_double)), ("fy", POINTER(c_double)), ("fz", POINTER(c_double))
    )

if _MPI:
    MDSYS._fields_ += [
        ("cx", POINTER(c_double)), ("cy", POINTER(c_double)), ("cz", POINTER(c_double)),
        ("rank", c_int),
        ("npes", c_int),
    ]

def read_params(file_path):
    params = []
    with open(file_path, 'r') as file:
        for line in file:
            words = line.split()
            if words:
                params.append(words[0])
    return params

def init_params(md, params, restart, xyz, data):
    md.natoms = int(params[0])
    md.mass = float(params[1])
    md.epsilon = float(params[2])
    md.sigma = float(params[3])
    md.rcut = float(params[4])
    md.box = float(params[5])
    restart[0] = params[6]
    xyz[0] = params[7]
    data[0] = params[8]
    md.nsteps = int(params[9])
    md.dt = float(params[10])
    md.nprint = int(params[11])

if __name__ == "__main__":
    md = MDSYS(natoms=0)  # Provide the appropriate value for natoms
    BLEN = 200
    LJMD_VERSION = 1.0
    file_path = 'argon_108.inp'
    params = []
    restart = [None]
    xyz = [None]
    data = [None]

    if _MPI:
        comm = MPI.COMM_WORLD
        md.rank = comm.Get_rank()
        md.npes = comm.Get_size()

    print("LJMD_VERSION ", LJMD_VERSION)
    t_start = mdlib.wallclock()

    if _MPI:
        md.cx = (c_double * md.natoms)()

    params = read_params(file_path)
    init_params(md, params, restart, xyz, data)
    
    mdlib.init_mdsys(byref(md))
