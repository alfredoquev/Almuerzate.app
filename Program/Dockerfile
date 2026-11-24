# ---------- Stage 1: compile main.so on Linux ----------
FROM debian:bookworm-slim AS cpp-builder
RUN apt-get update \
 && apt-get install -y --no-install-recommends g++ \
 && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY Backend/Business/main.cpp Backend/Business/clases.hpp ./
RUN g++ -std=c++17 -O2 -fPIC -shared -o main.so main.cpp

# ---------- Stage 2: PHP + PostgreSQL + FFI ----------
FROM php:8.3-apache-bookworm
RUN apt-get update \
 && apt-get install -y --no-install-recommends libpq-dev libffi-dev \
 && docker-php-ext-configure ffi --with-ffi \
 && docker-php-ext-install pdo_pgsql pgsql ffi \
 && apt-get purge -y --auto-remove libffi-dev \
 && rm -rf /var/lib/apt/lists/*
RUN echo 'ffi.enable=true' > /usr/local/etc/php/conf.d/zz-ffi.ini

COPY . /var/www/html/
COPY --from=cpp-builder /src/main.so /var/www/html/Backend/Business/main.so

# Make Apache listen on 6767 instead of 80
RUN sed -i 's/^Listen 80$/Listen 6767/' /etc/apache2/ports.conf \
 && sed -i 's/<VirtualHost \*:80>/<VirtualHost *:6767>/' /etc/apache2/sites-available/000-default.conf
EXPOSE 6767