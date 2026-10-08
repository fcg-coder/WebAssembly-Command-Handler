declare module 'virtual:notes-tree' {
  export interface NoteNode {
    name: string
    type: 'file' | 'folder'
    path?: string
    children?: NoteNode[]
  }

  export const tree: NoteNode[]
  export const contents: Record<string, string>
}