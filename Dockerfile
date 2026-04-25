FROM debian:trixie-slim AS builder

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    gcc-14 \
    g++-14 \
    libpqxx-dev \
    && rm -rf /var/lib/apt/lists/*

ENV CC=gcc-14 CXX=g++-14
ARG APP_NAME
WORKDIR /src
COPY . .

RUN cmake -B build -S . -DCMAKE_BUILD_TYPE=RELEASE -DAPP_NAME=${APP_NAME} \
    && cmake --build build \
    && cmake --install build --prefix /install

FROM debian:trixie-slim
ARG APP_NAME

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    libpqxx-7.10 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /install/bin/${APP_NAME} /usr/local/bin/${APP_NAME}
RUN chmod +x /usr/local/bin/${APP_NAME}

ENV EXE_NAME=${APP_NAME}
ENTRYPOINT /usr/local/bin/${EXE_NAME}
