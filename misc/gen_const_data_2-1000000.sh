#!/bin/bash

PROG=./build/tool/gen_data
APP_ID="--app-id=2"
CLIENT_ID="--client-id=1000000"
OPTION_FILE_PATH="--option=options/1app/2-deim2021.txt"
OUTPUT_DIR="--output-dir=const"

COMMON_ARGS="${APP_ID} ${CLIENT_ID} ${OPTION_FILE_PATH}"

for i in `seq 0 11`; do
    ${PROG} ${COMMON_ARGS} --ct --index=${i} --level=7  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.1" | bc`
done

for i in `seq 12 15`; do
    ${PROG} ${COMMON_ARGS} --ct --index=${i} --level=6  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.1" | bc`
done

for i in `seq 16 31`; do
    ${PROG} ${COMMON_ARGS} --ct --index=${i} --level=5  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.1" | bc`
done

for i in `seq 32 35`; do
    ${PROG} ${COMMON_ARGS} --ct --index=${i} --level=4  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.1" | bc`
done

for i in `seq 36 51`; do
    ${PROG} ${COMMON_ARGS} --ct --index=${i} --level=3  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.1" | bc`
done

for i in `seq 52 55`; do
    ${PROG} ${COMMON_ARGS} --ct --index=${i} --level=2  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.1" | bc`
done

for i in `seq 56 59`; do
    ${PROG} ${COMMON_ARGS} --ct --index=${i} --level=1  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.1" | bc`
done

${PROG} ${COMMON_ARGS} --ct --index=60 --level=0  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.1" | bc`


OUTPUT_DIR="--output-dir=input"
for i in `seq 0 2`; do
    ${PROG} ${COMMON_ARGS} --ct --index=${i} --level=7 ${OUTPUT_DIR} --scalar-value=${i}
done

