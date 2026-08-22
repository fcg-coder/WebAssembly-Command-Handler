(() => {

    const elements = {

        status: document.getElementById('status'),

        progress: document.getElementById('progress'),

        spinner: document.getElementById('spinner'),

        output: document.getElementById('output'),

        input: document.getElementById('input'),

        canvasContainer: document.getElementById('canvas-container'),

        canvas: document.getElementById('canvas')

    };

    const state = {

        moduleInitialized: false,

        screenPtr: null,

        mode: 0,

        resizeTimeout: null

    };

    const initCanvas = () => {

        elements.canvas.addEventListener("webglcontextlost", (e) => {

            alert('WebGL context lost. Please reload the page.');

            e.preventDefault();

        });

        elements.canvas.width = window.innerWidth;

        elements.canvas.height = window.innerHeight;

    };

    const updateCanvas = () => {

        if (!state.moduleInitialized || !Module._getScreen) {

            console.error("Module not initialized");

            return;

        }

        try {

            const screenPtr = Module._getScreen();

            if (!screenPtr || screenPtr === 0) {

                console.error("Invalid screen pointer");

                return;

            }

            const width = window.innerWidth;

            const height = window.innerHeight;

            elements.canvas.width = width;

            elements.canvas.height = height;

            const ctx = elements.canvas.getContext('2d');

            ctx.clearRect(0, 0, width, height);

            const imageData = ctx.createImageData(width, height);

            const data = imageData.data;

            const pixels = new Uint32Array(

                Module.HEAPU32.buffer,

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

                data[index] = color & 0xFF;

                data[index + 1] = (color >> 8) & 0xFF;

                data[index + 2] = (color >> 16) & 0xFF;

                data[index + 3] = (color >> 24) & 0xFF;

            }

            ctx.putImageData(imageData, 0, 0);

        } catch (error) {

            console.error("Canvas error:", error);

        }

    };

    const updateUI = () => {

        const isShellMode = state.mode === 0;

        elements.canvasContainer.style.display =
            isShellMode ? 'none' : 'block';

        elements.input.style.display =
            isShellMode ? 'block' : 'none';

    };

    const mainLoop = () => {

        if (!state.moduleInitialized) {

            requestAnimationFrame(mainLoop);

            return;

        }

        try {

            state.mode = Module._getMode();

            updateUI();

            if (state.mode !== 0) {

                updateCanvas();

            }

        } catch (error) {

            console.error("Loop error:", error);

        }

        setTimeout(() => {

            requestAnimationFrame(mainLoop);

        }, 1000 / 60);

    };

    const startMainLoop = () => {

        if (!isMainLoopRunning) {

            isMainLoopRunning = true;

            mainLoop();

        }

    };

    const handleInput = (event) => {

        if (event.key === 'Enter') {

            const command = elements.input.value.trim();

            if (!command) return;

            elements.input.value = '';

            Module.print(`> ${command}`);

            if (command.toLowerCase() === 'clear') {

                elements.output.innerHTML = '';

                return;

            }

            try {

                Module.ccall(
                    'input',
                    null,
                    ['string'],
                    [command]
                );

            } catch (e) {

                console.error("Command error:", e);

            }

        }

    };

    const handleMenuKeyPress = (event) => {

        console.log(
            `Key pressed: ${event.key} (code: ${event.code})`
        );

        switch (event.key) {

            case 'ArrowUp':

                try {

                    Module.ccall(
                        'pressButton',
                        null,
                        ['string'],
                        ['up']
                    );

                    event.preventDefault();

                } catch (e) {

                    console.error(
                        'Error in pressButton("up"):',
                        e
                    );

                }

                break;

            case 'ArrowDown':

                try {

                    Module.ccall(
                        'pressButton',
                        null,
                        ['string'],
                        ['down']
                    );

                    event.preventDefault();

                } catch (e) {

                    console.error(
                        'Error in pressButton("down"):',
                        e
                    );

                }

                break;

            case 'Escape':

                try {

                    Module.ccall(
                        'pressButton',
                        null,
                        ['string'],
                        ['escape']
                    );

                    event.preventDefault();

                } catch (e) {

                    console.error(
                        'Error in pressButton("escape"):',
                        e
                    );

                }

                break;

            default:

                break;

        }

    };

    const handleResize = () => {

        clearTimeout(state.resizeTimeout);

        state.resizeTimeout = setTimeout(() => {

            if (Module.ccall) {

                elements.canvas.width = window.innerWidth;

                elements.canvas.height = window.innerHeight;

                Module.ccall(
                    'setSize',
                    null,
                    ['number', 'number'],
                    [
                        window.innerHeight,
                        window.innerWidth
                    ]
                );

                updateCanvas();

            }

        }, 100);

    };

    window.Module = {

        preRun: [],

        postRun: [],

        print: (function () {

            return function (text) {

                if (arguments.length > 1) {

                    text = Array.from(arguments).join(' ');

                }

                console.log(text);

                if (elements.output) {

                    elements.output.innerHTML +=
                        text + "<br>";

                    elements.output.scrollTop =
                        elements.output.scrollHeight;

                }

            };

        })(),

        canvas: elements.canvas,

        onRuntimeInitialized: () => {

            console.log("WASM initialized");

            const initialWidth = window.innerWidth;

            const initialHeight = window.innerHeight;

            elements.canvas.width = initialWidth;

            elements.canvas.height = initialHeight;

            Module.ccall(
                'setSize',
                null,
                ['number', 'number'],
                [
                    initialHeight,
                    initialWidth
                ]
            );

            state.moduleInitialized = true;

            state.mode = Module._getMode();

            if (state.mode !== 0) {

                updateCanvas();

            }

            startMainLoop();

        },

        setStatus: (text) => {

            if (!Module.setStatus.last) {

                Module.setStatus.last = {
                    time: 0,
                    text: ''
                };

            }

            if (text === Module.setStatus.last.text) {

                return;

            }

            const now = Date.now();

            if (now - Module.setStatus.last.time < 30) {

                return;

            }

            Module.setStatus.last = {
                time: now,
                text
            };

            const match =
                text.match(/(.+?)\((\d+)\/(\d+)\)/);

            if (match) {

                elements.progress.value =
                    match[2] * 100;

                elements.progress.max =
                    match[3] * 100;

                elements.progress.hidden = false;

                elements.spinner.hidden = false;

            } else {

                elements.progress.hidden = true;

                elements.spinner.hidden = !text;

            }

            elements.status.textContent = text;

        }

    };

    window.addEventListener('load', () => {

        initCanvas();

        document.addEventListener(
            'keydown',
            handleMenuKeyPress
        );

        elements.input.addEventListener(
            'keydown',
            handleInput
        );

        window.addEventListener(
            'resize',
            handleResize
        );

        mainLoop();

    });

})();