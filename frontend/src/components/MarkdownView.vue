<template>
  <article
    class="markdown-view"
    v-html="rendered"
    @click="onClick"
  ></article>
</template>

<script setup>
import { computed } from 'vue'

import { marked } from 'marked'
import hljs from 'highlight.js'

import { contents } from 'virtual:notes-tree'


const props = defineProps({
  path: {
    type: String,
    required: true
  }
})


const emit = defineEmits([
  'navigate'
])


/* -------------------------------------------------------------------------- */
/*                              MARKDOWN                                      */
/* -------------------------------------------------------------------------- */

marked.setOptions({
  breaks: true,
  gfm: true,

  highlight(code, language) {
    if (
      language &&
      hljs.getLanguage(language)
    ) {
      return hljs.highlight(
        code,
        {
          language
        }
      ).value
    }

    return hljs.highlightAuto(code).value
  }
})


const rendered = computed(() => {
  const markdown = contents[props.path]

  if (markdown === undefined) {
    return `
      <div class="markdown-not-found">
        <h2>Заметка не найдена</h2>
        <code>${props.path}</code>
      </div>
    `
  }

  return marked.parse(markdown)
})


/* -------------------------------------------------------------------------- */
/*                                LINKS                                       */
/* -------------------------------------------------------------------------- */

function onClick(event) {
  const link = event.target.closest('a')

  if (!link) {
    return
  }

  const href = link.getAttribute('href')

  if (!href) {
    return
  }

  /*
   * Якоря внутри текущей страницы.
   */
  if (href.startsWith('#')) {
    return
  }

  /*
   * Внешние ссылки.
   */
  if (
    href.startsWith('http://') ||
    href.startsWith('https://')
  ) {
    link.target = '_blank'
    link.rel = 'noopener noreferrer'
    return
  }

  /*
   * Markdown-ссылки.
   */
  if (
    href.endsWith('.md') ||
    href.includes('.md#')
  ) {
    event.preventDefault()

    const [path, hash] = href.split('#')

    const resolved = resolveRelative(
      props.path,
      path
    )

    emit(
      'navigate',
      resolved
    )

    /*
     * Если ссылка содержит #anchor,
     * прокручиваем уже после смены заметки.
     */
    if (hash) {
      requestAnimationFrame(() => {
        document
          .getElementById(hash)
          ?.scrollIntoView({
            behavior: 'smooth'
          })
      })
    }
  }
}


/* -------------------------------------------------------------------------- */
/*                              PATH RESOLVER                                 */
/* -------------------------------------------------------------------------- */

function resolveRelative(
  currentPath,
  href
) {
  const baseParts = currentPath
    .split('/')
    .slice(0, -1)

  for (const part of href.split('/')) {

    if (
      part === '.' ||
      part === ''
    ) {
      continue
    }

    if (part === '..') {
      if (baseParts.length > 0) {
        baseParts.pop()
      }

      continue
    }

    baseParts.push(part)
  }

  return baseParts.join('/')
}
</script>