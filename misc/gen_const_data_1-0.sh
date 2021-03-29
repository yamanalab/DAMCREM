#!/bin/bash

PROG=./build/tool/gen_data
APP_ID="--app-id=1"
CLIENT_ID="--client-id=0"
OPTION_FILE_PATH="--option=options/1app/1-test.txt"
OUTPUT_DIR="--output-dir=const"

COMMON_ARGS="${APP_ID} ${CLIENT_ID} ${OPTION_FILE_PATH}"

${PROG} ${COMMON_ARGS} --pt --index=0 --level=5  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 1.0" | bc`
${PROG} ${COMMON_ARGS} --pt --index=1 --level=6  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 1.0" | bc`
${PROG} ${COMMON_ARGS} --pt --index=2 --level=6  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 1.0/2" | bc`
${PROG} ${COMMON_ARGS} --pt --index=3 --level=7  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 1.0/2/3" | bc`
${PROG} ${COMMON_ARGS} --pt --index=4 --level=6  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 1.0/2/3/4" | bc`
${PROG} ${COMMON_ARGS} --pt --index=5 --level=7  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 1.0/2/3/4/5" | bc`
${PROG} ${COMMON_ARGS} --pt --index=6 --level=7  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 1.0/2/3/4/5/6" | bc`
${PROG} ${COMMON_ARGS} --pt --index=7 --level=8  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 1.0/2/3/4/5/6/7" | bc`


OUTPUT_DIR="--output-dir=input"
${PROG} ${COMMON_ARGS} --ct --index=0 --level=8 ${OUTPUT_DIR} --scalar-value=1.3

