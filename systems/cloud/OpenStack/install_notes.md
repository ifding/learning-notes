
# OpenStack

```
$ openstack endpoint list --interface public

$ openstack endpoint set --url <modified> <endpoint_id>
```

(where <modified> is the URL with the host part substituted with the
FQDN; and <endpoint_id> is the endpoint you are changing the URL for.

Once you've done that for all the public endpoints, you can restart all
the services:

```
$ service apache2 restart (this restarts keystone)
$ service nova-api restart
$ service neutron-server restart
```

<br>
<br>
<br>
<hr>
<br>

Cloud Foundry deployment on Openstack keystone v3

1.Validate your OpenStack

```
# nano ~/.fog
:openstack:
  :openstack_auth_url:  http://130.127.133.140:5000/v3/auth/tokens
  :openstack_api_key:   b40da891efe76b0b9777
  :openstack_username:  adminapi
  :openstack_project_name:    admin
  :openstack_domain_id:   default
  :openstack_region:    RegionOne  # Optional


# fog openstack
>> Compute[:openstack].servers
  <Fog::Compute::OpenStack::Servers
    filters={}
    [
          
    ]
  >
>> 
```

2. CAN YOU ACCESS OPENSTACK METADATA SERVICE FROM A VIRTUAL MACHINE?
   Make sure you can ssh vcap＠ip -i bosh.pem to check that your private key is correct.

```
# curl http://169.254.169.254

1.0
2007-01-19
2007-03-01
2007-08-29
2007-10-10
2007-12-15
2008-02-01
2008-09-01
2009-04-04
```

3. Create `~/my-bosh/admin-openrcs.sh` file and copy the relative content in controller node: `/root/setup/admin-openrc.sh`.

4. Virtual machines paused due to low disk space
