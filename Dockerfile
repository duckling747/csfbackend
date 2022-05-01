FROM debian:bullseye-slim as builder
WORKDIR /app
RUN apt update && apt install -y build-essential libboost-contract-dev git libz-dev && \
    git clone https://github.com/duckling747/csfbackend.git && \
    cd ./csfbackend && \
    apt install -y ./crow-v1.0+2.deb && \
    g++  -pthread -static -DCROW_ENABLE_COMPRESSION -D_FORTIFY_SOURCE=2 \
    -fstack-protector-strong -fPIE -pie -O3 main.cpp -lz

FROM scratch
WORKDIR /app
COPY --from=builder /app/csfbackend/a.out \
    /app/csfbackend/*.txt /app/csfbackend/*png /app/csfbackend/*json \
    /app/csfbackend/*ico  ./
COPY --from=builder /app/csfbackend/static ./static
COPY --from=builder /app/csfbackend/templates ./templates
ENTRYPOINT ["./a.out"]
