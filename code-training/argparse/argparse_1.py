# -*- coding: utf-8 -*- 
import argparse 

parser = argparse.ArgumentParser()
parser.add_argument("echo", help="echo the string you use here", type=str)
parser.add_argument("batch", help="batchsize", type=int)
parser.add_argument("input_address", help="the address of input images or videos", type=str)

args = parser.parse_args()

print args.echo
print args.batch
print args.input_address
