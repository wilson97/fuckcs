#!/bin/bash
#SBATCH --mail-user=wilson97@uchicago.edu
#SBATCH --mail-type=ALL
#SBATCH --output=/home/wilson97/slurm/slurm_out/%j.%N.stdout
#SBATCH --error=/home/wilson97/slurm/slurm_out/%j.%N.stderr
#SBATCH --workdir=/home/wilson97/parallel
#SBATCH --job-name=wilson97-hw-1
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=15:00
#SBATCH --partition general
#SBATCH --exclusive

bash runSpeedTest.sh
