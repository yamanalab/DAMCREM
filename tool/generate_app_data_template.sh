#!/bin/bash

if [ $# -ne 2 ]; then
    echo "The number of arguments should be 2." 1>&2
    exit 1
fi

num_macrotasks=$1
output_file_path=$2

mkdir -p `dirname ${output_file_path}`

if [ -e "${output_file_path}" ]; then
    echo "${output_file_path} has already existed." 1>&2
    exit 1
fi

echo "#The number of macrotasks" >> ${output_file_path}
echo "num_macrotasks=${num_macrotasks}" >> ${output_file_path}
echo "" >> ${output_file_path}

echo "#Dependencies between macrotasks" >> ${output_file_path}
for i in `seq 0 ${num_macrotasks}`; do
    echo "dependency[${i}]=" >> ${output_file_path}
done
echo "" >> ${output_file_path}

echo "#Macrotask group if needed" >> ${output_file_path}
echo "macrotask_group=" >> ${output_file_path}
echo "" >> ${output_file_path}

echo "#Plaintext" >> ${output_file_path}
echo "num_const_pt=" >> ${output_file_path}
echo "num_input_pt=0" >> ${output_file_path}
echo "num_var_pt=0" >> ${output_file_path}
echo "" >> ${output_file_path}

echo "#Ciphertext" >> ${output_file_path}
echo "num_const_ct=" >> ${output_file_path}
echo "num_input_ct=" >> ${output_file_path}
echo "num_var_ct=" >> ${output_file_path}
echo "" >> ${output_file_path}

echo "#Operations" >> ${output_file_path}
for i in `seq 1 ${num_macrotasks}`; do
    echo "op[${i}]=" >> ${output_file_path}
done
echo "" >> ${output_file_path}

echo "#Indices of the result ciphertexts" >> ${output_file_path}
echo "result_ct_indices=" >> ${output_file_path}
echo "" >> ${output_file_path}

