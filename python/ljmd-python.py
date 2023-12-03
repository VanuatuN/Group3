from ctypes import *
import time

_MPI = False

if _MPI:
    mdlib = CDLL('../build-mpi/libmd_lib.dylib')
else:
    mdlib = CDLL('../build-serial/libmd_lib.dylib')

class MDSYS(Structure):
    _fields_ = (
        ("natoms", c_int), ("nfi", c_int), ("nsteps", c_int), ("nthreads", c_int), ("thid", c_int),
        ("dt", c_double), ("mass", c_double), ("epsilon", c_double), ("sigma", c_double), ("box", c_double), ("rcut", c_double),
        ("ekin", c_double), ("epot", c_double), ("temp", c_double),
        ("rx", POINTER(c_double)), ("ry", POINTER(c_double)), ("rz", POINTER(c_double)),
        ("vx", POINTER(c_double)), ("vy", POINTER(c_double)), ("vz", POINTER(c_double)),
        ("fx", POINTER(c_double)), ("fy", POINTER(c_double)), ("fz", POINTER(c_double)),
        ("cx", POINTER(c_double)), ("cy", POINTER(c_double)), ("cz", POINTER(c_double)),
        ("rank", c_int),
        ("npes", c_int),
    )

def read_params(file_path):
    params = []
    with open(file_path, 'r') as file:
        for line in file:
            words = line.split()
            if words:
                params.append(words[0])
    return params

def output(md, trajfile, ergfile):
    print(f"{md.nfi:8d} {md.temp:20.8f} {md.ekin:20.8f} {md.epot:20.8f} {(md.ekin + md.epot):20.8f}")
    
    with open(ergfile[0], 'a') as file1:
        file1.write(f"{md.nfi:8d} {md.temp:20.8f} {md.ekin:20.8f} {md.epot:20.8f} {(md.ekin + md.epot):20.8f}\n")
    
    with open(trajfile[0], 'a') as file2:
        file2.write(f"{md.natoms:8d}\n nfi={md.nfi:8d} etot={md.ekin+md.epot:20.8f}\n")
        
        for i in range(md.natoms):
            file2.write(f"{md.rx[i]:20.8f} {md.ry[i]:20.8f} {md.rz[i]:20.8f}\n")

def init_params(md, params, restfile, trajfile, ergfile):
    md.natoms = int(params[0])
    md.mass = float(params[1])
    md.epsilon = float(params[2])
    md.sigma = float(params[3])
    md.rcut = float(params[4])
    md.box = float(params[5])
    restfile[0] = params[6]
    trajfile[0] = params[7]
    ergfile[0] = params[8]
    md.nsteps = int(params[9])
    md.dt = float(params[10])
    md.nprint = int(params[11])

if __name__ == "__main__":
    md = MDSYS()
    BLEN = 200
    LJMD_VERSION = 1.0
    file_path = 'argon_108.inp'
    params = []
    restfile = [None]
    trajfile = [None]
    ergfile = [None]
    md.nthreads = 1
    md.thid = 0

    if _MPI:
        mdlib.mpi_init(byref(md))
        mdlib.mpi_get_rank(byref(md))
        mdlib.mpi_get_size(byref(md))

    params = read_params(file_path)
    init_params(md, params, restfile, trajfile, ergfile)

    if _MPI and md.rank == 0:
        print("LJMD_VERSION ", LJMD_VERSION)
        t_start = time.time()
        rfile = restfile[0].encode('utf-8')
        mdlib.init_mdsys(byref(md))
        mdlib.read_restart(byref(md), rfile)
    elif not _MPI:
        print("LJMD_VERSION ", LJMD_VERSION)
        t_start = time.time()
        rfile = restfile[0].encode('utf-8')
        mdlib.init_mdsys(byref(md))
        mdlib.read_restart(byref(md), rfile)
    
    if _MPI:
        mdlib.init_params(byref(md))
        mdlib.init_mpi_c(byref(md))

    if _MPI and md.rank != 0:
        mdlib.init_mpi_r(byref(md))

    md.nfi = 0
    mdlib.force(byref(md))

    if _MPI and md.rank == 0:
        mdlib.ekin(byref(md))

        print("Startup time: ", time.time()-t_start)
        print("Starting simulation with ", md.natoms, " atoms for ", md.nsteps)
        print("     NFI            TEMP            EKIN                 EPOT              ETOT")
        
        t_start = time.time()

    for i in range(md.nfi + 1, md.nsteps + 1, 1):
        md.nfi = i
        if _MPI and md.rank == 0:
            if ((md.nfi % md.nprint)==0):
                output(md, trajfile, ergfile)
        elif not _MPI:
            if ((md.nfi % md.nprint)==0):
                output(md, trajfile, ergfile)
        
        mdlib.velverlet(byref(md))

        if _MPI and md.rank == 0:
            mdlib.ekin(byref(md))
        elif not _MPI:
            mdlib.ekin(byref(md))

    if _MPI and md.rank == 0:
        print("Simulation Done. Run time: ", time.time()-t_start)
    elif not _MPI:
        print("Simulation Done. Run time: ", time.time()-t_start)

    if _MPI and md.rank == 0:
        mdlib.cleanup_mdsys(byref(md))
    elif not _MPI:
        mdlib.cleanup_mdsys(byref(md))

    if _MPI and md.rank > 0:
        mdlib.cleanup_mpi_r(byref(md))

    if _MPI:
        mdlib.cleanup_mpi_c

    if _MPI:
        mdlib.mpi_finalize()