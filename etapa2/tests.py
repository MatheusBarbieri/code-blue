import subprocess
import os

output = ''
errors = ''
error_files = ''
failures = 0
success = 0
dir_list = os.listdir('./tests/')
tests_number = len(dir_list)
tests_dir = os.getcwd() + '/tests/'


print('Running {} {}.'.format(str(tests_number), 'test' if tests_number == 1 else 'tests'))

for file in dir_list:

    with open('./tests/{}'.format(file)) as ifile:
        isError = ('ZZ ERROR' in ifile.read())
        ifile.seek(0, 0)

        execution = ['./etapa2']
        process = subprocess.Popen(execution, stdin=ifile, stdout=subprocess.PIPE)
        result = process.communicate()[0].decode('utf-8')

        if process.returncode and not isError or isError and not process.returncode:
            print('E', end='')
            failures = failures + 1
            output = output + 'Test {}: code: {}\n{}--------------------\n'.format(
                file,
                str(process.returncode),
                str(result)
            )
            ifile.seek(0, 0)
            failing_test = ifile.read()
            errors = errors + 'Test {}: code: {}\n{}\n{}\n====================\n\n'.format(
                file,
                str(process.returncode),
                str(result),
                failing_test
            )

        else:
            print('.', end='')
            success = success + 1
            output = output + 'Test {}: Success!\n'.format(
                file
            )
print('\033[92m\nSuccess: {}\033[91m\nFailures: {}\033[0m'.format(success, failures))
with open('results.txt', 'wb') as ofile:
    ofile.write(output.encode('utf-8'))

with open('errors.txt', 'wb') as ofile:
    ofile.write(errors.encode('utf-8'))
