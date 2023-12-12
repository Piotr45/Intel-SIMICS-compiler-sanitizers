# Intel-SIMICS-compiler-sanitizers

Develop a sanitizer backend for RISC-V. TODO

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

TODO: What things you need to install the software and how to install them

```
Give examples
```

### Installing

#### Simics

1. Install The Simics® simulator, ideally linux distribution
   [Simics Download](https://lemcenter.intel.com/productDownload/?Product=256660e5-a404-4390-b436-f64324d94959)
2. Create a new project
3. Copy to project's targets the risc-v-baremetal target provided by Jacob.
4. Launch it in simics to confirm correct configuration.

#### Riscv-gnu-toolchain

1. Get riscv-gnu-tollchain [GitHub](https://github.com/riscv-collab/riscv-gnu-toolchain).
2. Follow build instructions for ELF/Newlib toolchain.
3. Recompile snake game example located in risc-v-baremetal/images/src (you may remove build options for 32bit binaries).
4. Move built binary to the source directory.
5. Launch risc-v-baremetal example again to confirm correct toolchain configuration.
6. Try to add some code to e.g. snake.c file that uses functions from libc, e.g. malloc().
7. Test if building the game  works.

Remember to add /opt/riscv/bin to the PATH variable and to set CROSS_PREFIX (used in Makefile) to 'riscv-unknown-elf-'.


## Running the tests

TODO: Explain how to run the automated tests for this system

## Deployment

TODO: Add additional notes about how to deploy this on a live system.

## Built With

* [Simics](https://www.intel.com/content/www/us/en/developer/articles/tool/simics-simulator.html) - The Simics® simulator and Simics® Virtual Platform

## Authors

* **Piotr Baryczkowski** - *Initial work* - [Piotr45](https://github.com/Piotr45)
* **Paweł Strzelczyk** - *Initial work* - [pawelstrzelczyk](https://github.com/pawelstrzelczyk)
* **Marcin Strzesak** - *Initial work* - [MarcinStrzesak](https://github.com/MarcinStrzesak)
* **Maciej Szefliński** - *Initial work* - [rtzw](https://github.com/rtzw)

## License

This project is licensed under the Apache 2 License - see the [LICENSE.md](LICENSE.md) file for details.
