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
  sudo touch /home/pi/projects/bfhome/mosquitto/config/mosquitto.passwd
  sudo chown -R 1883:1883 /home/pi/projects/bfhome/mosquitto

  copy google assistant keys json to homeassistant/SERVICE_ACCOUNT.json

  docker-compose up -d
```
Set mosquitto password:
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

## Ssl:
nginx-proxy-manager - localhost:81 + duck dns.
https://nginxproxymanager.com/#quick-setup
https://dummylabs.com/ru/post/2020-01-27-the-easiest-way-to-https-home-assistant/



