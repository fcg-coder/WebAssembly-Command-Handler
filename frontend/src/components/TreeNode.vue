<template>

    <!-- =================================================
         FOLDER
         ================================================= -->

    <div
        v-if="node.type === 'folder'"
        class="tree-folder"
    >

        <button
            class="tree-item folder"
            :class="{
                expanded,
                matched: folderMatches
            }"
            type="button"
            @click="toggleFolder"
        >

            <span
                class="tree-arrow"
                :class="{ expanded }"
            >
                ›
            </span>

            <span class="tree-icon folder-icon">
                {{ expanded ? '▾' : '▸' }}
            </span>

            <span class="label">
                {{ node.name }}
            </span>

        </button>


        <!-- CHILDREN -->

        <div
            v-show="expanded"
            class="children"
        >

            <TreeNode
                v-for="child in node.children || []"
                :key="child.path || child.name"
                :node="child"
                :active-path="activePath"
                :search-query="searchQuery"
                @select="$emit('select', $event)"
            />

        </div>

    </div>


    <!-- =================================================
         FILE
         ================================================= -->

    <button
        v-else
        class="tree-item file"
        :class="{
            active: activePath === node.path,
            matched: fileMatches
        }"
        type="button"
        @click="$emit('select', node.path)"
    >

        <span class="tree-arrow spacer">
        </span>

        <span class="tree-icon file-icon">
            •
        </span>

        <span class="label">
            {{ node.name }}
        </span>

    </button>

</template>

<script setup>
import {
    computed,
    ref,
    watch
} from 'vue'

const props = defineProps({
    node: {
        type: Object,
        required: true
    },

    activePath: {
        type: String,
        default: ''
    },

    searchQuery: {
        type: String,
        default: ''
    }
})

defineEmits([
    'select'
])

/*
 * У каждой папки своё состояние.
 *
 * Ключ берём из полного path.
 */
const storageKey = computed(() => {
    return `tree-expanded:${props.node.path || props.node.name}`
})

function getInitialExpanded() {
    if (props.node.type !== 'folder') {
        return false
    }

    /*
     * Во время поиска папки автоматически открываются,
     * чтобы найденный файл был виден.
     */
    if (props.searchQuery) {
        return true
    }

    const saved = localStorage.getItem(
        storageKey.value
    )

    if (saved !== null) {
        return saved === 'true'
    }

    /*
     * По умолчанию папки открыты.
     */
    return true
}

const expanded = ref(
    getInitialExpanded()
)

function toggleFolder() {
    expanded.value = !expanded.value

    localStorage.setItem(
        storageKey.value,
        String(expanded.value)
    )
}

/*
 * При поиске автоматически раскрываем папки.
 */
watch(
    () => props.searchQuery,
    (query) => {
        if (query) {
            expanded.value = true
        }
    }
)

const folderMatches = computed(() => {
    if (!props.searchQuery) {
        return false
    }

    return props.node.name
        .toLowerCase()
        .includes(props.searchQuery.toLowerCase())
})

const fileMatches = computed(() => {
    if (!props.searchQuery) {
        return false
    }

    return props.node.name
        .toLowerCase()
        .includes(props.searchQuery.toLowerCase())
})
</script>