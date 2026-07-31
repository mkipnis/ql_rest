1. create docker swarm instance
	
$ docker swarm init

2. join workers

$ docker swarm join --token SWMTKN-1-3p8p57vsvnqcph0inugutjsovb7cqjq0ofdtg58apuuo1il2zo-4qqbm5tfrfzovniqwj6a1a803 192.168.65.3:2377

3. start on the single instance

docker stack deploy -c docker-compose-instance-per-node-1.yml options

4. check service
docker stack services options
