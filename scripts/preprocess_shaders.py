#!/usr/bin/env python3
import sys
import os
import re

def replace_includes(shader_path, common_dir, included_files=None):
    if included_files is None:
        included_files = set()

    with open(shader_path, 'r') as file:
        content = file.read()

    # Replace includes recursively
    def include_replacer(match):
        include_file = match.group(1)
        include_path = os.path.join(common_dir, include_file)
        
        if not os.path.isfile(include_path):
            print(f"Include file '{include_file}' not found in '{common_dir}'.")
            return ''
        
        if include_path in included_files:
            print(f"Circular include detected for '{include_file}'. Skipping.")
            return ''  # prevent circular includes

        included_files.add(include_path)
        return replace_includes(include_path, common_dir, included_files)

    return re.sub(r'#include <(.*?)>', include_replacer, content)

if __name__ == "__main__":
    input_shader = sys.argv[1]
    output_shader = sys.argv[2]
    common_dir = sys.argv[3]

    processed_content = replace_includes(input_shader, common_dir)

    with open(output_shader, 'w') as file:
        file.write(processed_content)
