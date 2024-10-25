#!/bin/bash
#SBATCH --job-name=mpi_ex4
#SBATCH --output=output_ex4.txt
#SBATCH --ntasks=4                 # Número total de tarefas MPI
#SBATCH --cpus-per-task=1          # Número de CPUs por tarefa
#SBATCH --mem-per-cpu=1Gb          # Memória por CPU
#SBATCH --time=00:05:00            # Tempo máximo de execução

# Executar o programa MPI com o número de tarefas especificado
mpirun ./ex4       # Use mpirun ou srun, dependendo do sistema