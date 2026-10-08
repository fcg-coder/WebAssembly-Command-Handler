# ─────────────────────────────────────────────────────────────
# Stage 1: build
# ─────────────────────────────────────────────────────────────
FROM node:20-alpine AS build

WORKDIR /app

# package.json находится в frontend/
COPY frontend/package.json frontend/package-lock.json* ./

RUN npm ci

# Копируем frontend в /app
COPY frontend/ .

# Копируем notes в /app/notes,
# чтобы vite-plugin-notes увидел /app/notes
COPY notes ./notes

# Сборка прод-бандла
RUN npm run build


# ─────────────────────────────────────────────────────────────
# Stage 2: serve
# ─────────────────────────────────────────────────────────────
FROM nginx:1.27-alpine

RUN printf '%s\n' \
    'server {' \
    '    listen       80;' \
    '    server_name  _;' \
    '' \
    '    root   /usr/share/nginx/html;' \
    '    index  index.html;' \
    '' \
    '    gzip on;' \
    '    gzip_types text/plain text/css application/javascript application/json image/svg+xml application/wasm;' \
    '    gzip_min_length 1024;' \
    '' \
    '    location ~* \.wasm$ {' \
    '        default_type application/wasm;' \
    '    }' \
    '' \
    '    location /wasm/ {' \
    '        try_files $uri $uri/ =404;' \
    '    }' \
    '' \
    '    location /assets/ {' \
    '        expires 1y;' \
    '        add_header Cache-Control "public, immutable";' \
    '        try_files $uri =404;' \
    '    }' \
    '' \
    '    location / {' \
    '        try_files $uri $uri/ /index.html;' \
    '    }' \
    '' \
    '    server_tokens off;' \
    '}' \
    > /etc/nginx/conf.d/default.conf

COPY --from=build /app/dist /usr/share/nginx/html

EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]