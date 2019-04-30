Installing Vagrant and VirtualBox on CentOS
=========
>02-06-2017

### Step 1: Installing Virtualbox
Installing VirtualBox 5.1 in Debian, Ubuntu and Linux Mint

```
$ sudo apt-get remove virtualbox-4*
$ sudo sh -c 'echo "deb http://download.virtualbox.org/virtualbox/debian $(lsb_release -cs) contrib" >> /etc/apt/sources.list.d/virtualbox.list'
$ wget -q https://www.virtualbox.org/download/oracle_vbox.asc -O- | sudo apt-key add -  
$ sudo apt-get update  
$ sudo apt-get install virtualbox-5.1  
```

### Step 2: Download and install vagrant

```
$ wget https://releases.hashicorp.com/vagrant/1.8.7/vagrant_1.8.7_x86_64.deb      
$ dpkg -i vagrant_1.8.7_x86_64.deb

```

### Step 3: Getting vagrant machine up

```
$ vagrant box add precise32 http://files.vagrantup.com/precise32.box
$ mkdir vagrant_project && cd vagrant_project
$ vagrant init
$ nano vargrantfile
  config.vm.box = "precise32"
$ vagrant up
connect using SSH
$ vagrant ssh
```

```
$ vagrant init ubuntu/trusty64
$ vagrant up
```

### Step 4: return to the host machine

```
$ exit
```

### Step 5: destroy the VM and remove the binary image

```
$ vagrant destroy
```
This command must be run from the directory which contains the vagrantfile you want to remove.


### Reference

* [Vagrant by HashiCorp](https://www.vagrantup.com/)
