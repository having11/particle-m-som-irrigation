<template>
  <v-card
    :disabled="pending"
    :loading="pending"
    max-width="250"
    class="mx-auto my-12"
    style="display: flex; flex-direction: column;"
    height="auto"
  >
  <v-card-title>
    Weather Forecast
    <span class="mdi mdi-weather-partly-cloudy" />
  </v-card-title>
  <v-divider></v-divider>
  <v-list-item
    density="compact"
    class="text-h3 mb-2 mt-3"
  >
    {{ currentForecast?.temperature ?? 0 }}&deg;F
  </v-list-item>
  <v-list-item
    density="compact"
    prepend-icon="mdi-weather-pouring"
    class="text-h6 mb-2"
  >
    <v-list-item-subtitle class="text-h6">{{ currentForecast?.probabilityOfPrecipitation?.value ?? 0 }}%</v-list-item-subtitle>
  </v-list-item>
  <v-list-item
    density="compact"
    prepend-icon="mdi-water-percent"
    class="text-h6 mb-2"
  >
    <v-list-item-subtitle class="text-h6">{{ currentForecast?.relativeHumidity?.value ?? 0 }}%</v-list-item-subtitle>
  </v-list-item>
  <v-list-item-subtitle class="pa-4 pb-9">{{ currentForecast?.detailedForecast }}</v-list-item-subtitle>
  </v-card>
</template>

<script setup lang="ts">
import type { GridpointForecastPeriod } from '~/integrations/weather';

const currentForecast = ref<GridpointForecastPeriod | null>(null);

const { data, pending, error, refresh } = await useLazyAsyncData<{properties: {periods?: Array<GridpointForecastPeriod>}}>('forecast',
  () => $fetch(`/api/forecast`, {
      method: 'get',
  })
);

watch(data, (newData, oldData) => {
  if (newData?.properties.periods && newData.properties?.periods[0]) {
    currentForecast.value = newData.properties.periods[0];
  }
});
</script>