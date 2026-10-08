class WebAssemblyApp {
    constructor() {
        // Инициализация элементов
        this.elements = {
            status: document.getElementById('status'),
            progress: document.getElementById('progress'),
            spinner: document.getElementById('spinner'),
            output: document.getElementById('output'),
            input: document.getElementById('input'),
            canvasContainer: document.getElementById('canvas-container'),
            canvas: document.getElementById('canvas')
        };

        // Инициализация состояния
        this.state = {
            moduleInitialized: false,
            screenPtr: null,
            mode: 0,
            resizeTimeout: null,
            isMainLoopRunning: false
        };

        // Привязка методов к экземпляру
        this.initCanvas = this.initCanvas.bind(this);
        this.updateCanvas = this.updateCanvas.bind(this);
        this.updateUI = this.updateUI.bind(this);
        this.mainLoop = this.mainLoop.bind(this);
        this.startMainLoop = this.startMainLoop.bind(this);
        this.handleInput = this.handleInput.bind(this);
        this.handleMenuKeyPress = this.handleMenuKeyPress.bind(this);
        this.handleResize = this.handleResize.bind(this);

        // Инициализация приложения
        this.initModule();
        this.initEventListeners();
    }

    initCanvas() {
        this.elements.canvas.addEventListener("webglcontextlost", (e) => {
            alert('WebGL context lost. Please reload the page.');
            e.preventDefault();
        });

        this.elements.canvas.width = window.innerWidth;
        this.elements.canvas.height = window.innerHeight;
    }

    updateCanvas() {
        if (!this.state.moduleInitialized || !window.Module._getScreen) {
            console.error("Module not initialized");
            return;
        }

        try {
            const screenPtr = window.Module._getScreen();
            if (!screenPtr || screenPtr === 0) {
                console.error("Invalid screen pointer");
                return;
            }

            const width = window.innerWidth;
            const height = window.innerHeight;

            this.elements.canvas.width = width;
            this.elements.canvas.height = height;

            const ctx = this.elements.canvas.getContext('2d');
            ctx.clearRect(0, 0, width, height);

            const imageData = ctx.createImageData(width, height);
            const data = imageData.data;

            const pixels = new Uint32Array(
                window.Module.HEAPU32.buffer,
                screenPtr,
                width * height
            );

            if (pixels.length < width * height) {
                console.error("Buffer too small");
                return;
            }

            for (let i = 0; i < pixels.length; i++) {
                const color = pixels[i];
                const index = i * 4;
                data[index] = (color & 0xFF);
                data[index + 1] = (color >> 8) & 0xFF;
                data[index + 2] = (color >> 16) & 0xFF;
                data[index + 3] = (color >> 24) & 0xFF;
            }

            ctx.putImageData(imageData, 0, 0);
        } catch (error) {
            console.error("Canvas error:", error);
        }
    }

    updateUI() {
        const isShellMode = this.state.mode === 0;
        this.elements.canvasContainer.style.display = isShellMode ? 'none' : 'block';
        this.elements.input.style.display = isShellMode ? 'block' : 'none';
    }

    mainLoop() {
        if (!this.state.moduleInitialized) {
            requestAnimationFrame(this.mainLoop);
            return;
        }

        try {
            this.state.mode = window.Module._getMode();
            this.updateUI();

            if (this.state.mode !== 0) {
                this.updateCanvas();
            }
        } catch (error) {
            console.error("Loop error:", error);
        }

        setTimeout(() => {
            requestAnimationFrame(this.mainLoop);
        }, 1000 / 60);
    }

    startMainLoop() {
        if (!this.state.isMainLoopRunning) {
            this.state.isMainLoopRunning = true;
            this.mainLoop();
        }
    }

    handleInput(event) {
        if (event.key === 'Enter') {
            const command = this.elements.input.value.trim();
            if (!command) return;

            this.elements.input.value = '';
            window.Module.print(`> ${command}`);

            if (command.toLowerCase() === 'clear') {
                this.elements.output.innerHTML = '';
                return;
            }

            try {
                window.Module.ccall('processInput', null, ['string'], [command]);
            } catch (e) {
                console.error("Command error:", e);
            }
        }
    }

    handleMenuKeyPress(event) {
        switch (event.key) {
            case 'ArrowUp':
                try {
                    window.Module.ccall('pressButton', null, ['string'], ['up']);
                    event.preventDefault();
                } catch (e) {
                    console.error('Error in pressButton("up"):', e);
                }
                break;

            case 'ArrowDown':
                try {
                    window.Module.ccall('pressButton', null, ['string'], ['down']);
                    event.preventDefault();
                } catch (e) {
                    console.error('Error in pressButton("down"):', e);
                }
                break;

            case 'Escape':
                try {
                    window.Module.ccall('pressButton', null, ['string'], ['escape']);
                    event.preventDefault();
                } catch (e) {
                    console.error('Error in pressButton("escape"):', e);
                }
                break;

            default:
                break;
        }
    }

    handleResize() {
        clearTimeout(this.state.resizeTimeout);
        this.state.resizeTimeout = setTimeout(() => {
            if (window.Module.ccall) {
                this.elements.canvas.width = window.innerWidth;
                this.elements.canvas.height = window.innerHeight;

                window.Module.ccall('setSize', null, ['number', 'number'],
                    [window.innerHeight, window.innerWidth]);
                this.updateCanvas();
            }
        }, 100);
    }

    initModule() {
        window.Module = {
            preRun: [],
            postRun: [],
            print: ((text) => {
                if (arguments.length > 1) {
                    text = Array.from(arguments).join(' ');
                }
                console.log(text);
                if (this.elements.output) {
                    this.elements.output.innerHTML += text + "<br>";
                    this.elements.output.scrollTop = this.elements.output.scrollHeight;
                }
            }),
            canvas: this.elements.canvas,
            onRuntimeInitialized: () => {
                console.log("WASM initialized");
                const initialWidth = window.innerWidth;
                const initialHeight = window.innerHeight;

                this.elements.canvas.width = initialWidth;
                this.elements.canvas.height = initialHeight;

                window.Module.ccall(
                    'setSize',
                    null,
                    ['number', 'number'],
                    [initialHeight, initialWidth]
                );

                this.state.moduleInitialized = true;
                this.state.mode = window.Module._getMode();

                if (this.state.mode !== 0) {
                    this.updateCanvas();
                }

                this.startMainLoop();
            },
            setStatus: (text) => {
                if (!window.Module.setStatus) window.Module.setStatus = {};
                if (!window.Module.setStatus.last) window.Module.setStatus.last = { time: 0, text: '' };
                if (text === window.Module.setStatus.last.text) return;

                const now = Date.now();
                if (now - window.Module.setStatus.last.time < 30) return;

                window.Module.setStatus.last = { time: now, text };
                const match = text.match(/(.+?)\((\d+)\/(\d+)\)/);

                if (match) {
                    this.elements.progress.value = match[2] * 100;
                    this.elements.progress.max = match[3] * 100;
                    this.elements.progress.hidden = false;
                    this.elements.spinner.hidden = false;
                } else {
                    this.elements.progress.hidden = true;
                    this.elements.spinner.hidden = !text;
                }

                this.elements.status.textContent = text;
            }
        };
    }

    initEventListeners() {
        window.addEventListener('load', () => {
            this.initCanvas();
            document.addEventListener('keydown', this.handleMenuKeyPress);
            this.elements.input.addEventListener('keydown', this.handleInput);
            window.addEventListener('resize', this.handleResize);
            this.mainLoop();
        });
    }
}

// Создание синглтон-экземпляра
new WebAssemblyApp();