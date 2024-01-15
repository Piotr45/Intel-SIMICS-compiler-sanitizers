# Intel-SIMICS-compiler-sanitizers

Develop a sanitizer backend for RISC-V.

The RISC-V Bare-Metal Sanitizer Integration Project focuses on enhancing the security and robustness of software applications running on RISC-V architectures by incorporating sanitization mechanisms. Key components of this project should include the application of sanitization techniques to RISC-V bare-metal environments, the development of a dedicated sanitizer backend, and seamless integration into GCC.

Key pieces:

1. Application to RISC-V bare-metal.

   * The project runs directly on hardware without operating system.

2. Sanitizer backend.
   * A dedicated software that implements sanitization mechanisms for detecting various types of software vulnerabilities, such as memory corruption, data races, and undefined behavior.

3. Integration with compiler.
   * Integration of the sanitizer into the backend of widely used compilers, specifically GCC. This integration ensures that sanitization processes become an integral part of the compilation pipeline.

4. Demo application for testing.

   * A demo application is provided to test effectiveness of the sanitizers. It includes intentionally broken code to trigger various types of vulnerabilities.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them

* Intel® Simics® Simulator
* RISC-V GNU Compiler Toolchain

### Installing

#### Simics

1. Install The Simics® simulator, ideally linux distribution
   [Simics Download](https://lemcenter.intel.com/productDownload/?Product=256660e5-a404-4390-b436-f64324d94959).
2. Create a new project.
3. Copy to project's targets the risc-v-baremetal directory.
4. Launch it in simics to confirm correct configuration.

#### Riscv-gnu-toolchain

1. Get riscv-gnu-tollchain [GitHub](https://github.com/riscv-collab/riscv-gnu-toolchain).

2. Configure the toolchain build options.

```bash
./configure --prefix=/opt/riscv --with-cmodel=medany --with-arch=rv64gc --with-abi=lp64
```

3. Build the toolchain.

```bash
make -j $(nproc)
```

4. Recompile snake game example located in `risc-v-baremetal/images/src` (you may remove build options for 32bit binaries).

5. Move built binary to the source directory.

6. Launch risc-v-baremetal example again to confirm correct toolchain configuration.

7. Try to add some code to e.g. snake.c file that uses functions from libc, e.g. `malloc()`.

8. Test if building the game  works.

Remember to add `/opt/riscv/bin` to the PATH variable and to set CROSS_PREFIX (used in Makefile) to 'riscv-unknown-elf-' or use `export.sh` script to build the application and to move binary file to target location.

## Deployment

To launch the project on risc-v bare-metal platform please follow these instructions:

1. Go to the project that contains bare-metal target plaform.

Run:

```bash
./simics
```

Then, in launched simics console, load the target:

```bash
simics> load-target target="risc-v-baremetal/bare-metal"
```

And run it:

```bash
simics> r
```

To close simulation, run:

```bash
simics> q
```

A serial console should open in which you should see output from the application.

## Built With

* [Simics](https://www.intel.com/content/www/us/en/developer/articles/tool/simics-simulator.html) - The Simics® simulator and Simics® Virtual Platform

* [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain) - RISC-V GNU Compiler Toolchain

## Authors

* **Piotr Baryczkowski** - *Initial work, documentation* - [Piotr45](https://github.com/Piotr45)
* **Paweł Strzelczyk** - *Initial work, research, documentation* - [pawelstrzelczyk](https://github.com/pawelstrzelczyk)
* **Marcin Strzesak** - *Testing* - [MarcinStrzesak](https://github.com/MarcinStrzesak)
* **Maciej Szefliński** - *Testing* - [rtzw](https://github.com/rtzw)

## License

This project is licensed under the Apache 2 License - see the [LICENSE.md](LICENSE.md) file for details.
