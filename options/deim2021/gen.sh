#!/bin/bash

# ALLOC_METHODS=(
#     "max_parallel"
#     "bits2020"
# )

# SCHED_METHODS=(
#     "fifo"
#     "fifo_fifo"
#     "deadline"
#     "deadline_deadline"
# )

for dt1 in 160 200 240 280 320 360 400; do
    for dt2 in 280 320 360 400 440 480 520 560 600; do
        for lb_priority in 1.0 0.9 0.8 0.7; do
            for i in `seq 0 9`; do
                for j in `seq 0 9`; do
                    dst_file_path=options/deim2021/options/all_path_${dt1}000_${dt2}000_${lb_priority}_${i}_${j}.txt
                    cp options/deim2021/template.txt ${dst_file_path}
                    echo "do_use_critical_path_for_prediction=false" >> ${dst_file_path}
                    echo "def_lowerbound_of_priority=${lb_priority}" >> ${dst_file_path}
                    echo "query_arrival_time_file_info[1]=received_time/received_time_M4096_mu${dt1}000_${i}.txt,4096" >> ${dst_file_path}
                    echo "query_arrival_time_file_info[2]=received_time/received_time_M4096_mu${dt2}000_${j}.txt,4096" >> ${dst_file_path}

                    dst_file_path=options/deim2021/options/critical_path_${dt1}000_${dt2}000_${lb_priority}_${i}_${j}.txt
                    cp options/deim2021/template.txt ${dst_file_path}
                    echo "do_use_critical_path_for_prediction=true" >> ${dst_file_path}
                    echo "def_lowerbound_of_priority=${lb_priority}" >> ${dst_file_path}
                    echo "query_arrival_time_file_info[1]=received_time/received_time_M4096_mu${dt1}000_${i}.txt,4096" >> ${dst_file_path}
                    echo "query_arrival_time_file_info[2]=received_time/received_time_M4096_mu${dt2}000_${j}.txt,4096" >> ${dst_file_path}
                    
                done
            done
        done
    done
done

