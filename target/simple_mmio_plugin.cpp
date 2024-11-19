#include <riscv/mmio_plugin.h>
#include <cstdio>

#define SIM_CTRL_BASE   0x20000
#define SIM_CTRL_OUT    0x00
#define SIM_CTRL_CTRL   0x08
#define SIM_CTRL_HIHASH 0x10
#define SIM_CTRL_LOHASH 0x14

struct simple_mmio_plugin
{

  simple_mmio_plugin(const std::string& args)
  {
    // fprintf(stderr, "INFO: Installing simple_system devices... (ARGS=%s)\n", args.c_str());
  }

  ~simple_mmio_plugin()
  {
    // fprintf(stderr, "INFO: Uninstalling simple_system devices...\n");
  }

  bool load(reg_t addr, size_t len, uint8_t* bytes)
  {
    if (addr == SIM_CTRL_CTRL && len == sizeof(uint64_t))
    {
      // indicate that the device is alive
      *(uint64_t *)bytes = 0;
      return true;
    }
    else
    {
      fprintf(stderr, "ERROR: Invalid device load... (addr=0x%lx len=%lu)\n", addr, len);
      return false;
    }
  }

  bool store(reg_t addr, size_t len, const uint8_t* bytes)
  {
    if (addr == SIM_CTRL_OUT && len == sizeof(uint32_t))
    {
      fprintf(stdout, "%c", (char)*(uint32_t *)bytes);
      return true;
    }
    else if (addr == SIM_CTRL_CTRL && len == sizeof(uint32_t))
    {
      // terminate simulation
      exit((*(uint32_t *)bytes) - 1);
    }
    else if (addr == SIM_CTRL_HIHASH && len == sizeof(uint32_t))
    {
      // print the execution hash
      uint32_t __hashval = *(uint32_t *)bytes;
      fprintf(stderr, "** hashval = 0x%08x", __hashval);
      return true;
    }
    else if (addr == SIM_CTRL_LOHASH && len == sizeof(uint32_t))
    {
      // print the execution hash
      uint32_t __hashval = *(uint32_t *)bytes;
      fprintf(stderr, "%08x\n", __hashval);
      return true;
    }
    else
    {
      fprintf(stderr, "ERROR: Invalid device store... (addr=0x%lx len=%lu)\n", addr, len);
      return false;
    }
  }
};

static mmio_plugin_registration_t<simple_mmio_plugin> simple_mmio_plugin_registration("simple_mmio_plugin");

