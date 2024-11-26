#!/bin/bash
#SBATCH --job-name=mpi_openmp_job      # Nome do job
#SBATCH --partition=prova             # Partição do cluster
#SBATCH --nodes=2                     # Número de máquinas (nós)
#SBATCH --ntasks=4                    # Número total de processos MPI
#SBATCH --cpus-per-task=5             # Número de threads OpenMP por processo
#SBATCH --mem=4G                      # Memória total por nó
#SBATCH --output=job_output.txt       # Arquivo de saída
#SBATCH --error=job_error.txt         # Arquivo de erro

# Exporta o número máximo de threads OpenMP
export OMP_NUM_THREADS=5

# Executa o programa MPI
mpirun ./mpi_openmp_sum_of_squares
