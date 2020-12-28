# Fly by wire 
### Compile
According to [Official Guide](https://cmake.org/cmake/help/latest/guide/tutorial/index.html#build-and-test)

```shell
mkdir build
```
Next, navigate to the build directory and run CMake to configure the project and generate a native build system:
```shell
cd build
cmake ..
```
Then call that build system to actually compile/link the project:
```shell
cmake --build .
./aeroplanetty
```
Finally, try to use the compiled project:
```
┌─┐┬ ┬ ┬┌┐ ┬ ┬┬ ┬┬┬─┐┌─┐
├┤ │ └┬┘├┴┐└┬┘││││├┬┘├┤ 
└  ┴─┘┴ └─┘ ┴ └┴┘┴┴└─└─┘
© 2020 Owanesh
```
### LICENSE
The code in this project is licensed under MIT license. See [LICENSE](https://github.com/Owanesh/unifi-fbw777/blob/master/LICENSE) for more information.