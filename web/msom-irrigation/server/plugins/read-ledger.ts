import { SmartIrrigationLedger } from '~/models/SmartIrrigationLedger';

const token: string = process.env.PARTICLE_API_TOKEN ?? '';
const ledgerName: string = process.env.PARTICLE_IRRIGATION_LEDGER_NAME ?? '';

export default defineNitroPlugin(nitroApp => {
  setInterval(async () => {
    const ledgerData = await $fetch<{instance: {data: SmartIrrigationLedger}}>(
      `https://api.particle.io/v1/ledgers/${ledgerName}/instances/owner`, {
        method: 'GET',
        headers: [
          ['Authorization', `Bearer ${token}`],
          ['Accept', 'application/json'],
        ]
      });
    // TODO: Save ledgerData in db
  }, 15 * 1000);
});