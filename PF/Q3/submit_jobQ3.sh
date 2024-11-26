#!/bin/bash
#SBATCH --job-name=mpi_Q3
#SBATCH --output=output_Q3.txt
#SBATCH --ntasks=1                 # Número total de tarefas MPI
#SBATCH --cpus-per-task=4          # Número de CPUs por tarefa
#SBATCH --mem-per-cpu=1Gb          # Memória por CPU
#SBATCH --time=00:05:00            # Tempo máximo de execução

# Executar o programa MPI com o número de tarefas especificado
./Q3       # Use mpirun ou srun, dependendo do sistema