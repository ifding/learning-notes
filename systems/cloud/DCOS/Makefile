.ONESHELL: all update 

all: update repo override docker boot cluster

update:
	sudo yum upgrade --assumeyes --tolerant
	sudo yum update --assumeyes
	uname -r
	sudo tee /etc/modules-load.d/overlay.conf <<-'EOF'
	overlay
	EOF
	reboot
	
repo:
	lsmod | grep overlay
	sudo tee /etc/yum.repos.d/docker.repo <<-'EOF'
	[dockerrepo]
	name=Docker Repository
	baseurl=https://yum.dockerproject.org/repo/main/centos/$releasever/
	enabled=1
	gpgcheck=1
	gpgkey=https://yum.dockerproject.org/gpg
	EOF
	sudo mkdir -p /etc/systemd/system/docker.service.d && sudo tee /etc/systemd/system/docker.service.d/override.conf <<- 'EOF'
	[Service]
	ExecStart=
	ExecStart=/usr/bin/docker daemon --storage-driver=overlay -H fd://
	EOF
docker:
	sudo yum install -y https://yum.dockerproject.org/repo/main/centos/7/Packages/docker-engine-1.11.2-1.el7.centos.x86_64.rpm --assumeyes --tolerant	
	sudo systemctl start docker
	sudo systemctl enable docker
	docker info
	
boot:
	sudo yum -y install epel-release
	sudo yum -y install python-pip
	sudo pip install virtualenv
	sudo systemctl stop firewalld && sudo systemctl disable firewalld
	sudo docker pull nginx
	sudo yum install -y ntp
cluster:
	sudo yum install -y tar xz unzip curl ipset nano ntp
	sudo sed -i s/SELINUX=enforcing/SELINUX=permissive/g /etc/selinux/config
	sudo groupadd nogroup
	sudo sysctl -w net.ipv6.conf.all.disable_ipv6=1
	sudo sysctl -w net.ipv6.conf.default.disable_ipv6=1
	sudo reboot
	
ssh:
	cat ~/.ssh/id_rsa.pub | ssh -i master.pem centos@10.11.10.12 "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"
	cat ~/.ssh/id_rsa.pub | ssh -i master.pem centos@10.11.10.16 "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"
	cat ~/.ssh/id_rsa.pub | ssh -i master.pem centos@10.11.10.17 "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"
	cat ~/.ssh/id_rsa.pub | ssh -i agent.pem centos@10.11.10.14 "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"
	cat ~/.ssh/id_rsa.pub | ssh -i agent.pem centos@10.11.10.18 "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"
	cat ~/.ssh/id_rsa.pub | ssh -i agent.pem centos@10.11.10.19 "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"
	cat ~/.ssh/id_rsa.pub | ssh -i agent.pem centos@10.11.10.20 "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"
	cat ~/.ssh/id_rsa.pub | ssh -i agent.pem centos@10.11.10.21 "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"
	
dcos:
	curl -O https://downloads.dcos.io/dcos/stable/dcos_generate_config.sh
	sudo bash dcos_generate_config.sh
	sudo docker run -d -p 8848:80 -v $PWD/genconf/serve:/usr/share/nginx/html:ro nginx
	
master:
	mkdir /tmp/dcos && cd /tmp/dcos
	curl -O http://10.11.10.7:8848/dcos_install.sh
	sudo bash dcos_install.sh master
agent:
	mkdir /tmp/dcos && cd /tmp/dcos
	curl -O http://10.11.10.7:8848/dcos_install.sh
	sudo bash dcos_install.sh slave
