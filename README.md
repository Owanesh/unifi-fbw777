# Fly by wire 
![Aeroplanetty CI](https://github.com/Owanesh/unifi-fbw777/workflows/Aeroplanetty%20CI/badge.svg)

#### Preface
The purpose of this project is to replicate an unstable and unaffidable -fly-by-wire- system used on Boeing777.
The main scope is to demonstrate how processes can communicate each others.

This repository is created only for the exam and will be no longer mantained.
### Project design
![ProjectSchema](https://raw.githubusercontent.com/Owanesh/unifi-fbw777/main/resources/concept-schema.png)

## Technical stuff
### Compile and run
```shell
make
```
```shell
./aeroplanetty resources/G18.txt
```

Finally, try to use the compiled project:
```
┌─┐┬ ┬ ┬┌┐ ┬ ┬┬ ┬┬┬─┐┌─┐
├┤ │ └┬┘├┴┐└┬┘││││├┬┘├┤ 
└  ┴─┘┴ └─┘ ┴ └┴┘┴┴└─└─┘
© 2020 Owanesh
```
Also run `make help` if you need more information 

### Compile Docs 📃
For compiling docs is required [Doxygen](https://github.com/doxygen/doxygen)

```shell
make docs
```
Then open `docs/html/index.html`



### LICENSE
The code in this project is licensed under MIT license. See [LICENSE](https://github.com/Owanesh/unifi-fbw777/blob/master/LICENSE) for more information.