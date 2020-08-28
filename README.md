# Create RPI os image:
- download
- burn using RPI burner
- enable ssh(file)

# Set up:
```
  ssh pi@[IP]
  sudo apt update
  sudo apt install docker docker-compose
```
Add github ssh keys.
```
  mkdir projects
  cd projects
  git clone git@github.com:BF-man/bfhome.git
  cd bfhome
```
```
  docker-compose up -d
```
Set mosquitto password. First time only:
```
  docker exec -it mosquitto sh
  mosquitto_passwd -c /mosquitto/config/mosquitto.passwd <username>`


