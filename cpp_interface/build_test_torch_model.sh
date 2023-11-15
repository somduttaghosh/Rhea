~/anaconda3/envs/mlenv/bin/cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/opt/gcc/8.1.0/bin/gcc -DCMAKE_CXX_COMPILER:FILEPATH=/opt/gcc/8.1.0/bin/g++ -S~/Rhea/cpp_interface -B~/Rhea/build -G Ninja

~/anaconda3/envs/mlenv/bin/cmake --build ~/Rhea/build --target all --
