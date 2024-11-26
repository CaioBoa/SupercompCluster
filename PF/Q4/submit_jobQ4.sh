#!/bin/bash
#SBATCH --job-name=mpi_Q4
#SBATCH --output=output_Q4.txt
#SBATCH --ntasks=4                 # Número total de tarefas MPI
#SBATCH --cpus-per-task=5          # Número de CPUs por tarefa
#SBATCH --mem-per-cpu=1Gb          # Memória por CPU
#SBATCH --time=00:05:00            # Tempo máximo de execução

export OMP_NUM_THREADS=10

# Executar o programa MPI com o número de tarefas especificado
mpirun ./Q4       # Use mpirun ou srun, dependendo do sistema