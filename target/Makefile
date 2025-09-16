CXX = g++
SPIKE_DIR = ../../riscv-isa-sim/riscv/

CFLAGS = -Wall -Werror -g -I $(SPIKE_DIR)

build: spike_mmio_plugin.so

spike_mmio_plugin.so: spike_mmio_plugin.cpp
	$(CXX) -shared -o spike_mmio_plugin.so $(CFLAGS) -fPIC spike_mmio_plugin.cpp

clean:
	-rm -f *.o spike_mmio_plugin.so
