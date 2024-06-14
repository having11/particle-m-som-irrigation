import { SmartIrrigationLedger } from '~/models/SmartIrrigationLedger';
import { saveSoilValues, saveValveStates } from '../utils/db';

const token: string = process.env.PARTICLE_API_TOKEN ?? '';
const ledgerName: string = process.env.PARTICLE_IRRIGATION_LEDGER_NAME ?? '';
const deviceId: string = process.env.PARTICLE_IRRIGATION_LEDGER_DEVICE_ID ?? '';

export default defineNitroPlugin(nitroApp => {
  setInterval(async () => {
    const ledgerData = await $fetch<{instance: {data: SmartIrrigationLedger}}>(
      `https://api.particle.io/v1/ledgers/${ledgerName}/instances/${deviceId}`, {
        method: 'GET',
        headers: [
          ['Authorization', `Bearer ${token}`],
          ['Accept', 'application/json'],
        ]
      });

    // Save ledgerData in db
    if (ledgerData.instance.data.valves)
      await saveValveStates(ledgerData.instance.data.valves.map((val, index) => ({
        index: index,
        state: val.state
      })));

    if (ledgerData.instance.data.soilSensors)
      await saveSoilValues(ledgerData.instance.data.soilSensors.map((val, index) => ({
        index: index,
        value: val.percentage
      })));
  }, 15 * 1000);
});