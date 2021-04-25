FROM markstreet/n64chain:latest

RUN apt-get update && apt-get install -y \
  make \
  python3

WORKDIR /sssv-precomp
