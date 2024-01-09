from epitechcontent/epitest-docker:latest

COPY . /app
WORKDIR /app

RUN cmake -B build
RUN cmake --build build

ENTRYPOINT [ "./build/r-type_server", "5554", "127.0.0.1" ]