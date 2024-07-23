FROM debian:bullseye


RUN apt-get update && \
    apt-get install -y g++ make curl libcriterion-dev \
    siege


WORKDIR /app


COPY ./src /app/src
COPY ./include /app/include
COPY ./examples_files /app/examples_files
COPY ./www/api /app/www/api
COPY ./Makefile /app/
COPY entrypoint.sh /app/

RUN chmod +x /app/entrypoint.sh


ENTRYPOINT [ "/app/entrypoint.sh" ]
