// simple_mmio_device.cc
#include "devices.h"   // abstract_device_t, reg_t, etc.
#include "dts.h"       // for DTS generation helpers (string assembly)
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <string>

// ---- Device implementation -----------------------------------------------

class simple_mmio_dev_t : public abstract_device_t {
public:
  // same offsets as your original plugin
  static constexpr reg_t OFF_OUT    = 0x00;
  static constexpr reg_t OFF_CTRL   = 0x08;
  static constexpr reg_t OFF_HIHASH = 0x10;
  static constexpr reg_t OFF_LOHASH = 0x14;

  // keep the region tight; you can make it 0x1000 if you prefer
  static constexpr reg_t REGION_SIZE = 0x18;

  bool load(reg_t addr, size_t len, uint8_t* bytes) override {
    // replicate: CTRL read of 64-bit returns 0 (device alive)
    if (addr == OFF_CTRL && len == sizeof(uint64_t)) {
      *(uint64_t*)bytes = 0;
      return true;
    }
    std::fprintf(stderr, "ERROR: Invalid device load... (addr=0x%lx len=%zu)\n",
                 (unsigned long)addr, len);
    return false;
  }

  bool store(reg_t addr, size_t len, const uint8_t* bytes) override {
    if (addr == OFF_OUT && len == sizeof(uint32_t)) {
      std::fprintf(stdout, "%c", (char)*(const uint32_t*)bytes);
      return true;
    } else if (addr == OFF_CTRL && len == sizeof(uint32_t)) {
      // terminate simulation: match original behavior (exit(code-1))
      std::exit((*(const uint32_t*)bytes) - 1);
    } else if (addr == OFF_HIHASH && len == sizeof(uint32_t)) {
      uint32_t hv = *(const uint32_t*)bytes;
      std::fprintf(stdout, "** hashval = 0x%08x", hv);
      return true;
    } else if (addr == OFF_LOHASH && len == sizeof(uint32_t)) {
      uint32_t hv = *(const uint32_t*)bytes;
      std::fprintf(stdout, "%08x\n", hv);
      return true;
    }
    std::fprintf(stderr, "ERROR: Invalid device store... (addr=0x%lx len=%zu)\n",
                 (unsigned long)addr, len);
    return false;
  }

  reg_t size() override { return REGION_SIZE; }
};

// ---- Factory: generate DTS + instantiate from args -----------------------

// Basic argument parsing: --device=simple_mmio,<base>[,<size>]
static inline void parse_args(const std::vector<std::string>& sargs,
                              reg_t& base, reg_t& size)
{
  auto parse_u = [](const std::string& s) -> unsigned long long {
    // base 0 lets 0x... be hex and digits be decimal
    return std::stoull(s, nullptr, 0);
  };

  base = 0x0000000000020000ULL; // default was SIM_CTRL_BASE in the old code
  size = simple_mmio_dev_t::REGION_SIZE;

  if (!sargs.empty()) {
    base = (reg_t)parse_u(sargs[0]);
  }
  if (sargs.size() >= 2) {
    size = (reg_t)parse_u(sargs[1]);
    if (size < simple_mmio_dev_t::REGION_SIZE)
      size = simple_mmio_dev_t::REGION_SIZE; // never smaller than our register file
  }
}

static std::string simple_mmio_generate_dts(const sim_t* /*sim*/,
                                            const std::vector<std::string>& sargs)
{
  reg_t base = 0, size = 0;
  parse_args(sargs, base, size);

  std::stringstream s;
  s << std::hex
    << "    simple_mmio@" << base << " {\n"
    << "      compatible = \"community,simple-mmio\";\n"
    << "      reg = <0x" << (uint32_t)(base >> 32) << " 0x" << (uint32_t)(base & 0xFFFFFFFFu)
    <<           " 0x" << (uint32_t)(size >> 32) << " 0x" << (uint32_t)(size & 0xFFFFFFFFu) << ">;\n"
    << "    };\n";
  return s.str();
}

static simple_mmio_dev_t*
simple_mmio_parse_from_fdt(const void* /*fdt*/, const sim_t* /*sim*/,
                           reg_t* base_out, const std::vector<std::string>& sargs)
{
  reg_t base = 0, size = 0;
  parse_args(sargs, base, size);
  *base = base;
  // No per-instance state required; if you need, add ctor params here
  return new simple_mmio_dev_t();
}

// Register the device so --device=simple_mmio,... works
REGISTER_DEVICE(simple_mmio, simple_mmio_parse_from_fdt, simple_mmio_generate_dts)

