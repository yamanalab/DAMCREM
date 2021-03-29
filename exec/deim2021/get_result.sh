#!/bin/bash

PROG_DIR=$1
max_parallel_process=$2

ALLOC_METHODS=(
    # "max_parallel"
    "bits2020"
)

SCHED_METHODS=(
    "fifo"
    "deadline"
    "fifo_fifo"
    "deadline_deadline"
    "deadline_fifo"
    "fifo_deadline"
)

dt1_list=(
    "160" "200" "240" "280" "320" "360" "400"
)

dt2_list=(
    "280" "320" "360" "400" "440" "480" "520" "560" "600"
)

for i in 0 2 4 6 8; do
    j=$((i + 1))
    for alloc_method in "${ALLOC_METHODS[@]}"; do
        for sched_method in "${SCHED_METHODS[@]}"; do
            # for lb_priority in 1.0 0.9 0.8 0.7; do
            for lb_priority in 0.7; do
                for dt1 in "${dt1_list[@]}"; do
                    for dt2 in "${dt2_list[@]}"; do
                        for path_mode in "all_path" "critical_path"; do
                            file_name="${path_mode}_${dt1}000_${dt2}000_${lb_priority}_${i}_${j}.txt"
                            result_file_path="result/deim2021/${alloc_method}_${sched_method}_${file_name}"
                            CMD="python3 exec/deim2021/get_result.py ${result_file_path}"
                            echo "${CMD}"
                        done
                    done
                done
            done
        done
    done
done | xargs -L 1 -P 1 -I{} bash -c '{}'


