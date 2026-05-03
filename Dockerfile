FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt full-upgrade -y && apt-get install -y git iptables vim openssl acl sudo g++ libseccomp-dev && rm -rf /var/lib/apt/lists/*

WORKDIR /sandbox

# Schimbam -b pe dev
COPY . .

EXPOSE 6000

#Si aici tre sa fie pe run_server.sh
CMD ["/bin/bash", "-c", "sudo bash init_server.sh && sudo bash run_tests.sh"]