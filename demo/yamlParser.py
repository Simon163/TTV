from __future__ import print_function

import os
import sys
import yaml

class yamlParser(object):
    @classmethod
    def read(cls, yaml_file):
        with open(yaml_file, 'r') as file:
            file_data = file.read()
        yaml_data = yaml.load_all(file_data, Loader=yaml.FullLoader)
        yaml_dict = {}
        for index, data in enumerate(yaml_data):
            yaml_dict[index] = data
        return yaml_dict

    @classmethod
    def write(cls, dict, yaml_file):
        with open(yaml_file, 'w') as file:
            yaml.dump(dict, file)
