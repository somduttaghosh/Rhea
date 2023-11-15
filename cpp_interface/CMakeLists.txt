cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(test_torch_model)

set(CMAKE_PREFIX_PATH "/mnt/lustre/foucart/sg1631/libtorch") # Adding the directory where torch as been installed
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=1) # Torch is compiled with CXX11_ABI, so your program needs to be also, or you may have conflicts in some libraries (such as GTest for example)

# Specifying we are using pthread for UNIX systems.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

find_package(Torch REQUIRED)
find_package(HDF5 COMPONENTS C CXX HL REQUIRED)
link_directories( ${HDF5_LIBRARY_DIRS} )
include_directories( ${HDF5_INCLUDE_DIRS} )

if(NOT Torch_FOUND)
    message(FATAL_ERROR "Pytorch Not Found!")
endif(NOT Torch_FOUND)

if(NOT HDF5_FOUND)
    message(FATAL ERROR "HDF5 not found!")
endif(NOT HDF5_FOUND)


message(STATUS "Pytorch status :")
message(STATUS "    libraries: ${TORCH_LIBRARIES}")
message(STATUS "    Torch Flags: ${TORCH_CXX_FLAGS}")
message(STATUS "    HDF5 libraries: ${HDF5_LIBRARIES}")
message(STATUS "    HDF5 include dirs: ${HDF5_INCLUDE_DIRS}")

# Program executable
add_executable(test_torch_model test_torch_model.cpp)

target_link_libraries(test_torch_model "${TORCH_LIBRARIES}" "${HDF5_LIBRARIES}")
set_property(TARGET test_torch_model PROPERTY CXX_STANDARD 17)
