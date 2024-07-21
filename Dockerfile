FROM debian:bullseye


RUN apt-get update && \
    apt-get install -y g++ make curl libcriterion-dev


WORKDIR /app


COPY ./src /app/src
COPY ./include /app/include
COPY ./exemple_files /app/exemple_files
COPY ./www/api /app/www/api
COPY ./Makefile /app/
COPY entrypoint.sh /app/

RUN chmod +x /app/entrypoint.sh


ENTRYPOINT [ "/app/entrypoint.sh" ]
