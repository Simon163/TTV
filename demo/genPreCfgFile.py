from __future__ import print_function

import os
import sys
from yamlParser import *

def usage():
    """ print usage info """
    print("usage: genPreCfgFile.py -h/--help")
    print("Usage: genPreCfgFile.py xxx.yaml")

def create_preprocess_config_file(preprocess_paras, preprocess_file):
    preprocess_lines = [[1, "uint32", 3, "input_channel"],
                        [2, "uint32", 224, "input_h"],
                        [3, "uint32", 224, "input_w"],
                        [4, "uint32",  1,  "mean_type"],
                        [5, "float", 103.94, "mean_value_r"],
                        [6, "float", 116.78, "mean_value_g"],
                        [7, "float", 123.68, "mean_value_b"],
                        [8, "string", "./mean.txt", "mean_map"],
                        [9, "float", 0.017, "scale_value"]]

    # print(preprocess_paras.keys())
    if "input_channel" in preprocess_paras.keys() and isinstance(preprocess_paras["input_channel"][1], (int)):
        preprocess_lines[0][2] = preprocess_paras["input_channel"][1]
    else:
        raise ValueError(
            "Error: missing or invalid channel value, it must be positive integer, please check the preprocess file!")

    if "input_h" in preprocess_paras.keys() and isinstance(preprocess_paras["input_h"][1], (int)):
        preprocess_lines[1][2] = preprocess_paras["input_h"][1]
    else:
        raise ValueError(
            "Error: missing or invalid height value, it must be positive integer, please check the preprocess file!")

    if "input_w" in preprocess_paras.keys() and isinstance(preprocess_paras["input_w"][1], (int)) and preprocess_paras["input_w"][1] > 0:
        preprocess_lines[2][2] = preprocess_paras["input_w"][1]
    else:
        raise ValueError(
            "Error: missing or invalid width value, it must be positive integer, please check the preprocess file!")

    if "mean_type" in preprocess_paras.keys() and preprocess_paras["mean_type"][1] in [0, 1, 2]:
        preprocess_lines[3][2] = preprocess_paras["mean_type"][1]
    else:
        raise ValueError(
            "Error: missing or invalid type of mean, it must be either 0 or 1 or 2, please check the preprocess file!")

    if "scale_value" in preprocess_paras.keys() and isinstance(preprocess_paras["scale_value"][1], (int, float)) and preprocess_paras["scale_value"][1] > 0 and preprocess_paras["scale_value"][1] <= 1.0:
        preprocess_lines[8][2] = float(preprocess_paras["scale_value"][1])
    else:
        raise ValueError(
            "Error: missing or invalid scale value, it must be float with range (0.0, 1.0], please check the preprocess file!")

    # print(preprocess_lines)

    with open(preprocess_file, 'w') as preprocess:
        for line in preprocess_lines:
            preprocess.writelines(str(line[0]) + " " + str(line[1]) +
                                    " " + str(line[2]) + " " + str(line[3]) + "\n")
            
    print("Generate preprocessing configuration file succeeded, now you can serilize preprocessing configuration parameters using TTV.")

    return preprocess_paras

def main():
    """ main funcion """
    # argc
    argc = len(sys.argv)
    #print(argc)

    if argc != 2:
        usage()

    if sys.argv[1] == "-h" or sys.argv[1] == "--help":
        usage()
        return

    yamlFile = sys.argv[1]
    if yamlFile.find(".yaml") == -1:
        print("Error: the input file is not a valid yaml file! Its name must contain the extension .yaml\n")
        usage()
        return

    single_file = os.path.split(yamlFile)[1]
    (filename, _) = os.path.splitext(single_file)
    txt_file = filename + '.txt'

    dict = yamlParser.read(yamlFile)
    print(dict)

    create_preprocess_config_file(dict[0], txt_file)

if __name__ == "__main__":
    main()



