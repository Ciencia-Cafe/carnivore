# Carnivore Syntax
* Overview:
```
print ["Hello world!"]
```

 - Functions take a list of variables matching its types, the print is an internal function and prints to the console according to the list of variables given.
```
calc_fib [n int] return int {
	[p0, p1] int = [0, 1]
	ret int = 0

	for(i int = 0, i < n, i++) {
		if(i % 2){
			p0 += p1
			ret = p0
		} else {
			p1 += p0
			ret = p1
		}
	}
	return ret
}
```

 - Calling functions
```
print [calc_fib [5]]
```

 - Carnivore doesn't have a main function, instead it reads the whole file as an entry point, arguments are passed through a global variable local to the file `.args`. The file scope is treated as something between functions and procedures, with the ".args" identifier reserved for passed arguments and uses the exit function to return an execution value to the system, if the exit function is not used the value "0" will be returned by default.
```
foreach(arg str, .args){
	print [arg]
}

exit [0] // optional, but a good practice to use
```

 - `.args` and others `.<identifiers>` are self referencing variables, functions with anonymous arguments may use them like this:
```
print_ints [int, int, int, int] return int {
	print [.args[0], .args[1], .args[2], .args[3]]
	return sum[.args]
}
```

 - Procedures are like functions but with no return type, here's a procedure that takes no arguments:
```
my_proc [] {
	print ["Hello world"]
}
```

 - Modules work like namespaces and can contain variables, functions and procedures. The local keyword is used to identify variables being used locally that can't be accessed outside.
```
my_module module {
	index local int = 0	

	get_index [] return index++

	set_index [n int] {
		index = n
	}
}
```
