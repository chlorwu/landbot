import js from '@eslint/js'
import globals from 'globals'
import reactHooks from 'eslint-plugin-react-hooks'
import reactRefresh from 'eslint-plugin-react-refresh'
import tseslint from 'typescript-eslint'
import { defineConfig, globalIgnores } from 'eslint/config'

/**
 * Layer boundaries (ARCHITECTURE_PLAN.md §5.1).
 *
 *   features/  library, host, robot, stats, achievements, settings
 *   games/     one folder per game
 *   ui/        design system    — zero game knowledge
 *   engine/    game primitives  — zero app knowledge
 *   core/      points, stats, achievements, storage, sync, registry
 *
 * Without mechanical enforcement this structure rots by game #6, so every rule
 * below is an `error`, never a warning.
 *
 * Each forbidden layer is expressed twice: once as an `@/` alias import and
 * once as a relative one, because a determined `../../` traversal is exactly
 * how a boundary gets crossed by accident.
 */
const layerPatterns = (layer) => [`@/${layer}`, `@/${layer}/**`, `**/${layer}/**`]

const forbidLayers = (layers, message) => ({
  'no-restricted-imports': ['error', { patterns: [{ group: layers.flatMap(layerPatterns), message }] }],
})

export default defineConfig([
  globalIgnores(['dist', 'coverage', 'node_modules']),

  // ── Base ──────────────────────────────────────────────────────────────────
  {
    files: ['**/*.{ts,tsx}'],
    extends: [js.configs.recommended, tseslint.configs.recommended, reactHooks.configs.flat.recommended],
    languageOptions: {
      globals: globals.browser,
      parserOptions: { ecmaFeatures: { jsx: true } },
    },
    rules: {
      '@typescript-eslint/no-unused-vars': ['error', { argsIgnorePattern: '^_', varsIgnorePattern: '^_' }],
      // Games mutate a ref-held world object; the loop must never re-render React.
      'no-console': ['error', { allow: ['warn', 'error'] }],
    },
  },

  // Fast-refresh hygiene applies to component files only.
  {
    files: ['src/**/*.tsx'],
    extends: [reactRefresh.configs.vite],
  },

  // ── engine/ imports nothing else from the app ─────────────────────────────
  {
    files: ['src/engine/**/*.{ts,tsx}'],
    rules: forbidLayers(
      ['core', 'ui', 'games', 'features', 'app'],
      'engine/ is a standalone primitive layer: it must not import app code. Move the shared piece down into engine/, or invert the dependency by passing it in as an argument.',
    ),
  },

  // ── ui/ may import engine/math only ───────────────────────────────────────
  {
    files: ['src/ui/**/*.{ts,tsx}'],
    rules: {
      'no-restricted-imports': [
        'error',
        {
          patterns: [
            {
              group: [
                ...['core', 'games', 'features', 'app'].flatMap(layerPatterns),
                // engine is forbidden except for the pure math helpers.
                '@/engine/**',
                '**/engine/**',
                '!@/engine/math',
                '!@/engine/math/**',
                '!**/engine/math/**',
              ],
              message:
                'ui/ is the design system and has zero game or app knowledge. It may import engine/math and nothing else — take the value as a prop instead.',
            },
          ],
        },
      ],
    },
  },

  // ── core/ sits below the app; it never reaches upward ─────────────────────
  {
    files: ['src/core/**/*.{ts,tsx}'],
    rules: forbidLayers(
      ['ui', 'games', 'features', 'app'],
      'core/ is the bottom layer. Nothing here may depend on presentation or on a specific game — core/registry/games.ts is the single exception, and it is the manifest.',
    ),
  },

  // ── The one file allowed to know games exist ──────────────────────────────
  {
    files: ['src/core/registry/games.ts'],
    rules: {
      'no-restricted-imports': [
        'error',
        {
          patterns: [
            {
              group: ['ui', 'features', 'app'].flatMap(layerPatterns),
              message: 'The game manifest imports game metadata only.',
            },
          ],
        },
      ],
    },
  },

  // ── Nothing else imports games/*, and games never touch app state ─────────
  {
    files: ['src/{app,features,ui,engine}/**/*.{ts,tsx}'],
    rules: {
      'no-restricted-imports': [
        'error',
        {
          patterns: [
            {
              group: layerPatterns('games'),
              message:
                'Only core/registry/games.ts may import from games/. Route the game through the registry so it stays a lazy-loaded chunk.',
            },
          ],
        },
      ],
    },
  },

  {
    files: ['src/games/**/*.{ts,tsx}'],
    rules: {
      'no-restricted-imports': [
        'error',
        {
          paths: [
            {
              name: '@/core/points/pointsStore',
              message:
                'Games never award points. Emit onEnd(result) and let GameHost run the economy — this is the rule that makes game #20 take an afternoon.',
            },
            {
              name: '@/core/stats/statsStore',
              message: 'Games never record stats. Emit onEnd(result); GameHost owns the write.',
            },
          ],
          patterns: [
            {
              group: [
                '@/core/sync',
                '@/core/sync/**',
                '**/core/sync/**',
                '@/features/**',
                '**/features/**',
                // A game importing another game is always a mistake.
                '@/games/*/**',
              ],
              message:
                'Games are pure participants: they receive `phase` and emit `onEnd`. They never touch Firebase, the host, or another game.',
            },
          ],
        },
      ],
    },
  },

  // ── Tests and workers ─────────────────────────────────────────────────────
  {
    files: ['src/**/*.test.{ts,tsx}', 'src/test/**/*.{ts,tsx}'],
    rules: {
      'no-restricted-imports': 'off',
      'no-console': 'off',
    },
  },
  {
    files: ['src/**/*.worker.ts'],
    languageOptions: { globals: globals.worker },
  },

  // ── Root config files ─────────────────────────────────────────────────────
  {
    files: ['*.{ts,js}'],
    languageOptions: { globals: globals.node },
    rules: { 'no-restricted-imports': 'off' },
  },
])
