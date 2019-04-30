# Quick Tutorial for PBS

The Portable Batch System (PBS) system is designed to manage the distribution of batch jobs and interactive sessions across the available nodes in the cluster.

##  PBS Commands

The following is a list of the basic PBS commands.

### Job Control
- qsub: Submit a job
- qdel: Delete a batch job
- qsig: Send a signal to batch job
- qhold: Hold a batch job
- qrls: Release held jobs
- qrerun: Rerun a batch job
- qmove: Move a batch job to another queue

### Job Monitoring
- qstat: Show status of batch jobs
- qselect: Select a specific subset of jobs

### Node Status
- pbsnodes: List the status and attributes of all nodes in the cluster

### Miscellaneous
- qalter: Alter a batch job
- qmsg: Send a message to a batch job

## Setting PBS Job Attributes

PBS job attributes can be set in two ways:
- as command-line arguments to qsub, or
- as PBS directives in a control script submitted to qsub.

The following is a list of some common PBS job attributes.

| Attribute    | Values            | Description |
|:-------------|:------------------|:------------|
| -l           | comma separated list of required resources | e.g. nodes=2, cput=00:10:00  |
| -N | name for the job   | Declares a name for the job  |
| -o           | [hostname:]pathname      | Defines the path to be used for STDOUT stream  |
| -e           | [hostname:]pathname | Defines the path to be used for STDERR stream |
| -p | interger between -1024 and +1023 | Defines the priority of the job. Higher values correspond to higher priorities |
| -q | name of destination queue, server, or queue at a server | Defines the destination of the job. The default settings is sufficient for most purposes |

Below is a table summarizing some commands that can be used inside PBS job scripts.

| PBS command | Effect |
|:------------|:-------|
| #!/bin/bash -l | Specifies how the PBS file should be read (by the bash interpreter). A statement like this is required to be first line of a PBS script) |
| #PBS -l walltime=2:00:00, nodes=1:ppn=8, pmem=2500mb | The resource request (required). The resource request must specify the job walltime, number of nodes, processor cores per node (ppn), memory per processor core (pmem), the required total memory (mem) |
| #PBS -m abe | Makes the PBS system send message emails when the job aborts, begins, or ends |
| #PBS -M sample@gmail.com | Specifies the email address that should be used when the PBS system sends message emails |
| #PBS -N jobname | Specifies a name for the job that will appear in the job queue |
| #PBS -o output_filename | Directs the job standard output to be placed in the named file |
| #PBS -e error_filename | Directs the job error output to be placed in the named file |
| #PBS -q queue_name | Specifies that the job should be run in the named queue |

## PBS Script Format
```sh
#!/bin/bash -l        
#PBS -l walltime=8:00:00,nodes=3:ppn=8,mem=10gb 
#PBS -m abe 
#PBS -M sample_email@umn.edu 
cd ~/program_directory
module load intel 
module load ompi/intel 
mpirun -np 24 program_name < inputfile > outputfile
```

A PBS script should contain the appropriate change directory commands to get to the job execution location. It also needs to contain module load commands for any software modules that the calculation might need. The last line contains an execution command to start a program which uses MPI communication to run on 24 processor cores.

## Submitting Job Scripts

Once a job script is made it is submitted using the `qsub` command:
```sh
$ qsub -q queuename script_name
```

To view all of the jobs submitted by a particular user use the command:
```sh
$ qstat -u username
```

To cancel a submitted job use the command:
```sh
$ qdel job_id_number
```

## Reference
- [Job Submission and Scheduling (PBS Scripts)](https://www.msi.umn.edu/content/job-submission-and-scheduling-pbs-scripts)
- [Quick Tutorial for Portable Batch System (PBS)](https://albertsk.files.wordpress.com/2011/12/pbs.pdf)
- [Tutorial - Submitting a job using qsub](https://wikis.nyu.edu/display/NYUHPC/Tutorial+-+Submitting+a+job+using+qsub)
