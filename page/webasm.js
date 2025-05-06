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

    // 1. Исправляем инициализацию канваса
    const initCanvas = () => {
        elements.canvas.addEventListener("webglcontextlost", (e) => {
            alert('WebGL context lost. Please reload the page.');
            e.preventDefault();
        });
        
        // Важно: Устанавливаем размеры канваса
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
                console.error("Invalid screen pointer, screenPtr =", screenPtr); 
                return;
            }
    
            const width = window.innerWidth;
            const height = window.innerHeight;
    
            elements.canvas.width = width;
            elements.canvas.height = height;
    
            const ctx = elements.canvas.getContext('2d');
            ctx.clearRect(0, 0, width, height); // Очистка канваса
    
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
                data[index] = (color & 0xFF);          // R
                data[index + 1] = (color >> 8) & 0xFF; // G
                data[index + 2] = (color >> 16) & 0xFF;// B
                data[index + 3] = (color >> 24) & 0xFF;// A
            }
    
            ctx.putImageData(imageData, 0, 0);
        } catch (error) {
            console.error("Canvas error:", error);
        }
    };

    const updateUI = () => {
        const mode = state.mode;
    
        if (mode === 0) {
            // Только shell
            elements.canvasContainer.style.display = 'none';
            elements.output.style.display = 'block';
            elements.input.style.display = 'block';
            elements.output.style.width = '100%';
            elements.input.style.width = '100%';
        } else if (mode === 1) {
            // Только canvas
            elements.canvasContainer.style.display = 'block';
            elements.output.style.display = 'none';
            elements.input.style.display = 'none';
            elements.canvasContainer.style.width = '100%';
        } else if (mode === 3) {
            // BOTH: 50% canvas, 50% shell
            elements.canvasContainer.style.display = 'inline-block';
            elements.canvasContainer.style.width = '50%';
    
            elements.output.style.display = 'inline-block';
            elements.output.style.width = '50%';
            elements.input.style.display = 'inline-block';
            elements.input.style.width = '50%';
        }
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
    
        // Задержка перед следующим кадром (≈60 FPS)
        setTimeout(() => {
            requestAnimationFrame(mainLoop);
        }, 1000 / 60); // 16.67 мс
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

            // 5. Добавляем обработку ошибок вызова
            try {
                Module.ccall('processInput', null, ['string'], [command]);
            } catch (e) {
                console.error("Command error:", e);
            }
        }
    };

    const handleMenuKeyPress = (event) => {
        // Логируем информацию о нажатой клавише
        console.log(`Key pressed: ${event.key} (code: ${event.code})`);
        
       
        // Обрабатываем только нужные клавиши
        document.addEventListener('keydown', (event) => {
            switch(event.key) {
                case 'ArrowUp':
                    
                    try {
                        // Используем ccall для вызова C++ функции
                        Module.ccall(
                            'pressButton',  // Имя C++ функции
                            null,          // Возвращаемый тип (void)
                            ['string'],    // Типы аргументов (массив)
                            ['up']         // Аргументы (массив)
                        );
                       
                        event.preventDefault();
                    } catch (e) {
                        console.error('Error in pressButton("up"):', e);
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
                        console.error('Error in pressButton("down"):', e);
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
                        console.error('Error in pressButton("down"):', e);
                    }
                    break;
 
                default:
                    break;
            }
        });
    
 
        
    };

    const handleResize = () => {
        clearTimeout(state.resizeTimeout);
        state.resizeTimeout = setTimeout(() => {
            if (Module.ccall) {
                // 6. Исправляем порядок параметров размера
                elements.canvas.width = window.innerWidth;
                elements.canvas.height = window.innerHeight;
                
      

                Module.ccall('setSize', null, ['number', 'number'], 
                    [ window.innerHeight, window.innerWidth,]);
                updateCanvas();
            }
        }, 100);
    };

    // 7. Выносим Module в глобальную область видимости
    window.Module = {
        preRun: [],
        postRun: [],
        print: (function() {
            return function(text) {
                if (arguments.length > 1) {
                    text = Array.from(arguments).join(' ');
                }
                console.log(text);
                if (elements.output) {
                    elements.output.innerHTML += text + "<br>";
                    elements.output.scrollTop = elements.output.scrollHeight;
                }
            };
        })(),
        canvas: elements.canvas,
        onRuntimeInitialized: () => {
            console.log("WASM initialized");
            // 1. Получаем текущие размеры окна
            const initialWidth = window.innerWidth;
            const initialHeight = window.innerHeight;
            
            // 2. Синхронизируем размеры канваса
            elements.canvas.width = initialWidth;
            elements.canvas.height = initialHeight;
            
            // 3. Передаем размеры в WASM модуль (порядок [height, width])
            Module.ccall(
                'setSize', 
                null, 
                ['number', 'number'], 
                [initialHeight, initialWidth]
            );
            
            // 4. Обновляем состояние
            state.moduleInitialized = true;
            state.mode = Module._getMode();
            
            // 5. Форсируем первое обновление канваса
            if (state.mode !== 0) {
                updateCanvas();
            }
            
            startMainLoop();
        },
        setStatus: (text) => {
            if (!Module.setStatus.last) Module.setStatus.last = { time: 0, text: '' };
            if (text === Module.setStatus.last.text) return;
            
            const now = Date.now();
            if (now - Module.setStatus.last.time < 30) return;
            
            Module.setStatus.last = { time: now, text };
            const match = text.match(/(.+?)\((\d+)\/(\d+)\)/);
            
            if (match) {
                elements.progress.value = match[2] * 100;
                elements.progress.max = match[3] * 100;
                elements.progress.hidden = false;
                elements.spinner.hidden = false;
            } else {
                elements.progress.hidden = true;
                elements.spinner.hidden = !text;
            }
            
            elements.status.textContent = text;
        }
    };

    // 8. Исправляем инициализацию событий
    window.addEventListener('load', () => {
        initCanvas();
        document.addEventListener('keydown', handleMenuKeyPress);

        elements.input.addEventListener('keydown', handleInput);
        window.addEventListener('resize', handleResize);
        
        // 9. Принудительно запускаем первый цикл
        mainLoop();
    });
})();