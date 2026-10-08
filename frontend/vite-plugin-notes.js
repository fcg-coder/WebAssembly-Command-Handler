import fs from 'node:fs'
import path from 'node:path'

const VIRTUAL_ID = 'virtual:notes-tree'
const RESOLVED_ID = '\0' + VIRTUAL_ID

const toPosix = (value) => value.split(path.sep).join('/')

function buildTree(dir, rootDir) {
  const entries = fs.readdirSync(dir, {
    withFileTypes: true
  })

  const folders = []
  const files = []

  for (const entry of entries) {
    if (entry.name.startsWith('.')) {
      continue
    }

    const fullPath = path.join(dir, entry.name)

    if (entry.isDirectory()) {
      folders.push({
        name: entry.name,
        type: 'folder',
        children: buildTree(fullPath, rootDir)
      })

      continue
    }

    if (entry.isFile() && entry.name.endsWith('.md')) {
      files.push({
        name: entry.name.replace(/\.md$/, ''),
        type: 'file',
        path: toPosix(path.relative(rootDir, fullPath))
      })
    }
  }

  const compare = (a, b) => {
    return a.name.localeCompare(b.name, 'ru')
  }

  folders.sort(compare)
  files.sort(compare)

  return [
    ...folders,
    ...files
  ]
}

function collectContents(dir, rootDir, contents = {}) {
  const entries = fs.readdirSync(dir, {
    withFileTypes: true
  })

  for (const entry of entries) {
    if (entry.name.startsWith('.')) {
      continue
    }

    const fullPath = path.join(dir, entry.name)

    if (entry.isDirectory()) {
      collectContents(
        fullPath,
        rootDir,
        contents
      )

      continue
    }

    if (entry.isFile() && entry.name.endsWith('.md')) {
      const relativePath = toPosix(
        path.relative(rootDir, fullPath)
      )

      contents[relativePath] = fs.readFileSync(
        fullPath,
        'utf-8'
      )
    }
  }

  return contents
}

export default function notesPlugin(options = {}) {
  const notesDir = options.dir || 'notes'

  let absoluteNotesDir = ''

  return {
    name: 'vite-plugin-notes',

    enforce: 'pre',

    configResolved(config) {
      absoluteNotesDir = path.resolve(
        config.root,
        notesDir
      )

      console.log('NOTES PLUGIN')
      console.log('root:', config.root)
      console.log('notes:', absoluteNotesDir)
    },

    resolveId(id) {
      if (id === VIRTUAL_ID) {
        return RESOLVED_ID
      }

      return null
    },

    load(id) {
      if (id !== RESOLVED_ID) {
        return null
      }

      if (!fs.existsSync(absoluteNotesDir)) {
        this.warn(
          `Папка ${notesDir}/ не найдена`
        )

        return {
          code:
            'export const tree = []\n' +
            'export const contents = {}\n',
          map: null
        }
      }

      const tree = buildTree(
        absoluteNotesDir,
        absoluteNotesDir
      )

      const contents = collectContents(
        absoluteNotesDir,
        absoluteNotesDir
      )

      const code =
        'export const tree = ' +
        JSON.stringify(tree, null, 2) +
        '\n' +
        'export const contents = ' +
        JSON.stringify(contents) +
        '\n'

      console.log('=== virtual:notes-tree ===')
      console.log(code)
      console.log('=========================')

      return {
        code,
        map: null
      }
    },

    handleHotUpdate(ctx) {
      if (!absoluteNotesDir) {
        return
      }

      const relative = path.relative(
        absoluteNotesDir,
        ctx.file
      )

      if (
        relative.startsWith('..') ||
        path.isAbsolute(relative)
      ) {
        return
      }

      const module =
        ctx.server.moduleGraph.getModuleById(
          RESOLVED_ID
        )

      if (!module) {
        return
      }

      ctx.server.moduleGraph.invalidateModule(
        module
      )

      return [
        module,
        ...ctx.modules
      ]
    }
  }
}
