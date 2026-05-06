#!/usr/bin/env python3

import os
import re
import subprocess

def set_cwd_to_project_root():
    script_path = os.path.abspath(__file__)
    script_dir = os.path.dirname(script_path)
    os.chdir(script_dir)
    os.chdir("..")


def get_project_tree(project_root):
    filetree = []
    for base_path, dirs, files in os.walk(project_root):
        for file in files:
            filetree.append(os.path.join(base_path, file))
    return filetree


def filter_files_for_analysis(files):
    folder_filter = re.compile(".*/(build|ThirdParty|tests|Tests|.git|.cache)/.*")
    file_filter = re.compile("^.*(/).*(.cpp|.c|.hpp|.h)$")
    files_for_analysis = []
    for file in files:
        if not folder_filter.match(file) and file_filter.match(file):
            files_for_analysis.append(file)
    return files_for_analysis


def run_analysis(files):
    if not files:
        print("No files to analyze.")
        return
    abs_files = [re.escape(os.path.abspath(f)) for f in files]
    cmd = ["python3", "./scripts/run-clang-tidy.py", "-config-file", "./.clang-tidy", "-p", "."] + abs_files
    subprocess.run(cmd)


set_cwd_to_project_root()
files = filter_files_for_analysis(get_project_tree("."))
print(files)
run_analysis(files)
