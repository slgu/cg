#!/bin/env python
# coding=utf-8
#     File Name: test.py
#     Author: Gu Shenlong
#     Mail: blackhero98@gmail.com
#     Created Time: Sun Mar 13 03:04:36 2016


import sys,os,math,time,logging,json

if __name__ == "__main__":
    for i in range(1, 11):
        filename = "test/test" + str(i)
        exec_gen_2 = "time ./prog_out " + filename + " " + filename + "_2.exr 2" 
        print(exec_gen_2)
        os.system(exec_gen_2)
        exec_gen_3 = "time ./prog_out " + filename + " " + filename + "_3.exr 3" 
        print(exec_gen_3)
        os.system(exec_gen_3)

