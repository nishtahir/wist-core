# wist-core

Experimental implementation of the [wist](https://github.com/willowtreeapps/wist) parser using WebAssembly.

## Buidling

You can either Download and install the Emscripten SDK or initialize a new vagrant box using the included Vagrant file which sets includes the emsdk environment. 

If you are using the vagrant environment you need to build in elevated sudo mode before sourcing the enviroment variables

```sh
$ sudo -s
$ source /emsdk/emsdk_env.sh
```

You should now be able to build the WASM module using the `Makefile`

```sh
$ cd /vagrant/
$ emmake make
```
