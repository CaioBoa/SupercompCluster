#!/bin/bash
#SBATCH --job-name=matrix_multiply    # Nome do job
#SBATCH --partition=prova             # Partição
#SBATCH --nodes=1                     # Número de máquinas
#SBATCH --cpus-per-task=2             # Mude para 2, 4 ou 8 para testar
#SBATCH --mem=8G                      # Memória alocada
#SBATCH --output=job_output_%j.txt    # Arquivo de saída
#SBATCH --error=job_error_%j.txt      # Arquivo de erro

# Configurar o número de threads OpenMP
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Executar o programa
./matrix_multiply
