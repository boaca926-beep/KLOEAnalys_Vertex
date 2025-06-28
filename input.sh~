#!/bin/bash

path_header=path.h
echo -e 'const TString rootFile = "";' > $path_header
echo -e 'const TString sampleFile = "";' >> $path_header

INPUT_FILE=../../path_small/sig_path1 # sig_path1, sig_path_sum
ROOT_FILE=../../sig_sum
echo $INPUT_FILE
echo $ROOT_FILE

sed -i 's|\(const TString rootFile =\)\(.*\)|\1 "'"${INPUT_FILE}"'";|' "$path_header"
sed -i 's|\(const TString sampleFile =\)\(.*\)|\1 "'"${ROOT_FILE}"'";|' "$path_header"

log_input=log_input.txt
echo "" > ${log_input}
touch $log_input

run_script=run_script.C
echo "void run_script() {" > $run_script
echo '  gROOT->ProcessLine(".L MyClass.C");' >> $run_script
echo '  gROOT->ProcessLine(".L Analys_class.C");' >> $run_script
echo '  gROOT->ProcessLine("Analys_class(rootFile, sampleFile)");' >> $run_script
echo '}' >> $run_script
root -l -n -q -b $run_script >> ${log_input}

rm $run_script
