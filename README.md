# Create RPI os image:
- download
- burn using RPI burner
- enable ssh(file)

# Set up:
```
  ssh pi@[IP]
  sudo apt update

  // this way docker version is old. Sept of 2019
  sudo apt install docker docker-compose
  sudo groupadd docker
  sudo usermod -aG docker $USER
  newgrp docker
```
Add github ssh keys.
```
  mkdir projects
  cd projects
  git clone git@github.com:BF-man/bfhome.git
  cd bfhome
```
```
  sudo chown -R 1883:1883 ~/mosquitto
  docker-compose up -d
```
Set mosquitto password. First time only:
```
  docker exec -it mosquitto sh
  mosquitto_passwd -c /mosquitto/config/mosquitto.passwd <username>
```

Restart:
```
docker-compose build
docker-compose stop
docker-compose rm -f
docker-compose up -d
```



