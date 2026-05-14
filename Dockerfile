FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt full-upgrade -y && apt-get install -y git iptables vim openssl acl sudo g++ rustc golang dotnet-sdk-8.0 clang zlib1g-dev libseccomp-dev libwebsockets-dev debootstrap python3 && rm -rf /var/lib/apt/lists/*

RUN cd /tmp && dotnet new console -n Warmup && cd Warmup && dotnet publish -c Release -r linux-x64 -p:PublishAot=true && cd / && rm -rf /tmp/Warmup

COPY . . 

EXPOSE 6000

#Si aici tre sa fie pe run_server.sh
CMD ["/bin/bash", "-c", "sudo bash init_server.sh && sudo bash run_server.sh"]