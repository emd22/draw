import os

compiler = "g++"

options = "-g -std=c++14" #-std=c++14

src_dir = "./src"
bin_dir = "./bin"

if not os.path.exists(bin_dir):
    os.makedirs(bin_dir)

command = "{} {} -o {}/outfile -Iinclude/".format(compiler, options, bin_dir)

for dirpath, dirnames, filenames in os.walk(src_dir):
    for file in [f for f in filenames]:
        if file.endswith(".cpp"):
            print("{}/{}...".format(dirpath, file))
            command = "{} {}/{} ".format(command, dirpath, file)

if os.system("{}".format(command)) == 0:
    #success!
    os.system("clear; ./bin/outfile")
else:
    #failure
    print("Build Failed; stopping")

print("Build complete")
