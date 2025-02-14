// https://nuxt.com/docs/api/configuration/nuxt-config

import vuetify, { transformAssetUrls } from 'vite-plugin-vuetify';

export default defineNuxtConfig({
  sourcemap: {
    server: true,
    client: true
  },
  devtools: {
    enabled: true,

    timeline: {
      enabled: true
    }
  },
  build: {
    transpile: ['vuetify'],
  },
  modules: [
    (_options, nuxt) => {
      nuxt.hooks.hook('vite:extendConfig', (config) => {
        // @ts-expect-error
        config.plugins.push(vuetify({ autoImport: true }))
      })
    }
  ],
  vite: {
    build: {
      target: 'esnext'
    },
    esbuild: {
      supported: {
        'top-level-await': true,
      }
    },
    vue: {
        template: {
          transformAssetUrls,
        },
    },
  },
  nitro: {
    esbuild: {
      options: {
        target: 'esnext'
      }
    },
  },
  typescript: {
    tsConfig: {
        compilerOptions: {
            esModuleInterop: true,
        }
    }
  },
  server: {     
    port: 3000, // default: 3000     
    host: '0.0.0.0', // default: localhost   
  },   // other configs 
})