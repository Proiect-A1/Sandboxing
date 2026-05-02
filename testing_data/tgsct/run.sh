old=$(pwd)
cd $SANDBOX_PATH
cd ../
g++ -fsanitize=address,undefined -std=c++17 -I headers -o main testing_data/tgsct/main.cpp src/Tasks/test_generation_script_compiler_task.cpp src/Utilities/general_utilities.cpp src/Utilities/architecture_utilities.cpp && ./main
cd $old