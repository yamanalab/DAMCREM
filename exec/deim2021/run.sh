#!/bin/bash

if [ $# -ne 2 ]; then
    echo "specify the binary directory such \"build\" and the maximum number of parallel process" 2>/dev/null
    exit 1
fi

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

LB_PRIORITIES=(
    # 1.0
    # 0.9
    # 0.8
    0.7
)

dt1_list=(
    "160" "200" "240" "280" "320" "360" "400"
)

dt2_list=(
    "280" "320" "360" "400" "440" "480" "520" "560" "600"
)

for i in 0 2 4 6 8; do
    j=$((${i} + 1))
    for alloc_method in "${ALLOC_METHODS[@]}"; do
        for sched_method in "${SCHED_METHODS[@]}"; do
            for lb_priority in "${LB_PRIORITIES[@]}"; do
                for dt1 in "${dt1_list[@]}"; do
                    for dt2 in "${dt2_list[@]}"; do
                        for path_mode in "all_path" "critical_path"; do
                            file_name="${path_mode}_${dt1}000_${dt2}000_${lb_priority}_${i}_${j}.txt"
                            option_file_path="options/deim2021/options/${file_name}"
                            result_file_path="result/deim2021/${alloc_method}_${sched_method}_${file_name}"
                            if [ -e "${result_file_path}" ]; then
                                continue
                            fi
                            CMD="./${PROG_DIR}/example/server/simulation_evaluation_${alloc_method}_${sched_method} --option-file=${option_file_path}"
                            echo "${CMD} > ${result_file_path} 2>/dev/null"
                        done
                    done
                done
            done
        done
    done
done | xargs -L 1 -P ${max_parallel_process} -I{} bash -c 'echo "{}"; {}'


