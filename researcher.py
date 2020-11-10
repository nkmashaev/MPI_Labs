import os
import sys
import subprocess
from typing import List, Dict, Any

def mpi_exec_search(dir_path : str) -> List[str]:
    file_list = []
    for curr_dir, sub_dirs, files in os.walk(dir_path):
        if not os.path.isabs(curr_dir):
            curr_dir = os.path.abspath(curr_dir)

        if len(files) != 0:
            file_list += [os.path.join(curr_dir, i) for i in files if i.endswith("_mpi")]
    return file_list

def mpi_run_com(mpi_exec_list : List[str],
                ps_start : int = 1,
                ps_end : int = 4,
                ps_step : int = 1
               ) -> List[List]:
    PS_NUMB = slice(2, 3, 1)

    mpi_pss = []
    for file in mpi_exec_list:
        ps = ["mpirun", "-np", "1", file]
        for i in range(ps_start, ps_end + 1, ps_step):
            ps[PS_NUMB] = str(i)
            curr_ps = [j for j in ps]
            mpi_pss.append(curr_ps)
    return mpi_pss

def researcher_parameters( rdict : Dict[str, Any]):
    argv = sys.argv[1:]
    i = 0
    
    while i < len(argv):
        key = argv[i]
        assert argv[i] in rdict

        i += 1
        if key == "--path":
            rdict[key] = argv[i]
        else:
            rdict[key] = int(argv[i])
        i += 1

rdict = {
            "--path": ".",
            "--pstart": 1,
            "--pend": 4,
            "--pstep": 1,
            "--iter": 10
        }
researcher_parameters(rdict)
mpi_exec_list = mpi_exec_search(rdict["--path"])
mpi_run_list = mpi_run_com(mpi_exec_list,
                            rdict["--pstart"],
                            rdict["--pend"],
                            rdict["--pstep"])
print(mpi_exec_list)
print(*mpi_run_list, sep="\n")
for val in mpi_run_list:
    min_time = None
    max_time = None
    total_time = 0.0
    for i in range(0, rdict["--iter"]):
        with subprocess.Popen(val, stdout=subprocess.PIPE) as ps:
            stdout, stderr = ps.communicate()
            rec_data_list = stdout.decode().split("\n")
            exec_time = float(rec_data_list[-2].split()[2])
        total_time += exec_time
        if min_time is None or min_time > exec_time:
            min_time = exec_time
        if max_time is None or max_time < exec_time:
            max_time = exec_time
    average = total_time / rdict["--iter"]
    name = os.path.basename(val[-1])
    print(
        name.ljust(20, " "),
        val[-2],
        "{:.11E}".format(min_time).center(20, " "),
        "{:.11E}".format(max_time).center(20, " "),
        "{:.11E}".format(average).center(20, " "),
        sep="|",
    )
