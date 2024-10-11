#!/bin/bash
#SBATCH --job-name=omp
#SBATCH --output=output_omp_schedulers.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem-per-cpu=1Gb
#SBATCH --time=00:05:00

./omp_schedulers