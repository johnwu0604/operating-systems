Vagrant.configure("2") do |config|
    config.vm.box = "ubuntu/xenial64"
    config.vm.provider "virtualbox" do |v|
    	v.memory = 1024
	v.cpus = 1
    config.vm.network "forwarded_port", guest: 8100, host: 8000
    end
end

