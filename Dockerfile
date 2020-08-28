FROM debian:latest
RUN apt-get update

RUN apt-get update && \
    apt-get install -y build-essential libzmq3-dev python3-yaml \
    python3-jinja2 libgtest-dev libgflags-dev gcovr pkg-config git

RUN mkdir /prologin
RUN git clone https://github.com/prologin/stechec2 /prologin/stechec2
RUN apt-get install -y wget unzip
RUN wget https://wsend.net/dca6730b4e9192824417b7103f9341ce/prologin2020_plante-master.zip && unzip prologin2020_plante-master.zip
RUN mkdir /prologin/stechec2/games/prologin2020_plante && cp -r prologin2020_plante-master/* /prologin/stechec2/games/prologin2020_plante

RUN ( cd /prologin/stechec2 && python3 ./waf.py configure --with-games=prologin2020_plante --prefix=/usr && python3 ./waf.py build install && stechec2-generator player prologin2020_plante player_env )

RUN rm -rf /prologin/stechec2 prologin2020_plante-master prologin2020_plante-master.zip

RUN apt-get install -y python3-pip && pip3 install websockets

RUN stechec2-generator player prologin2020_plante /prologin
