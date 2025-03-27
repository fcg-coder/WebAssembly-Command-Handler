var statusElement = document.getElementById('status');
        var progressElement = document.getElementById('progress');
        var spinnerElement = document.getElementById('spinner');
        var outputElement = document.getElementById('output');
        var inputElement = document.getElementById('input');
        var canvasContainer = document.getElementById('canvas-container');
        var canvas = document.getElementById('canvas');

        let moduleInitialized = false;
        let screenPtr = null;  // Переменная для хранения указателя на экран
        let mode = 0;
        // Функция для обновления канваса
        function updateCanvas() {
            if (!moduleInitialized) {
                console.error("WebAssembly не загружен!");
                return;
            }

            if (typeof Module !== "undefined" && Module._getScreen) {
                screenPtr = Module._getScreen();

                if (!screenPtr) {
                    console.error("Ошибка: ptr равен нулю!");
                    return;
                }

                const width = window.innerWidth;
                const height = window.innerHeight;
                console.log("updateCanvas");
                let pixels = new Uint32Array(Module.HEAPU32.buffer, screenPtr, width * width);
                let ctx = canvas.getContext('2d');
                let imageData = ctx.createImageData(width, width);
                let data = imageData.data;



                for (let i = 0; i < pixels.length; i++) {
                    let color = pixels[i];
                    let index = i * 4;
                    data[index] = (color & 0xFF);          // R
                    data[index + 1] = (color >> 8) & 0xFF; // G
                    data[index + 2] = (color >> 16) & 0xFF;// B
                    data[index + 3] = (color >> 24) & 0xFF;// A
                }

                ctx.putImageData(imageData, 0, 0);
            } else {
                console.error("WebAssembly не загружен или метод _getScreen не найден!");
            }
        }

        // Загружаем WebAssembly
        function loadWasm() {
            if (typeof Module !== "undefined") {
                Module.onRuntimeInitialized = function () {
                    console.log("WebAssembly загружен и готов.");
                    moduleInitialized = true;  // Устанавливаем флаг, что WebAssembly инициализировано
                    mode = Module._getMode();
                    console.log(mode);
                };


            } else {
                console.error("WebAssembly не был правильно загружен!");
            }
        }

        // Загружаем WebAssembly и вызываем загрузку после завершения
        window.onload = function () {

            loadWasm();

        };

        var Module = {
            preRun: [],
            postRun: [],
            print: (function () {
                return function (text) {
                    if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
                    console.log("Print:", text); // Логируем вывод
                    if (outputElement) {
                        outputElement.innerHTML += text + "<br>";
                        outputElement.scrollTop = outputElement.scrollHeight;
                    }
                };
            })(),
            canvas: (() => {
                canvas.addEventListener("webglcontextlost", (e) => {
                    alert('WebGL context lost. You will need to reload the page.');
                    e.preventDefault();
                }, false);
                return canvas;
            })(),
            setStatus: (text) => {
                if (!Module.setStatus.last) Module.setStatus.last = { time: Date.now(), text: '' };
                if (text === Module.setStatus.last.text) return;
                var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
                var now = Date.now();
                if (m && now - Module.setStatus.last.time < 30) return;
                Module.setStatus.last.time = now;
                Module.setStatus.last.text = text;
                if (m) {
                    text = m[1];
                    progressElement.value = parseInt(m[2]) * 100;
                    progressElement.max = parseInt(m[4]) * 100;
                    progressElement.classList.remove('hidden');
                    spinnerElement.classList.remove('hidden');
                } else {
                    progressElement.value = null;
                    progressElement.max = null;
                    progressElement.classList.add('hidden');
                    if (!text) spinnerElement.style.display = 'none';
                }
                statusElement.innerHTML = text;
            },
            totalDependencies: 0,
            monitorRunDependencies: (left) => {
                this.totalDependencies = Math.max(this.totalDependencies, left);
                Module.setStatus(left ? 'Preparing... (' + (this.totalDependencies - left) + '/' + this.totalDependencies + ')' : 'All downloads complete.');

            }
        };







        function loop() {


            if (mode == 1) // SCREEN
            {
                // Переключаемся на канвас
                outputElement.style.display = 'none';
                // inputElement.style.display = 'none';
                canvasContainer.style.display = 'block';
                updateCanvas(); // Обновляем канвас
            }
            else if (mode == 0) { // SHELL
                outputElement.style.display = 'block';
                // inputElement.style.display = 'block';
                canvasContainer.style.display = 'none';
            }

        }

        loop();

        inputElement.addEventListener('keydown', (event) => {

            if (event.key === 'Enter') {

                const command = inputElement.value.trim();

                console.log("Введена команда:", command); // Логируем команду
                if (command) {
                    if (command.toLowerCase() === 'clear') {
                        console.log("Очистка вывода"); // Логируем очистку
                        outputElement.innerHTML = '';
                    }

                    else {
                        Module.print("> " + command);
                        if (Module.ccall) {
                            console.log("Вызов processInput с командой:", command); // Логируем вызов
                            Module.ccall('processInput',
                                null,
                                ['string'],
                                [command]
                            );

                        } else {
                            console.error("Module.ccall не доступен!"); // Логируем ошибку
                        }

                        loop();

                    }
                    inputElement.value = '';
                }
            }


        });

        window.addEventListener('resize', () => {
            const width = window.innerWidth;
            const height = window.innerHeight;
            if (Module.ccall) {
                Module.ccall('setSize',
                    null,
                    ['number', 'number'],
                    [height, width]

                );

                console.log("set size!");
                updateCanvas();

                mode = Module._getMode();
                console.log(mode);
                loop();

            }

        });

