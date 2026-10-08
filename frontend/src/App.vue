<script setup>
import { ref, onMounted } from 'vue'

import FileTree from './components/FileTree.vue'
import MarkdownView from './components/MarkdownView.vue'
import { useSplitter } from './composables/useSplitter.js'
import { contents } from 'virtual:notes-tree'

const WEBASM_URL = '/webasm/index.html'

/*
 * ---------------------------------------------------------
 * THEME
 * ---------------------------------------------------------
 */

const theme = ref(localStorage.getItem('theme') || 'light')

function toggleTheme() {
    theme.value = theme.value === 'light' ? 'dark' : 'light'
    localStorage.setItem('theme', theme.value)
}

/*
 * ---------------------------------------------------------
 * LEFT FILE TREE
 * ---------------------------------------------------------
 */

const leftCollapsed = ref(
    localStorage.getItem('fileTreeCollapsed') === 'true'
)

function toggleFileTree() {
    leftCollapsed.value = !leftCollapsed.value

    localStorage.setItem(
        'fileTreeCollapsed',
        String(leftCollapsed.value)
    )
}

/*
 * ---------------------------------------------------------
 * NOTES
 * ---------------------------------------------------------
 */

const FALLBACK = 'notes/index.md'

const exists = (path) => {
    return typeof path === 'string' &&
        contents[path] !== undefined
}

const saved = localStorage.getItem('lastNote')

const initialPath = exists(saved)
    ? saved
    : exists(FALLBACK)
        ? FALLBACK
        : ''

const activePath = ref(initialPath)

if (initialPath) {
    localStorage.setItem('lastNote', initialPath)
}

/*
 * ---------------------------------------------------------
 * 404
 * ---------------------------------------------------------
 */

const notFound = ref('')

let notFoundTimer = null

function show404(path) {
    notFound.value = path

    clearTimeout(notFoundTimer)

    notFoundTimer = setTimeout(() => {
        notFound.value = ''
    }, 3000)
}

function openNote(path) {
    if (!exists(path)) {
        show404(path)
        return
    }

    activePath.value = path

    localStorage.setItem(
        'lastNote',
        path
    )

    notFound.value = ''
}

/*
 * ---------------------------------------------------------
 * SPLITTER
 *
 * Разделитель находится МЕЖДУ Markdown и WebASM.
 *
 * Левая панель вообще не участвует в изменении
 * ширины Markdown.
 * ---------------------------------------------------------
 */

const splitRef = ref(null)
const leftRef = ref(null)
const dividerRef = ref(null)

const {
    width: middleWidth,
    isDragging,
    onPointerDown,
} = useSplitter({
    min: 320,
    rightMin: 360,
    initial: 650,
    key: 'middleWidth',

    containerRef: splitRef,
    leftRef,
    dividerRef,
})

/*
 * ---------------------------------------------------------
 * WEBASM
 * ---------------------------------------------------------
 */

const webasmAvailable = ref(false)
const webasmChecked = ref(false)

async function checkWebAsm() {
    webasmChecked.value = false
    webasmAvailable.value = false

    try {
        const response = await fetch(
            WEBASM_URL,
            {
                method: 'GET',
                cache: 'no-store',
            }
        )

        if (!response.ok) {
            return
        }

        const contentType =
            response.headers.get('content-type') || ''

        /*
         * Если Vite вернул не HTML,
         * это точно не наш index.html.
         */
        if (!contentType.includes('text/html')) {
            return
        }

        /*
         * Важно:
         *
         * Vite может вернуть основной index.html
         * даже если /webasm/index.html не существует.
         *
         * Поэтому читаем ответ и проверяем,
         * что это действительно WebASM.
         */

        const html = await response.text()

        /*
         * Если это наше Vue-приложение,
         * там будет root #app.
         *
         * WebASM index.html такого root иметь
         * не должен.
         */
        if (
            html.includes('<div id="app">') ||
            html.includes('<div id="app"')
        ) {
            return
        }

        webasmAvailable.value = true
    } catch {
        webasmAvailable.value = false
    } finally {
        webasmChecked.value = true
    }
}

onMounted(() => {
    checkWebAsm()
})
</script>

<template>
    <div
        ref="splitRef"
        class="app-shell"
        :data-theme="theme"
        :class="{
            'file-tree-collapsed': leftCollapsed,
            'is-dragging': isDragging
        }"
    >

        <!-- =================================================
             LEFT — FILE TREE
             ================================================= -->

        <aside
            ref="leftRef"
            class="left-panel"
            :class="{ collapsed: leftCollapsed }"
        >
            <FileTree
                v-if="!leftCollapsed"
                :active-path="activePath"
                :theme="theme"
                @select="openNote"
                @toggle-theme="toggleTheme"
                @collapse="toggleFileTree"
            />

            <button
                v-else
                class="tree-expand-button"
                type="button"
                title="Развернуть дерево файлов"
                @click="toggleFileTree"
            >
                ›
            </button>
        </aside>


        <!-- =================================================
             MIDDLE — MARKDOWN
             ================================================= -->

        <main
            class="middle-panel"
            :style="{ width: `${middleWidth}px` }"
        >
            <MarkdownView
                v-if="activePath"
                :path="activePath"
                @navigate="openNote"
            />

            <div
                v-else
                class="welcome"
            >
                <div class="welcome-icon">
                    ◇
                </div>

                <h2>
                    Выберите заметку
                </h2>

                <p>
                    Выберите файл в дереве слева
                </p>
            </div>
        </main>


        <!-- =================================================
             DIVIDER
             ================================================= -->

        <div
            ref="dividerRef"
            class="divider"
            :class="{ dragging: isDragging }"
            @pointerdown="onPointerDown"
        >
            <div class="divider-grip">
                <span></span>
                <span></span>
                <span></span>
            </div>
        </div>


        <!-- =================================================
             RIGHT — WEBASM
             ================================================= -->

        <aside class="right-panel">

            <div class="webasm-header">
                <div class="webasm-title">
                    <span
                        class="webasm-status"
                        :class="{
                            online: webasmAvailable,
                            offline: webasmChecked && !webasmAvailable
                        }"
                    ></span>

                    <span>
                        WebASM
                    </span>
                </div>

                <button
                    class="webasm-refresh"
                    type="button"
                    title="Проверить WebASM"
                    @click="checkWebAsm"
                >
                    ↻
                </button>
            </div>


            <!-- CHECKING -->

            <div
                v-if="!webasmChecked"
                class="webasm-empty"
            >
                <div class="webasm-error-code">
                    ...
                </div>

                <div class="webasm-error-title">
                    Checking WebASM
                </div>

                <div class="webasm-error-path">
                    {{ WEBASM_URL }}
                </div>
            </div>


            <!-- WEBASM -->

            <iframe
                v-else-if="webasmAvailable"
                class="webasm-frame"
                :src="WEBASM_URL"
                title="WebASM"
            ></iframe>


            <!-- 404 -->

            <div
                v-else
                class="webasm-empty"
            >
                <div class="webasm-error-code">
                    404
                </div>

                <div class="webasm-error-title">
                    WebASM not found
                </div>

                <div class="webasm-error-path">
                    {{ WEBASM_URL }}
                </div>
            </div>

        </aside>


        <!-- =================================================
             NOTE 404
             ================================================= -->

        <transition name="fade">
            <div
                v-if="notFound"
                class="toast-404"
            >
                <strong>
                    404
                </strong>

                <span>
                    Заметка не найдена:
                    <code>{{ notFound }}</code>
                </span>
            </div>
        </transition>

    </div>
</template>