Stichwort is a simple tool for C++ keyword arguments. The library provides rather
clean and simple API for creating keywords and handling them as function parameters.
For example with predefined keywords `port` (as a static instance of the 
`stichwort::ParameterKeyword<unsigned int>` class) and `address` 
(as a static instance of the `stichwort::ParameterKeyword<std::string>` class) 
you may use the following syntax:

	spawn_server(handler,stichwort::kwargs[port=80,address="127.0.0.1"]);

To use the library in your code just copy the headers from the `include/` folder
and include them. There is an attempt to avoid using compiler-specific in the code 
and it is supposed to be portable enough. In case of any troubles with the code 
please don't hesitate to fire 
[an issue at github](https://github.com/lisitsyn/stichwort/issues/new).

This project uses [Travis CI](https://travis-ci.org/lisitsyn/stichwort).

