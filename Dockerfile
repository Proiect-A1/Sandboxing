FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt full-upgrade -y && apt-get install -y git iptables vim openssl acl sudo g++ && rm -rf /var/lib/apt/lists/*

WORKDIR /sandbox

# Schimbam -b pe dev
RUN git clone https://github.com/Proiect-A1/Sandboxing.git -b sochu 

WORKDIR /sandbox/Sandboxing  

EXPOSE 6000

#Si aici tre sa fie pe run_server.sh
CMD ["/bin/bash", "-c", "sudo bash init_server.sh && sudo bash run_tests.sh"]