#!/bin/bash
#SBATCH --job-name=mpi_ex2B
#SBATCH --output=output_ex2B.txt
#SBATCH --ntasks=1                 # Número total de tarefas MPI
#SBATCH --cpus-per-task=4          # Número de CPUs por tarefa
#SBATCH --mem-per-cpu=1Gb          # Memória por CPU
#SBATCH --time=00:05:00            # Tempo máximo de execução

# Executar o programa MPI com o número de tarefas especificado
./Q2B       # Use mpirun ou srun, dependendo do sistema