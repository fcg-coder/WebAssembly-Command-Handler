# Для vs code tasks.json 


```
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Docker: Build and Run Container",
            "type": "shell",
            "command": "rm -rf build && docker compose down && docker compose up --build",
            "group": {
                "kind": "build"
            },
            "problemMatcher": [],
            "isBackground": false,
            "runOptions": {
                "reevaluateOnRerun": true,
                "promptOnClose": false
            }
        },
        {
            "label": "CMake: Build Linux",
            "type": "shell",
            "command": "rm -rf build && mkdir build && cd build && cmake ../src -DUSE_TERMINAL=ON -DUSE_WEBASM=OFF && make -j6",
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [],
            "isBackground": false,
            "runOptions": {
                "reevaluateOnRerun": true,
                "promptOnClose": false
            }
        },
        {
            "label": "CMake: Build WebAsm",
            "type": "shell",
            "command": "source ~/emsdk/emsdk_env.sh && rm -rf build && mkdir -p build && emcmake cmake -S src -B build -DUSE_TERMINAL=OFF -DUSE_WEBASM=ON && cmake --build build -j6 && cp page/index.html page/fonts/*.ttf page/styles.css page/webasm.js build/",
            "group": {
                "kind": "build"
            },
            "problemMatcher": [],
            "isBackground": false,
            "runOptions": {
                "reevaluateOnRerun": true,
                "promptOnClose": false
            }
        },
        {
            "label": "Run: Linux Terminal",
            "type": "shell",
            "command": "./build/main",
            "problemMatcher": [],
            "isBackground": false,
            "runOptions": {
                "reevaluateOnRerun": true,
                "promptOnClose": false
            }
        },
        {
            "label": "Run: WebAsm",
            "type": "shell",
            "command": "python3 server.py",
            "problemMatcher": [],
            "isBackground": true,
            "runOptions": {
                "reevaluateOnRerun": true,
                "promptOnClose": false
            }
        },
        {
            "label": "Clean: Remove Build Directory",
            "type": "shell",
            "command": "rm -rf build",
            "problemMatcher": [],
            "runOptions": {
                "reevaluateOnRerun": true,
                "promptOnClose": false
            }
        }
    ]
}
```