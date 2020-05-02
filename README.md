Originally created in 2013

# Universal C++ Language Compiler: 
Universal C++ is standard C++11/14 with the addition of the "borrow" and "repl" keywords. "borrow" automatically deletes memory allocated in the same scope or within a class after it was allocated. Almost a garbage collector, further work would have had to been done to make a "compile time garbage collector". "repl" was a keyword for an explicit optimization, replace the name of one variable with another. That way the variable can continued to be used under a different name, allowing for clean code, an understanding of what happened and guaranteed efficiency. (Neither of these were particularly good, borrow could have been if it were further developed). Universal C++ takes Universal C++ code as input and outputs ISO Standard C++ code and is built on top of ISO Standard C++. That way it is totally platform independent, and standard C++ tools: gcc, clang, google native client, msvc could continued to be used.
# Package Library Generator: 
Google Native Client targets architectures, meaning no OS calls are made, and only calls to the Pepper API are made. That can be exploited to make "Packaged Libraries" a system similar to Java's JARs. The code is compiled to assembly and "packaged" then the "package" is read (all the assembly files are found inside the folder) and consumed when compiling (all the assembly files are compiled with the project) when ready to use.

# Web Generator: 
This puts together the executable for Desktop and the Web, its simple but convenient. It puts together the needed files for Native Client. Makes a "Chrome Packaged App" (for use on desktop) and a web page (based ona  template. Really a simple build tool.
			
# Universal C++ Compiler: 
The Universal C++ Compiler combines all the previously mentioned tools in a simple command line interface for use in a simple build step.
