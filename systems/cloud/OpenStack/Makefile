.ONESHELL: all rvm bosh secgroup ssh bosh-init cf-stub upload deploy

all: rvm bosh secgroup ssh bosh-init cf-stub upload deploy

rvm:
	sudo apt-get update
	sudo apt-get install curl
	curl -sSL https://rvm.io/mpapis.asc | sudo gpg --import -
	curl -L https://get.rvm.io | bash -s stable --ruby
	source /usr/local/rvm/scripts/rvm
	rvm -v
	ruby -v

bosh:
	gem install bosh_cli --no-ri --no-rdoc
	bosh -v

secgroup:
	source ./admin-openrc.sh
	nova secgroup-create bosh bosh
	nova secgroup-create cf cf
	nova secgroup-add-group-rule bosh bosh tcp 1 65535
	nova secgroup-add-rule bosh tcp 22 22 0.0.0.0/0
	nova secgroup-add-rule bosh tcp 6868 6868 0.0.0.0/0
	nova secgroup-add-rule bosh tcp 25555 25555 0.0.0.0/0
	nova secgroup-add-rule cf tcp 80 80 0.0.0.0/0
	nova secgroup-add-rule cf tcp 443 443 0.0.0.0/0
	nova secgroup-add-rule cf udp 68 68 0.0.0.0/0
	nova secgroup-add-rule cf tcp 4443 4443 0.0.0.0/0
	nova secgroup-add-group-rule cf cf tcp 1 65535
	nova secgroup-add-rule cf tcp 22 22 0.0.0.0/0
	nova secgroup-add-rule cf icmp -1 -1 0.0.0.0/0

ssh:
	ssh-keygen -t rsa -b 4096 -C "bosh" -N "" -f "bosh.pem"
	mv bosh.pem.pub bosh.pub
	nova keypair-add --pub-key ~/my-bosh/bosh.pub  bosh

bosh-init:
	wget https://s3.amazonaws.com/bosh-init-artifacts/bosh-init-0.0.96-linux-amd64
	chmod +x ~/my-bosh/bosh-init-*
	sudo mv ~/my-bosh/bosh-init-* /usr/local/bin/bosh-init
	bosh-init -v
	bosh-init deploy ./bosh.yml

cf-stub:
	git clone https://github.com/cloudfoundry/cf-release.git
	cd cf-release
	./scripts/update
	wget https://github.com/cloudfoundry-incubator/spiff/releases/download/v1.0.7/spiff_linux_amd64.zip
	sudo apt-get install unzip
	unzip spiff_linux_amd64.zip
	mv spiff /usr/bin
	chmod +x /usr/bin/spiff
	spiff -v

upload:		
	./scripts/generate_deployment_manifest openstack /root/my-bosh/cf-stub.yml > cf-deployment.yml
	bosh deployment cf-deployment.yml
	wget --content-disposition https://bosh.io/d/stemcells/bosh-openstack-kvm-ubuntu-trusty-go_agent?v=3263.5
	bosh upload stemcell /root/my-bosh/cf-release/bosh-stemcell-3263.5-openstack-kvm-ubuntu-trusty-go_agent.tgz
	bosh create release
	bosh upload release releases/cf-244.yml
  
deploy:
	bosh deploy

clean:
	rm ~/my-bosh/bosh-init-*
	rm ~/my-bosh/cf-release/spiff_linux_*
	rm ~/my-bosh/cf-release/bosh-stemcell-*
