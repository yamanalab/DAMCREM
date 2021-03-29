#!/bin/bash

PROG=./build/tool/gen_data
APP_ID="--app-id=1"
CLIENT_ID="--client-id=1000000000"
OPTION_FILE_PATH="--option=options/1app/1-demo.txt"
OUTPUT_DIR="--output-dir=const"

COMMON_ARGS="${APP_ID} ${CLIENT_ID} ${OPTION_FILE_PATH}"

${PROG} ${COMMON_ARGS} --pt --index=0 --level=5  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 1.0" | bc`
${PROG} ${COMMON_ARGS} --pt --index=1 --level=6  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 1.0" | bc`
${PROG} ${COMMON_ARGS} --pt --index=2 --level=6  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.0" | bc`
${PROG} ${COMMON_ARGS} --pt --index=3 --level=7  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.0" | bc`
${PROG} ${COMMON_ARGS} --pt --index=4 --level=6  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.0" | bc`
${PROG} ${COMMON_ARGS} --pt --index=5 --level=7  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.0" | bc`
${PROG} ${COMMON_ARGS} --pt --index=6 --level=7  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.0" | bc`
${PROG} ${COMMON_ARGS} --pt --index=7 --level=8  ${OUTPUT_DIR} --scalar-value=`echo "scale=9; 0.0" | bc`


