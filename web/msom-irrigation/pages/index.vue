<template>
  <div style="width: 80%;">
    <v-row class="mt-10">
      <v-col
        cols="9"
      >
      <v-row>
        <v-col v-for="valve in data">
          <ValveController :valve="valve" :refresh="refresh" />
        </v-col>
      </v-row>
      </v-col>
      <v-col>
        <WeatherView />
      </v-col>
    </v-row>
  </div>
</template>

<script setup lang="ts">
import type { Valve } from '~/models/Valve';

const { data, pending, error, refresh } = await useLazyAsyncData<Valve[]>('valveStates',
  () => $fetch(`/api/valves`, {
      method: 'get',
  })
);
</script>