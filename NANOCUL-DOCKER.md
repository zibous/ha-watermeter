# Docker + wmbusmeters with NANO-CUL (mbus) 868 Mhz
wmbusmeters docker is able to detect attachment and removal of wmbus dongles and to provide that functionality within docker image it must be started in privileged mode to have access to hosts /dev/ content.

see: https://hub.docker.com/r/weetmuts/wmbusmeters
<br>

![nanoCUL868](docs/nanocul_v3/nanocul.jpg)

More Info see: [Testcase Flash CUL-Stick](docs/nanocul_v3/README.md)

https://www.smart-home-komponente.de/nano-cul/nano-cul-868-extra/


## Docker wmbusmeters

With this bash script, wmbusmeters is installed as a docker application


```bash
#!/bin/bash
# ----------------------------------
# sudo sh wmbusmeters.sh
# ----------------------------------
DOCKER_APPSDIR=/apps/
DOCKER_TIMEZONE=Europe/Berlin
DOCKERIMAGE=weetmuts/wmbusmeters
CONTAINERLABEL=wmbusmeters
APPSDATA=$PWD${DOCKER_APPSDIR}${CONTAINERLABEL}

echo "Create persistent data folder and log folder"
mkdir -p ${APPSDATA} 2>&1

echo "Try to remove previuos installation..."
docker stop ${CONTAINERLABEL} >/dev/null 2>&1
docker rm ${CONTAINERLABEL} >/dev/null 2>&1

echo "Install Docker container ${CONTAINERLABEL}."
docker run --detach --interactive \
  --privileged \
  --name=${CONTAINERLABEL} \
  --restart=always \
  --volume /etc/localtime:/etc/localtime:ro \
  --volume ${APPSDATA}:/wmbusmeters_data \
  --volume /dev/:/dev/ \
  ${DOCKERIMAGE}

echo "Docker container ${CONTAINERLABEL} ready."
```

