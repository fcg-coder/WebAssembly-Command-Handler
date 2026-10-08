<template>
    <aside class="file-tree">

        <!-- HEADER -->

        <div class="sidebar-header">

            <div class="sidebar-title-row">

                <div class="vault-name">
                    <span class="vault-icon">
                        ◈
                    </span>

                    <span>
                        Vault
                    </span>
                </div>

                <button
                    class="sidebar-action"
                    type="button"
                    title="Свернуть дерево"
                    @click="$emit('collapse')"
                >
                    ‹
                </button>

            </div>


            <!-- SEARCH -->

            <div class="search-wrapper">

                <span class="search-icon">
                    ⌕
                </span>

                <input
                    v-model="query"
                    type="text"
                    placeholder="Поиск…"
                    class="search"
                />

                <button
                    v-if="query"
                    class="search-clear"
                    type="button"
                    @click="query = ''"
                >
                    ×
                </button>

            </div>

        </div>


        <!-- TREE -->

        <nav class="tree">

            <TreeNode
                v-for="node in filteredVault"
                :key="node.path || node.name"
                :node="node"
                :active-path="activePath"
                :search-query="query.trim()"
                @select="$emit('select', $event)"
            />

            <div
                v-if="!filteredVault.length"
                class="tree-empty"
            >
                <div class="tree-empty-icon">
                    ⌕
                </div>

                <span>
                    Ничего не найдено
                </span>
            </div>

        </nav>


        <!-- FOOTER -->

        <div class="sidebar-footer">

            <button
                class="theme-button"
                type="button"
                @click="$emit('toggle-theme')"
            >
                <span class="theme-icon">
                    {{ theme === 'dark' ? '☀' : '☾' }}
                </span>

                <span>
                    {{ theme === 'dark' ? 'Светлая тема' : 'Тёмная тема' }}
                </span>
            </button>

        </div>

    </aside>
</template>

<script setup>
import { computed, ref } from 'vue'
import TreeNode from './TreeNode.vue'
import { tree } from 'virtual:notes-tree'

const props = defineProps({
    activePath: {
        type: String,
        default: ''
    },

    theme: {
        type: String,
        default: 'light'
    }
})

defineEmits([
    'select',
    'toggle-theme',
    'collapse'
])

const query = ref('')

/*
 * Фильтрация дерева.
 *
 * Если совпадает файл — показываем его.
 *
 * Если совпадает папка или один из её
 * дочерних файлов — показываем папку
 * вместе с подходящими детьми.
 */
function filterNodes(nodes, q) {
    if (!q) {
        return nodes
    }

    const lower = q.toLowerCase()

    return nodes
        .map((node) => {

            if (node.type === 'file') {
                return node.name
                    .toLowerCase()
                    .includes(lower)
                    ? node
                    : null
            }

            const children = filterNodes(
                node.children || [],
                q
            )

            if (
                node.name.toLowerCase().includes(lower) ||
                children.length
            ) {
                return {
                    ...node,
                    children
                }
            }

            return null
        })
        .filter(Boolean)
}

const filteredVault = computed(() => {
    return filterNodes(
        tree,
        query.value.trim()
    )
})
</script>