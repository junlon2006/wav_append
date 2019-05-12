# wav append silence  
step1. sudo apt-get install cmake  
step2. cd build && rm -rf *  
step3. cmake ../  
step4. make -j4  
step5. get executable file {tools} in current directory  
step6. ./tools [wav file directory] [silence msecond]  such as : ./tools ../wav_directory 1000  
