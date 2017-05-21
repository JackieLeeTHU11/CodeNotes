#!/bin/bash
## notes
	# protoc -I=$SRC_DIR --python_out=$DST_DIR $SRC_DIR/addressbook.proto
	# -I source dir, and --python_out or --cpp_out is the output dir, the last is the intput proto file
	
# for python:
protoc --python_out=./ ./addressbook.proto

# for c++:
protoc --cpp_out=./ ./addressbook.proto

