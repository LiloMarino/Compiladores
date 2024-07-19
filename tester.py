import os
import subprocess
import filecmp
import difflib

def run_program(executable, input_folder, output_folder):
    input_files: list[str] = [f for f in os.listdir(input_folder) if f.endswith('.txt')]
    for input_file in input_files:
        input_path = os.path.join(input_folder, input_file)
        output_path = os.path.join(output_folder, input_file.replace("entrada","saida"))
        with open(input_path, 'r') as infile, open(output_path, 'w') as outfile:
            subprocess.run([executable], stdin=infile, stdout=outfile)

def compare_files(output_folder, expected_folder):
    output_files = [f for f in os.listdir(output_folder) if f.endswith('.txt')]
    identical_count = 0
    total_files = len(output_files)
    differences = []

    for output_file in output_files:
        output_path = os.path.join(output_folder, output_file)
        expected_path = os.path.join(expected_folder, output_file)

        if filecmp.cmp(output_path, expected_path, shallow=False):
            identical_count += 1
        else:
            with open(output_path, 'r') as out_file, open(expected_path, 'r') as exp_file:
                diff = list(difflib.unified_diff(
                    out_file.readlines(),
                    exp_file.readlines(),
                    fromfile='output',
                    tofile='expected'
                ))
                differences.append((output_file, diff))

    return identical_count, total_files, differences

def main():
    executable = './l6e1'
    input_folder = 'entrada'
    output_folder = 'saida'
    expected_folder = 'saida_esperada'

    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    run_program(executable, input_folder, output_folder)
    identical_count, total_files, differences = compare_files(output_folder, expected_folder)

    if differences:
        with open("diff.txt","w",encoding="utf-8") as diff_file:
            print('Diferenças encontradas:', file=diff_file)
            for output_file, diff in differences:
                print(f'Arquivo: {output_file}', file=diff_file)
                for line in diff:
                    print(line, end='', file=diff_file)

    percentage_identical = (identical_count / total_files) * 100
    print(f'\n{percentage_identical:.2f}% dos arquivos estão idênticos.')

if __name__ == '__main__':
    main()
