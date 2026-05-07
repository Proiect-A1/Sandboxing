FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt full-upgrade -y && apt-get install -y git iptables vim openssl acl sudo g++ rustc libseccomp-dev && rm -rf /var/lib/apt/lists/*

COPY . . 

EXPOSE 6000

#Si aici tre sa fie pe run_server.sh
CMD ["/bin/bash", "-c", "sudo bash init_server.sh && sudo bash run_tests.sh && sudo bash run_server.sh server"]