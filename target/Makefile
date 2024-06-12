CXX = g++
SPIKE_DIR = ../../../riscv-isa-sim/

CFLAGS = -Wall -Werror -g -I $(SPIKE_DIR)

build: simple_mmio_plugin.so

simple_mmio_plugin.so: simple_mmio_plugin.cpp
	$(CXX) -shared -o simple_mmio_plugin.so $(CFLAGS) -fPIC simple_mmio_plugin.cpp

clean:
	-rm -f *.o simple_mmio_plugin.so
