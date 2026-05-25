FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
	&& apt-get install -y --no-install-recommends \
		iptables acl openssl sudo \
		g++ zlib1g-dev libseccomp-dev libwebsockets-dev \
		debootstrap \
		unzip zip \
		ca-certificates \
	&& rm -rf /var/lib/apt/lists/* /var/cache/apt/archives/*

WORKDIR /app
COPY . . 

EXPOSE 6000

#Si aici tre sa fie pe run_server.sh
CMD ["/bin/bash", "-c", "sudo bash init_server.sh && sudo bash run_server.sh"]
