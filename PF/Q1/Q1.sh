#!/bin/bash
#SBATCH --job-name=prova_job          # Nome do job
#SBATCH --partition=prova             # Partição chamada "prova"
#SBATCH --nodes=5                     # Número de máquinas (nós)
#SBATCH --ntasks-per-node=1           # Número de cores por máquina
#SBATCH --mem=16G                      # Memória solicitada por nó (3 gigabytes)
#SBATCH --output=job_output.txt       # Arquivo de saída
#SBATCH --error=job_error.txt         # Arquivo de erro
#SBATCH --time=01:00:00
#SBATCH --cpus-per-task=4

# Exporta a variável de ambiente para usar 10 threads
export OMP_NUM_THREADS=15

# Comando para executar o programa fictício
./programa