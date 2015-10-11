MPSoCBench
============

A benchmark composed of a scalable set of MPSoCs to enable the development and evaluation of ESL tools, 
methodologies and components designed at University of Campinas by the ArchC team in the Computer Systems Laboratory, Institute of Computing

License
-------
 - ArchC tools are provided under the GNU GPL license.
   See [License](LICENSE) file for details on this license.


Initial Setup
-------------

``` bash
./setup.sh /path/where/archc/was/installed/
```

This script will copy an environment file from ArchC path to MPSoCBench path.
Run the script and follow the instruction on the terminal. 

Building Platforms
--------------------

``` bash
./MPSoCBench -h
```


More
----

Remember that ArchC models, SystemC library and MPSoCBench components must be compiled with
the same GCC version, otherwise you will get compilation problems.

You can find language overview, models, and documentation at
http://www.archc.org



Thanks for the interest. We hope you enjoy using ArchC!

The ArchC Team
Computer Systems Laboratory (LSC)
IC-UNICAMP
http://www.lsc.ic.unicamp.br


