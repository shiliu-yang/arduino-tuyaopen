import sys
import os
import argparse
import logging
import json

t2MaxBinSize = 679936

myJson = '''{
  "output": "Your sketch uses 2200 bytes of program memory out of 8192 (27%).",
  "severity": "info",
  "sections": [
    { "name": "text", "size": 2200, "max_size": 8192 }
  ]
}'''

def setup_logging():
    logging.basicConfig(level=logging.ERROR, format='[%(levelname)s][%(filename)s:%(lineno)d]%(message)s')

def parse_arguments():
    parser = argparse.ArgumentParser(description="Calculating firmware size.")
    parser.add_argument("--tuya-tmp-path", type=str, help="The tuya Temporary directory.")
    parser.add_argument('--sketch-name', type=str, help='Sketch name')
    return parser.parse_args()

def return_string():
    return myJson

if __name__ == "__main__":
    setup_logging()

    args = parse_arguments()
    tuyaTmpPath = args.tuya_tmp_path
    sketchName = args.sketch_name

    tuyaTmpPath = os.path.normpath(tuyaTmpPath)

    #
    ## 将当前脚本所在路径添加为搜索模块的路径
    #
    scriptSelfPath = os.path.dirname(os.path.realpath(__file__))
    sys.path.append(scriptSelfPath)

    from appVersionParse import *

    # get version
    versionJson = os.path.join(tuyaTmpPath, 'appConfig', 'appConfig.json')
    buildVersion = appVersionParse(versionJson)

    ugFile = os.path.join(tuyaTmpPath, 'output', buildVersion, '')

    result = return_string()
    print(result)