import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import notesPlugin from './vite-plugin-notes.js'

export default defineConfig({
  plugins: [
    vue(),
    notesPlugin({ dir: 'notes' })
  ]
})