#!/bin/bash

#SBATCH --job-name="ljmd"
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --cpus-per-task=1
#SBATCH -A ICT23_MHPC
#SBATCH --hint=nomultithread 
#SBATCH --time=00:04:00
#SBATCH --gres=gpu:4
#SBATCH --mem=490000MB
#SBATCH -p boost_usr_prod
#SBATCH -e error-%j.err
#SBATCH -o out-%j.out

export OMP_NUM_THREADS=8
mpirun -np 2 ./md<argon_108.inp