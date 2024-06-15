<template>
  <v-card
    :disabled="model?.loading"
    :loading="model?.loading"
    max-width="250"
    class="mx-auto my-12"
    style="display: flex; flex-direction: column;"
    height="100%"
  >
  <template v-slot:loader="{ isActive }">
    <v-progress-linear
      :active="isActive"
      color="dark-blue"
      height="4"
      indeterminate
    ></v-progress-linear>
  </template>
  <v-card-item>
    <v-card-title>{{props.valve.name}}</v-card-title>
  </v-card-item>
  <v-divider class=" mx-4 mb-2" />
  <v-card-title>State: 
    <span
      class="mdi mdi-valve-open"
      style="color: lightgreen;"
      v-if="props.valve.state" />
    <span
      class="mdi mdi-valve-closed"
      style="color: red;"
      v-else="props.valve.state" />
  </v-card-title>
  <v-card-title v-if="props.valve.soil_value">Soil sensor: {{props.valve.soil_value}}%</v-card-title>
  <v-card-title v-else>No sensor connected</v-card-title>

  <v-card-actions class="mt-auto">
    <v-btn
      color="blue"
      text="Activate"
      block
      border
      :disabled="props.valve.state"
      @click="activateValve"
    />
  </v-card-actions>
  </v-card>
</template>

<script setup lang="ts">
import type { Valve } from '~/models/Valve';

export type ValveControllerModel = {
  loading: boolean;
};

export type ValveControllerProps = {
  valve: Valve;
  refresh: () => void;
};

const model = defineModel<ValveControllerModel>();
const props = defineProps<ValveControllerProps>();

async function activateValve() {
  if (model.value) model.value.loading = true;

  const result = await $fetch('/api/valves', {
    method: 'POST',
    body: {
      index: props.valve.index,
      state: true,
    },
  });

  if (result == 1) {
    if (model.value) model.value.loading = false;

    props.refresh();
  }
}
</script>