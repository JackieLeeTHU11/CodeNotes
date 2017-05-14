### argparse
详细资料查看: https://docs.python.org/3/library/argparse.html#module-argparse

class argparse.ArgumentParser(prog=None, usage=None, description=None, epilog=None, parents=[], formatter_class=argparse.HelpFormatter, prefix_chars='-', fromfile_prefix_chars=None, argument_default=None, conflict_handler='error', add_help=True, allow_abbrev=True)

Create a new ArgumentParser object. 

parser = argparse.ArgumentParser(): 默认

ArgumentParser.add_argument(name or flags...[, action][, nargs][, const][, default][, type][, choices][, required][, help][, metavar][, dest])

Define how a single command-line argument should be parsed. Each parameter has its own more detailed description below, but in short they are:

name or flags - Either a name or a list of option strings, e.g. foo or -f, --foo.

action - The basic type of action to be taken when this argument is encountered at the command line.

	store:
	
	store_const: 指定是常量,则需要指定const属性,且不能重新赋值
	
	store_true: 在设定该属性时候,属性值为True,如果不设置,则为False
	
	store_false: 在设定该属性时候,属性值为False,如果不设置,则为True
	
	append:
	
		This stores a list, and appends each argument value to the list. This is useful to allow an option to be specified multiple times. 
		
		parser.add_argument('--foo', action='append'), parser.parse_args('--foo 1 --foo 2'.split())
		
nargs - The number of command-line arguments that should be consumed.

	parser = argparse.ArgumentParser()
	
	parser.add_argument('--foo', nargs=2)
	
	parser.add_argument('bar', nargs=1)
	
	parser.parse_args('c --foo a b'.split())
	
	Namespace(bar=['c'], foo=['a', 'b'])

const - A constant value required by some action and nargs selections.

default - The value produced if the argument is absent from the command line.

	parser = argparse.ArgumentParser()
	
	parser.add_argument('--foo', default=42)
	
	parser.parse_args(['--foo', '2'])
	
type - The type to which the command-line argument should be converted.

	int,str,open(可以是读文件,写文件等)
	
choices - A container of the allowable values for the argument.
required - Whether or not the command-line option may be omitted (optionals only).

help - A brief description of what the argument does.

metavar - A name for the argument in usage messages.

dest - The name of the attribute to be added to the object returned by parse_args().

