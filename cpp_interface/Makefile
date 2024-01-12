COMP = g++
ifeq ($(LIBTORCH_DIR),)
LIBTORCH_DIR = /mnt/lustre/foucart/sg1631/libtorch
endif

INCLUDE_FLAGS = -I$(LIBTORCH_DIR)/include -I$(LIBTORCH_DIR)/include/torch/csrc/api/include
BUILD_FLAGS = -g -DUSE_C10D_GLOO -DUSE_DISTRIBUTED -DUSE_RPC -DUSE_TENSORPIPE -D_GLIBCXX_USE_CXX11_ABI=1 -g -std=gnu++17
LINK_FLAGS = -Wl,-rpath -L/mnt/lustre/foucart/sg1631/libtorch/lib -ltorch_cpu -lc10 -lkineto

test_torch_model:
	$(COMP) $(BUILD_FLAGS) -o test_torch_model test_torch_model.cpp $(INCLUDE_FLAGS) $(LINK_FLAGS)

clean:
	rm -rf test_torch_model.o test_torch_model

