import {
    ref,
    onMounted,
    onBeforeUnmount
} from 'vue'

export function useSplitter({
    min = 320,
    rightMin = 360,
    initial = 650,
    key = 'splitWidth',

    containerRef,
    leftRef,
    dividerRef
} = {}) {

    const width = ref(initial)
    const isDragging = ref(false)

    let startX = 0
    let startWidth = 0


    /*
     * Максимальная ширина Markdown.
     *
     * Левая панель НЕ изменяется.
     *
     * Формула:
     *
     * весь экран
     * - file tree
     * - divider
     * - минимальная ширина WebASM
     */
    function getMax() {
        const containerWidth =
            containerRef?.value?.clientWidth ??
            window.innerWidth

        const leftWidth =
            leftRef?.value?.offsetWidth ?? 280

        const dividerWidth =
            dividerRef?.value?.offsetWidth ?? 8

        return Math.max(
            min,
            containerWidth -
            leftWidth -
            dividerWidth -
            rightMin
        )
    }


    function clamp(value) {
        const max = getMax()

        return Math.min(
            Math.max(value, min),
            max
        )
    }


    /*
     * НАЧАЛО DRAG
     */
    function onPointerDown(event) {

        /*
         * Работаем только с основной кнопкой мыши.
         */
        if (
            event.pointerType === 'mouse' &&
            event.button !== 0
        ) {
            return
        }

        event.preventDefault()

        isDragging.value = true

        startX = event.clientX
        startWidth = width.value

        /*
         * Pointer capture гарантирует,
         * что мы не потеряем drag,
         * даже если курсор выйдет за пределы divider.
         */
        dividerRef?.value?.setPointerCapture?.(
            event.pointerId
        )

        document.body.style.cursor =
            'col-resize'

        document.body.style.userSelect =
            'none'
    }


    /*
     * DRAG
     */
    function onPointerMove(event) {

        if (!isDragging.value) {
            return
        }

        const delta =
            event.clientX - startX

        width.value = clamp(
            startWidth + delta
        )
    }


    /*
     * END DRAG
     */
    function onPointerUp(event) {

        if (!isDragging.value) {
            return
        }

        isDragging.value = false

        dividerRef?.value?.releasePointerCapture?.(
            event.pointerId
        )

        document.body.style.cursor = ''
        document.body.style.userSelect = ''

        localStorage.setItem(
            key,
            String(width.value)
        )
    }


    /*
     * Если окно изменило размер,
     * Markdown не должен вылезти за пределы WebASM.
     */
    function handleResize() {
        width.value = clamp(width.value)
    }


    onMounted(() => {

        const saved = Number(
            localStorage.getItem(key)
        )

        if (saved > 0) {
            width.value = clamp(saved)
        } else {
            width.value = clamp(initial)
        }


        window.addEventListener(
            'resize',
            handleResize
        )

        document.addEventListener(
            'pointermove',
            onPointerMove
        )

        document.addEventListener(
            'pointerup',
            onPointerUp
        )
    })


    onBeforeUnmount(() => {

        window.removeEventListener(
            'resize',
            handleResize
        )

        document.removeEventListener(
            'pointermove',
            onPointerMove
        )

        document.removeEventListener(
            'pointerup',
            onPointerUp
        )

        document.body.style.cursor = ''
        document.body.style.userSelect = ''
    })


    return {
        width,
        isDragging,
        onPointerDown
    }
}